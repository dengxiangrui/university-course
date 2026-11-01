//
// Created by MI on 2025/10/18.
//

#ifndef YDBD_N_SERVO_H
#define YDBD_N_SERVO_H

#pragma once
#include "decodeTask.h"
#include "tim.h"

// 舵机角度限制定义
#define SERVO_MIN_ANGLE 50    /**< 舵机最小角度值 */
#define SERVO_MAX_ANGLE 250   /**< 舵机最大角度值 */

#ifdef __cplusplus
/**
 * @brief 舵机基类
 * 
 * 提供舵机控制的基本功能，使用PWM信号控制舵机角度
 */
class Servo {
public:
    /**
     * @brief 构造函数
     * @param htim 定时器句柄
     * @param channel 定时器通道
     */
    Servo(TIM_HandleTypeDef* htim, uint32_t channel);

    /**
     * @brief 初始化舵机PWM
     */
    void Servo_Init() const;
    
    /**
     * @brief 初始化所有舵机
     */
    static void Servo_Init_all();
    
    /**
     * @brief 纯虚函数，设置舵机角度
     * 
     * 派生类必须实现此函数以提供具体的角度控制逻辑
     */
    virtual void Servo_Set() = 0;
    
    /**
     * @brief 设置所有舵机角度
     */
    static void Servo_Set_all();

    TIM_HandleTypeDef* m_htim;  /**< 定时器句柄 */
    uint16_t m_angle;           /**< 舵机角度值 */
    uint32_t m_channel;         /**< 定时器通道 */
};

/**
 * @brief 舵机1类
 * 
 * 继承自Servo基类，控制第一个舵机
 * 使用L1/R1按键控制
 */
class Servo1 : public Servo {
public:
    /**
     * @brief 构造函数
     * @param htim 定时器句柄
     * @param channel 定时器通道
     */
    Servo1(TIM_HandleTypeDef* htim, uint32_t channel) : Servo(htim, channel) {}
    
    /**
     * @brief 设置舵机1角度
     * 
     * 使用L1/R1按键控制舵机角度
     */
    void Servo_Set() override;
};

/**
 * @brief 舵机2类
 * 
 * 继承自Servo基类，控制第二个舵机
 * 使用L2/R2按键控制
 */
class Servo2 : public Servo {
public:
    /**
     * @brief 构造函数
     * @param htim 定时器句柄
     * @param channel 定时器通道
     */
    Servo2(TIM_HandleTypeDef* htim, uint32_t channel) : Servo(htim, channel) {}
    
    /**
     * @brief 设置舵机2角度
     * 
     * 使用L2/R2按键控制舵机角度
     */
    void Servo_Set() override;
};

#endif
#endif //YDBD_N_SERVO_H