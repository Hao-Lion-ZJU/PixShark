/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : depth_task.cpp
 * @brief     : 解析深度计发送过来的数据,利用DMA传输方式节约CPU
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

#include "depth_task.hpp"
#include "depth.hpp"
#include "bsp_usart.h"
#include "RobotConfig.h"
#include "main.h"

#include "CJsonObject.hpp"
#include "communication_task.hpp"

static osThreadId depth_task_handle;
static osThreadId depth_request_task_handle;

static bool is_depth_task_runing = false;

static KELLER keller(0x01);

//接收原始数据，为9个字节，给了2*9个字节长度，防止DMA传输越界
static uint8_t depth_rx_buf[2][2*KELLER::DEPTH_DATA_LENGTH];



static void depth_task(void const * argument)
{
    Depth* dm = &keller;

    neb::CJsonObject depth_json;
    depth_json.Add("depth", (float)0.0);
    
    //初始化串口
    bsp_usart_init(DEPTH_UART_CHANNEL, depth_rx_buf[0], depth_rx_buf[1], 2*KELLER::DEPTH_DATA_LENGTH);
    UARTDMAStatus_t status = UART_DMA_NO_OK;
    for(;;)
    {
        //等待串口中断
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        {
        }
        //清除Cache缓存
		SCB_InvalidateDCache_by_Addr ((uint32_t *)depth_rx_buf, 2*2*KELLER::DEPTH_DATA_LENGTH);
        //轮询判断是否接收到了定长的数据
        status = bsp_usart_receive_dma(DEPTH_UART_CHANNEL, 0, KELLER::DEPTH_DATA_LENGTH);
        if(status == UART_DMA_BUF0_OK)
        {
            dm->depth_data_solve(depth_rx_buf[0]);
        }
        else if(status == UART_DMA_BUF1_OK)
        {
            dm->depth_data_solve(depth_rx_buf[1]);
        }
        else
        {
            //do nothing
        }

        //上传数据
        depth_json.Replace("depth", dm->get_depth_data());
        std::string data = depth_json.ToFormattedString();
        publish(DEPTH_TOPIC, data.c_str(), data.size());
    }

}


/**
 * @brief          请求深度计数据线程
*/
static void depth_request_task(void const * argument)
{
    //父类指针无法直接指向子类的新成员变量
    Depth* dm = &keller;
    KELLER* kellerPtr = dynamic_cast<KELLER*>(dm);
    //任务开始前延时一段时间,等待接收线程准备就绪
    vTaskDelay(357);
    
    for(;;)
    {
        kellerPtr->request_depth_data();
        osDelay(1000/DEPTH_FREQUNCY);
    }
}


/**
  * @brief  UART6 IDLE irq callback.
  * @param  none
  * @retval none
  */
void depth_irq_callback(void)
{
    //wake up the task
    //唤醒任务

    //调度器还未启动时不发送任务通知
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED && is_depth_task_runing)
    {
        static BaseType_t xHigherPriorityTaskWoken;
        vTaskNotifyGiveFromISR(depth_task_handle, &xHigherPriorityTaskWoken);
        //有优先级高的任务准备就绪，立即进行一次任务切换
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

}


const osThreadDef_t os_thread_def_depth = {
    .name = (char*)"depth",
    .pthread = depth_task,
    .tpriority = osPriorityRealtime,
    .instances = 0,
    .stacksize = 128
};

const osThreadDef_t os_thread_def_depth_request = {
    .name = (char*)"depth_request",
    .pthread = depth_request_task,
    .tpriority = osPriorityNormal,
    .instances = 0,
    .stacksize = 128
};

void depth_task_start(void)
{
    depth_task_handle = osThreadCreate(&os_thread_def_depth, NULL);

    depth_request_task_handle = osThreadCreate(&os_thread_def_depth_request, NULL);

    is_depth_task_runing = true;
}