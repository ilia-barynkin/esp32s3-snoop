#include "model.h"
#include "lvgl.h"

QueueHandle_t ui_update_queue = NULL;
TaskHandle_t model_task_handle = NULL;
ring_buffer_t transaction_ring_buffer;
uint32_t LV_EVENT_CAN_RECV = 0;

static void model_init(void) {
    ui_update_queue = xQueueCreate(10, sizeof(can_extended_message_t));
    
    LV_EVENT_CAN_RECV = lv_event_register_id();

    ring_buffer_init(&transaction_ring_buffer);
}
void model_start (void) {
    model_init();
    xTaskCreate(model_task, "model_task", 4096, NULL, 5, &model_task_handle);
}

void model_task(void *pvParameters) {
    uint32_t ulNotificationValue;

    while (1) {
        if (xTaskNotifyWait(0x00, ULONG_MAX, &ulNotificationValue, portMAX_DELAY) == pdPASS) {
            if (ulNotificationValue & (1 << 0)) { // UI request 
                can_extended_message_t msg_to_send;
                if (xQueueReceive(ui_update_queue, &msg_to_send, 0) == pdPASS) {
                    //TODO: "inheritance"
                    twai_message_t raw_msg = {
                        .identifier = msg_to_send.identifier,
                        .data_length_code = msg_to_send.data_length_code,
                        .flags = TWAI_MSG_FLAG_NONE
                    };
                    ESP_LOGI("CAN_UI", "Request from UI to send CAN message: %lu", msg_to_send.identifier);
                    ring_buffer_add(&transaction_ring_buffer, msg_to_send.identifier, msg_to_send.ui_event_ref);
                    SEND_TO_QUEUE(can_tx_queue, raw_msg);
                }
            }
            if (ulNotificationValue & (1 << 1)) { // CAN response
                twai_message_t recv_msg;
                if (xQueueReceive(can_rx_queue, &recv_msg, 0) == pdPASS) {
                    ESP_LOGI("CAN_UI", "CAN message sent by UI response received: %lu", recv_msg.identifier);
                    lv_event_t* lv_can_cb_event = ring_buffer_find_and_remove(&transaction_ring_buffer, recv_msg.identifier);
                    if (lv_can_cb_event != NULL) {
                        lv_event_send(lv_can_cb_event->target, LV_EVENT_CAN_RECV, recv_msg.data);
                    }
                }
            }
        }

        // TODO: add failure handling
    }
}

void notify_model_ui_event(can_extended_message_t *msg) {
    xQueueSend(ui_update_queue, msg, portMAX_DELAY);
    xTaskNotify(model_task_handle, (1 << 0), eSetBits);
}

void notify_model_can_response(twai_message_t *msg) {
    xQueueSend(can_rx_queue, msg, portMAX_DELAY);
    xTaskNotify(model_task_handle, (1 << 1), eSetBits);
}