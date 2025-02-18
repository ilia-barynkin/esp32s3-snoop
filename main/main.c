/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "main.h"
#include "controller_can.h"
#include "waveshare_rgb_lcd_port.h"
#include "model.h"
#include "i2c_periph.h"

void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init();
    
    model_start();
    controller_can_start();
}
