/*
 * @Author: your name
 * @Date: 2021-09-02 13:56:53
 * @LastEditTime: 2021-10-18 23:26:27
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \BK3633\SDK\src\FitShow\SpeedSensor.h
 */

#ifndef __SPEEDSENSOR_H__
#define __SPEEDSENSOR_H__

#include "stdint.h"
#include "PulseSensorBase.h"





typedef struct SpeedSensor
{
    PulseSensorBase_t xPulseSensorBase;
    void (*Init)(void);
    void (*DeInit)(void);
    void (*Loop)(void);
    void (*Start)(void);
    void (*Stop)(void);
    void (*Pause)(void);
    void (*CalcHandler)(void *param);
    void (*CleanSpeedBuff)(void);
    void (*EncoderModeAcquire)(void *param);
    void (*Trigger)(void* param);
    void (*CleanData)(void);
    /* --------------- wave filtering --------------- */
    uint32_t ulTimeDiff;
    uint32_t ulPreTimeStamp;
    uint16_t usMaxInterval;
    uint16_t usMinInterval;
    
    /* ---------------------------------------------- */
    uint8_t AcquisitionMode;
    uint32_t NumberOfCount;
    uint8_t ucEnable;
    
    uint8_t ucInterruptLevel;

} SpeedSensor_t;



void vEncoderModePulseAcquire(uint8_t Pin);
SpeedSensor_t *pGetSpeedSensorHandler(void);



#endif
