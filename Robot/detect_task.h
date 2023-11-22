/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       detect_task.c/h
  * @brief      detect error task, judged by receiving data time. provide detect
                hook function, error exist function.
  *             ���������� ͨ����������ʱ�����ж�.�ṩ ��⹳�Ӻ���,������ں���.        
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     MAR-3-2023     HaoLion(������)    1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  */
	
	
	
#ifndef DETECT_TASK_H
#define DETECT_TASK_H
#include "struct_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif


//设备目录
enum errorList
{
    TCP_TOE = 0,
    THRUSTER1_TOE,
    THRUSTER2_TOE,
    THRUSTER3_TOE,
    THRUSTER4_TOE,
	  THRUSTER5_TOE,
	  THRUSTER6_TOE,
    TEM_HUM_TOE,
    IMU_TOE,
    OLED_TOE,
    ERROR_LIST_LENGHT,
};

typedef  struct
{
    uint32_t new_time;
    uint32_t last_time;
    uint32_t lost_time;
    uint32_t work_time;
    uint16_t set_offline_time : 12;
    uint16_t set_online_time : 12;
    uint8_t enable : 1;
    uint8_t priority : 4;
    uint8_t error_exist : 1;
    uint8_t is_lost : 1;
    uint8_t data_is_error : 1;

    fp32 frequency;
    bool_t (*data_is_error_fun)(void);
    void (*solve_lost_fun)(void);
    void (*solve_data_error_fun)(void);
} dev_error_t;


extern void detetc_task_start(void);



#ifdef __cplusplus
}
#endif


#endif
