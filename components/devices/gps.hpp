#ifndef _GPS_HPP_
#define _GPS_HPP_


#include "struct_typedef.h"
/* ----------------------- Data Struct ------------------------------------- */
#pragma pack(push, 1)


#pragma pack(pop)
/* ----------------------- extern Function ----------------------------------- */


/**
 * @brief   GPS 基类
*/
class GPS
{
public:
	GPS();
	virtual ~GPS() {}
	virtual void write(void * data, int len) = 0;
	/**
	 * @brief          返回GPS数据指针
	*/
	const gps_data_t& get_gps_data_point(void) 
	{
		return gps_data;
	}
	/**
	 * @brief          GPS协议解析
	 * @param[in]      gps_frame: 原生数据指针
	 * @return  	   none
	*/
	virtual void gps_data_solve(volatile const uint8_t *gps_frame) = 0;

	
protected:
	gps_data_t gps_data;
};

#endif /* _GPS_HPP_ */
