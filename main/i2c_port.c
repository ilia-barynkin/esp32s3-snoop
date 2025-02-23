#include "i2c_port.h"

static TAG = "i2c_port";

SemaphoreHandle_t i2c_bus_mutex = NULL;

esp_err_t i2c_port_init(void) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    esp_err_t err = i2c_param_config(I2C_PORT, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C param config error: %s", esp_err_to_name(err));
        return err;
    }

    err = i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "I2C driver install error: %s", esp_err_to_name(err));
        return err;
    }

    // Создаем глобальный мьютекс для шины I2C
    i2c_bus_mutex = xSemaphoreCreateMutex();
    if (i2c_bus_mutex == NULL) {
        ESP_LOGE(TAG, "Failed to create I2C bus mutex");
        return ESP_FAIL;
    }

    return ESP_OK;
}