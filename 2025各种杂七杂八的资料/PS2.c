//
// Created by MI on 2025/10/5.
//

#include "PS2.h"

JSDATA jsdata;

uint8_t sendjsdata[2]={0x01,0x42};
uint8_t receivejsdata[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned char extract_bit(unsigned char data,int n)
{
    return (data >> n) & 1;
}

void getJSDATA(void) {
    HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);

    HAL_SPI_Transmit(&hspi1, &sendjsdata[0], 1,2);
    HAL_SPI_Transmit(&hspi1, &sendjsdata[1], 1,2);
    HAL_SPI_Receive(&hspi1, &receivejsdata[1], 8, 20);

    HAL_GPIO_WritePin(PS2_CS_GPIO_Port, PS2_CS_Pin, GPIO_PIN_SET);
}

void saveJSDATA(void) {
    jsdata.leftX    =(int)receivejsdata[5]-128;
    jsdata.leftY    =(int)receivejsdata[6]-128;
    jsdata.rightX   =(int)receivejsdata[7]-128;
    jsdata.rightY   =(int)receivejsdata[8]-128;
    jsdata.up       =extract_bit(receivejsdata[3],4);
    jsdata.right    =extract_bit(receivejsdata[3],5);
    jsdata.down     =extract_bit(receivejsdata[3],6);
    jsdata.left     =extract_bit(receivejsdata[3],7);
    jsdata.select   =extract_bit(receivejsdata[3],0);
    jsdata.start    =extract_bit(receivejsdata[3],3);
    jsdata.triangle =extract_bit(receivejsdata[4],4);
    jsdata.circle   =extract_bit(receivejsdata[4],5);
    jsdata.cross    =extract_bit(receivejsdata[4],6);
    jsdata.square   =extract_bit(receivejsdata[4],7);
    jsdata.L2       =extract_bit(receivejsdata[4],0);
    jsdata.R2       =extract_bit(receivejsdata[4],1);
    jsdata.L1       =extract_bit(receivejsdata[4],2);
    jsdata.R1       =extract_bit(receivejsdata[4],3);
    //死区处理
    if (jsdata.leftY<JS_DEAD_ZONE && jsdata.leftY>-JS_DEAD_ZONE) {
        jsdata.leftY=0;
    }
    if (jsdata.leftX<JS_DEAD_ZONE && jsdata.leftX>-JS_DEAD_ZONE) {
        jsdata.leftX=0;
    }
    if (jsdata.rightY<JS_DEAD_ZONE && jsdata.rightY>-JS_DEAD_ZONE) {
        jsdata.rightY=0;
    }
    if (jsdata.rightX<JS_DEAD_ZONE && jsdata.rightX>-JS_DEAD_ZONE) {
        jsdata.rightX=0;
    }
}

void saveJSDATA_test(void) {
    jsdata.leftX    =64;
    jsdata.leftY    =64;
    jsdata.rightX   =0;
    jsdata.rightY   =0;
    jsdata.up       =extract_bit(receivejsdata[3],4);
    jsdata.right    =extract_bit(receivejsdata[3],5);
    jsdata.down     =extract_bit(receivejsdata[3],6);
    jsdata.left     =extract_bit(receivejsdata[3],7);
    jsdata.select   =extract_bit(receivejsdata[3],0);
    jsdata.start    =extract_bit(receivejsdata[3],3);
    jsdata.triangle =extract_bit(receivejsdata[4],4);
    jsdata.circle   =extract_bit(receivejsdata[4],5);
    jsdata.cross    =extract_bit(receivejsdata[4],6);
    jsdata.square   =extract_bit(receivejsdata[4],7);
    jsdata.L2       =extract_bit(receivejsdata[4],0);
    jsdata.R2       =extract_bit(receivejsdata[4],1);
    jsdata.L1       =extract_bit(receivejsdata[4],2);
    jsdata.R1       =extract_bit(receivejsdata[4],3);
    //死区处理
    if (jsdata.leftY<JS_DEAD_ZONE && jsdata.leftY>-JS_DEAD_ZONE) {
        jsdata.leftY=0;
    }
    if (jsdata.leftX<JS_DEAD_ZONE && jsdata.leftX>-JS_DEAD_ZONE) {
        jsdata.leftX=0;
    }
    if (jsdata.rightY<JS_DEAD_ZONE && jsdata.rightY>-JS_DEAD_ZONE) {
        jsdata.rightY=0;
    }
    if (jsdata.rightX<JS_DEAD_ZONE && jsdata.rightX>-JS_DEAD_ZONE) {
        jsdata.rightX=0;
    }
}