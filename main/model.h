#ifndef MODEL_H
#define MODEL_H

#include "main.h"
#include "controller_can.h"
#include "ring_buffer.h"
#include "ui/ui_model_bridge.h"

extern uint32_t LV_EVENT_CAN_RECV;

extern ring_buffer_t transaction_ring_buffer;

extern QueueHandle_t ui_update_queue;
extern TaskHandle_t model_task_handle;

void model_start(void);
void model_task(void *pvParameters);
void notify_model_ui_event(can_extended_message_t *msg);
void notify_model_can_response(twai_message_t *msg);
void can_task(void *pvParameters);

#endif