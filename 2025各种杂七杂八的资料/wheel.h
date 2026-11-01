//
// Created by MI on 2025/10/17.
//

#ifndef YDBD_N_WHEEL_H
#define YDBD_N_WHEEL_H

#pragma once
#include "decodeTask.h"
#include "math.h"

#ifdef __cplusplus
typedef enum {
    FL, //左前
    FR, //右前
    BL, //左后
    BR  //右后
} Wheel_id;

/**
 * @brief 轮子运行模式枚举
 * Forward: 前进
 * Backward: 后退
 * Brake: 刹车
 * Coast: 滑行
 */
typedef enum {
    Forward,   /**< 前进模式 */
    Backward,  /**< 后退模式 */
    Brake,     /**< 刹车模式 */
    Coast      /**< 滑行模式 */
} Wheel_mode;

/**
 * @brief PWM衰减模式枚举
 * SLOW_DECAY: 慢衰减模式，通过高电平维持电机转动
 * FAST_DECAY: 快衰减模式，通过快速切换电平减少能量损耗
 */
typedef enum {
    SLOW_DECAY,  /**< 慢衰减模式 */
    FAST_DECAY   /**< 快衰减模式 */
} DecayMode;

class Wheel {
public:

    Wheel(Wheel_id id);

    void calculate_state();

    static void calculate_state_all();

    /**
 * @brief 设置衰减模式
 * @param mode 衰减模式
 */
    static void SetDecayMode(DecayMode mode);


    Wheel_id    m_id;              /**< 轮子编号 */
    uint16_t     m_speed;           /**< 轮子速度值(0-1000) */
    Wheel_mode  m_mode;            /**< 轮子运行模式 */
    int         m_origin_speed;    /**< 轮子原始速度值 */

    /** @brief 当前衰减模式 */
    static  DecayMode currentDecayMode;
};

extern Wheel w_FL;
extern Wheel w_FR;
extern Wheel w_BL;
extern Wheel w_BR;

#endif
#endif //YDBD_N_WHEEL_H
