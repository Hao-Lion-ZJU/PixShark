/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : bsp_usart.c
 * @brief     : 所有的串口驱动进行进一步抽象，为所有的串口、232、485设备提供统一的抽象接口
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-10-12       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */

#include "usart.h"
#include "bsp_usart.h"

extern UART_HandleTypeDef huart2;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_usart6_rx;





/**
 * @brief 串口初始化
*/
void bsp_usart_init(UARTChannel_t uart_channel,uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
    switch (uart_channel)
    {
    case CHANNEL_USART1:
        usart1_init(rx1_buf, rx2_buf, dma_buf_num);
        break;
    case CHANNEL_USART2:
        usart2_tx_dma_init();
        break;
    case CHANNEL_USART3:
        usart3_init(rx1_buf, rx2_buf, dma_buf_num);
        break;
    case CHANNEL_UART4:
        uart4_init(rx1_buf, rx2_buf, dma_buf_num);
        break;
    case CHANNEL_USART6:
        usart6_init(rx1_buf, rx2_buf, dma_buf_num);
        break;
    
    default:
        break;
    }
}

/**
 * @brief 串口发送数据统一接口
 * @param uart_channel 串口通道
 * @param data 发送数据缓冲区
 * @param len 发送数据长度
*/
void bsp_usart_transmit(UARTChannel_t uart_channel,uint8_t *data, uint16_t len)
{
    switch (uart_channel)
    {
    case CHANNEL_USART1:
        usart1_tx_dma_enable(data, len);
        break;
    case CHANNEL_USART2:
        //stm32的485硬件流控在使用DMA的时候有些问题，切勿连续发送
        // HAL_UART_Transmit(&huart2, data, len, 100);
        usart2_tx_dma_enable(data, len);
        break;
    case CHANNEL_USART3:
        usart3_tx_dma_enable(data, len);
        break;
    case CHANNEL_UART4:
        uart4_tx_dma_enable(data, len);
        break;
    case CHANNEL_USART6:
        usart6_tx_dma_enable(data, len);
        break;
    
    default:
        break;
    }
}

/**
 * @brief 串口DMA双缓冲接收数据
 * @param uart_channel 串口通道
 * @param receive_style 接收方式，0为接收到定长数据才正确返回，1为接收到数据就正确返回，用于不定长数据的接收
 * @param rx1_buf 接收缓冲区1
 * @param rx2_buf 接收缓冲区2
 * @param data_num 需要接收的数据大小
 * @return UARTDMAStatus_t 接收状态
*/
UARTDMAStatus_t bsp_usart_receive_dma(UARTChannel_t uart_channel, bool_t receive_style , uint16_t data_num)
{
    DMA_HandleTypeDef* dma_handle_ptr;
    uint16_t* this_time_rx_len_ptr;
    //设定长度为2倍的定长数据，防止dma传输越界
    uint16_t dma_buf_num = 2 * data_num;
    static uint16_t usart1_this_time_rx_len = 0;
    static uint16_t usart2_this_time_rx_len = 0;
    static uint16_t usart3_this_time_rx_len = 0; 
    static uint16_t uart4_this_time_rx_len = 0;
    static uint16_t usart6_this_time_rx_len = 0;

    static uint16_t this_remain = 0;
        
    switch (uart_channel)
    {
    case CHANNEL_USART1:
        dma_handle_ptr = &hdma_usart1_rx;
        this_time_rx_len_ptr = &usart1_this_time_rx_len;
        break;
    case CHANNEL_USART2:
        // dma_handle_ptr = &hdma_usart2_rx;
        // this_time_rx_len_ptr = &usart2_this_time_rx_len;
        break;
    case CHANNEL_USART3:
        dma_handle_ptr = &hdma_usart3_rx;
        this_time_rx_len_ptr = &usart3_this_time_rx_len;
        break;
    case CHANNEL_UART4:
        dma_handle_ptr = &hdma_uart4_rx;
        this_time_rx_len_ptr = &uart4_this_time_rx_len;
        break;
    case CHANNEL_USART6:
        dma_handle_ptr = &hdma_usart6_rx;
        this_time_rx_len_ptr = &usart6_this_time_rx_len;
        break;
    
    default:
        break;
    }

    this_remain = __HAL_DMA_GET_COUNTER(dma_handle_ptr);

    if ((((DMA_Stream_TypeDef   *)dma_handle_ptr->Instance)->CR & DMA_SxCR_CT) == RESET)
        {
            /* Current memory buffer used is Memory 0 */

            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(dma_handle_ptr);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            *this_time_rx_len_ptr = dma_buf_num - ((DMA_Stream_TypeDef   *)dma_handle_ptr->Instance)->NDTR;
                
            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)dma_handle_ptr->Instance)->NDTR = dma_buf_num;

            //set memory buffer 1
            //设定缓冲区1
            ((DMA_Stream_TypeDef   *)dma_handle_ptr->Instance)->CR |= DMA_SxCR_CT;
            
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(dma_handle_ptr);
            if(!receive_style)
            {
                /**接收到定长数据才返回正确*/
                if((*this_time_rx_len_ptr) == data_num)
                {
                    return UART_DMA_BUF0_OK;
                }
                else
                {
                    return UART_DMA_NO_OK;
                }  
            }
            else
            {
                if((*this_time_rx_len_ptr) > 0)
                /**接收到数据就返回正确*/
                    return UART_DMA_BUF0_OK;
                else
                    return UART_DMA_NO_OK;
            }
            
		}
        else
        {
            /* Current memory buffer used is Memory 1 */
            //disable DMA
            //失效DMA
            __HAL_DMA_DISABLE(dma_handle_ptr);

            //get receive data length, length = set_data_length - remain_length
            //获取接收数据长度,长度 = 设定长度 - 剩余长度
            *this_time_rx_len_ptr = dma_buf_num -  __HAL_DMA_GET_COUNTER(dma_handle_ptr);
                
            //reset set_data_lenght
            //重新设定数据长度
            ((DMA_Stream_TypeDef   *)dma_handle_ptr->Instance)->NDTR = dma_buf_num;

            //set memory buffer 0
            //设定缓冲区0
            ((DMA_Stream_TypeDef   *)dma_handle_ptr->Instance)->CR &= ~(DMA_SxCR_CT);
            
            //enable DMA
            //使能DMA
            __HAL_DMA_ENABLE(dma_handle_ptr);
            if(!receive_style)
            {
                /**接收到定长数据才返回正确*/
                if((*this_time_rx_len_ptr) == data_num)
                {
                    return UART_DMA_BUF1_OK;
                }
                else
                {
                    return UART_DMA_NO_OK;
                }  
            }
            else
            {
                if((*this_time_rx_len_ptr) > 0)
                /**接收到数据就返回正确*/
                    return UART_DMA_BUF1_OK;
                else
                    return UART_DMA_NO_OK;
            }
        }

    
}

