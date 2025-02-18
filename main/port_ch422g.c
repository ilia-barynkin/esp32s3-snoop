#include "port_ch422g.h"

static i2c_master_bus_handle_t bus_handle = NULL;
static i2c_master_dev_handle_t dev_handle = NULL;
i2c_slave_dev_handle_t ch422g_dev = NULL;

static void init_i2c_bus() {
    i2c_master_bus_config_t bus_conf = {
        .scl_io_num = I2C_SCL_GPIO_NUM,
        .sda_io_num = I2C_SDA_GPIO_NUM,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .flags.enable_internal_pullup = true,
    };
    TEST_ESP_OK(i2c_new_master_bus(&bus_conf, &bus_handle));

    i2c_device_config_t dev_conf = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x00,
        .scl_speed_hz = 100000,
    };

    TEST_ESP_OK(i2c_master_bus_add_device(bus_handle, &dev_conf, &dev_handle));
}

static i2c_slave_dev_handle_t init_i2c_slave(uint16_t addr) {
    i2c_slave_config_t i2c_slv_config = {
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .send_buf_depth = 256,
        .scl_io_num = I2C_SCL_GPIO_NUM,
        .sda_io_num = I2C_SDA_GPIO_NUM,
        .slave_addr = addr,
        .flags.broadcast_en = false,
    };

    i2c_slave_dev_handle_t slave_handle;
    TEST_ESP_OK(i2c_new_slave_device(&i2c_slv_config, &slave_handle));
    
    return slave_handle;
}

void init_i2c_ch422g() {
    init_i2c_bus();

    ch422g_dev = init_i2c_slave(I2C_CH422G_ADD);
}