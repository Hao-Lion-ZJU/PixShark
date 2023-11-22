/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : thruster_task.cpp
 * @brief     : 推进器执行任务
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

#include "thruster_task.hpp"
#include "thruster.hpp"
#include "communication_task.hpp"
#include "bsp_can.h"
#include "RobotConfig.h"
#include "main.h"

osThreadId thruster_task_handle;

/**根据水下机器人类型修改*/
static VESC* vescs = new VESC[THRUSTER_NUM]{
    VESC(Thruster_Vertical_Left_ID),
    VESC(Thruster_Vertical_Right_ID),
    VESC(Thruster_Horizontal_TopLeft_ID),
    VESC(Thruster_Horizontal_TopRight_ID),
    VESC(Thruster_Horizontal_BottomRight_ID),
    VESC(Thruster_Horizontal_BottomLeft_ID)
};


static void thruster_task(void const * argument)
{
    Thruster* thrustersPtr[THRUSTER_NUM];
    for(int i =0 ; i < THRUSTER_NUM; i++)
    {
        thrustersPtr[i] = &vescs[i];
    }
    const int32_t* thruster_cmd = get_thruster_cmdPrt();
    //初始化can
    can_filter_init();
    for(;;)
    {
        for(int i =0 ; i < THRUSTER_NUM; i++)
        {
            osDelay(5);
            thrustersPtr[i]->set_thruster_expect_speed(thruster_cmd[i]);//;
        }

    }
}



// void thruster_recevice_callback(uint32_t ID, volatile const uint8_t *can_frame, uint8_t dlc)
// {
//     uint8_t cmd_type;
//     uint8_t thruster_id;
//     uint8_t rx_data[8];
//     memcpy(rx_data, (const uint8_t*)can_frame, dlc);
//     /*提取命令类型*/
//     cmd_type = ID >> 8;
//     /*提取推进器编号*/
//     thruster_id = ID & 0xff;
//     /*推进器id到数组索引的映射*/
//     uint8_t thruster_index = thruster_id & 0x0f;
//     if(thruster_index > THRUSTER_NUM)
//         return;
//     if(cmd_type == VESC::CAN_PACKET_STATUS)
//     {
//         int32_t rpm_feedback = (int32_t)rx_data[0] << 24 | (int32_t)rx_data[1] << 16 | (int32_t)rx_data[2] << 8 | (int32_t)rx_data[3];
//         int16_t current_feedback = (int16_t)rx_data[4] << 8 | (int16_t)rx_data[5];
//         int16_t duty_feedback = (int16_t)rx_data[6] << 8 | (int16_t)rx_data[7];
//         vescs[thruster_index].set_thruster_status(rpm_feedback, current_feedback, duty_feedback);
//     }

//     //上传数据
//     robotcomPtr->thruster_feedback_[thruster_index] = vescs[thruster_index].get_thruster_rpm();
//     upload_data(THRUSTER_ID, vescs[thruster_index].get_thruster_rpm());
		

// }

const osThreadDef_t os_thread_def_thruster = {
    .name = (char*)"thruster",
    .pthread = thruster_task,
    .tpriority = osPriorityAboveNormal,
    .instances = 0,
    .stacksize = 256
};

void thruster_task_start(void)
{
    thruster_task_handle = osThreadCreate(&os_thread_def_thruster, NULL);
}