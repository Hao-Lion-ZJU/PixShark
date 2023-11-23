/** 
 *****************************Copyright (c) 2023  ZJU****************************
 * @file      : filter.cpp
 * @brief     : 各种滤波器的实现接口
 * @history   :
 *  Version     Date            Author          Note
 *  V1.0.0    2023-6-20       Hao Lion        1. <note>
 *******************************************************************************
 * @verbatim :
 *==============================================================================
 *                                                                              
 *                                                                              
 *==============================================================================
 * @endverbatim :
 *****************************Copyright (c) 2023  ZJU****************************
 */

#include "filter.hpp"

first_order_filter::first_order_filter(const float num, float frame_period_ms)
{
    
    this->num_ = num;
    this->input_ = 0.0f;
    this->out_ = 0.0f;
    this->frame_period_ = frame_period_ms/1000.0f;
}

/**
 * @brief          低通滤波器计算
 * @param[in]      input: 输入数据
 * @retval         低通滤波输出
 */
float first_order_filter::calc(float input)
{
    this->input_ = input;
    this->out_ =
        this->num_ / (this->num_ + this->frame_period_) * this->out_ + this->frame_period_ / (this->num_ + this->frame_period_) * this->input_;
    return this->out_;
}