#ifndef I2C_PORT_H
#define I2C_PORT_H

#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#define I2C_MASTER_SCL_IO           GPIO_NUM_9       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           GPIO_NUM_8       /*!< GPIO number used for I2C master data  */
#define I2C_PORT                    I2C_NUM_0       /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TIMEOUT_MS       1000

extern SemaphoreHandle_t i2c_bus_mutex;

esp_err_t i2c_port_init(void);

#endif