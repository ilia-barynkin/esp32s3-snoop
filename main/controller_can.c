#include "controller_can.h"
#include "model.h"
//#include "waveshare_rgb_lcd_port.h"

extern QueueHandle_t can_tx_queue = NULL;
extern QueueHandle_t can_tx_status_queue = NULL;
extern QueueHandle_t can_rx_queue = NULL;


static void controller_can_init(void) {
#ifdef TEST_CAN_LOOPBACK
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(19, 20, TWAI_MODE_NO_ACK);
#else
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(19, 20, TWAI_MODE_NORMAL);
#endif
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS(); // Скорость 500 кбит/с
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // Принимать все сообщения

    // Install TWAI driver
    esp_err_t err = twai_driver_install(&g_config, &t_config, &f_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install TWAI driver %s", esp_err_to_name(err));
    }

    can_tx_queue = xQueueCreate(10, sizeof(twai_message_t));
    can_tx_status_queue = xQueueCreate(10, sizeof(can_tx_status_t));
    can_rx_queue = xQueueCreate(10, sizeof(twai_message_t));

    // Start TWAI driver
    err = twai_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start TWAI driver %s", esp_err_to_name(err));
    }
}

void controller_can_task(void *pvParameters) {
    controller_can_init();

    twai_message_t message;
    can_tx_status_t tx_status;

    while (1) {
        if (xQueueReceive(can_tx_queue, &message, portMAX_DELAY) == pdPASS) {
            if (twai_transmit(&message, portMAX_DELAY) == ESP_OK) {
                tx_status.message_id = message.identifier;
                tx_status.success = true;
                ESP_LOGI("CAN TX", "ID=0x%lu", message.identifier);
            } else {
                tx_status.message_id = message.identifier;
                tx_status.success = false;
            }
            xQueueSend(can_tx_status_queue, &tx_status, portMAX_DELAY);
        }
        if (twai_receive(&message, portMAX_DELAY) == ESP_OK) {
            ESP_LOGI("CAN RX", "ID=0x%lu", message.identifier);
            xQueueSend(can_rx_queue, &message, portMAX_DELAY);
        }

#ifdef TEST_CAN_LOOPBACK
        vTaskDelay(pdMS_TO_TICKS(100000) / portTICK_PERIOD_MS);
#endif
    }
}