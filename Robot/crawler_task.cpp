/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : crawler_task.cpp
 * @brief     : 履带电机执行任务
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-10-21       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "crawler_task.hpp"
#include "crawler.hpp"
#include <array>
#include "bsp_can.h"
#include "RobotConfig.h"
#include "main.h"

osThreadId crawler_task_handle;

#if CRAWLER_NUM



static GIANT* giants = new GIANT[CRAWLER_NUM]{
    GIANT(Crawler_Left_ID),
    GIANT(Crawler_Right_ID)
};

/**
 * @brief          履带电机执行任务
 * @param[in]      argument: Not used
 * @retval         None
*/
static void crawler_task(void const * argument)
{
    Crawler* crawlersPtr[CRAWLER_NUM];
    for(int i =0 ; i < CRAWLER_NUM; i++)
    {
        crawlersPtr[i] = &giants[i];
    }
    std::array<int16_t, CRAWLER_NUM> crawler_cmd;
    //初始化can
    can_filter_init();
    for(;;)
    {
        //等待控制指令
        while (ulTaskNotifyTake(pdTRUE, portMAX_DELAY) != pdPASS)
        {
        }
        crawler_cmd = robotcomPtr->crawler_cmd_;
        for(int i =0 ; i < CRAWLER_NUM; i++)
        {
            crawlersPtr[i]->set_crawler_expect_speed(crawler_cmd.at(i));
        }
        
    }
}


void crawler_recevice_callback(uint32_t ID, volatile const uint8_t *can_frame, uint8_t dlc)
{
    uint16_t current_feedback = can_frame[0] | can_frame[1] << 8;
    uint16_t speed_feedback = can_frame[2] | can_frame[3] << 8;
    uint32_t position_feedback = can_frame[4] | can_frame[5] << 8 | can_frame[6] << 16 | can_frame[7] << 24;

    if(ID == Crawler_Left_ID)
    {
        giants[0].set_crawler_status(current_feedback, speed_feedback, position_feedback);
        robotcomPtr->crawler_feedback_[0] = giants[0].get_crawler_rpm();
    }
    else if(ID == Crawler_Right_ID)
    {
        giants[1].set_crawler_status(current_feedback, speed_feedback, position_feedback);
        robotcomPtr->crawler_feedback_[1] = giants[1].get_crawler_rpm();
    }
    //上传数据
    upload_data(CRAWLER_ID, robotcomPtr->crawler_feedback_.data(), robotcomPtr->crawler_feedback_.size());
}


const osThreadDef_t os_thread_def_crawler = {
    .name = (char*)"crawler",
    .pthread = crawler_task,
    .tpriority = osPriorityAboveNormal,
    .instances = 0,
    .stacksize = 256
};

void crawler_task_start(void)
{
    crawler_task_handle = osThreadCreate(&os_thread_def_crawler, NULL);
}

#endif