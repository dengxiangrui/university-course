//
// Created by MI on 2025/10/5.
//

#include "analysis_js_data.h"

//计算原始轮速
int Calculate_origin_wheel_speed(Wheel wheel) {
    int speedL,speedR,speed;//左摇杆产生的轮速、右摇杆产生的轮速、最终轮速
    switch (wheel) {
        case FL: {
            speedL=(int)((jsdata.leftX+jsdata.leftY)/1.414);
            speedR=(int)jsdata.rightX;
            speed=speedL+speedR;
            return speed;
        }
        case FR: {
            speedL=(int)((jsdata.leftY-jsdata.leftX)/1.414);
            speedR=-(int)jsdata.rightX;
            speed=speedL+speedR;
            return speed;
        }
        case BL: {
            speedL=(int)((jsdata.leftY-jsdata.leftX)/1.414);
            speedR=(int)jsdata.rightX;
            speed=speedL+speedR;
            return speed;
        }
        case BR: {
            speedL=(int)((jsdata.leftX+jsdata.leftY)/1.414);
            speedR=-(int)jsdata.rightX;
            speed=speedL+speedR;
            return speed;
        }
        default:
            return 0;
    }
}

//获取轮模式
Wheel_mode  Get_wheel_mode(Wheel wheel) {
    int speed=Calculate_origin_wheel_speed(wheel);
    if(speed>0){
        return Forward;
    }
    else if(speed<0){
        return Backward;
    }
    else if(speed==0){
        return Brake;
    }
    else{
        return Coast;
    }
}

//获取所有原始轮速
void Calculate_all_origin_wheel_speed(void) {
    for (int i = 0; i < 4; i++) {
        wheel_state[i].origin_speed=Calculate_origin_wheel_speed(wheel_state[i].wheel);
    }
}

//获取最终轮速
uint8_t Get_wheel_speed(Wheel wheel) {
    int MAX_speed = max(max(wheel_state_FR.origin_speed,wheel_state_BR.origin_speed),
        max(wheel_state_FL.origin_speed,wheel_state_BL.origin_speed));
    switch (wheel) {
        case FL:
            return abs(wheel_state_FL.origin_speed/MAX_speed*MAX_SPEED);//MAX_speed指的是最大初始轮速，MAX_SPEED指的是最大最终轮速
        case FR:
            return abs(wheel_state_FR.origin_speed/MAX_speed*MAX_SPEED);
        case BL:
            return abs(wheel_state_BL.origin_speed/MAX_speed*MAX_SPEED);
        case BR:
            return abs(wheel_state_BR.origin_speed/MAX_speed*MAX_SPEED);
        default:
            return 0;
    }
}

// 计算轮状态
void Calculate_wheel_state(void) {
    for (int i = 0; i < 4; i++) {
        wheel_state[i].wheel_mode=Get_wheel_mode(wheel_state[i].wheel);
        wheel_state[i].wheel_speed=Get_wheel_speed(wheel_state[i].wheel);
    }
}