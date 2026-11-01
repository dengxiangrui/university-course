/**
 * @file wheel.cpp
 * @brief 麦轮控制实现文件
 * 
 * 该文件实现了麦轮机器人的运动控制算法，包括轮子状态计算、
 * 速度分解和运动控制等功能。
 */

#include "wheel.h"

#define MAX_SPEED 1000    /**< 最大速度值 */

/**
 * @brief 轮子构造函数
 * @param id 轮子编号
 */
Wheel::Wheel(Wheel_id id) {
    m_id=id;
}

Wheel w_FL(FL);
Wheel w_FR(FR);
Wheel w_BL(BL);
Wheel w_BR(BR);

/**
 * @brief 计算单个轮子的状态
 * 
 * 根据PS2手柄的摇杆数据计算轮子的运动方向和速度
 * 左摇杆控制前后左右移动，右摇杆控制旋转
 * 使用向量合成的方法计算每个轮子的实际输出
 */
void Wheel::calculate_state() {
    int speedL=0, speedR=0, origin_speed=0;

    switch (m_id) {
        case FL: {
            // 左前轮：左摇杆X/Y轴分量叠加右摇杆旋转分量
            speedL=static_cast<int>((ps2Data.lx + ps2Data.ly) / 1.414);
            speedR=static_cast<int>(ps2Data.rx);
            break;
        }
        case FR: {
            // 右前轮：左摇杆X/Y轴分量叠加右摇杆反向旋转分量
            speedL=static_cast<int>((ps2Data.ly - ps2Data.lx) / 1.414);
            speedR=-static_cast<int>(ps2Data.rx);
            break;
        }
        case BL: {
            // 左后轮：左摇杆X/Y轴分量叠加右摇杆旋转分量
            speedL=static_cast<int>((ps2Data.ly - ps2Data.lx) / 1.414);
            speedR=static_cast<int>(ps2Data.rx);
            break;
        }
        case BR: {
            // 右后轮：左摇杆X/Y轴分量叠加右摇杆反向旋转分量
            speedL=static_cast<int>((ps2Data.lx + ps2Data.ly) / 1.414);
            speedR=-static_cast<int>(ps2Data.rx);
            break;
        }
    }

    origin_speed=speedL+speedR;
    m_origin_speed=origin_speed;

    // 根据计算出的速度确定轮子运行模式
    if (origin_speed>0) {
        m_mode=Forward;
    }
    else if (origin_speed<0) {
        m_mode=Backward;
    }
    else {
        m_mode=Brake;
    }

}

/**
 * @brief 获取两个整数中的最大值
 * @param a 第一个整数
 * @param b 第二个整数
 * @return 较大的那个整数
 */
static inline int max(int a, int b) {
    return a>b?a:b;
}

/**
 * @brief 计算所有轮子的状态并进行速度归一化
 * 
 * 依次计算每个轮子的状态，然后找出最大速度值，
 * 将所有轮子的速度按比例缩放到0-100范围内
 */
void Wheel::calculate_state_all() {
    // 计算每个轮子的状态
    w_FL.calculate_state();
    w_FR.calculate_state();
    w_BL.calculate_state();
    w_BR.calculate_state();

    // 找出最大的原始速度值用于归一化
    int max_speed=0;
    max_speed =max(max(abs(w_BL.m_origin_speed), abs(w_BR.m_origin_speed)),
        max(abs(w_FL.m_origin_speed), abs(w_FR.m_origin_speed)));

    // 如果所有轮子速度都为0，则全部设置为0
    if (max_speed==0) {
        w_BL.m_speed=0;
        w_BR.m_speed=0;
        w_FL.m_speed=0;
        w_FR.m_speed=0;
        return;
    }
    
    // 按最大速度进行归一化处理，映射到0-1000范围
    w_BL.m_speed=
        static_cast<uint16_t>(abs(w_BL.m_origin_speed/max_speed*MAX_SPEED));
    w_BR.m_speed=
        static_cast<uint16_t>(abs(w_BR.m_origin_speed/max_speed*MAX_SPEED));
    w_FL.m_speed=
        static_cast<uint16_t>(abs(w_FL.m_origin_speed/max_speed*MAX_SPEED));
    w_FR.m_speed=
        static_cast<uint16_t>(abs(w_FR.m_origin_speed/max_speed*MAX_SPEED));

}

DecayMode Wheel::currentDecayMode = SLOW_DECAY;

void Wheel::SetDecayMode(DecayMode mode)
{
    currentDecayMode = mode;
}