#ifndef _MAIN_H_
#define _MAIN_H_

#include "sdkconfig.h"
#include "inttypes.h"
#include "esp_log.h"
#include "esp_err.h"
//#include <driver/gpio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
//#include "waveshare_rgb_lcd_port.h"
#include "driver/twai.h"
#include "mvc_helpers.h"

extern QueueHandle_t can_tx_queue;

#define SEND_CAN_MSG(id, data, len, ui_event) \
    do { \
        uint8_t raw_msg[8 + sizeof(uint32_t)]; \
        memcpy(raw_msg, &id, sizeof(uint32_t)); \
        memcpy(raw_msg + sizeof(uint32_t), data, len); \
        ring_buffer_add(&transaction_ring_buffer, id, ui_event); \
        SEND_TO_QUEUE(can_tx_queue, raw_msg); \
    } while (0)

#endif //_MAIN_H_