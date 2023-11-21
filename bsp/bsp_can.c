#include "bsp_can.h"
#include "main.h"
#include <assert.h>
#include "RobotConfig.h"

extern FDCAN_HandleTypeDef hfdcan1;

void can_filter_init(void)
{
	/* USER CODE BEGIN FDCAN1_Init 2 */
	//配置RX滤波器 
	FDCAN_FilterTypeDef FDCAN1_RXFilter;
	FDCAN1_RXFilter.IdType=FDCAN_STANDARD_ID;                       //标准ID
	FDCAN1_RXFilter.FilterIndex=0;                                  //滤波器索引                   
	FDCAN1_RXFilter.FilterType=FDCAN_FILTER_MASK;                   //滤波器类型
	FDCAN1_RXFilter.FilterConfig=FDCAN_FILTER_TO_RXFIFO0;           //过滤器0关联到FIFO0  
	FDCAN1_RXFilter.FilterID1=0x0000;                               //32位ID
	FDCAN1_RXFilter.FilterID2=0x0000;                               //如果FDCAN配置为传统模式的话，这里是32位掩码
	if(HAL_FDCAN_ConfigFilter(&hfdcan1,&FDCAN1_RXFilter)!=HAL_OK) //滤波器初始化
	{
		Error_Handler();
	}
	HAL_FDCAN_Start(&hfdcan1);                               //开启FDCAN
	HAL_FDCAN_ActivateNotification(&hfdcan1,FDCAN_IT_RX_FIFO0_NEW_MESSAGE,0);
/* USER CODE END FDCAN1_Init 2 */
}


static FDCAN_TxHeaderTypeDef  thruster_tx_message;


/**
 * @brief  发送can数据
 * @param  ID: 帧ID
 * @param  pData: 数据指针
 * @param  len: 数据长度
*/
void bsp_can_transmit(uint32_t ID, uint8_t *pData, uint8_t len)
{
	uint16_t i = 0;
	assert(len <= 8);
    thruster_tx_message.Identifier = ID;											//控制帧ID	
    thruster_tx_message.IdType = FDCAN_EXTENDED_ID;									//标准ID
    thruster_tx_message.TxFrameType = FDCAN_DATA_FRAME;								//数据帧
    thruster_tx_message.DataLength = (uint32_t)(len << 16);							//数据长度
	thruster_tx_message.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	thruster_tx_message.BitRateSwitch = FDCAN_BRS_OFF;								//关闭数据波特率切换
	thruster_tx_message.FDFormat=FDCAN_CLASSIC_CAN;                					//传统的CAN模式
    thruster_tx_message.TxEventFifoControl=FDCAN_NO_TX_EVENTS;     					//无发送事件
    thruster_tx_message.MessageMarker=0;
	// HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1,&thruster_tx_message,pData);
	while(HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1,&thruster_tx_message,pData)!=HAL_OK)
	{
		i++;
		/*防止卡死*/
		if(i > 0xfff)
			break;
	}
}

__weak void  thruster_recevice_callback(uint32_t ID, volatile const uint8_t *can_frame, uint8_t dlc)
{

}
__weak void crawler_recevice_callback(uint32_t ID, volatile const uint8_t *can_frame, uint8_t dlc)
{

}

/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    // uint8_t can_id;
    // uint8_t rx_data[8];
	// if((RxFifo0ITs&FDCAN_IT_RX_FIFO0_NEW_MESSAGE)!= RESET)
	// {
	// 	FDCAN_RxHeaderTypeDef  rx_header;
	// 	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, rx_data);
	// 	can_id = rx_header.Identifier;
	// 	if(can_id == Crawler_Left_ID | can_id == Crawler_Right_ID)
	// 	{
	// 		crawler_recevice_callback(rx_header.Identifier, rx_data, rx_header.DataLength>>16);
	// 	}
	// 	else
	// 	{
	// 		thruster_recevice_callback(rx_header.Identifier, rx_data, rx_header.DataLength>>16);
	// 	}
	// }
}