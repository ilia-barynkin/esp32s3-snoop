#include <stdlib.h>
#include <inttypes.h>
#include "esp_log.h"
#include "ring_buffer.h"

// TODO: fix it
transaction_ring_buffer_t transaction_ring_buffer;

void ring_buffer_init(ring_buffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}
void ring_buffer_add(ring_buffer_t *rb, uint32_t transaction_id, void *ui_event_ref) {
    if (rb->count >= CONFIG_RING_BUFFER_SIZE) {
        ESP_LOGW("RING_BUFFER", "Buffer overflow, dropping transaction_id: %" PRIu32, transaction_id);
        return;
    }
    rb->buffer[rb->head].transaction_id = transaction_id;
    rb->buffer[rb->head].ui_event_ref = ui_event_ref;
    rb->head = (rb->head + 1) % CONFIG_RING_BUFFER_SIZE;
    rb->count++;
}

void *ring_buffer_find_and_remove(ring_buffer_t *rb, uint32_t transaction_id) {
    for (int i = 0; i < rb->count; i++) {
        int index = (rb->tail + i) % CONFIG_RING_BUFFER_SIZE;
        if (rb->buffer[index].transaction_id == transaction_id) {
            void *ui_event_ref = rb->buffer[index].ui_event_ref;
            for (int j = i; j < rb->count - 1; j++) {
                int cur_index = (rb->tail + j) % CONFIG_RING_BUFFER_SIZE;
                int next_index = (rb->tail + j + 1) % CONFIG_RING_BUFFER_SIZE;
                rb->buffer[cur_index] = rb->buffer[next_index];
            }
            rb->tail = (rb->tail + 1) % CONFIG_RING_BUFFER_SIZE;
            rb->count--;
            return ui_event_ref;
        }
    }
    
    return NULL;
}