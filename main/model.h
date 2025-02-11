#ifndef _MODEL_H_
#define _MODEL_H_


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef enum {
    EVENT_BUTTON_PRESS,
    EVENT_CAN_RESPONSE,
    EVENT_UART_RESPONSE,
    EVENT_SPI_RESPONSE,
    EVENT_WIFI_RESPONSE,
    EVENT_ERROR
} model_event_type_t;

typedef struct model_event_t {
    model_event_type_t type;
    bool success;
    void* data;
} model_event_t;

typedef enum {
    STATE_IDLE,
    STATE_PROCESSING,
    STATE_WAITING_RESPONSE,
    STATE_ERROR
} model_state_t;

#define MODEL_QUEUE_SIZE 10

extern QueueHandle_t model_event_queue;

void model_task(void* pvParameters);

#endif _MODEL_H_