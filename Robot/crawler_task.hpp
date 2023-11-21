#ifndef _CRAWLER_TASK_HPP_
#define _CRAWLER_TASK_HPP_

#ifdef __cplusplus


extern "C" {
#endif

#include "cmsis_os.h"
#include "struct_typedef.h"
void crawler_task_start(void);
extern osThreadId crawler_task_handle;


#ifdef __cplusplus
}
#endif

#endif /* _CRAWLER_TASK_HPP_ */

