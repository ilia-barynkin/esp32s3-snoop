#ifndef CH422G_PORT_H
#define CH422G_PORT_H

#include "i2c_port.h"
#include "esp_log.h"

static const char * TAG = "ch422g_port";

#define CH422_PORT_I2C_PORT I2C_NUM_0

// from what i understand, each register can be accessed by writing "1" to 0x24 address, and then registers are exposed to common i2c bus 
// address space, and each reg address is "output byte 1" devided by 2
// so, if i want to "set system parameter" of ch422g, i need to kick "1" to 0x24 address and then write some value to (0x48 >> 1), i.e. 36
// terminology taken from ch422g datasheet, don't judge me :)

#define CH422_PORT_DEV_ADDR 0x24

// command bytes ("output byte 1")
#define CH422_PORT_SET_SYSTEM_PARAMETER_CMD             (0x48)
#define CH422_PORT_SET_GENERAL_IO_CMD                   (0x46)
#define CH422_PORT_SET_BIDIRECTIONAL_IO_CMD             (0x70)
#define CH422_PORT_READ_BIDIRECTIONAL_IO_CMD            (0x4D)

// register addresses mapped by commands
#define CH422_PORT_SET_SYSTEM_PARAMETER_CMD_ADDR                      (CH422_PORT_SET_SYSTEM_PARAMETER_CMD >> 1)
#define CH422_POER_SET_GENERAL_IO_CMD_ADDR                            (CH422_PORT_SET_GENERAL_IO_CMD >> 1)
#define CH422_PORT_REG_SET_BIDIRECTIONAL_IO_CMD_ADDR                  (CH422_PORT_SET_BIDIRECTIONAL_IO_CMD >> 1)
#define CH422_PORT_REG_READ_BIDIRECTIONAL_IO_CMD_ADDR                 (CH422_PORT_READ_BIDIRECTIONAL_IO_CMD >> 1)

#endif