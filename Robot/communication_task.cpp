/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : communication_task.cpp
 * @brief     : 接收上位机的控制指令，包括推进器，舵机，灯光等
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-11-21       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */
#include "CJsonObject.hpp"
#include "communication_task.hpp"
#include "mqtt_client.hpp"
#include "RobotConfig.h"
#include "freertos_inc.h"

static osThreadId commuication_task_handle;
static std::string recv_topic;
static std::string pay_load;
static mqtt::Client* recv_nodePtr = NULL;



static uint32 brightness;
static int32 servo_angle;
static int32 thruster_cmd[6] = {0};


using namespace std;

static void check_connection(void const *argument)
{
    if(!recv_nodePtr->isConnected())
    {
        recv_nodePtr->connect(MQTT_SERVER_IP);
    }
}

static void on_message(void *this_client, const char *topic, u32_t tot_len)
{
    recv_topic = std::string(topic);
}

static void on_data(void *this_client, const u8_t *data, u16_t len, u8_t flags)
{
    
    pay_load = std::string((const char*)data, len);
    
    xTaskNotifyGive(commuication_task_handle);
}

void communication_task(const void *arg)
{
    //初始化mqtt
    recv_nodePtr = new mqtt::Client("recv_node");
    recv_nodePtr->on_message_ = on_message;
    recv_nodePtr->on_data_ = on_data;

    recv_nodePtr->connect(MQTT_SERVER_IP);

    recv_nodePtr->subscribe(LED_TOPIC);
    // recv_nodePtr->subscribe("/crawler");
    recv_nodePtr->subscribe(SERVO_TOPIC);
    recv_nodePtr->subscribe(THRUSTER_TOPIC);

    //创建一个定时器，定时检测连接状态了
    osTimerDef(check_timer, check_connection);
    osTimerId check_timer_handle = osTimerCreate(osTimer(check_timer), osTimerPeriodic,(void*)0);
    osTimerStart(check_timer_handle, 5000);  //5s检查一次

    for(;;)
    {
        //等待控制指令
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)//
        {
        }
        neb::CJsonObject result(pay_load);
        
        if(recv_topic.compare(LED_TOPIC) == 0){
            result.Get("brightness", brightness);
            xTaskNotifyGive(led_task_handle);
        }
            
        else if(recv_topic.compare(SERVO_TOPIC) == 0){
            result.Get("servo_angle", servo_angle);
            xTaskNotifyGive(servo_task_handle);
        }
            
        else if(recv_topic.compare(THRUSTER_TOPIC) == 0)
        {
            result.Get("vl", thruster_cmd[0]);
            result.Get("vr", thruster_cmd[1]);
            result.Get("htl", thruster_cmd[2]);
            result.Get("htr", thruster_cmd[3]);
            result.Get("hbr", thruster_cmd[4]);
            result.Get("hbl", thruster_cmd[5]);
        }

        
    }
}


const uint32_t* get_brightnessPrt() 
{
    return &brightness;

}

const int32_t* get_servo_anglePrt() 
{
    return &servo_angle;
}

const int32_t* get_thruster_cmdPrt()
{
    return thruster_cmd;
}

const osThreadDef_t os_thread_def_commuication = {
    .name = (char*)"commuication",
    .pthread = communication_task,
    .tpriority = osPriorityAboveNormal,
    .instances = 0,
    .stacksize = 512
};

void commuication_task_start(void)
{
    commuication_task_handle = osThreadCreate(&os_thread_def_commuication, NULL);
    
}