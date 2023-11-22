/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       servo_task.cpp
  * @brief      servo控制任务         
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     OCT-7-2023     HaoLion(郝亮亮)    1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  */

#include "servo_task.hpp"
#include "servo.hpp"
#include "RobotConfig.h"
#include "communication_task.hpp"
osThreadId servo_task_handle;

static AUSServo aus(0x00);

static void servo_task(void const * argument)
{
    Servo* servo = &aus;
    usart2_tx_dma_init();
    const int32_t* servo_dataPtr = NULL;
    
    servo_dataPtr = get_servo_anglePrt();
    for(;;)
    {
        //等待控制指令
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        {
        }
        servo->set_servo_angle(*servo_dataPtr);
    }
    
}


const osThreadDef_t os_thread_def_servo = {
    .name = (char*)"servo",
    .pthread = servo_task,
    .tpriority = osPriorityNormal,
    .instances = 0,
    .stacksize = 128
};

void servo_task_start(void)
{
    servo_task_handle = osThreadCreate(&os_thread_def_servo, NULL);
}