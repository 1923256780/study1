/*
 * @Author: your name
 * @Date: 2021-04-23 15:24:50
 * @LastEditTime: 2021-09-03 18:16:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \BK3431Q\SDK\sdk\FitShow\LongShortWave.h
 */

#ifndef __LONG_SHOT_WAVE_H__
#define __LONG_SHOT_WAVE_H__

#include "stdint.h"

typedef enum LongShortWaveState
{
    LongShortWave_IdleState,
    LongShortWave_AccelerateState, // Acceleration
    LongShortWave_DecelerateState, // deceleration
} LongShortWaveState_e;

typedef enum PulseTypeList
{
    LongPulseType,
    ShortPulseType,
} PulseTypeList_e;


typedef enum LongShortWaveEvent
{
    LongShortWave_AccelerationEvent, //
    LongShortWave_DecelerationEvent,
    LongShortWave_TimeOutEvent,

    LongShortWave_ResetEvent,
} LongShortWaveEvent_e;

typedef struct LongShortWave
{
    void (*Init)(void);
    void (*EventTrigger)(uint8_t Event);
    void (*Scan)(void);
    void (*Reset)(void*);
    uint8_t Flag;
    uint8_t ChangeOfRate;
    uint8_t EventCount;
    uint32_t PreTimeStamp;
    uint8_t PrePulseState;
    uint32_t FirstAccelerationTimeStamp;
    uint32_t PreTimeDiff;
    uint32_t TimeDiff;
    uint8_t Enable;
} LongShortWave_t;

LongShortWave_t *pGetLongShortWaveHandler(void);

#endif
