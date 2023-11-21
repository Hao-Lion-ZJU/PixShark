#ifndef _BSP_USART1_H
#define _BSP_USART1_H

#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

#define USART1_DMA_TX_ISR   DMA_HISR_TCIF7
#define USART1_DMA_RX_ISR   DMA_LISR_TCIF3 

extern void usart1_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void usart1_tx_dma_enable(uint8_t *data, uint16_t len);
void usart1_receive_irq_callback(void);

#ifdef __cplusplus
}
#endif

#endif /* _BSP_USART1_H */
