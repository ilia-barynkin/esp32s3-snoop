#ifndef _CONTROLLER_CAN_H_
#define _CONTROLLER_CAN_H_

#include "main.h"
#include "driver/twai.h"

#define TEST_CAN_LOOPBACK

#define I2C_MASTER_SCL_IO           9       /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           8       /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0       /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

extern QueueHandle_t can_tx_queue;
extern QueueHandle_t can_tx_status_queue;
extern QueueHandle_t can_rx_queue;
 
typedef struct {
    uint32_t message_id;
    bool success;
} can_tx_status_t;

extern twai_general_config_t g_config;
extern twai_timing_config_t t_config;
extern twai_filter_config_t f_config;

void controller_can_task(void *pvParameters);

#endif // _CONTROLLER_CAN_H_