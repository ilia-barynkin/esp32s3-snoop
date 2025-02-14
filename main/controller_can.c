#include "controller_can.h"
#include "ring_buffer.h"

QueueHandle_t can_tx_queue = NULL;
QueueHandle_t can_tx_status_queue = NULL;
QueueHandle_t can_rx_queue = NULL;

extern transaction_ring_buffer_t transaction_ring_buffer;

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
        ESP_LOGE("CAN", "Failed to install TWAI driver %s", esp_err_to_name(err));
    }

    can_tx_queue = xQueueCreate(10, sizeof(twai_message_t));
    can_tx_status_queue = xQueueCreate(10, sizeof(can_tx_status_t));
    can_rx_queue = xQueueCreate(10, sizeof(twai_message_t));

    // Start TWAI driver
    err = twai_start();
    if (err != ESP_OK) {
        ESP_LOGE("CAN", "Failed to start TWAI driver %s", esp_err_to_name(err));
    }

    ring_buffer_init(&transaction_ring_buffer);
}

void controller_can_task(void *pvParameters) {
    controller_can_init();

    can_extended_message_t ext_message;
    can_tx_status_t tx_status;

    while (1) {
        ESP_LOGI("CAN", "CAN task started");
        if (xQueueReceive(can_tx_queue, &ext_message, portMAX_DELAY) == pdPASS) {
            ESP_LOGI("CAN_TX", "Received message ID=0x%lu", ext_message.message.identifier);
#ifdef TEST_CAN_LOOPBACK
            ext_message.message.flags = TWAI_MSG_FLAG_SELF;
#endif
            esp_err_t err = twai_transmit(&ext_message.message, portMAX_DELAY);
            if (err == ESP_OK) {
                tx_status.message_id = ext_message.message.identifier;
                tx_status.success = true;
                SEND_TO_QUEUE(can_tx_status_queue, tx_status);
            }
            else {
                tx_status.message_id = ext_message.message.identifier;
                tx_status.success = false;
                ESP_LOGE("CAN TX error", "ID=0x%lu %s", ext_message.message.identifier, esp_err_to_name(err));
                SEND_TO_QUEUE(can_tx_status_queue, tx_status);
            }
        }

        if (twai_receive(&ext_message.message, portMAX_DELAY) == ESP_OK) {
            void *ui_event_ref = ring_buffer_find_and_remove(&transaction_ring_buffer, ext_message.message.identifier);
            if (ui_event_ref) {
                ext_message.ui_event_ref = ui_event_ref;
            }
            SEND_TO_QUEUE(can_rx_queue, ext_message);
        }
    }
}