#ifndef UI_MODEL_BRIDGE_H
#define UI_MODEL_BRIDGE_H

#include <stdint.h>

typedef struct {
    uint32_t identifier;
    uint8_t data[8];
    uint8_t data_length_code;
    void *ui_event_ref;
} can_extended_message_t;

// Функция для отправки события из UI в модель
extern void notify_model_ui_event(can_extended_message_t *msg);

#endif // UI_MODEL_BRIDGE_H