#ifndef __FREERTOS_H
#define __FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif

//List of mutexes
extern osMutexId pub_mutex;

extern osSemaphoreId altimeter_sem;
extern osSemaphoreId imu_sem;
extern osSemaphoreId com_sem;
// List of Tasks
/*--------------------------------- System Tasks -------------------------------------*/
extern osThreadId defaultTaskHandle;      // Usage: 1024 Bytes stack

/*---------------------------------- User Tasks --------------------------------------*/


/*---------------- 60K (used) / 64K (for FreeRTOS on ccram) ------------------*/


#ifdef __cplusplus
}
#endif

#endif
