/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : altimeter.cpp
 * @brief     : 高度计设备驱动文件
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-14         WPJ        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 高度计数据读取通过上位机设置为定时发送
 * 指令ID122
 * 返回数据帧格式为：ddd.ddd\r\n (直接是字符串形式ASCII码)                                        
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "altimeter.hpp"
#include "bsp_usart.h"
#include "DeviceConfig.h"

/**
 * @brief Altimeter基类的构造函数，初始化Altimeter_data
 * @retval none
*/
Altimeter::Altimeter(){
    memset(&altimeter_data,0,sizeof(altimeter_data));
}


/**
 * @brief ISA500构造函数
*/
ISA500::ISA500()
{
  
}

/**
 * @brief 复写发送函数
 * @param[in] p_data:  数据指针
 * @param[in] len:  数据长度
 * @return  	   none
*/
void ISA500::write(void *data, int len){
   bsp_usart_transmit(ALTIMETER_UART_CHANNEL, (uint8_t *)data, len);
}






/**
  * @brief          高度计协议解析
  * @param[in]      am_frame: 原生数据指针
  * @param[in]     am_data: 高度计数据指针
  * @retval         none
  */
void ISA500::altimeter_data_solve(volatile const uint8_t *am_frame)
{
	/*数据校验*/
    if (am_frame == NULL)
    {
        return;
    }
    //TODO:进行高度计数据解析
    this->altimeter_data = atof((char *)am_frame);

}