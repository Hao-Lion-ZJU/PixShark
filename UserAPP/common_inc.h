#ifndef _COMMON_INC_H_
#define _COMMON_INC_H_

/*---------------------------- C Scope ---------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "freertos_inc.h"

#include "RobotConfig.h"

#include "detect_task.h"


void Main(void);


#ifdef __cplusplus
}

/*---------------------------- C++ Scope ---------------------------*/
#include "CJsonObject.hpp"
#include "common.hpp"
#include "communication_task.hpp"
#include "cabin_info_task.hpp"
#include "imu_task.hpp"
#include "led_task.hpp"
#include "servo_task.hpp"
#include "altimeter_task.hpp"
#include "depth_task.hpp"
#include "thruster_task.hpp"

#endif
#endif //_COMMON_INC_H_
