/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_usart2.c
 * @brief     : 串口2DMA板级支持包
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-01       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 这里串口2的DMA配置为                                                                    
 * TX:DMA1 Stream6
 *                                                                      
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */
#include "bsp_usart2.h"
#include "main.h"

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_tx;


void usart2_tx_dma_init(void)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart2.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart2.Instance->CR3, USART_CR3_DMAT);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart2_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart2_tx);
    }

    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->PAR = (uint32_t) & (USART2->TDR);
    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->M0AR = (uint32_t)(NULL);
    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->NDTR = 0;


}


void usart2_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart2_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart2_tx);
    }

    __HAL_DMA_CLEAR_FLAG(&hdma_usart2_tx, USART2_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart2_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart2_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart2_tx);
}
