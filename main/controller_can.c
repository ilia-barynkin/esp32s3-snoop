#include "controller_can.h"
#include "model.h"
#include "ui/ui_model_bridge.h"
#include "esp_io_expander_ch422g.h"

QueueHandle_t can_tx_queue = NULL;
QueueHandle_t can_tx_status_queue = NULL;
QueueHandle_t can_rx_queue = NULL;

static void log_twai_status() {
    twai_status_info_t status;
    esp_err_t err = twai_get_status_info(&status);

    if (err != ESP_OK) {
        ESP_LOGE("TWAI", "Failed to get TWAI status, error: %d", err);
        return;
    }

    ESP_LOGI("TWAI", " ==== TWAI Status: ==== ");
    if (status.state >= TWAI_STATE_STOPPED && status.state <= TWAI_STATE_RECOVERING) {
        const char *state_str = (status.state == TWAI_STATE_STOPPED) ? "STOPPED" :
                                (status.state == TWAI_STATE_RUNNING) ? "RUNNING" :
                                (status.state == TWAI_STATE_BUS_OFF) ? "BUS OFF" :
                                (status.state == TWAI_STATE_RECOVERING) ? "RECOVERING" : "UNKNOWN";
        ESP_LOGI("TWAI", "  State: %s", state_str);
    } else {
        ESP_LOGW("TWAI", "  State: UNKNOWN");
    }

    ESP_LOGI("TWAI", "  Msgs to TX: %lu", status.msgs_to_tx);
    ESP_LOGI("TWAI", "  Msgs to RX: %lu", status.msgs_to_rx);
    ESP_LOGI("TWAI", "  TX ERR: %lu, RX ERR: %lu", status.tx_error_counter, status.rx_error_counter);
    ESP_LOGI("TWAI", "  TX Failed: %lu", status.tx_failed_count);
    ESP_LOGI("TWAI", "  RX Missed: %lu", status.rx_missed_count);
    ESP_LOGI("TWAI", "  RX Overrun: %lu", status.rx_overrun_count);
    ESP_LOGI("TWAI", "  Arb Lost: %lu", status.arb_lost_count);
    ESP_LOGI("TWAI", "  Bus Error: %lu", status.bus_error_count);
    ESP_LOGI("TWAI", " ==== TWAI Status: ==== ");
}

//#define TEST_CAN_LOOPBACK

static void controller_can_init(void) {
#ifdef TEST_CAN_LOOPBACK
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(19, 20, TWAI_MODE_NO_ACK);
#else
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(20, 21, TWAI_MODE_NORMAL);
#endif
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    uint32_t alerts_to_enable = TWAI_ALERT_TX_IDLE | TWAI_ALERT_TX_SUCCESS | TWAI_ALERT_TX_FAILED | TWAI_ALERT_ERR_PASS | TWAI_ALERT_BUS_ERROR;

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
    else {
        ESP_LOGI("CAN", "TWAI driver started");
    }

    err = twai_reconfigure_alerts(alerts_to_enable, NULL);
    if (err == ESP_OK)
    {
        ESP_LOGI("CAN", "CAN Alerts reconfigured"); // Log alerts reconfigured
    }
    else
    {
        ESP_LOGE("CAN", "Failed to reconfigure CAN alerts %s", esp_err_to_name(err));
    }
}

void controller_can_tx_task(void *pvParameters) {
    twai_message_t msg;
    can_tx_status_t tx_status;
    ESP_LOGI("CAN", "CAN task started");

    while (1) {
        if (xQueueReceive(can_tx_queue, &msg, portMAX_DELAY) == pdPASS) {
            log_twai_status();

            #ifdef TEST_CAN_LOOPBACK
            msg.flags = TWAI_MSG_FLAG_SELF;
            #else
            msg.flags = TWAI_MSG_FLAG_NONE;
            #endif

            ESP_LOGI("CAN_TX", "Sending message ID=0x%lu", msg.identifier);
            esp_io_expander_set_level(ch422g_handle, 0x20, 1);
            esp_err_t err = twai_transmit(&msg, pdMS_TO_TICKS(1000));
            tx_status.message_id = msg.identifier;
            tx_status.status = err;
            xQueueSend(can_tx_status_queue, &tx_status, portMAX_DELAY);
        }
    }
}

void can_tx_status_task(void *pvParameters) {
    can_tx_status_t tx_status;
    while (1) {
        if (xQueueReceive(can_tx_status_queue, &tx_status, portMAX_DELAY) == pdPASS) {
            uint32_t alerts;
            twai_read_alerts(&alerts, portMAX_DELAY);
            switch (alerts) {
                case TWAI_ALERT_ERR_PASS:
                    ESP_LOGI("CAN", "TWAI Error Passive Alert");
                    break;
                case TWAI_ALERT_BUS_ERROR:
                    ESP_LOGI("CAN", "TWAI Bus Error Alert");
                    break;
                case TWAI_ALERT_TX_FAILED:
                    ESP_LOGI("CAN", "TWAI TX Failed Alert");
                    break;
                case TWAI_ALERT_TX_SUCCESS:
                    ESP_LOGI("CAN", "TWAI TX Success Alert");
                    break;
                default:
                    break;
            }
        }
    }
}

void controller_can_rx_task(void *pvParameters) {
    twai_message_t received_msg;
    while (1) {
        if (twai_receive(&received_msg, portMAX_DELAY) == ESP_OK) {
            ESP_LOGI("CAN RX", "CAN message received ID=0x%lu", received_msg.identifier);
            notify_model_can_response(&received_msg);
        }
    }
}

void controller_can_start(void) {
    controller_can_init();
    xTaskCreate(controller_can_tx_task, "controller_can_tx_task", 4096, NULL, 5, NULL);
    xTaskCreate(can_tx_status_task, "can_tx_status_task", 4096, NULL, 5, NULL);
    xTaskCreate(controller_can_rx_task, "controller_can_rx_task", 4096, NULL, 5, NULL);
}