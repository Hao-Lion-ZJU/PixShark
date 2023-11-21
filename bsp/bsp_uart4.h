#ifndef _BSP_UART4_H_
#define _BSP_UART4_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "struct_typedef.h"

#define UART4_DMA_RX_ISR   DMA_HISR_TCIF7
#define UART4_DMA_TX_ISR   DMA_LISR_TCIF0

extern void uart4_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void uart4_tx_dma_enable(uint8_t *data, uint16_t len);

void uart4_receive_irq_callback(void);

#ifdef __cplusplus
}
#endif

#endif /* _BSP_UART4_H_ */
