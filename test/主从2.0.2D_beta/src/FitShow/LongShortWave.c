/*
 * @Author: your name
 * @Date: 2021-04-23 15:24:37
 * @LastEditTime: 2021-11-26 22:48:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \BK3431Q\SDK\sdk\FitShow\LongShortWave.c
 */


#include "LongShortWave.h"
#include "FSM.h"
#include "CircleQueue.h"

static void LongShortWaveFSM_Init(void);
static void prvvSensorEvent(uint8_t Event);
static void prvvSensorAccelerate(void* param);
static void prvvSensorDecelerate(void* param);
static void prvvResetSensor(void* param);
// static void prvvSensorStart(void* param);
static void prvvSensorLoop(void);

#define QueueSize 5
static CircleQueue_t xLongShortWaveQueue;
static QueueElement_t xLongShortWaveQueueBuff[QueueSize];

static LongShortWave_t xLongShortWaveSensor = 
{
    .Init = LongShortWaveFSM_Init,
    .EventTrigger = prvvSensorEvent,
    .ChangeOfRate = 10,
    .Scan = prvvSensorLoop,
    .Flag = 0,
    .Reset = prvvResetSensor,
};





static FSM_t xLongShortFSM;



const FSM_Table_t LongShortWaveTable[] = 
{
    // {LongShortWave_IdleState, LongShortWave_AccelerationEvent, LongShortWave_AccelerateState, prvvSensorStart},

    // {LongShortWave_AccelerateState, LongShortWave_TimeOutEvent, LongShortWave_IdleState, prvvResetSensor},
    {LongShortWave_AccelerateState, LongShortWave_DecelerationEvent, LongShortWave_DecelerateState, prvvSensorDecelerate},

    // {LongShortWave_DecelerateState, LongShortWave_TimeOutEvent, LongShortWave_IdleState, prvvResetSensor},
    {LongShortWave_DecelerateState, LongShortWave_AccelerationEvent, LongShortWave_AccelerateState, prvvSensorAccelerate},

};


static void LongShortWaveFSM_Init(void)
{
    vQueueInit(&xLongShortWaveQueue, xLongShortWaveQueueBuff, sizeof(xLongShortWaveQueueBuff)/sizeof(QueueElement_t));

    xLongShortFSM.xFSM_Table = LongShortWaveTable;
    xLongShortFSM.ucCurrentState = LongShortWave_DecelerateState;
    xLongShortFSM.ucFSM_MaxTransferNumber = sizeof(LongShortWaveTable)/sizeof(FSM_Table_t);
    
    prvvResetSensor(NULL);
    // xLongShortWaveSensor.ChangeOfRate;
    // xLongShortWaveSensor.State = xLongShortFSM.ucCurrentState;
}

// static void prvvSensorStart(void* param)
// {
//     (void)param;
//     xLongShortWaveSensor.EventTrigger(LongShortWave_AccelerationEvent);
// }

static void prvvSensorAccelerate(void* param)
{
    // uint32_t SysTick = ulGetDeviceSysTick();
    xLongShortWaveSensor.Flag = 1;
    // xLongShortWaveSensor.TimeDiff = SysTick - xLongShortWaveSensor.PreAccelerationTimeStamp;
    // xLongShortWaveSensor.PreAccelerationTimeStamp = SysTick;
    // uart_printf("%s->%s->%d:++++++++++++++++++++++++ \r\n", __FILE__, __func__, __LINE__);
}

static void prvvSensorDecelerate(void* param)
{
    // uart_printf("%s->%s->%d:------------------------ \r\n", __FILE__, __func__, __LINE__);
}

static void prvvResetSensor(void* param)
{
    xLongShortWaveSensor.Flag = 0;
    // xLongShortWaveSensor.EventCount = 0;
    xLongShortWaveSensor.PrePulseState = LongPulseType;
    xLongShortWaveSensor.PreTimeDiff = 0xffff;
}

static void prvvSensorEvent(uint8_t Event)
{    
    QueueElement_t xEvent;
    xEvent.Event = Event;
    xEvent.EventID = 0;
    vQueuePush(&xLongShortWaveQueue, xEvent);
}

static void prvvSensorLoop(void)
{
    if (xLongShortWaveQueue.ucCount > 0)
    {
        QueueElement_t xEvent;
        vQueuePop(&xLongShortWaveQueue, &xEvent);
        // uart_printf("%s->%s->%d: %d \r\n", __FILE__, __func__, __LINE__, xEvent.Event);
        vFSM_EventHandler(&xLongShortFSM, xEvent.Event, NULL);
    }
    
}

LongShortWave_t *pGetLongShortWaveHandler(void)
{
    return &xLongShortWaveSensor;
}
















