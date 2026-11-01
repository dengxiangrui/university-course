//
// Created by MI on 2025/10/5.
//

#ifndef YDBD_X_PS2_H
#define YDBD_X_PS2_H

#include "main.h"
#include "spi.h"
#include "analysis_js_data.h"

//手柄数据结构
typedef struct {
    int leftX,leftY,rightX,rightY;
    uint8_t L2,R2;
    uint8_t L1,R1;
    uint8_t triangle,circle,cross,square;
    uint8_t select,start;
    uint8_t up,down,left,right;
}JSDATA;

extern JSDATA jsdata;

unsigned char extract_bit(unsigned char data,int n);

void getJSDATA(void);

void saveJSDATA(void);

void saveJSDATA_test(void);

#endif //YDBD_X_PS2_H