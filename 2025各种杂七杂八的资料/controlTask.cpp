/**
 * @file controlTask.cpp
 * @brief 机器人控制任务实现文件
 * 
 * 该文件实现了机器人的主控制任务，
 * 包括电机和舵机的控制逻辑。
 */

#include "../Inc/controlTask.h"

#include "L298N.h"
#include "servo.h"
#include "wheel.h"


/**
 * @brief 控制任务主循环函数
 * 
 * 初始化电机和舵机，然后持续计算轮子状态并控制执行器
 */
void controlTask::run() {

    // 初始化L298N电机驱动和舵机PWM
    L298N_Init();
    Servo::Servo_Init_all();

    for (;;) {
        // 计算所有轮子的状态
        Wheel::calculate_state_all();
        // 控制所有电机
        L298N_Set_all();
        // 控制所有舵机
        Servo::Servo_Set_all();
        // 延时110ms
        osDelay(110);
    }
}

extern "C"{

    static controlTask control_task;

    void ControlTask_Init() {
        control_task.start((char*)"ControlTask", 256, osPriorityNormal);
    }

}