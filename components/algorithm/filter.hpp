
#ifndef FILTER_H
#define FILTER_H

/**
 * 一阶低通互补滤波器
*/
class first_order_filter
{
private:
    float input_;        //输入数据
    float out_;          //滤波输出的数据
    float num_;         //滤波参数
    float frame_period_; //滤波的时间间隔 单位 s
public:
    first_order_filter(const float num, float frame_period_ms);
    ~first_order_filter() = default;

    /**
     * @brief          低通滤波器计算
     * @param[in]      input: 输入数据
     * @retval         低通滤波输出
     */
    float calc(float input);

};






#endif /* FILTER_H */
