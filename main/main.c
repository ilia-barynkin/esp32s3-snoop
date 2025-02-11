/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "main.h"
#include "model.h"

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

// TODO: replace

void uart_controller_task(void *pvParameters) {
    uart_message_t message;
    uint8_t data[128];
    while (1) {
        if (xQueueReceive(uart_tx_queue, &message, portMAX_DELAY) == pdPASS) {
            uart_write_bytes(UART_NUM_1, (const char *)message.data, message.length);
        }
        int length = uart_read_bytes(UART_NUM_1, data, sizeof(data), pdMS_TO_TICKS(100));
        if (length > 0) {
            uart_message_t response;
            memcpy(response.data, data, length);
            response.length = length;
            xQueueSend(uart_rx_queue, &response, portMAX_DELAY);
        }
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
}
