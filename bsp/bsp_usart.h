#ifndef _BSP_USART_H_
#define _BSP_USART_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_usart3.h"
#include "bsp_uart4.h"
#include "bsp_usart6.h"
#include "struct_typedef.h"

typedef  enum
{
    CHANNEL_USART1 = 0,
    CHANNEL_USART2,
    CHANNEL_USART3,
    CHANNEL_UART4,
    CHANNEL_USART6
}UARTChannel_t;

typedef  enum
{
    UART_DMA_NO_OK = 0,
    UART_DMA_BUF0_OK,
    UART_DMA_BUF1_OK
}UARTDMAStatus_t;


void bsp_usart_transmit(UARTChannel_t uart_channel,uint8_t *data, uint16_t len);
void bsp_usart_init(UARTChannel_t uart_channel,uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
UARTDMAStatus_t bsp_usart_receive_dma(UARTChannel_t uart_channel, bool_t receive_style , uint16_t dma_buf_num);




#ifdef __cplusplus
}
#endif

#endif /* _BSP_USART_H_ */
