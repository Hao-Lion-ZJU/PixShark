#ifndef COMMON_HPP
#define COMMON_HPP

#include "struct_typedef.h"
#include "FreeRTOS.h"
#include <array>
#include <type_traits>
#include <assert.h>
namespace uuv
{
    /**
     * @brief 复写内存分配算法，使用FreeRTOS的内存分配算法
     * @tparam T 
     * @tparam Ts 
     * @param  params           My Param doc
     * @return T*
     */
    template<typename T, typename... Ts>
    T* make_malloc(Ts&&... params)
    {
        return (T*)(pvPortMalloc(sizeof(T)));
    }

    template <typename T>

  struct is_array_class : std::false_type {};
  template <typename T, size_t N>
  struct is_array_class<std::array<T, N>> : std::true_type {};
    
  




}



/**
  * @brief          将4个字节数据byte[4]转化为浮点数
  * @param[in]      four_byte: 待转换四字节指针
  * @retval         浮点数float(32字节)
  */
inline float Byte_to_Float(const uint8_t *four_byte, bool isBigEndian = false)
{
	float float_data=0;
  uint8_t tmp[4] = {0};
  if(!isBigEndian)
  {
    memcpy(tmp, four_byte, 4);
  }
  else
  {
    tmp[0] = four_byte[3];
    tmp[1] = four_byte[2];
    tmp[2] = four_byte[1];
    tmp[3] = four_byte[0];
  }
  float_data = *(float*)tmp;
	return float_data;
}

/**
  * @brief          将3个字节的BCD格式(SXXXYY)数据转化为浮点数
  * @param[in]      four_byte: 待转换三字节指针
  * @retval         浮点数float(32字节)
  */
inline float BCD_SXXXYY_to_Float(uint8_t *four_byte)
{
	float float_data=0;
	float_data = ((*four_byte)& 0x01)*100 + (*(four_byte + 1) & 0x10)*10 + (*(four_byte + 1) & 0x01)*1 + (*(four_byte + 2) & 0x10)*0.1 + (*(four_byte + 2) & 0x01)*0.01 ;
	if(((*four_byte) & 0x10)){
    float_data = -float_data;
    }
	return float_data;
}

/**
  * @brief          将3个字节的BCD格式(SXYYYY)数据转化为浮点数
  * @param[in]      four_byte: 待转换三字节指针
  * @retval         浮点数float(32字节)
  */
inline float BCD_SXYYYY_to_Float(uint8_t *four_byte)
{
	float float_data=0;
	float_data = ((*four_byte)& 0x01) + (*(four_byte + 1) & 0x10)*0.1 + (*(four_byte + 1) & 0x01)*0.01 + (*(four_byte + 2) & 0x10)*0.001 + (*(four_byte + 2) & 0x01)*0.0001 ;
	if(((*four_byte) & 0x10)){
    float_data = -float_data;
    }
	return float_data;
}

#endif /* COMMON_HPP */
