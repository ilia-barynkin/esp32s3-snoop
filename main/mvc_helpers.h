#ifndef _MVC_HELPERS_H_
#define _MVC_HELPERS_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "ring_buffer.h"
#include "memory.h"
#include "esp_log.h"

extern transaction_ring_buffer_t transaction_ring_buffer;

#define SEND_TO_QUEUE(queue, item) \
do { \
    if (xQueueSend(queue, &item, portMAX_DELAY) != pdPASS) { \
        ESP_LOGE("QUEUE", "Failed to send message to queue: %s", #queue); \
    } \
} while (0)

// extern QueueHandle_t can_tx_queue;

// inline void send_can_msg(uint32_t id, uint8_t *data, uint8_t len, void *ui_event) {
//     uint8_t raw_msg[8 + sizeof(uint32_t)];
//     memcpy(raw_msg, &id, sizeof(uint32_t));
//     memcpy(raw_msg + sizeof(uint32_t), data, len);
//     ring_buffer_add(&transaction_ring_buffer, id, ui_event);
//     ESP_LOGI("CAN", "Sending CAN message: %lu", id);
//     SEND_TO_QUEUE(can_tx_queue, raw_msg);
// }

#endif //_MVC_HELPERS_H_