/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_adc.c
 * @brief     : DMA方式采集ADC数据
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2022-12-18       Hao Lion        1. <note>
 *  V1.1.0    2023-10-30       Hao Lion        1. 修改为DMA方式读取
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 由于ADC采集外部电压存在波动，首先需要借助内部参考电压VREFINT进行校准
 * 根据《STM32H743数据手册》, VDDA = 3.3V*VREFINT_CAL/VREFINT_DATA
 * VREFINT_CAL为芯片出厂时的内部参考电压值，存放在(0x1FF1E860)
 * VREFINT_DATA为ADC采集到的内部参考电压值                                                                  
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */


#include "bsp_adc.h"
#include "main.h"

#define ADC_VDDA               3.3f
#define FULL_SCALE             65535U
extern ADC_HandleTypeDef hadc3;

volatile uint16_t VREFINT_CAL;

uint32_t adc_value[3];  //DMA存放转换数值的地方，3个通道按顺序为ch1、温度传感器、内部参考电压

static fp32 get_real_vdda(void);

/**
	* @brief          初始化AD采样
  * @param[in]      none 
  * @retval         none
  */
void bsp_adc_init(void)
{
		
		HAL_ADCEx_Calibration_Start(&hadc3,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc3, adc_value, 3);
    VREFINT_CAL = *(__IO uint16_t *)(0x1FF1E860);

}


fp32 get_cpu_temprate(void)
{
    fp32 temperate;
		uint16_t TS_CAL1;
		uint16_t TS_CAL2;
    TS_CAL1 = *(__IO uint16_t *)(0x1FF1E820);
		TS_CAL2 = *(__IO uint16_t *)(0x1FF1E840);
		temperate = ((110.0f - 30.0f) / (TS_CAL2 - TS_CAL1)) * (adc_value[1] - TS_CAL1) + 30.0f;

    return temperate;
}

/**
 * @brief  获取实际的VDDA电压值
 *         VDDA = 3.3V*VREFINT_CAL/VREFINT_DATA
 * @param  none
 * @retval none
*/
static fp32 get_real_vdda(void)
{
    fp32 voltage_vdda_real = 0;
    voltage_vdda_real = ADC_VDDA*VREFINT_CAL/(adc_value[2]);
    return voltage_vdda_real;
}

fp32 get_water_level(void)
{
    fp32 voltage;
		fp32 voltage_vdda_real = 0;
    voltage_vdda_real = get_real_vdda();
		//分压电路所以实际电压值应该×2
    voltage = 2 * (fp32)adc_value[0] /FULL_SCALE * voltage_vdda_real;
    return voltage;
}
