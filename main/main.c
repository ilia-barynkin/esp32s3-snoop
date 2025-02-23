/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "main.h"
#include "controller_can.h"
#include "waveshare_rgb_lcd_port.h"
#include "model.h"
//#include "i2c_periph.h"
#include "driver/gpio.h"

#include "ui/ui.h"
#include "ui/ui_events.h"

#define TWAI_TEST 


void app_main()
{
    waveshare_esp32_s3_rgb_lcd_init();
    
    model_start();
    controller_can_start();

    /*#ifdef TWAI_TEST
    xTaskCreate(twai_test_task, "twai_test_task", 4096, NULL, 5, NULL);
    #endif*/
}
