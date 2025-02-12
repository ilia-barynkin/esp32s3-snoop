#include "controller_can.h"
#include "model.h"
#include "waveshare_rgb_lcd_port.h"

extern QueueHandle_t can_tx_queue = NULL;
extern QueueHandle_t can_tx_status_queue = NULL;
extern QueueHandle_t can_rx_queue = NULL;

twai_general_config_t g_config;
twai_timing_config_t t_config;
twai_filter_config_t f_config;

static void controller_can_init(void) {
    uint8_t write_buf = 0x01;
    i2c_master_write_to_device(I2C_MASTER_NUM, 0x24, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    write_buf = 0x20;
    i2c_master_write_to_device(I2C_MASTER_NUM, 0x38, &write_buf, 1, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    // Install TWAI driver
    if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK)
    {
        ESP_LOGI(TAG, "Driver installed"); // Log driver installed
    }
    else
    {
        ESP_LOGI(TAG, "Failed to install driver"); // Log installation failure
    }

    // Start TWAI driver
    if (twai_start() == ESP_OK)
    {
        ESP_LOGI(TAG, "Driver started"); // Log driver started
    }
    else
    {
        ESP_LOGI(TAG, "Failed to start driver"); // Log start failure
    }

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        ESP_LOGI(TAG, "TWAI error");
        vTaskDelete(NULL);
    }

    can_tx_queue = xQueueCreate(10, sizeof(twai_message_t));
    can_tx_status_queue = xQueueCreate(10, sizeof(can_tx_status_t));
    can_rx_queue = xQueueCreate(10, sizeof(twai_message_t));

    twai_start();
}

static void send_can_loopback_message() {
    twai_message_t loopback_msg = {
        .identifier = 0x123,
        .data_length_code = 8,
        .data = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x11, 0x22},
        .flags = TWAI_MSG_FLAG_NONE
    };
    xQueueSend(can_tx_queue, &loopback_msg, portMAX_DELAY);
}

void controller_can_task(void *pvParameters) {
    controller_can_init();

    twai_message_t message;
    can_tx_status_t tx_status;

    twai_general_config_t g_config = {
        .controller_id = 0,
    #ifdef TEST_CAN_LOOPBACK
        .mode = TWAI_MODE_NO_ACK,
    #else
        .mode = TWAI_MODE_NORMAL,       
    #endif
        .tx_io = CONFIG_CAN_RX_GPIO_NUM,
        .rx_io = CONFIG_CAN_TX_GPIO_NUM,
        .clkout_io = TWAI_IO_UNUSED,
        .bus_off_io = TWAI_IO_UNUSED,
        .tx_queue_len = 5,
        .rx_queue_len = 5,
        .alerts_enabled = TWAI_ALERT_NONE,
        .clkout_divider = 0,
        .intr_flags = ESP_INTR_FLAG_LEVEL1
    };

    // t_config = TWAI_TIMING_CONFIG_500KBITS();
    // f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    twai_timing_config_t t_config = {
        .clk_src = TWAI_CLK_SRC_DEFAULT,
        .quanta_resolution_hz = 10000000,
        .brp = 0,
        .tseg_1 = 15,
        .tseg_2 = 4,
        .sjw = 3,
        .triple_sampling = false
    };
    
    twai_filter_config_t f_config = {
        .acceptance_code = 0,
        .acceptance_mask = 0xFFFFFFFF,
        .single_filter = true
    };

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK) {
        ESP_LOGE("CAN", "TWAI driver install error");
        vTaskDelete(NULL);
    }

    if(twai_start() != ESP_OK) {
        ESP_LOGE("CAN", "TWAI start error");
        vTaskDelete(NULL);
    }

    while (1) {
        if (xQueueReceive(can_tx_queue, &message, portMAX_DELAY) == pdPASS) {
            if (twai_transmit(&message, portMAX_DELAY) == ESP_OK) {
                tx_status.message_id = message.identifier;
                tx_status.success = true;
                //ESP_LOGI("CAN TX", "ID=0x%X", message.identifier);
            } else {
                tx_status.message_id = message.identifier;
                tx_status.success = false;
            }
            xQueueSend(can_tx_status_queue, &tx_status, portMAX_DELAY);
        }
        if (xQueueReceive(can_rx_queue, &message, portMAX_DELAY) == pdPASS) {
            //ESP_LOGI("CAN RX", "ID=0x%X", message.identifier);
            xQueueSend(model_event_queue, &message, portMAX_DELAY);
        }

        #ifdef TEST_CAN_LOOPBACK
        send_can_loopback_message();
        vTaskDelay(pdMS_TO_TICKS(100000) / portTICK_PERIOD_MS);
        #endif
    }
}