/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : imu_task.cpp
 * @brief     : 解析IMU发送过来的数据,利用DMA传输方式节约CPU
  *             资源，利用串口空闲中断来拉起处理函数，同时提供一些掉线重启DMA，串口
  *             的方式保证热插拔的稳定性。
  * @note       该任务是通过串口中断进行任务通知，比信号量的开销更小
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-09-28       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "imu_task.hpp"
#include "imu.hpp"
#include "bsp_usart.h"
#include "RobotConfig.h"
#include "main.h"
#include "freertos_inc.h"
#include "CJsonObject.hpp"
#include "mqtt_client.hpp"

static osThreadId imu_task_handle;

static LPMS lpms;

static bool is_imu_task_runing = false;

static mqtt::Client* imu_nodePtr;
static std::string imu_data;
//接收原始数据，为200个字节，给了2*200个字节长度，防止DMA传输越界
static uint8_t imu_rx_buf[2][2*LPMS::IMU_DATA_LENGTH];

static void imu_pub(void const *argument)
{
     imu_nodePtr->publish(IMU_TOPIC, imu_data.c_str(), imu_data.size());
}


static void imu_task(void const * argument)
{
    IMU* imu = &lpms;

    //初始化mqtt
    mqtt::Client imu_node = mqtt::Client("imu_node");
    imu_nodePtr = &imu_node;
    imu_node.connect(MQTT_SERVER_IP);
    
    neb::CJsonObject imu_json;
    imu_json.Add("x", (float)0.0);
    imu_json.Add("y", (float)0.0);
    imu_json.Add("z",(float)0.0);
    //初始化串口
    bsp_usart_init(IMU_UART_CHANNEL, imu_rx_buf[0], imu_rx_buf[1], 2*LPMS::IMU_DATA_LENGTH);
    is_imu_task_runing = true;
    UARTDMAStatus_t status = UART_DMA_NO_OK;

    //创建一个定时器，定时发布
    osTimerDef(imu_timer, imu_pub);
    osTimerId imu_timer_handle = osTimerCreate(osTimer(imu_timer), osTimerPeriodic,(void*)0);
    osTimerStart(imu_timer_handle, 100);  //100ms pub once
    for(;;)
    {
        //等待串口中断
        // while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        // {
        // }
        osSemaphoreWait(imu_sem, osWaitForever);
        //清除Cache缓存
		SCB_InvalidateDCache_by_Addr ((uint32_t *)imu_rx_buf, 2*2*LPMS::IMU_DATA_LENGTH);
		//轮询判断是否接收到了定长的数据
        status = bsp_usart_receive_dma(IMU_UART_CHANNEL, 0, LPMS::IMU_DATA_LENGTH);
        if(status == UART_DMA_BUF0_OK)
        {
            imu->imu_data_solve(imu_rx_buf[0]);
        }
        else if(status == UART_DMA_BUF1_OK)
        {
            imu->imu_data_solve(imu_rx_buf[1]);
        }
        else
        {
            //do nothing
        }
        
        //上传数据
        imu_json.Replace("x", imu->get_imu_data().Angle_x);
        imu_json.Replace("y", imu->get_imu_data().Angle_y);
        imu_json.Replace("z", imu->get_imu_data().Angle_z);
        imu_data = imu_json.ToFormattedString();
        // imu_node.publish(IMU_TOPIC, data.c_str(), data.size());
    }
}


/**
  * @brief  UART3 IDLE irq callback.
  * @param  none
  * @retval none
  */
void imu_irq_callback(void)
{
    //wake up the task
        //唤醒任务
    //调度器还未启动时不发送任务通知
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED && is_imu_task_runing)
    {
        // static BaseType_t xHigherPriorityTaskWoken;
        // vTaskNotifyGiveFromISR(imu_task_handle, &xHigherPriorityTaskWoken);
        // //有优先级高的任务准备就绪，立即进行一次任务切换
        // portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        osSemaphoreRelease(imu_sem);
    }
}

const osThreadDef_t os_thread_def_imu = {
    .name = (char*)"imu",
    .pthread = imu_task,
    .tpriority = osPriorityRealtime,
    .instances = 0,
    .stacksize = 256
};

void imu_task_start(void)
{
    imu_task_handle = osThreadCreate(&os_thread_def_imu, NULL);
    static bool is_imu_task_runing = true;
}
