/**
  ****************************(C) COPYRIGHT 2023 ZJU****************************
  * @file       detect_task.c/h
  * @brief      detect error task, judged by receiving data time. provide detect
                hook function, error exist function.
  *             检测错误任务， 通过接收数据时间来判断.提供 检测钩子函数,错误存在函数.        
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     MAR-3-2023     HaoLion(郝亮亮)    1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2023 ZJU****************************
  */
	
#include "detect_task.h"
#include "cmsis_os.h"
#include "main.h"
#include "iwdg.h"

extern IWDG_HandleTypeDef hiwdg1;
static osThreadId detect_task_handle;

static void feed_dog(void const *argument)
{
  //喂狗
  HAL_IWDG_Refresh(&hiwdg1);
}

/**
  * @brief          get toe error status
  * @param[in]      toe: table of equipment
  * @retval         true (eror) or false (no error)
  */
/**
  * @brief          获取设备对应的错误状态
  * @param[in]      toe:设备目录
  * @retval         true(错误) 或者false(没错误)
  */
bool_t toe_is_error(uint8_t toe)
{
    return (0);
}

/**
  * @brief          检测任务
  * @param[in]      pvParameters: NULL
  * @retval         none
  */
static void detect_task(void const *pvParameters)
{
    static uint32_t system_time;
    system_time = xTaskGetTickCount();
    //创建一个定时器，定时喂狗
    osTimerDef(IWDG, feed_dog);
    osTimerId IWDG_handle = osTimerCreate(osTimer(IWDG), osTimerPeriodic,(void*)0);
    feed_dog(NULL);//先喂一次狗
    osTimerStart(IWDG_handle, 1000);  //
    while (1)
    {
      osDelay(10000);
    }
}


const osThreadDef_t os_thread_def_detetc = {
    .name = (char*)"detetc",
    .pthread = detect_task,
    .tpriority = osPriorityNormal,
    .instances = 0,
    .stacksize = 128
};

void detetc_task_start(void)
{
    detect_task_handle = osThreadCreate(&os_thread_def_detetc, NULL);
    
}