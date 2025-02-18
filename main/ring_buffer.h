#ifndef RING_BUFFER_H
#define RING_BUFFER_H

typedef struct {
    uint32_t id;
    void* data;
} rb_entry_t;

typedef struct {
    rb_entry_t buffer[CONFIG_RING_BUFFER_SIZE];
    int head;
    int tail;
    int count;
} ring_buffer_t;

typedef ring_buffer_t transaction_ring_buffer_t;
typedef void* ui_event_ref_t_ptr;
typedef rb_entry_t can_transaction_entry_t;

void ring_buffer_init(ring_buffer_t *rb);
void ring_buffer_add(ring_buffer_t *rb, uint32_t id, ui_event_ref_t_ptr ui_event_ref);
void *ring_buffer_find_and_remove(ring_buffer_t *rb, uint32_t id);

#endif // RING_BUFFER_H