/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "main.h"
#include "controller_uart.h"
#include "controller_can.h"
#include "waveshare_rgb_lcd_port.h"

// TODO: replace

void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init();
    
    xTaskCreate(controller_can_task, "controller_can_task", 8192, NULL, 5, NULL);
}
