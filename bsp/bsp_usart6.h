#ifndef BSP_USART6_H
#define BSP_USART6_H

#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

#define USART6_DMA_TX_ISR   DMA_HISR_TCIF6 //DMA发送中断完成标志位
#define USART6_DMA_RX_ISR   DMA_LISR_TCIF1 //DMA接收中断完成标志位


extern void usart6_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart6_tx_dma_enable(uint8_t *data, uint16_t len);

void usart6_receive_irq_callback(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_USART6_H */
