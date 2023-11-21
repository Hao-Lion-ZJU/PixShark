#ifndef _LED_HPP_
#define _LED_HPP_


#include "struct_typedef.h"


class LED
{
public:
    LED(){};
    virtual ~LED(){};
    virtual void write(void * data, int len) = 0;
    virtual void on(uint8_t brightness) = 0;
private:

};

/**
 * 霸勒斯LED灯，485通讯
*/
class Barlus : public LED
{
public:
    Barlus();
    ~Barlus() = default;
    /**
     * @brief 写入数据
    */
    void write(void * data, int len);
    /**
     * @brief 关闭LED灯
    */
    void off(void);
    /**
     * @brief 灯光增强
    */
   void enhance(void);
    /**
     * @brief 灯光减弱
    */
    void weaken(void);
    /**
     * @brief 灯光亮度存储
    */
    void save(void);
    /**
     * @brief 灯光亮度恢复
    */
    void recover(void);
    /**
     * @brief 开启灯光，亮度映射到0-255
     * 
    */
    void on(uint8_t brightness);

private:
    uint8_t led_tx_buffer[7];
};


#endif /* _LED_HPP_ */
