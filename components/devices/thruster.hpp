#ifndef _THRUSTER_HPP_
#define _THRUSTER_HPP_

#include "struct_typedef.h"


class Thruster
{
public:
    Thruster()
    {

    }
    virtual ~Thruster(){};
    virtual void write(void * data, int len) = 0;
    virtual void set_thruster_expect_speed(int32_t speed_set) = 0;
    virtual int16_t get_thruster_rpm() = 0;
protected:

};




class VESC : public Thruster
{
public:
    VESC(uint8_t id)
    {
        this->thruster_id = id;
    }
    ~VESC(){};
    void write(void * data, int len);
    void write(uint8_t can_packet_id, void * data, int len);
    /**
     * @brief 设置推进器期望转速,单位rpm
     * @param rpm_set 转速值
     * @return None
    */
    void set_thruster_expect_speed(int32_t rpm_set);
    void set_thruster_status(int32_t rpm_feedback, int16_t current_feedback, int16_t duty_feedback)
    {
        this->thruster_status_msg_.rpm = rpm_feedback;
        this->thruster_status_msg_.current = current_feedback;
        this->thruster_status_msg_.duty = duty_feedback;
    }
    int16_t get_thruster_rpm()
    {
        return this->thruster_status_msg_.rpm;
    }
    /**
     * @brief 设置推进器期望电流,单位mA
     * @param current_set 电流值
     * @return None
    */
    void set_thruster_expect_current(int32_t current_set);

public:
    static enum {
    CAN_PACKET_SET_DUTY = 0,
    CAN_PACKET_SET_CURRENT = 1,
    CAN_PACKET_SET_RPM = 3,
    CAN_PACKET_SET_POS = 4,
    CAN_PACKET_STATUS = 9,
    } CAN_PACKET_ID;  //VESC CAN commands，可以在源码里面看到完整定义
private:
    uint8_t thruste_tx_buffer[8];
   uint8_t thruster_id; 
    struct {
        int32_t rpm;
        int16_t current;
        int16_t duty;
    } thruster_status_msg_;  //回传电机状态
    // CAN commands


};





#endif /* _THRUSTER_HPP_ */
