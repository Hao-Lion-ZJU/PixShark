#ifndef _THRUSTER_TASK_HPP_
#define _THRUSTER_TASK_HPP_

#ifdef __cplusplus


extern "C" {
#endif

#include "cmsis_os.h"
#include "struct_typedef.h"
void thruster_task_start(void);
extern osThreadId thruster_task_handle;


#ifdef __cplusplus
}
#endif

#endif /* _THRUSTER_TASK_HPP_ */
