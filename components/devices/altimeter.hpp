#ifndef _ALTIMETER_HPP_
#define _ALTIMETER_HPP_

#include "struct_typedef.h"
#include "stdlib.h"

/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)


#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */

/**
 * @brief   高度计基类
*/
class Altimeter
{
public:
	Altimeter();
	virtual ~Altimeter()
	{
		
	}
	virtual void write(void * data, int len) = 0;
	/**
	 * @brief          返回高度计数据指针
	*/
	const float& get_altimeter_data(void) 
	{
		return altimeter_data;
	}
	/**
	 * @brief          高度计协议解析
	 * @param[in]      depth_frame: 原生数据指针
	 * @return  	   none
	*/
	virtual void altimeter_data_solve(volatile const uint8_t *depth_frame) = 0;

	

	float altimeter_data;		//高度,单位为m
};

class ISA500 : public Altimeter
{
public:
	ISA500();
	~ISA500()
	{

	}
	void write(void * data, int len);

	/**
	 * @brief          高度计协议解析
	 * @param[in]      depth_frame: 原生数据指针
	 * @return  	   none
	*/
	void altimeter_data_solve(volatile const uint8_t *depth_frame);
public:
	/**这里高度计为ASCII字符输出，设置ID为107，即ddd.ddm<CR><LF>*/
	static constexpr auto AM_DATA_LENGTH = 9; //高度计上传数据长度

};


#endif /* _ALTIMETER_HPP_ */
