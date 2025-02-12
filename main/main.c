/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "main.h"
#include "model.h"
#include "controller_uart.h"
#include "controller_can.h"

// TODO: replace

// void view_task(void* pvParameters) {
//     if (lvgl_port_lock(-1)) {
//         // waveshare_esp32_s3_rgb_lcd_init();
//         // ui_init();

//         while(1) {
//             vTaskDelay(pdMS_TO_TICKS(10));
//         }

//         lvgl_port_unlock();
//     }
// }

QueueHandle_t model_event_queue;


void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init();

    //queues
    model_event_queue = xQueueCreate(10, sizeof(model_event_t));
    //ui_update_queue = xQueueCreate(10, sizeof(ui_update_t));
    //tasks
    //xTaskCreate(view_task, "view_task", 4096, NULL, 5, NULL);
    xTaskCreate(model_task, "model_task", 8192, NULL, 5, NULL);
    xTaskCreate(uart_controller_task, "uart_controller_task", 8192, NULL, 5, NULL);
    xTaskCreate(controller_can_task, "controller_can_task", 8192, NULL, 5, NULL);
}
