/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : imu.cpp
 * @brief     : IMU设备驱动文件
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-12         WPJ        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "imu.hpp"
#include "bsp_usart.h"
#include "DeviceConfig.h"
#include "common.hpp"




/**
 * @brief IMU基类的构造函数，初始化imu_data
 * @retval none
*/
IMU::IMU(){
    memset(&imu_data,0,sizeof(imu_data_t));
}




LPMS::LPMS()
{
  memset(imu_tx_buf, 0, sizeof(imu_tx_buf));
}

/**
 * @brief          复写发送函数
 * @param[in]      p_data:  数据指针
 * @param[in]      len:  数据长度
 * @return  	   none
*/
void LPMS::write(void * data, int len)
{
   bsp_usart_transmit(IMU_UART_CHANNEL, (uint8_t *)data, len);
}

/**
* @brief     发送内容打包
* @param[in] cmd_type:  命令内容ID
* @retval			返回要发送的数据大小
*/
uint16_t LPMS::send_imu_pack(uint8_t cmd_type, uint8_t *p_data, uint16_t len)
{

  memset(imu_tx_buf, 0, sizeof(imu_tx_buf));
  uint16_t send_len = 9 + len;
  imu_tx_buf[0] = IMU_HEAD_SOF;
  imu_tx_buf[1] = IMU_ID & 0xFF;
  imu_tx_buf[2] = IMU_ID >> 8;
  imu_tx_buf[3] = cmd_type & 0xFF;
  imu_tx_buf[4] = cmd_type;
  imu_tx_buf[5] = len & 0xFF;
  imu_tx_buf[6] = len >> 8;
  if(len == 0)
  {
    imu_tx_buf[7] = get_LRC_sum(imu_tx_buf, send_len-4) & 0xFF;
    imu_tx_buf[8] = get_LRC_sum(imu_tx_buf, send_len-4) >> 8;
    send_len = send_len + 2;
  }
  else
  {
    memcpy(&imu_tx_buf[7], p_data, len);
  }
  imu_tx_buf[send_len-4] = get_LRC_sum(imu_tx_buf, send_len-4) & 0xFF;
  imu_tx_buf[send_len-3] = get_LRC_sum(imu_tx_buf, send_len-4) >> 8;
  imu_tx_buf[send_len-2] = IMU_END_SOF & 0xFF;
  imu_tx_buf[send_len-1] = IMU_END_SOF >> 8;
  write(imu_tx_buf, send_len);
  return send_len;
}

/**
 * @brief 进入IMU命令模式
 * @retval none
*/
void LPMS::GOTO_IMU_Command_Mode(void)
{ 
  send_imu_pack(Goto_Command_Mode, NULL, 0);
}

/**
 * @brief 进入IMU数据流发送
 * @retval none
*/
void LPMS::GOTO_IMU_Streaming_Mode(void)
{ 
  send_imu_pack(Goto_Streaming_Mode, NULL, 0);
}

/**
 * @brief IMU参数保存
 * @retval none
*/
void LPMS::GOTO_IMU_Save_Setting(void)
{ 
  send_imu_pack(Save_Setting, NULL, 0);
}



/**
 * @brief          阿鲁比 IMU 协议解析
 * @param[in]      imu_frame: 原生数据指针
 * @return  	   none
*/
void LPMS::imu_data_solve(volatile const uint8_t *imu_frame)
{
  uint8_t* imu_frame_temp = (uint8_t*)imu_frame;
  /*数据校验*/
  if (imu_frame_temp == NULL)
  {
      return;
  }
  //TODO:进行IMU数据解析，校验和不包括包头3A
  if (imu_frame_temp[0] == 0x3A && imu_frame_temp[1] == 0x01 && imu_frame_temp[2] == 0x00 ) //&& LRC_check(imu_frame_temp+1, IMU_DATA_LENGTH)
  {
    this->imu_data.Acc_x = Byte_to_Float(&imu_frame_temp[23]);
    this->imu_data.Acc_y = Byte_to_Float(&imu_frame_temp[27]);
    this->imu_data.Acc_z = Byte_to_Float(&imu_frame_temp[31]);

    this->imu_data.Gyrol_x = Byte_to_Float(&imu_frame_temp[83]);
    this->imu_data.Gyrol_y = Byte_to_Float(&imu_frame_temp[87]);
    this->imu_data.Gyrol_z = Byte_to_Float(&imu_frame_temp[91]);

    this->imu_data.Angle_x = Byte_to_Float(&imu_frame_temp[147]);
    this->imu_data.Angle_y = Byte_to_Float(&imu_frame_temp[151]);
    this->imu_data.Angle_z = Byte_to_Float(&imu_frame_temp[155]);
  }
}




BWIMU::BWIMU()
{
}

/**
 * @brief          复写发送函数
 * @param[in]      p_data:  数据指针
 * @param[in]      len:  数据长度
 * @return  	   none
*/
void BWIMU::write(void * data, int len)
{
   bsp_usart_transmit(IMU_UART_CHANNEL, (uint8_t *)data, len);
}

/**
 * @brief          北微传感 IMU 协议解析。需要根据接收命令字来判断是哪种数据。
 * @param[in]      imu_frame: 原生数据指针
 * @return  	   none
*/
void BWIMU::imu_data_solve(volatile const uint8_t *imu_frame)
{
  uint8_t* imu_frame_temp = (uint8_t*)imu_frame;

  /*数据校验*/
  if (imu_frame_temp == NULL)
  {
      return;
  }
  //TODO:读取三轴角度 PITCH、ROLL、YAW
  if (imu_frame_temp[0] == 0x77 && imu_frame_temp[1] == 0x0D && imu_frame_temp[3]== 0x84 && imu_check(imu_frame_temp))
  {
    this->imu_data.Angle_x = BCD_SXXXYY_to_Float(imu_frame_temp+4);
    this->imu_data.Angle_y = BCD_SXXXYY_to_Float(imu_frame_temp+7);
    this->imu_data.Angle_z = BCD_SXXXYY_to_Float(imu_frame_temp+10);
  }

  //TODO:读取加速度计输出
  else if (imu_frame_temp[0] == 0x77 && imu_frame_temp[1] == 0x0D && imu_frame_temp[3]== 0x54 && imu_check(imu_frame_temp))
  {
    this->imu_data.Acc_x = BCD_SXYYYY_to_Float(imu_frame_temp+4);
    this->imu_data.Acc_y = BCD_SXYYYY_to_Float(imu_frame_temp+7);
    this->imu_data.Acc_z = BCD_SXYYYY_to_Float(imu_frame_temp+10);
  }


  //TODO:读取BCD格式的全部数据
  else if (imu_frame_temp[0] == 0x77 && imu_frame_temp[1] == 0x2F && imu_frame_temp[3]== 0x59 && imu_check(imu_frame_temp))
  {
    this->imu_data.Angle_x = BCD_SXXXYY_to_Float(imu_frame_temp+4);
    this->imu_data.Angle_y = BCD_SXXXYY_to_Float(imu_frame_temp+7);
    this->imu_data.Angle_z = BCD_SXXXYY_to_Float(imu_frame_temp+10);

    this->imu_data.Acc_x = BCD_SXYYYY_to_Float(imu_frame_temp+13);
    this->imu_data.Acc_y = BCD_SXYYYY_to_Float(imu_frame_temp+16);
    this->imu_data.Acc_z = BCD_SXYYYY_to_Float(imu_frame_temp+19);

    this->imu_data.Gyrol_x = BCD_SXXXYY_to_Float(imu_frame_temp+22);
    this->imu_data.Gyrol_y = BCD_SXXXYY_to_Float(imu_frame_temp+25);
    this->imu_data.Gyrol_z = BCD_SXXXYY_to_Float(imu_frame_temp+28);
  }

  //TODO:自动输出模式
  //发送 77 05 00 56 05 60 为【角度、加速度、 陀螺仪、 四元数输出（BCD 码）】
  //发送 77 05 00 56 06 61 为【角度、加速度、 陀螺仪（FLOAT 类型输出）】 建议用这个！！！！
  else if (imu_frame_temp[0] == 0x77 && imu_frame_temp[3]== 0x56 && imu_check(imu_frame_temp))
  {
    this->imu_data.Angle_x = Byte_to_Float(imu_frame_temp+4);
    this->imu_data.Angle_y = Byte_to_Float(imu_frame_temp+8);
    this->imu_data.Angle_z = Byte_to_Float(imu_frame_temp+12);

    this->imu_data.Acc_x = BCD_SXYYYY_to_Float(imu_frame_temp+16);
    this->imu_data.Acc_y = BCD_SXYYYY_to_Float(imu_frame_temp+20);
    this->imu_data.Acc_z = BCD_SXYYYY_to_Float(imu_frame_temp+24);

    this->imu_data.Gyrol_x = BCD_SXXXYY_to_Float(imu_frame_temp+28);
    this->imu_data.Gyrol_y = BCD_SXXXYY_to_Float(imu_frame_temp+32);
    this->imu_data.Gyrol_z = BCD_SXXXYY_to_Float(imu_frame_temp+36);
  }

}

bool_t BWIMU::imu_check(uint8_t *data_message)
{
  uint8_t temp = 0;
  uint8_t len = data_message[1];//北微的数据长度，不包括第一个字节标示符
  data_message = data_message + 1;//北微的校验和不包括第一个字节标示符
  len = len - 1;//先长度减一：最后一位为校验标志
  while (len)
  {
    temp = temp + *data_message;
    data_message++;
    len--;
  }
  temp = temp & 0xff;
  if(*data_message == temp){
    return 1;
  }
  else{return 0;}
}
