#ifndef BSP_USART2_H
#define BSP_USART2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

#define USART2_DMA_TX_ISR   DMA_HISR_TCIF6 


extern void usart2_tx_dma_init(void);
extern void usart2_tx_dma_enable(uint8_t *data, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* BSP_USART2_H */
