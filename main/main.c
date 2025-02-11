/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "main.h"
#include "model.h"
#include "controller_uart.h"

// TODO: replace

void view_task(void* pvParameters) {
    waveshare_esp32_s3_rgb_lcd_init();

    if (lvgl_port_lock(-1)) {
        ui_init();
        
        while(1) {
            ESP_LOGI("FreeRTOS", "UI main task");
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        lvgl_port_unlock();
    }
}

QueueHandle_t model_event_queue;

void app_main()
{
    //queues
    if(!(model_event_queue = xQueueCreate(MODEL_QUEUE_SIZE, sizeof(model_event_t)))) {
        ESP_LOGE("Model", "Failed to create model event queue");
        abort();
    };
    
    //tasks
    xTaskCreate(view_task, "view_task", 2048, NULL, 5, NULL);
    xTaskCreate(model_task, "model_task", 2048, NULL, 5, NULL);
    xTaskCreate(uart_controller_task, "uart_controller_task", 2048, NULL, 5, NULL);
}
