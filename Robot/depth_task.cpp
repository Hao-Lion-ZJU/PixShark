/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : depth_task.cpp
 * @brief     : ������ȼƷ��͹���������,����DMA���䷽ʽ��ԼCPU
  *             ��Դ�����ô��ڿ����ж�������������ͬʱ�ṩһЩ��������DMA������
  *             �ķ�ʽ��֤�Ȳ�ε��ȶ��ԡ�
  * @note       ��������ͨ�������жϽ�������֪ͨ�����ź����Ŀ�����С
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

//����ԭʼ���ݣ�Ϊ9���ֽڣ�����2*9���ֽڳ��ȣ���ֹDMA����Խ��
static uint8_t depth_rx_buf[2][2*KELLER::DEPTH_DATA_LENGTH];



static void depth_task(void const * argument)
{
    Depth* dm = &keller;

    neb::CJsonObject depth_json;
    depth_json.Add("depth", (float)0.0);
    
    //��ʼ������
    bsp_usart_init(DEPTH_UART_CHANNEL, depth_rx_buf[0], depth_rx_buf[1], 2*KELLER::DEPTH_DATA_LENGTH);
    UARTDMAStatus_t status = UART_DMA_NO_OK;
    for(;;)
    {
        //�ȴ������ж�
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        {
        }
        //���Cache����
		SCB_InvalidateDCache_by_Addr ((uint32_t *)depth_rx_buf, 2*2*KELLER::DEPTH_DATA_LENGTH);
        //��ѯ�ж��Ƿ���յ��˶���������
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

        //�ϴ�����
        depth_json.Replace("depth", dm->get_depth_data());
        std::string data = depth_json.ToFormattedString();
        publish(DEPTH_TOPIC, data.c_str(), data.size());
    }

}


/**
 * @brief          ������ȼ������߳�
*/
static void depth_request_task(void const * argument)
{
    //����ָ���޷�ֱ��ָ��������³�Ա����
    Depth* dm = &keller;
    KELLER* kellerPtr = dynamic_cast<KELLER*>(dm);
    //����ʼǰ��ʱһ��ʱ��,�ȴ������߳�׼������
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
    //��������

    //��������δ����ʱ����������֪ͨ
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED && is_depth_task_runing)
    {
        static BaseType_t xHigherPriorityTaskWoken;
        vTaskNotifyGiveFromISR(depth_task_handle, &xHigherPriorityTaskWoken);
        //�����ȼ��ߵ�����׼����������������һ�������л�
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