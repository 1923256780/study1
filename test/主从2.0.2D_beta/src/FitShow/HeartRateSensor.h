/*
 * @Author: your name
 * @Date: 2021-09-02 13:56:30
 * @LastEditTime: 2021-11-19 14:24:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \BK3633\SDK\src\FitShow\HeartRateSensor.h
 */

#ifndef __HEARTRATESENSOR_H__
#define __HEARTRATESENSOR_H__


// #include "PulseSensorBase.h"


typedef struct HeartRateSensor
{
    PulseSensorBase_t xPulseSensorBase;
    void (*Init)(void);
    void (*DeInit)(void);
    void (*Loop)(void);
    void (*Start)(void);
    void (*Stop)(void);
    void (*PulseAcquire)(void *param);
    void (*CalcHandler)(void *param);
    void (*CleanData)(void);
    void (*Trigger)(void* param);
    /* --------------- wave filtering --------------- */
    uint32_t ulTimeDiff;
    uint32_t ulPreTimeStamp;
    uint16_t usMaxInterval;
    uint16_t usMinInterval;
    /* ---------------------------------------------- */
    uint8_t ucEnable;
    uint8_t AcquisitionMode;
    uint8_t ucInterruptLevel;

} HeartRateSensor_t;

HeartRateSensor_t *pGetHeartRateSensorHandler(void);

#endif
