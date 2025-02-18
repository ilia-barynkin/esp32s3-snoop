#include "ui_to_if.h"
#include "memory.h"
#include "../../components/lvgl__lvgl/lvgl.h"

extern uint32_t LV_EVENT_CAN_RECV;

void ui_send_can_message(uint32_t id, uint8_t *data, uint8_t len, void *ui_event)
{
    can_extended_message_t msg = {
        .identifier = id,
        .data_length_code = len,
        .ui_event_ref = ui_event
    };

    memcpy(msg.data, data, len);
    
    notify_model_ui_event(&msg);
}
