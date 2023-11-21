#include "bsp_i2c.h"
#include "main.h"


extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c4;


void bsp_I2C_master_transmit(IICChannel_t iic_channel, uint16_t I2C_address, uint8_t *data, uint16_t len)
{
        switch (iic_channel)
        {
        case CHANNEL_IIC1:
            HAL_I2C_Master_Transmit(&hi2c1, I2C_address, data, len, 100);
            break;
        case CHANNEL_IIC4:
            HAL_I2C_Master_Transmit(&hi2c4, I2C_address, data, len, 100);
            break;
        default:
                break;
        }
                
}

int bsp_I2C_master_receive(IICChannel_t iic_channel, uint16_t I2C_address, uint8_t *data, uint16_t len)
{
    HAL_StatusTypeDef status;
    switch (iic_channel)
    {
    case CHANNEL_IIC1:
        status = HAL_I2C_Master_Receive(&hi2c1, I2C_address, data, len, 1000);
        break;
    case CHANNEL_IIC4:
        status = HAL_I2C_Master_Receive(&hi2c4, I2C_address, data, len, 1000);
        break;
    default:
        status = HAL_ERROR;
        break;
    }
    return status;
}


