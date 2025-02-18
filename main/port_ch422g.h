#ifndef _I2C_PERIPH_H_
#define _I2C_PERIPH_H_

#include "driver/i2c_master.h"

#define I2C_SCL_GPIO_NUM GPIO_NUM_9
#define I2C_SDA_GPIO_NUM GPIO_NUM_8

#define I2C_CH422G_ADDR    (0x24)


// extern i2c_master_bus_handle_t bus_handle;
// extern i2c_master_dev_handle_t dev_handle;
extern i2c_slave_dev_handle_t ch422g_dev;

void init_i2c_ch422g();

#endif