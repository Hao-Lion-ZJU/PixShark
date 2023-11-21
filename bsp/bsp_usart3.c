/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_usart3.c
 * @brief     : 串口3DMA驱动
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-08-01         WPJ        1. <note>
 *  V1.0.0    2023-09-12        Hao Lion    1.modify
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 * 这里串口3的DMA配置为                                                                    
 * TX:DMA1 Stream0
 * RX:DMA1 Stream1                                                                     
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */
#include "bsp_usart3.h"
#include "main.h"

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;


void usart3_init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{

    //enable the DMA transfer for the receiver and tramsmit request
    //使能DMA串口接收和发送
    SET_BIT(huart3.Instance->CR3, USART_CR3_DMAR);
    SET_BIT(huart3.Instance->CR3, USART_CR3_DMAT);

    //enalbe idle interrupt
    //使能串口空闲中断，接收不定长数据
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);  				//清除IDLE标志
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);

    __HAL_UART_CLEAR_NEFLAG(&huart3);
    __HAL_UART_CLEAR_FEFLAG(&huart3);
    __HAL_UART_CLEAR_OREFLAG(&huart3);
    // __HAL_UART_ENABLE_IT(&huart3, UART_IT_ERR);

    //disable DMA
    //失效DMA
    __HAL_DMA_DISABLE(&hdma_usart3_rx);
    
    while(((DMA_Stream_TypeDef   *)hdma_usart3_rx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart3_rx);
    }
    //清除接收中断标志物位
    __HAL_DMA_CLEAR_FLAG(&hdma_usart3_rx, USART3_DMA_RX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart3_rx.Instance)->PAR = (uint32_t) & (USART3->RDR);
    //memory buffer 1
    //内存缓冲区1
    ((DMA_Stream_TypeDef   *)hdma_usart3_rx.Instance)->M0AR = (uint32_t)(rx1_buf);
    //memory buffer 2
    //内存缓冲区2
    ((DMA_Stream_TypeDef   *)hdma_usart3_rx.Instance)->M1AR = (uint32_t)(rx2_buf);
    //data length
    //数据长度
    __HAL_DMA_SET_COUNTER(&hdma_usart3_rx, dma_buf_num);

    //enable double memory buffer
    //使能双缓冲区
    SET_BIT(((DMA_Stream_TypeDef   *)hdma_usart3_rx.Instance)->CR, DMA_SxCR_DBM);

    //enable DMA
    //使能usart3_rx的DMA
    __HAL_DMA_ENABLE(&hdma_usart3_rx);


    //disable DMA
    //失效usart3_tx的DMA
    __HAL_DMA_DISABLE(&hdma_usart3_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart3_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart3_tx);
    }

    ((DMA_Stream_TypeDef   *)hdma_usart3_tx.Instance)->PAR = (uint32_t) & (USART3->TDR);

}


void usart3_tx_dma_enable(uint8_t *data, uint16_t len)
{
    //disable DMA
    //失效usart3_tx的DMA
    __HAL_DMA_DISABLE(&hdma_usart3_tx);

    while(((DMA_Stream_TypeDef   *)hdma_usart3_tx.Instance)->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart3_tx);
    }
    //清除DMA发送中断标志物位
    __HAL_DMA_CLEAR_FLAG(&hdma_usart3_tx, USART3_DMA_TX_ISR);

    ((DMA_Stream_TypeDef   *)hdma_usart3_tx.Instance)->M0AR = (uint32_t)(data);
    __HAL_DMA_SET_COUNTER(&hdma_usart3_tx, len);

    __HAL_DMA_ENABLE(&hdma_usart3_tx);
}


/**
  * @brief This function handles UART4 global interrupt.
  */
void USART3_IRQHandler(void)
{  
	if(USART3->ISR & UART_FLAG_IDLE)
	{
        //清除空闲中断标志后
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);

        //application define callback function
        //应用层定义回调函数
        usart3_receive_irq_callback();
    }
    else if(HAL_UART_GetError(&huart3) & HAL_UART_ERROR_NE){ /*!< Noise error             */
		//噪声错误
		__HAL_UART_CLEAR_NEFLAG(&huart3);
	}else if(HAL_UART_GetError(&huart3) & HAL_UART_ERROR_FE){ /*!< Frame error             */
		//帧格式错误
		__HAL_UART_CLEAR_FEFLAG(&huart3);
	}else if(HAL_UART_GetError(&huart3) & HAL_UART_ERROR_ORE){ /*!< Overrun error           */
		//数据太多串口来不及接收错误
		__HAL_UART_CLEAR_OREFLAG(&huart3);
	}
	
}

/**
  * @brief  UART3 IDLE irq callback.
  * @param  none
  * @retval none
  */
__weak void usart3_receive_irq_callback(void)
{

  /* NOTE : 当串口接收完不定长数据时候会产生空闲中断，这里需要用户自己实现
            空闲中断的回调函数
   */

}