//
// Created by MI on 2025/10/18.
//

#ifndef YDBD_N_L298N_H
#define YDBD_N_L298N_H

#pragma once
#include "wheel.h"
#include "tim.h"

/**
 * @brief PWM引脚枚举
 * 
 * 定义L298N驱动模块的所有PWM控制引脚
 */
typedef enum{
    A_IN1,  /**< A组输入1 */
    A_IN2,  /**< A组输入2 */
    A_IN3,  /**< A组输入3 */
    A_IN4,  /**< A组输入4 */
    B_IN1,  /**< B组输入1 */
    B_IN2,  /**< B组输入2 */
    B_IN3,  /**< B组输入3 */
    B_IN4   /**< B组输入4 */
} PWM_Pin;

/**
 * @brief PWM输入枚举
 * 
 * 定义L298N驱动模块的输入组
 */
typedef enum {
    IN_A,   /**< 输入组A */
    IN_B    /**< 输入组B */
} PWM_IN;

/**
 * @brief 获取指定轮子的PWM引脚
 * @param wheel 轮子编号
 * @param IN PWM输入组
 * @return 对应的PWM引脚
 */
PWM_Pin Get_Pin(Wheel_id wheel,PWM_IN IN);

/**
 * @brief 初始化L298N
 * 
 * 启动所有PWM通道以控制L298N驱动模块
 */
void L298N_Init();

/**
 * @brief 控制电机前进
 * @param speed 速度值（0-100）
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Forward(uint16_t speed,PWM_Pin Pin1,PWM_Pin Pin2);

/**
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Backward(uint16_t speed,PWM_Pin Pin1,PWM_Pin Pin2);

/**
 * @brief 电机刹车
 * 
 * 通过给电机两端施加电压实现快速制动
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Brake(PWM_Pin Pin1,PWM_Pin Pin2);

/**
 * @brief 电机滑行
 * 
 * 释放电机两端，让电机自由旋转直至停止
 * @param Pin1 正向控制引脚
 * @param Pin2 反向控制引脚
 */
void L298N_Coast(PWM_Pin Pin1,PWM_Pin Pin2);

/**
 * @brief 设置单个轮子的状态
 * @param wheel 轮子对象指针
 */
void L298N_Set(Wheel* wheel);

/**
 * @brief 设置所有轮子的状态
 */
void L298N_Set_all();

#endif //YDBD_N_L298N_H