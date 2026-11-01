//
// Created by MI on 2025/10/5.
//

#ifndef YDBD_X_ANALYSIS_JS_DATA_H
#define YDBD_X_ANALYSIS_JS_DATA_H

#include "main.h"
#include <stdlib.h>

#include "L298N.h"
#include "PS2.h"

#define max(a,b)(a>b?a:b)
#define JS_MIN -127
#define JS_MAX 128
#define JS_MID 0
#define JS_DEAD_ZONE 15
#define MAX_SPEED 100

int Calculate_origin_wheel_speed(Wheel wheel);

Wheel_mode  Get_wheel_mode(Wheel wheel);

int Get_MAX_origin_speed(void);

void Calculate_all_origin_wheel_speed(void);

uint8_t Get_wheel_speed(Wheel wheel);

void Calculate_wheel_state(void);

#endif //YDBD_X_ANALYSIS_JS_DATA_H