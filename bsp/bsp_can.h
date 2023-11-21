#ifndef BSP_CAN_H
#define BSP_CAN_H
#include "struct_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void can_filter_init(void);
extern void bsp_can_transmit(uint32_t ID, uint8_t *pData, uint8_t len);


void thruster_recevice_callback(uint32_t ID, volatile const uint8_t *can_frame, uint8_t dlc);
void crawler_recevice_callback(uint32_t ID, volatile const uint8_t *can_frame, uint8_t dlc);

#ifdef __cplusplus
}
#endif

#endif
