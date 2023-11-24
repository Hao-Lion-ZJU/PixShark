#ifndef COMMUNICATION_TASK_HPP
#define COMMUNICATION_TASK_HPP

#ifdef __cplusplus


extern "C" {
#endif

#include "cmsis_os.h"
#include "struct_typedef.h"
void commuication_task_start(void);

const uint32_t* get_brightnessPrt();
const int32_t* get_servo_anglePrt();
const int32_t* get_thruster_cmdPrt();

#ifdef __cplusplus
}
#endif


#include "servo_task.hpp"
#include "led_task.hpp"
#include "thruster_task.hpp"
#include <string>


#endif /* COMMUNICATION_TASK_HPP */
