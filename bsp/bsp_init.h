/**
  ****************************(C) COPYRIGHT 2023 ZJU****************************
  * @file       bsp_init.c/h
  * @brief      初始化板载的各种外设，因为有些外设可能会在任务创建时候初始化，所以
  *             这里主要初始化一些中断任务的外设，比如串口、CAN等
  *             
  * @note       bsp_init()应该放在HAL库初始化代码之后
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     MAR-21-2023     HaoLion(郝亮亮)    1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2023 ZJU****************************
  */

#ifndef BSP_INIT_H
#define BSP_INIT_H
#include "struct_typedef.h"


/**
	* @brief          初始化中断任务的所有外设
	* @param[in]      none
  * @retval         none
  */
	
extern void bsp_init(void);


#endif
