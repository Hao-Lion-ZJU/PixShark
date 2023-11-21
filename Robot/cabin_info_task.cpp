/**
  ****************************(C) COPYRIGHT 2022 ZJU****************************
  * @file       sht31_task.cpp
  * @brief      SHT31温湿度传感器数据读取任务         
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

#include "cabin_info_task.hpp"
#include "sht31.hpp"
#include "bsp_adc.h"
#include "RobotConfig.h"
#include "CJsonObject.hpp"
#include "communication_task.hpp"
// #include "mqtt_client.hpp"

static osThreadId sht31_task_handle;

static SHT31 sht31;

static void cabin_info_task(void const * argument)
{
    cabin_status_t cabin_status;
    sht31.reset();
    bsp_adc_init();
    vTaskDelay(100);
    neb::CJsonObject cabin_info_json;
    cabin_info_json.Add("cabin_temperature", (float)0.0);
    cabin_info_json.Add("cabin_humidity", (float)0.0);
    cabin_info_json.Add("cpu_temperature",(float)0.0);
    cabin_info_json.Add("cabin_water_level", (float)0.0);
    std::string data;

    for(;;)
    {
        sht31.read_data(cabin_status.cabin_temperature, cabin_status.cabin_humidity);
        cabin_status.cpu_temperature = get_cpu_temprate();
        cabin_status.cabin_water_level = get_water_level();

        cabin_info_json.Replace("cabin_temperature", cabin_status.cabin_temperature);
        cabin_info_json.Replace("cabin_humidity", cabin_status.cabin_humidity);
        cabin_info_json.Replace("cpu_temperature", cabin_status.cpu_temperature);
        cabin_info_json.Replace("cabin_water_level", cabin_status.cabin_water_level);
        data = cabin_info_json.ToFormattedString();
        publish(CABIN_INFO_TOPIC, data.c_str(), data.size());

        osDelay(1000);
    }
}


const osThreadDef_t os_thread_def_cabin_info = {
    .name = (char*)"cabin_info",
    .pthread = cabin_info_task,
    .tpriority = osPriorityNormal,
    .instances = 0,
    .stacksize = 128
};

void cabin_info_task_start(void)
{
    sht31_task_handle = osThreadCreate(&os_thread_def_cabin_info, NULL);
}