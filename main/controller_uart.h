#ifndef _CONTROLLER_UART_H
#define _CONTROLLER_UART_H

#include "main.h"
#include "driver/uart.h"
#include "driver/gpio.h"

extern QueueHandle_t uart_tx_queue;
extern QueueHandle_t uart_rx_queue;

typedef struct {
    uint32_t length;
    void* data;
} uart_message_t;

uart_message_t* uart_message_alloc(void);

void uart_controller_init(void);
void uart_controller_task(void *pvParameters);

#endif _CONTROLLER_UART_H