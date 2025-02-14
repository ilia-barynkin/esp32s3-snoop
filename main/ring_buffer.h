#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef struct {
    uint32_t transaction_id;
    void *ui_event_ref;
} can_transaction_entry_t;

typedef struct {
    can_transaction_entry_t buffer[CONFIG_RING_BUFFER_SIZE];
    int head;
    int tail;
    int count;
} ring_buffer_t;

typedef ring_buffer_t transaction_ring_buffer_t;

void ring_buffer_init(ring_buffer_t *rb);
void ring_buffer_add(ring_buffer_t *rb, uint32_t transaction_id, void *ui_event_ref);
void *ring_buffer_find_and_remove(ring_buffer_t *rb, uint32_t transaction_id);

extern transaction_ring_buffer_t transaction_ring_buffer;

#endif // RING_BUFFER_H