#include <stdlib.h>
#include <inttypes.h>
#include "esp_log.h"
#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}
void ring_buffer_add(ring_buffer_t *rb, uint32_t id, ui_event_ref_t_ptr data) {
    if (rb->count >= CONFIG_RING_BUFFER_SIZE) {
        ESP_LOGW("RING_BUFFER", "Buffer overflow, dropping id: %" PRIu32, id);
        return;
    }
    rb->buffer[rb->head].id = id;
    rb->buffer[rb->head].data = data;
    rb->head = (rb->head + 1) % CONFIG_RING_BUFFER_SIZE;
    rb->count++;
}

ui_event_ref_t_ptr ring_buffer_find_and_remove(ring_buffer_t *rb, uint32_t id) {
    for (int i = 0; i < rb->count; i++) {
        int index = (rb->tail + i) % CONFIG_RING_BUFFER_SIZE;
        if (rb->buffer[index].id == id) {
            void *data = rb->buffer[index].data;
            for (int j = i; j < rb->count - 1; j++) {
                int cur_index = (rb->tail + j) % CONFIG_RING_BUFFER_SIZE;
                int next_index = (rb->tail + j + 1) % CONFIG_RING_BUFFER_SIZE;
                rb->buffer[cur_index] = rb->buffer[next_index];
            }
            rb->tail = (rb->tail + 1) % CONFIG_RING_BUFFER_SIZE;
            rb->count--;
            return data;
        }
    }
    
    return NULL;
}