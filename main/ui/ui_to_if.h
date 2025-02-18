#ifndef _UI_TO_IF_H_
#define _UI_TO_IF_H_

#include "stdint.h"
#include "ui_model_bridge.h"

// Common functions to access peripherals from UI

void ui_send_can_message(uint32_t id, uint8_t *data, uint8_t len, void *ui_event);

// TODO: uart, etc

#endif /* _UI_TO_IF_H_ */