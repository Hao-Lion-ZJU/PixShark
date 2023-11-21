/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       led_task.cpp
  * @brief      led控制任务         
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

#include "led_task.hpp"
#include "led.hpp"
#include "RobotConfig.h"
#include "communication_task.hpp"


osThreadId led_task_handle;

static Barlus barlus;


static void led_task(void const * argument)
{
    Barlus* led = &barlus;
    const uint32_t* led_brightnessPtr = NULL;
    led_brightnessPtr = get_brightnessPrt();
    usart2_tx_dma_init();
    led->off();
    osDelay(1000);
    led->enhance();
    osDelay(1000);
    led->off();
    uint8_t current_brightness_level = 0;  //当前barlus灯光亮度档位
    for(;;)
    {
        //等待控制指令
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)//
        {
        }
        uint8_t brightness_level = *led_brightnessPtr/255*10;
        while (current_brightness_level != brightness_level)
        {
            if(current_brightness_level < brightness_level)
                led->enhance();
            if(current_brightness_level > brightness_level)
                led->weaken();
            osDelay(1000);
        }
    }
    

}


const osThreadDef_t os_thread_def_led = {
    .name = (char*)"led",
    .pthread = led_task,
    .tpriority = osPriorityNormal,
    .instances = 0,
    .stacksize = 128
};

void led_task_start(void)
{
    led_task_handle = osThreadCreate(&os_thread_def_led, NULL);
}