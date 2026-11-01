//
// Created by MI on 2025/10/12.
//

#include "servo.h"

#define S_TIM_1 &htim4
#define S_CH_1 TIM_CHANNEL_2

static uint16_t s_angle = 150;

void Servo_Init(void) {
    HAL_TIM_PWM_Start(S_TIM_1, S_CH_1);
}

void Servo_Set(void) {
    __HAL_TIM_SET_COMPARE(S_TIM_1, S_CH_1, s_angle);
}

void Calculate_Servo_Angle(void) {
    if (jsdata.L1==1) {
        s_angle -= 25 ;
    }
    if (jsdata.R1==1) {
        s_angle += 25;
    }
    if (s_angle > 250) {
        s_angle = 250;
    }
    if (s_angle < 50) {
        s_angle = 50;
    }
}