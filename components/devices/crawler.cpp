#include"crawler.hpp"
#include "bsp_can.h"
#include "DeviceConfig.h"
#include <assert.h>

GIANT::GIANT(int _id)
{
    this->CAN_ID = _id;
}

void GIANT::write(void *data, int len)
{
    assert(len <= 8);
    bsp_can_transmit(this->CAN_ID, (uint8_t *)data, len);
}


void GIANT::set_crawler_expect_speed(int16_t speed_set)
{
    int32_t speed_temp = (int32_t)speed_set;
    uint8_t speed_buf[5] = {0};
    uint16_t send_len = 5;
    speed_buf[0] = SPEED_MODE;

    memcpy(&speed_buf[1],&speed_temp,4);

    // speed_buf[1] = speed_temp & 0x000F; //低位在前 16 进制
    // speed_buf[2] = (speed_temp & 0x00F0)>>8; 
    // speed_buf[3] = (speed_temp & 0x0F00)>>16; 
    // speed_buf[4] = (speed_temp & 0x0F00)>>24; 

    this->write(speed_buf,send_len);

}

void GIANT::request_motor_data()
{
    uint8_t cmd_buf[1];
    cmd_buf[0] = GET_CURRENT_SPEED_POSITION;
    this->write(cmd_buf,1);
}
