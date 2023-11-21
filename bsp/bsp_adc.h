#ifndef BSP_ADC_H
#define BSP_ADC_H
#include "struct_typedef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern fp32 get_water_level(void);
extern fp32 get_cpu_temprate(void);
extern void bsp_adc_init(void);


#ifdef __cplusplus
}
#endif

#endif
