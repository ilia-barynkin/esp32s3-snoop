#include "main.h"
#include "model.h"

model_state_t current_state = STATE_IDLE;

void model_task(void* pvParameters) {
    model_event_t event = {};

    while(1) {
        if (xQueueReceive(model_event_queue, &event, portMAX_DELAY) == pdPASS) {
            switch (current_state) {
                case STATE_IDLE:
                    current_state = STATE_PROCESSING;
                    break;
                case STATE_PROCESSING:
                    current_state = STATE_WAITING_RESPONSE;
                    break;
                case STATE_WAITING_RESPONSE:
                    current_state = (event.success) ? STATE_IDLE : STATE_ERROR;
                    break;
                case STATE_ERROR:
                    ESP_LOGI(TAG, "Error");
                    current_state = STATE_IDLE;
                    break;
            }
        }
    }   
}