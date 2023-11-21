#ifndef BSP_USART3_H
#define BSP_USART3_H

#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

#define USART3_DMA_TX_ISR   DMA_LISR_TCIF0 //DMA发送中断完成标志位
#define USART3_DMA_RX_ISR   DMA_LISR_TCIF2 //DMA接收中断完成标志位


extern void usart3_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart3_tx_dma_enable(uint8_t *data, uint16_t len);
void usart3_receive_irq_callback(void);

#ifdef __cplusplus
}
#endif

#endif /* BSP_USART3_H */
