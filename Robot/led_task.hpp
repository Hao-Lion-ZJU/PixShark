#ifndef _LED_TASK_HPP_
#define _LED_TASK_HPP_

#ifdef __cplusplus


extern "C" {
#endif

#include "cmsis_os.h"
#include "struct_typedef.h"
void led_task_start(void);

extern osThreadId led_task_handle;

#ifdef __cplusplus
}
#endif



#endif /* _LED_TASK_HPP_ */
