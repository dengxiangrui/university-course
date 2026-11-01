/**
 * @file servo.cpp
 * @brief 舵机控制实现文件
 * 
 * 该文件实现了舵机控制的相关功能，
 * 包括舵机初始化和角度控制。
 */

#include "../Inc/servo.h"

#include "decodeTask.h"
#include "PS2.h"

/**
 * @brief 舵机类构造函数
 * @param htim 定时器句柄
 * @param channel 定时器通道
 */
Servo::Servo(TIM_HandleTypeDef* htim,uint32_t  channel) {
    m_htim = htim;
    m_channel = channel;
    m_angle = 150;  // 初始角度设为中间位置
}

Servo1 servo1(&htim4, TIM_CHANNEL_2);
Servo2 servo2(&htim4, TIM_CHANNEL_3);

/**
 * @brief 初始化舵机PWM
 * 
 * 启动定时器的PWM输出通道
 */
void Servo::Servo_Init() const {
    HAL_TIM_PWM_Start(m_htim, m_channel);
}

/**
 * @brief 初始化所有舵机
 * 
 * 依次初始化两个舵机
 */
void Servo::Servo_Init_all() {
    servo1.Servo_Init();
    servo2.Servo_Init();
}

/**
 * @brief 设置舵机1角度
 * 
 * 使用L1/R1按键控制舵机角度，每次调整25个单位
 * L1按键增加角度，R1按键减小角度
 */
void Servo1::Servo_Set() {
    if (ps2Data.l1) m_angle += 25;  // L1按键增加角度
    if (ps2Data.r1) m_angle -= 25;  // R1按键减小角度
    
    // 角度限制在有效范围内
    if (m_angle > SERVO_MAX_ANGLE) m_angle = SERVO_MAX_ANGLE;
    if (m_angle < SERVO_MIN_ANGLE) m_angle = SERVO_MIN_ANGLE;

    // 设置PWM比较值以控制舵机角度
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, m_angle);
}

/**
 * @brief 设置舵机2角度
 * 
 * 使用L2/R2按键控制舵机角度，每次调整25个单位
 * L2按键增加角度，R2按键减小角度
 */
void Servo2::Servo_Set() {
    if (ps2Data.l2) m_angle += 25;  // L2按键增加角度
    if (ps2Data.r2) m_angle -= 25;  // R2按键减小角度
    
    // 角度限制在有效范围内
    if (m_angle > SERVO_MAX_ANGLE) m_angle = SERVO_MAX_ANGLE;
    if (m_angle < SERVO_MIN_ANGLE) m_angle = SERVO_MIN_ANGLE;

    // 设置PWM比较值以控制舵机角度
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, m_angle);
}

/**
 * @brief 设置所有舵机角度
 * 
 * 依次设置两个舵机的角度
 */
void Servo::Servo_Set_all() {
    servo1.Servo_Set();
    servo2.Servo_Set();
}