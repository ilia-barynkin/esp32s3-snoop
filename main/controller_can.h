#ifndef _CONTROLLER_CAN_H_
#define _CONTROLLER_CAN_H_

#include "main.h"
#include "driver/twai.h"

#define TEST_CAN_LOOPBACK

extern QueueHandle_t can_tx_queue;
extern QueueHandle_t can_tx_status_queue;
extern QueueHandle_t can_rx_queue;
 
typedef struct {
    uint32_t message_id;
    bool success;
} can_tx_status_t;

//extern twai_general_config_t g_config;
//extern twai_timing_config_t t_config;
//extern twai_filter_config_t f_config;

void controller_can_task(void *pvParameters);

#endif // _CONTROLLER_CAN_H_