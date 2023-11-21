#ifndef _DEVICECONFIG_H_
#define _DEVICECONFIG_H_

#include "bsp_usart.h"
#include "bsp_i2c.h"

/**add more peripheral API*/


/******include end********/

/**更改设备与硬件的映射关系*/
/*device communication method mapping**/
/**IMU Class*/
#define IMU_UART_CHANNEL               CHANNEL_USART3     
/**irq function mapping*/
#define imu_irq_callback            usart3_receive_irq_callback
/**Altimeter Class*/
#define ALTIMETER_UART_CHANNEL             CHANNEL_UART4
/**irq function mapping*/
#define altimeter_irq_callback      uart4_receive_irq_callback     
/**GPS Class*/

/**Depth meter Class*/
#define DEPTH_UART_CHANNEL             CHANNEL_USART6
/**irq function mapping*/
#define depth_irq_callback           usart6_receive_irq_callback
/**LED Class*/
#define LED_UART_CHANNEL             CHANNEL_USART2
/**Servo Motor Class*/
#define SERVO_UART_CHANNEL           CHANNEL_USART2
/**temperature and humidty sensor*/
#define SHT31_IIC_CHANNEL            CHANNEL_IIC4
/**Motor Class*/



#endif /* _DEVICECONFIG_H_ */
