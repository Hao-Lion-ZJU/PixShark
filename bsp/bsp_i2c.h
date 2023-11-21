#ifndef BSP_I2C_H
#define BSP_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

typedef  enum
{
    CHANNEL_IIC1 = 0,
    CHANNEL_IIC4,
}IICChannel_t;






extern void bsp_I2C_master_transmit(IICChannel_t iic_channel, uint16_t I2C_address, uint8_t *data, uint16_t len);
extern int bsp_I2C_master_receive(IICChannel_t iic_channel, uint16_t I2C_address, uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
