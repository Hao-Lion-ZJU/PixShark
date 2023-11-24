/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : altimeter_task.cpp
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


#include "altimeter_task.hpp"
#include "altimeter.hpp"
#include "bsp_usart.h"
#include "RobotConfig.h"
#include "main.h"

#include "CJsonObject.hpp"
#include "mqtt_client.hpp"

static osThreadId altimeter_task_handle;

static ISA500 isa500;

static bool is_altimeter_task_runing = false;

//接收原始数据，为9个字节，给了2*9个字节长度，防止DMA传输越界
uint8_t altimeter_rx_buf[2][2*ISA500::AM_DATA_LENGTH];

static void altimeter_task(void const * argument)
{
    Altimeter* altimeter = &isa500;

    //初始化mqtt
    mqtt::Client altimeter_node = mqtt::Client("altimeter_node");
    altimeter_node.connect(MQTT_SERVER_IP);

    neb::CJsonObject alitimeter_json;
    alitimeter_json.Add("height", (float)0.0);

    //初始化串口
    bsp_usart_init(ALTIMETER_UART_CHANNEL, altimeter_rx_buf[0], altimeter_rx_buf[1], 2*ISA500::AM_DATA_LENGTH);
    UARTDMAStatus_t status = UART_DMA_NO_OK;
    for(;;)
    {
        //等待串口中断
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        {
        }
        //清除Cache缓存
		SCB_InvalidateDCache_by_Addr ((uint32_t *)altimeter_rx_buf, 2*2*ISA500::AM_DATA_LENGTH);
        //轮询判断是否接收到了定长的数据
        status = bsp_usart_receive_dma(ALTIMETER_UART_CHANNEL, 0, ISA500::AM_DATA_LENGTH);
        if(status == UART_DMA_BUF0_OK)
        {
            altimeter->altimeter_data_solve(altimeter_rx_buf[0]);
        }
        else if(status == UART_DMA_BUF1_OK)
        {
            altimeter->altimeter_data_solve(altimeter_rx_buf[1]);
        }
        else
        {
            //do nothing
        }
        //上传数据
        alitimeter_json.Replace("height", altimeter->get_altimeter_data());
        std::string data = alitimeter_json.ToFormattedString();
        altimeter_node.publish(ALTIMETER_TOPIC, data.c_str(), data.size());
        
    }
}


/**
  * @brief  UART4 IDLE irq callback.
  * @param  none
  * @retval none
  */
void altimeter_irq_callback(void)
{
    //wake up the task
        //唤醒任务

    //调度器还未启动时不发送任务通知
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED && is_altimeter_task_runing)
    {
        static BaseType_t xHigherPriorityTaskWoken;
        vTaskNotifyGiveFromISR(altimeter_task_handle, &xHigherPriorityTaskWoken);
        //有优先级高的任务准备就绪，立即进行一次任务切换
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/**
 * @brief  获取高度计对象指针
*/
Altimeter* get_altimeter_point(void)
{
    return &isa500;
}


const osThreadDef_t os_thread_def_altimeter = {
    .name = (char*)"altimeter",
    .pthread = altimeter_task,
    .tpriority = osPriorityHigh,
    .instances = 0,
    .stacksize = 128
};

void altimeter_task_start(void)
{
    altimeter_task_handle = osThreadCreate(&os_thread_def_altimeter, NULL);
    is_altimeter_task_runing = true;
}