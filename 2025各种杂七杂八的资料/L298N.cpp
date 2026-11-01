//
// Created by MI on 2025/10/18.
//

/**
 * @file L298N.cpp
 * @brief L298N双电机驱动器控制实现
 * 
 * 该文件实现了基于L298N驱动芯片的直流电机控制功能，
 * 支持前进、后退、刹车和滑行四种模式。
 */

#include "L298N.h"

/** @brief A_IN1引脚的定时器 */
#define A_IN1_TIM &htim2
/** @brief A_IN2引脚的定时器 */
#define A_IN2_TIM &htim2
/** @brief A_IN3引脚的定时器 */
#define A_IN3_TIM &htim2
/** @brief A_IN4引脚的定时器 */
#define A_IN4_TIM &htim2
/** @brief B_IN1引脚的定时器 */
#define B_IN1_TIM &htim3
/** @brief B_IN2引脚的定时器 */
#define B_IN2_TIM &htim3
/** @brief B_IN3引脚的定时器 */
#define B_IN3_TIM &htim3
/** @brief B_IN4引脚的定时器 */
#define B_IN4_TIM &htim3


/** @brief A_IN1引脚的定时器通道 */
#define A_IN1_CH TIM_CHANNEL_1
/** @brief A_IN2引脚的定时器通道 */
#define A_IN2_CH TIM_CHANNEL_2
/** @brief A_IN3引脚的定时器通道 */
#define A_IN3_CH TIM_CHANNEL_3
/** @brief A_IN4引脚的定时器通道 */
#define A_IN4_CH TIM_CHANNEL_4
/** @brief B_IN1引脚的定时器通道 */
#define B_IN1_CH TIM_CHANNEL_1
/** @brief B_IN2引脚的定时器通道 */
#define B_IN2_CH TIM_CHANNEL_2
/** @brief B_IN3引脚的定时器通道 */
#define B_IN3_CH TIM_CHANNEL_3
/** @brief B_IN4引脚的定时器通道 */
#define B_IN4_CH TIM_CHANNEL_4



/** @brief 最大速度值 */
#define MAX_SPEED 1000


/**
 * @brief 获取指定轮子的PWM引脚
 * 
 * 根据轮子编号和输入组确定对应的PWM引脚
 * 
 * 轮子与引脚映射关系：
 * - FL(左前): IN_A->A_IN1, IN_B->A_IN2
 * - FR(右前): IN_A->B_IN1, IN_B->B_IN2
 * - BL(左后): IN_A->B_IN3, IN_B->B_IN4
 * - BR(右后): IN_A->A_IN3, IN_B->A_IN4
 * 
 * @param wheel 轮子编号
 * @param IN PWM输入组
 * @return 对应的PWM引脚
 */
PWM_Pin Get_Pin(Wheel_id wheel,PWM_IN IN) {
    if (wheel==FL && IN==IN_A) {
        return A_IN1;
    }
    if (wheel==FL && IN==IN_B) {
        return A_IN2;
    }
    if (wheel==FR && IN==IN_A) {
        return B_IN1;
    }
    if (wheel==FR && IN==IN_B) {
        return B_IN2;
    }
    if (wheel==BL && IN==IN_A) {
        return B_IN3;
    }
    if (wheel==BL && IN==IN_B) {
        return B_IN4;
    }
    if (wheel==BR && IN==IN_A) {
        return A_IN3;
    }
    if (wheel==BR && IN==IN_B) {
        return A_IN4;
    }
    return A_IN1;
}

/**
 * @brief 设置指定引脚的PWM占空比
 * 
 * 根据引脚枚举值设置对应定时器通道的比较值
 * 
 * @param duty 占空比值(0-100)
 * @param Pin PWM引脚枚举值
 */
static void SetPinPWM(uint16_t duty,PWM_Pin Pin)
{
    switch (Pin)
    {
        case A_IN1:
            __HAL_TIM_SET_COMPARE(A_IN1_TIM, A_IN1_CH, duty);
            break;
        case A_IN2:
            __HAL_TIM_SET_COMPARE(A_IN2_TIM, A_IN2_CH, duty);
            break;
        case A_IN3:
            __HAL_TIM_SET_COMPARE(A_IN3_TIM, A_IN3_CH, duty);
            break;
        case A_IN4:
            __HAL_TIM_SET_COMPARE(A_IN4_TIM, A_IN4_CH, duty);
            break;
        case B_IN1:
            __HAL_TIM_SET_COMPARE(B_IN1_TIM, B_IN1_CH, duty);
            break;
        case B_IN2:
            __HAL_TIM_SET_COMPARE(B_IN2_TIM, B_IN2_CH, duty);
            break;
        case B_IN3:
            __HAL_TIM_SET_COMPARE(B_IN3_TIM, B_IN3_CH, duty);
            break;
        case B_IN4:
            __HAL_TIM_SET_COMPARE(B_IN4_TIM, B_IN4_CH, duty);
            break;
        default:
            break;
    }
}

/**
 * @brief 初始化L298N
 * 
 * 启动所有用于控制L298N的PWM通道
 */
void L298N_Init()
{
    HAL_TIM_PWM_Start(A_IN1_TIM, A_IN1_CH);
    HAL_TIM_PWM_Start(A_IN2_TIM, A_IN2_CH);
    HAL_TIM_PWM_Start(A_IN3_TIM, A_IN3_CH);
    HAL_TIM_PWM_Start(A_IN4_TIM, A_IN4_CH);
    HAL_TIM_PWM_Start(B_IN1_TIM, B_IN1_CH);
    HAL_TIM_PWM_Start(B_IN2_TIM, B_IN2_CH);
    HAL_TIM_PWM_Start(B_IN3_TIM, B_IN3_CH);
    HAL_TIM_PWM_Start(B_IN4_TIM, B_IN4_CH);
}



/**
 * @brief 控制电机前进
 * 
 * 根据当前衰减模式设置电机正转
 * 
 * @param speed 速度值（0-100）
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Forward(uint16_t speed,PWM_Pin Pin1,PWM_Pin Pin2)
{
    // 限制速度在有效范围内
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;

    // 根据衰减模式设置PWM输出
    if (Wheel::currentDecayMode == FAST_DECAY) {
        // 快衰减模式：正向引脚输出速度，反向引脚输出0
        SetPinPWM(speed,Pin1);
        SetPinPWM(0,Pin2);
    } else {
        // 慢衰减模式：正向引脚输出最大值，反向引脚输出(最大值-速度)
        SetPinPWM(MAX_SPEED,Pin1);
        SetPinPWM(MAX_SPEED - speed,Pin2);
    }
}

/**
 * @brief 控制电机后退
 * 
 * 根据当前衰减模式设置电机反转
 * 
 * @param speed 速度值（0-100）
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Backward(uint16_t speed,PWM_Pin Pin1,PWM_Pin Pin2)
{
    // 限制速度在有效范围内
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;

    // 根据衰减模式设置PWM输出
    if (Wheel::currentDecayMode == FAST_DECAY) {
        // 快衰减模式：正向引脚输出0，反向引脚输出速度
        SetPinPWM(0,Pin1);
        SetPinPWM(speed,Pin2);
    } else {
        // 慢衰减模式：正向引脚输出(最大值-速度)，反向引脚输出最大值
        SetPinPWM(MAX_SPEED - speed,Pin1);
        SetPinPWM(MAX_SPEED,Pin2);
    }
}

/**
 * @brief 电机刹车
 * 
 * 通过给电机两端施加电压实现快速制动
 * 
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Brake(PWM_Pin Pin1,PWM_Pin Pin2)
{
    // 两个引脚都输出最大值，实现快速制动
    SetPinPWM(MAX_SPEED,Pin1);
    SetPinPWM(MAX_SPEED,Pin2);
}

/**
 * @brief 电机滑行
 * 
 * 释放电机两端，让电机自由旋转直至停止
 * 
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Coast(PWM_Pin Pin1,PWM_Pin Pin2)
{
    // 两个引脚都输出0，让电机自由滑行
    SetPinPWM(0,Pin1);
    SetPinPWM(0,Pin2);
}

/**
 * @brief 设置单个轮子的状态
 * 
 * 根据轮子的模式和速度设置对应的电机控制信号
 * 
 * @param wheel 轮子对象指针
 */
void L298N_Set(Wheel* wheel)
{
    switch (wheel->m_mode) {
        case Forward:
            // 设置轮子前进
            L298N_Forward(wheel->m_speed,Get_Pin(wheel->m_id,IN_A),Get_Pin(wheel->m_id,IN_B));
            break;
        case Backward:
            // 设置轮子后退
            L298N_Backward(wheel->m_speed,Get_Pin(wheel->m_id,IN_A),Get_Pin(wheel->m_id,IN_B));
            break;
        case Brake:
            // 设置轮子刹车
            L298N_Brake(Get_Pin(wheel->m_id,IN_A),Get_Pin(wheel->m_id,IN_B));
            break;
        case Coast:
            // 设置轮子滑行
            L298N_Coast(Get_Pin(wheel->m_id,IN_A),Get_Pin(wheel->m_id,IN_B));
            break;
    }
}

/**
 * @brief 设置所有轮子的状态
 * 
 * 依次设置四个轮子的电机控制信号
 */
void L298N_Set_all()
{
    L298N_Set(&w_FL);
    L298N_Set(&w_FR);
    L298N_Set(&w_BL);
    L298N_Set(&w_BR);
}