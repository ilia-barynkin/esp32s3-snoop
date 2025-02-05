/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "waveshare_rgb_lcd_port.h"
#include "ui/ui.h"

void ui_task(void* pvParameters) {
    waveshare_esp32_s3_rgb_lcd_init();

    if (lvgl_port_lock(-1)) {
        ui_init();
        ESP_LOGI("lvgl", "UI inited");
        // lv_demo_stress();
        // lv_demo_benchmark();
        // lv_demo_music();
        //lv_demo_widgets();
        // example_lvgl_demo_ui();
        // Release the mutex
        while(1) {
            ESP_LOGI("FreeRTOS", "UI main task");
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        lvgl_port_unlock();
    }
}

void app_main()
{   
    xTaskCreate(ui_task, "UI Task", 8192, NULL, 5, NULL);

    ESP_LOGI("FreeRTOS", "Start scheduler");
    vTaskStartScheduler();
}
