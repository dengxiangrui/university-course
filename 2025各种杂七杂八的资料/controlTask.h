//
// Created by MI on 2025/10/16.
//

#ifndef YDBD_N_CONTROLTASK_H
#define YDBD_N_CONTROLTASK_H

//YDBD_N_CONTROLTASK_H

#pragma once
#include "TaskBase.h"

#ifdef __cplusplus
/**
 * @brief 控制任务类
 * 
 * 继承自TaskBase，负责控制机器人的运动执行
 */
class controlTask : public TaskBase {

public:
    /**
     * @brief 任务主循环函数
     * 
     * 控制舵机和电机执行相应的动作
     */
    void run() override;

};

extern "C"{
#endif

    /**
     * @brief 控制任务初始化函数
     * 
     * 创建并启动控制任务
     */
    void ControlTask_Init();

#ifdef __cplusplus
}
#endif
#endif
