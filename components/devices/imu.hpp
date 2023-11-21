#ifndef _IMU_HPP_
#define _IMU_HPP_

#include "struct_typedef.h"





/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)	

#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */






/**
 * @brief   IMU 基类
*/
class IMU
{
public:
	IMU();
	virtual ~IMU() {}
	virtual void write(void * data, int len) = 0;
	/**
	 * @brief          返回IMU数据指针
	*/
	const imu_data_t& get_imu_data(void) 
	{
		return imu_data;
	}
	/**
	 * @brief          IMU协议解析
	 * @param[in]      imu_frame: 原生数据指针
	 * @return  	   none
	*/
	virtual void imu_data_solve(volatile const uint8_t *imu_frame) = 0;

	
protected:
	imu_data_t imu_data;
};

/**
 * @brief   阿鲁比 IMU 具体型号：LPMS-IG1 mini
*/
class LPMS : public IMU
{
public:
	LPMS();
	~LPMS() = default;
	/**
	 * @brief          IMU协议解析
	 * @param[in]      imu_frame: 原生数据指针
	 * @return  	   none
	*/
	void imu_data_solve(volatile const uint8_t *imu_frame);
	/**
	 * @brief          发送内容打包
	 * @param[in]      cmd_type:  命令内容ID
	 * @param[in]      p_data:  数据指针
	 * @param[in]      len:  数据长度
	 * @return  	   返回要发送的数据大小
	*/
	uint16_t send_imu_pack(uint8_t cmd_type, uint8_t *p_data, uint16_t len);
	/**
	 * @brief          复写发送函数
	 * @param[in]      p_data:  数据指针
	 * @param[in]      len:  数据长度
	 * @return  	   none
	*/
	void write(void * data, int len);
	/**
	 * @brief          计算前n字节累加校验和
	 * @param[in]      LRC_message: 数据
	 * @param[in]      LRC_length: 数据和校验的长度
	 * @retval         计算完的校验和
	 */
	static uint8_t get_LRC_sum(uint8_t *LRC_message, uint16_t LRC_length){
		uint16_t check_sum = 0;
		uint16_t len = LRC_length;
		if (LRC_message == nullptr)
		{
			return 0XFF;
		}
		while(--len)
		{
			check_sum += *LRC_message++;
		}
		return check_sum;
	};

	/**
	 * @brief          LRC校验
	 * @param[in]      LRC_message: 数据
	 * @param[in]      LRC_length: 数据的长度
	 * @retval         计算完的校验和
	 */
	static bool_t LRC_check(uint8_t *data_message, uint32_t LRC_length){
		uint16_t temp = 0;
		uint16_t len = LRC_length;
		temp = get_LRC_sum(data_message, len-4);
		if(data_message[len-4] == (temp & 0xFF) && data_message[len-3] == (temp >> 8))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	};

	/**
	 * @brief 进入IMU命令模式
	 * @retval none
	*/
	void GOTO_IMU_Command_Mode(void);

	/**
	 * @brief 进入IMU数据流发送
	 * @retval none
	*/
	void GOTO_IMU_Streaming_Mode(void);

	/**
	 * @brief IMU参数保存
	 * @retval none
	*/
	void GOTO_IMU_Save_Setting(void);

public:
	static constexpr auto IMU_HEAD_SOF = 0x3A; //帧头
	static constexpr auto IMU_ID = 0x0001; //IMU设备ID
	static constexpr auto IMU_END_SOF = 0x0A0D; //帧尾
	static constexpr auto IMU_CMD_LENGTH = 11; //IMU指令长度
	static constexpr auto IMU_DATA_LENGTH = 167; //IMU回传数据大小

private:
	
	typedef enum
	{
		Goto_Command_Mode   = 0x0006,  //进入命令模式
		Goto_Streaming_Mode = 0x0007, //进入数据发送模式
		Save_Setting        = 0x0004, //保存IMU参数设置

	}imu_cmd_e;
	uint8_t imu_tx_buf[IMU_CMD_LENGTH]={0};
	
};

/**
 * @brief   北微传感 IMU 具体型号：
*/
class BWIMU : public IMU
{
public:
	BWIMU();
	~BWIMU() = default;
	/**
	 * @brief          IMU协议解析
	 * @param[in]      imu_frame: 原生数据指针
	 * @return  	   none
	*/
	void imu_data_solve(volatile const uint8_t *imu_frame);
	/**
	 * @brief          发送内容打包
	 * @param[in]      cmd_type:  命令内容ID
	 * @param[in]      p_data:  数据指针
	 * @param[in]      len:  数据长度
	 * @return  	   返回要发送的数据大小
	*/
	uint16_t send_imu_pack(uint8_t cmd_type, uint8_t *p_data, uint16_t len);
	/**
	 * @brief          发送内容打包
	 * @param[in]      cmd_type:  命令内容ID
	 * @param[in]      p_data:  数据指针
	 * @param[in]      len:  数据长度
	 * @return  	   返回要发送的数据大小
	*/
	void write(void * data, int len);
	/**
	 * @brief          IMU数据校验
	 * @param[in]      imu_frame: 原生数据指针
	 * @return  	   bool
	*/
	bool_t imu_check(uint8_t *data_message);

private:
	
};

#endif /* _IMU_HPP_ */
