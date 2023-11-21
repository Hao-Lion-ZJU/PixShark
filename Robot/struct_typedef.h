#ifndef STRUCT_TYPEDEF_H
#define STRUCT_TYPEDEF_H

#ifdef __cplusplus
#include <cstring>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined ( __ICCARM__ ) /*!< IAR Compiler */


#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;

#elif defined ( __GNUC__ ) /* GNU Compiler */ 
#include <stdint.h>

/* exact-width unsigned integer types */

typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;
#endif

/**user define device data type begin*/
#pragma pack(push, 1)

typedef struct 
{
    float cabin_temperature;//舱内温度
    float cabin_humidity;//舱内湿度
    float cabin_water_level;//舱内水位
    float cpu_temperature;//CPU温度
}cabin_status_t;



typedef  struct 
{
    float P_set;
    float I_set;
    float D_set;
    float MAX_set;
    float IMAX_set;
}ext_pid_set_t;

typedef  struct
{
	//TODO: 添加gps数据结构体
	float latitude;       //正数为北纬，负数为南纬
    float longitude;      //正数为东经，负数为西经
} gps_data_t;	

typedef  struct
{
	//TODO: 添加IMU数据结构体
	//加速度 单位：g(9.8m/s)
	float Acc_x;
	float Acc_y;
	float Acc_z;
	//角度 单位：rad
	float Angle_x;
	float Angle_y;
	float Angle_z;
	//角速度 单位：rad/s
	float Gyrol_x;
	float Gyrol_y;
	float Gyrol_z;
} imu_data_t;

#pragma pack(pop)


/**user define device data type end**/


#ifdef __cplusplus
}
#endif



#endif
