
#ifndef __L298N_H_
#define __L298N_H_

#include "main.h"
#include <stdint.h>
#include "tim.h"

/**
 * @brief 衰减模式枚举
 */

typedef enum {
    SLOW_DECAY,  /**< 慢衰减模式 */
    FAST_DECAY   /**< 快衰减模式 */
} DecayMode;

typedef enum{
	A_IN1,
	A_IN2,
	A_IN3,
	A_IN4,
	B_IN1,
	B_IN2,
	B_IN3,
	B_IN4
} PWM_Pin;

typedef enum {
	IN_A,
	IN_B
} PWM_IN;

typedef enum {
	FL, //左前
	FR, //右前
	BL, //左后
	BR  //右后
} Wheel;

typedef enum {
	Forward,
	Backward,
	Brake,
	Coast
} Wheel_mode;

typedef struct {
	Wheel wheel;
	uint8_t wheel_speed;
	int origin_speed;
	Wheel_mode wheel_mode;
} Wheel_state;

extern Wheel_state wheel_state[4];

#define wheel_state_FL wheel_state[0]
#define wheel_state_FR wheel_state[1]
#define wheel_state_BL wheel_state[2]
#define wheel_state_BR wheel_state[3]

void SetPWM_Pin(PWM_Pin Pin);

PWM_Pin Get_Pin(Wheel wheel,PWM_IN IN);

/**
 * @brief 初始化L298N
 */
void L298N_Init(void);

/**
 * @brief 设置衰减模式
 * @param mode 衰减模式
 */
void L298N_SetDecayMode(DecayMode mode);

/**
 * @brief 控制电机前进
 * @param speed 速度值（0-100）
 */
void L298N_Forward(uint8_t speed,PWM_Pin Pin1,PWM_Pin Pin2);

/**
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
 */
void L298N_Backward(uint8_t speed,PWM_Pin Pin1,PWM_Pin Pin2);

/**
 * @brief 电机刹车
 */
void L298N_Brake(PWM_Pin Pin1,PWM_Pin Pin2);

/**
 * @brief 电机滑行
 */
void L298N_Coast(PWM_Pin Pin1,PWM_Pin Pin2);

void L298N_Set(Wheel_state* wheel_state);

void L298N_Set_all(void);

#endif /* __L298N_H_ */