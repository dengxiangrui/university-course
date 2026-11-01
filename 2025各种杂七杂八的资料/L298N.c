/**
 * @file L298N.c
 * @brief L298N双电机驱动器控制实现
 */

#include "L298N.h"

Wheel_state wheel_state[4];

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
#define MAX_SPEED 100


/** @brief 当前衰减模式 */
static DecayMode currentDecayMode = SLOW_DECAY;

/** @brief 当前PWM口 */
static PWM_Pin currentPin = A_IN1;

/**
 * @brief 获取PWM引脚
 * @param wheel 轮子
 * @param IN 引脚
 * @return PWM引脚
 */
PWM_Pin Get_Pin(Wheel wheel,PWM_IN IN) {
    if (wheel==FL && IN==IN_A) {
        return A_IN1;
    }
    else if (wheel==FL && IN==IN_B) {
        return A_IN2;
    }
    else if (wheel==FR && IN==IN_A) {
        return B_IN1;
    }
    else if (wheel==FR && IN==IN_B) {
        return B_IN2;
    }
    else if (wheel==BL && IN==IN_A) {
        return B_IN3;
    }
    else if (wheel==BL && IN==IN_B) {
        return B_IN4;
    }
    else if (wheel==BR && IN==IN_A) {
        return A_IN3;
    }
    else if (wheel==BR && IN==IN_B) {
        return A_IN4;
    }
}

/**
 * @brief 设置Pin引脚的PWM占空比
 * @param duty 占空比值
 */
static inline void __SetPinPWM(uint8_t duty,PWM_Pin Pin)
{
	SetPWM_Pin(Pin);
	switch (currentPin)
	{
	case A_IN1:
		{__HAL_TIM_SET_COMPARE(A_IN1_TIM, A_IN1_CH, duty);return;}
	case A_IN2:
		{__HAL_TIM_SET_COMPARE(A_IN2_TIM, A_IN2_CH, duty);return;}
	case A_IN3:
		{__HAL_TIM_SET_COMPARE(A_IN3_TIM, A_IN3_CH, duty);return;}
	case A_IN4:
		{__HAL_TIM_SET_COMPARE(A_IN4_TIM, A_IN4_CH, duty);return;}
	case B_IN1:
		{__HAL_TIM_SET_COMPARE(B_IN1_TIM, B_IN1_CH, duty);return;}
	case B_IN2:
		{__HAL_TIM_SET_COMPARE(B_IN2_TIM, B_IN2_CH, duty);return;}
	case B_IN3:
		{__HAL_TIM_SET_COMPARE(B_IN3_TIM, B_IN3_CH, duty);return;}
	case B_IN4:
		{__HAL_TIM_SET_COMPARE(B_IN4_TIM, B_IN4_CH, duty);return;}
	default:return;
	}
}


void SetPWM_Pin(PWM_Pin Pin)
{
	currentPin = Pin;
}

/**
 * @brief 初始化L298N
 */
void L298N_Init(void)
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
 * @brief 设置衰减模式
 * @param mode 衰减模式
 */
void L298N_SetDecayMode(DecayMode mode)
{
    currentDecayMode = mode;
}

/**
 * @brief 控制电机前进
 * @param speed 速度值（0-100）
 */


/**
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
 */
void L298N_Backward(uint8_t speed,PWM_Pin Pin1,PWM_Pin Pin2)
{
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    
    if (currentDecayMode == FAST_DECAY) {
        __SetPinPWM(0,Pin1);
        __SetPinPWM(speed,Pin2);
    } else {
        __SetPinPWM(MAX_SPEED - speed,Pin1);
        __SetPinPWM(MAX_SPEED,Pin2);
    }
}

/**
 * @brief 电机刹车
 */
void L298N_Brake(PWM_Pin Pin1,PWM_Pin Pin2)
{
    __SetPinPWM(MAX_SPEED,Pin1);
    __SetPinPWM(MAX_SPEED,Pin2);
}

/**
 * @brief 电机滑行
 */
void L298N_Coast(PWM_Pin Pin1,PWM_Pin Pin2)
{
    __SetPinPWM(0,Pin1);
    __SetPinPWM(0,Pin2);
}

void L298N_Set(Wheel_state* wheel_state)
{
    switch (wheel_state->wheel_mode) {
        case Forward:
            L298N_Forward(wheel_state->wheel_speed,Get_Pin(wheel_state->wheel,IN_A),Get_Pin(wheel_state->wheel,IN_B));
            break;
        case Backward:
            L298N_Backward(wheel_state->wheel_speed,Get_Pin(wheel_state->wheel,IN_A),Get_Pin(wheel_state->wheel,IN_B));
            break;
        case Brake:
            L298N_Brake(Get_Pin(wheel_state->wheel,IN_A),Get_Pin(wheel_state->wheel,IN_B));
            break;
        case Coast:
            L298N_Coast(Get_Pin(wheel_state->wheel,IN_A),Get_Pin(wheel_state->wheel,IN_B));
            break;
    }
}

void L298N_Set_all(void)
{
    for (int i=0;i<4;i++)
    {
        L298N_Set(&wheel_state[i]);
    }
}