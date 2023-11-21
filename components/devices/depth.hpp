#ifndef _DEPTH_HPP_
#define _DEPTH_HPP_

#include "struct_typedef.h"



#define WATER_DENSITY 	    1000.0f
#define g 	                9.8f

/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)


#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */

/**
 * @brief   Depth 基类
*/
class Depth
{
public:
	Depth();
	virtual ~Depth(){}
	virtual void write(void * data, uint16_t len) = 0;
	/**
	 * @brief          返回深度计数据指针
	*/
	const float& get_depth_data(void) 
	{
		return depth_data;
	}
	/**
	 * @brief          深度计协议解析
	 * @param[in]      depth_frame: 原生数据指针
	 * @return  	   none
	*/
	virtual void depth_data_solve(volatile const uint8_t *depth_frame) = 0;

	

	float depth_data;
};

class KELLER : public Depth
{

public:
	/**
	 * @brief   KELLER构造函数
	 * @param[in] dev_addr: 设备地址
	*/
	KELLER(uint8_t dev_addr)
	{
  		this->dev_addr_ = dev_addr;
	}
	~KELLER(){}

	void write(void * data, uint16_t len);

	/**
	 * @brief          深度计协议解析，直接读取压力值，不进行modbus协议解析
	 * @param[in]      depth_frame: 原生数据指针
	 * @return  	   none
	*/
	void depth_data_solve(volatile const uint8_t *depth_frame);
	/**
	 * @brief          深度计协议解析，采用状态机方式解析modbus，比较规范
	 * @param[in]      none
	 * @return  	   none
	*/
	void depth_data_solve(void);

	/**
	 * @brief          读取深度计寄存器
	 * @param[in]      StAdd: 寄存器地址
	 * @param[in]      len: 需要读取的寄存器个数
	 * @return  	   none
	*/
	void read_depth_register(uint16_t StAdd, uint8_t reg_len);
	/**
	 * @brief          写入深度计单个寄存器
	 * @param[in]      StAdd: 寄存器地址
	 * @param[in]      data: 数据
	*/
	void write_depth_single_register(uint16_t StAdd, uint16_t data);
	/**
	 * @brief          请求读取一次压力值
	 * @param[in]      none
	 * @return  	   none
	*/
	void request_depth_data(void);

public:
	static constexpr auto DEPTH_CMD_LENGTH = 8; //深度计发送缓冲区长度
	static constexpr auto DEPTH_DATA_LENGTH = 9; //深度计一帧数据长度

private:

	bool_t restart_flag = false; //复位标志，上电后需要采集空气中的压力值作为大气压参考值
	uint8_t dev_addr_;
	uint8_t depth_tx_buf[DEPTH_CMD_LENGTH]={0};

	float atmosphere; 		//陆上气压,单位为bar

	typedef enum
	{
		Register_Read = 0x03,	//读寄存器
		Single_Register_Write = 0x06, //写单个寄存器
		Echo_Test = 0x08		//回传测试
	}depth_function;

	typedef enum
	{
		P1 = 0x0002,      //压力传感器1寄存器地址，单位bar
		P2 = 0x0004,      //压力传感器2寄存器地址，单位bar
		T =  0x0006,	 //温度，单位为℃
		UART  = 0x0200   //串口相关设置		
	}depth_StAdd;//更多地址参见科勒说明书



	
};

#endif /* _DEPTH_HPP_ */
