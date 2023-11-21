/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       sth31.cpp
  * @brief      这里是STH31温湿度模块的驱动函数，包括芯片的初始化，配置寄存器
	*							，完成发送和读取等
  * @note       STH31只支持IIC读取
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     NOV-12-2022     HaoLion(郝亮亮)    1. done
  *  V1.0.1     OCT-15-2023     HaoLion(郝亮亮)    1. 修改为C++格式
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  */

#include "sht31.hpp"
#include "bsp_i2c.h"
#include "DeviceConfig.h"

static uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value);

/**
 * @brief          复写底层iic发送
 * @param[in]      data: 数据
 * @param[in]      len: 数据长度
 * @retval         none
*/
void SHT31::write(void * data, int len)
{
    bsp_I2C_master_transmit(SHT31_IIC_CHANNEL, WRITE_ADDR, (uint8_t*)data, len);
}


/**
 * @brief          复写底层iic接收
 * @param[out]     data: 数据
 * @param[in]      len: 数据长度
 * @retval         读取状态，0为成功
*/
int SHT31::read(void * data, int len)
{
    
    return bsp_I2C_master_receive(CHANNEL_IIC4, READ_ADDR, (uint8_t*)data, len);
}

/**
 * @brief          向SHT31发送一条指令(16bit)
 * @param[in]      cmd: 命令 （在SHT31_MODE中枚举定义）
 * @retval         none
*/
void SHT31::send_cmd(SHT31_CMD cmd)
{
    uint8_t cmd_buffer[2];
    cmd_buffer[0] = cmd >> 8;
    cmd_buffer[1] = cmd;
    this->write(cmd_buffer, 2);
}

/**
 * @brief          软件复位
 * @param[in]      none
 * @retval         none
*/
void SHT31::reset()
{
    this->send_cmd(SHT31_CMD::SOFT_RESET_CMD);
}


#define CRC8_POLYNOMIAL 0x31
static uint8_t CheckCrc8(uint8_t* const message, uint8_t initial_value)
{
    uint8_t  remainder;	    //余数
    uint8_t  i = 0, j = 0;  //循环变量

    /* 初始化 */
    remainder = initial_value;

    for(j = 0; j < 2;j++)
    {
        remainder ^= message[j];

        /* 从最高位开始依次计算  */
        for (i = 0; i < 8; i++)
        {
            if (remainder & 0x80)
            {
                remainder = (remainder << 1)^CRC8_POLYNOMIAL;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
    }

    /* 返回计算的CRC码 */
    return remainder;
}

/**
 * @brief          读取数据
 * @param[out]     temperature: 温度
 * @param[out]     humidity: 湿度
 * @retval         none
*/
void SHT31::read_data(float &temperature, float &humidity)
{
    uint8_t dat[6];
	send_cmd(MEDIUM_1_CMD);
	if(read(dat, 6) == 0)//读取成功
    {
        uint16_t recv_temperature = 0;
	    uint16_t recv_humidity = 0;

        /* 校验温度数据和湿度数据是否接收正确 */
        if(CheckCrc8(dat, 0xFF) != dat[2] || CheckCrc8(&dat[3], 0xFF) != dat[5])
            return;
        
        /* 转换温度数据 */
        recv_temperature = ((uint16_t)dat[0]<<8)|dat[1];
        temperature = -45 + 175*((float)recv_temperature/65535);
        
        /* 转换湿度数据 */
        recv_humidity = ((uint16_t)dat[3]<<8)|dat[4];
        humidity = 100 * ((float)recv_humidity / 65535);
    }
}




