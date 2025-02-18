#ifndef _CONTROLLER_CAN_H_
#define _CONTROLLER_CAN_H_

#include "main.h"
#include "driver/twai.h"
#include "ring_buffer.h"

// #define TEST_CAN_LOOPBACK

extern QueueHandle_t can_tx_queue;
extern QueueHandle_t can_rx_queue;

//extern twai_general_config_t g_config;
//extern twai_timing_config_t t_config;
//extern twai_filter_config_t f_config;

typedef struct {
    uint32_t message_id;
    esp_err_t status;
} can_tx_status_t;

void controller_can_tx_task(void *pvParameters);
void controller_can_rx_task(void *pvParameters);
void controller_can_start(void);

#endif // _CONTROLLER_CAN_H_