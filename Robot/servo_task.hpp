#ifndef _SERVO_TASK_HPP_
#define _SERVO_TASK_HPP_

#ifdef __cplusplus


extern "C" {
#endif

#include "cmsis_os.h"
#include "struct_typedef.h"
void servo_task_start(void);

extern osThreadId servo_task_handle;

#ifdef __cplusplus
}
#endif



#endif /* _SERVO_TASK_HPP_ */
