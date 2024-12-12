/*
 * @Author: your name
 * @Date: 2021-09-02 13:56:19
 * @LastEditTime: 2021-12-22 12:07:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \BK3633\SDK\src\FitShow\HeartRateSensor.c
 */

#include "stdio.h"
#include "Device.h"
#include "HeartRateSensor.h"
#include "PulseSensorBase.h"
#include "xSoftTimer.h"
#include "SystemConfig.h"

#define BUF_SIZE 5
static uint16_t pulse_data[BUF_SIZE]; //采集
static uint16_t pulse_sort[BUF_SIZE]; //排序
static uint16_t pulse_avg[BUF_SIZE] = {72, 72, 72, 72, 72};  //平均

// static PulseAcquire_t xHeartRatePulseAcquire;
static HeartRateSensor_t xHeartRateSensor;

static FitShowSystemConfig_t *pFitShowSystemConfig;

uint16_t _avg_(uint16_t *arr, uint16_t len, uint16_t value);
uint16_t _sort_(uint16_t *arr, uint16_t len);

static void prvv_HR_SensorLoop(void);
static void prvv_HR_SensorInit(void);
static void prvvHeartRateSensorStop(void);
static void prvvHeartRateSensorStart(void);
static void prvvHeartRateDataClean(void);
static void prvvDeviceHeartRatePulseAcquire(void *param);
static void prvvHeartRateCalct(void *param);

static void prvvHeartRateSensorPulseEventTrigger(void *param);

static QueueElement_t xPulseEventQueueBuff[10];
static CircleQueue_t xPulseEventQueue;

static HeartRateSensor_t xHeartRateSensor = 
{
    .Init = prvv_HR_SensorInit,
	.Trigger = prvvHeartRateSensorPulseEventTrigger,
};


static void prvv_HR_SensorInit(void)
{
	pFitShowSystemConfig = pGetFitShowSystemConfig();
	vQueueInit(&xPulseEventQueue, xPulseEventQueueBuff, sizeof(xPulseEventQueueBuff) / sizeof(PulseEvent_t));
	xHeartRateSensor.Loop = prvv_HR_SensorLoop;
	xHeartRateSensor.Start = prvvHeartRateSensorStart;
	xHeartRateSensor.Stop = prvvHeartRateSensorStop;
	xHeartRateSensor.CleanData = prvvHeartRateDataClean;
	xHeartRateSensor.PulseAcquire = prvvDeviceHeartRatePulseAcquire;
	
	xHeartRateSensor.usMaxInterval = pFitShowSystemConfig->pSystemConfigInformation->xHeartPulseSensor.xPulsetSensorPin.usMaxInterval;
	xHeartRateSensor.usMinInterval = pFitShowSystemConfig->pSystemConfigInformation->xHeartPulseSensor.xPulsetSensorPin.usMinInterval;
	xHeartRateSensor.AcquisitionMode = pFitShowSystemConfig->pSystemConfigInformation->xHeartPulseSensor.xPulseSensorCalcParam.CountAcquisitionMode;
	xHeartRateSensor.xPulseSensorBase.NumberOfPulse = 1;

	static SinglePulseAcquisition_t xSinglePulseAcquisition;
	xHeartRateSensor.xPulseSensorBase.PulseConfig = &xSinglePulseAcquisition;
	xSinglePulseAcquisition.Pin = pFitShowSystemConfig->pSystemConfigInformation->xHeartPulseSensor.xPulsetSensorPin.xGPIO.Pin;
	xSinglePulseAcquisition.Properties.Pull = (Pull_Type)pFitShowSystemConfig->pSystemConfigInformation->xHeartPulseSensor.xPulsetSensorPin.xGPIO.PullType;
	xSinglePulseAcquisition.Properties.IntterruptMode = (int_mode)pFitShowSystemConfig->pSystemConfigInformation->xHeartPulseSensor.xPulsetSensorPin.xGPIO.InterruptMode;

	if (xSinglePulseAcquisition.Properties.IntterruptMode == RISE_EDGE)
	{
		xHeartRateSensor.ucInterruptLevel = VOLTAGE_HIGHT;
	}
	if (xSinglePulseAcquisition.Properties.IntterruptMode == FALL_EDGE)
	{
		xHeartRateSensor.ucInterruptLevel = VOLTAGE_LOW;
	}

	xHeartRateSensor.CalcHandler = prvvHeartRateCalct;

	gpio_config(xSinglePulseAcquisition.Pin, FLOAT, xSinglePulseAcquisition.Properties.Pull);
}

static void prvv_HR_SensorLoop(void)
{
	if (xHeartRateSensor.ucEnable == false)
	{
		return;
	}
	if (xPulseEventQueue.ucCount == 0)
    {
        return;
    }
	PulseEvent_t xPulseEvent;
    vQueuePop(&xPulseEventQueue, (QueueElement_t*)&xPulseEvent);
    if (xPulseEvent.TimeStamp <= xHeartRateSensor.ulPreTimeStamp)
    {
        return;
    }
	xHeartRateSensor.ulTimeDiff = xPulseEvent.TimeStamp - xHeartRateSensor.ulPreTimeStamp;
	xHeartRateSensor.ulPreTimeStamp = xPulseEvent.TimeStamp;
	if (xHeartRateSensor.ulTimeDiff > xHeartRateSensor.usMaxInterval || xHeartRateSensor.ulTimeDiff < xHeartRateSensor.usMinInterval)
	{
		return;
	}
	// 
	xHeartRateSensor.PulseAcquire(&xHeartRateSensor);
}

static void prvvHeartRateSensorPulseEventTrigger(void *param)
{
    PulseEvent_t *pPulseEvent = (PulseEvent_t*)param;
    QueueElement_t xElement;
    xElement.Event = pPulseEvent->TimeStamp;
    xElement.EventID = pPulseEvent->Pin;
    vQueuePush(&xPulseEventQueue, xElement);
	SoftTimerReset(pHeartRateCalcTimeOutTimer);
	SoftTimerStart(pHeartRateCalcTimeOutTimer);
}


static void prvvHeartRateSensorStop(void)
{
	xHeartRateSensor.ucEnable = false;
	xHeartRateSensor.CleanData();

	SinglePulseAcquisition_t *pSinglePulseAcquisition = NULL;
	pSinglePulseAcquisition = xHeartRateSensor.xPulseSensorBase.PulseConfig;
	gpio_config(pSinglePulseAcquisition->Pin, FLOAT, pSinglePulseAcquisition->Properties.Pull);
	REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);

	SoftTimerPause(pHeartRateCalctTimer);
	SoftTimerPause(pHeartRateCalcTimeOutTimer);
}

static void prvvHeartRateSensorStart(void)
{
	xHeartRateSensor.ucEnable = true;
	xHeartRateSensor.ulTimeDiff = 0;
	xHeartRateSensor.ulPreTimeStamp = ulGetDeviceSysTick();
	SinglePulseAcquisition_t *pSinglePulseAcquisition = xHeartRateSensor.xPulseSensorBase.PulseConfig;
	gpio_config(pSinglePulseAcquisition->Pin, INPUT, pSinglePulseAcquisition->Properties.Pull);
	gpio_set_int_mode(pSinglePulseAcquisition->Pin, pSinglePulseAcquisition->Properties.IntterruptMode);
	REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);
	
	SoftTimerReset(pHeartRateCalctTimer);
	SoftTimerStart(pHeartRateCalctTimer);
	SoftTimerReset(pHeartRateCalcTimeOutTimer);
	SoftTimerStart(pHeartRateCalcTimeOutTimer);
}

static void prvvDeviceHeartRatePulseAcquire(void *param)
{
	HeartRateSensor_t *pHeartRateSensor = (HeartRateSensor_t*)param;
	uint16_t v = 60000 / pHeartRateSensor->ulTimeDiff;
	if (v < 50 || v > 220)
		return;

	for (uint16_t i = 0; i < BUF_SIZE - 1; i++)
	{
		pulse_data[i] = pulse_data[i + 1];
	}
	pulse_data[BUF_SIZE - 1] = v;
	// 
}

static void prvvHeartRateCalct(void *param)
{
	memcpy(pulse_sort, pulse_data, BUF_SIZE * 2);
	uint8_t value = _sort_(pulse_sort, BUF_SIZE); //
	if (xFitShowAllData.Heart == 0 || pulse_data[0] == 0)
	{
		if (value)
			value = 72;
	}
	else if (pulse_sort[0] - pulse_sort[BUF_SIZE - 1] > 15)
	{
		value = ((uint16_t)xFitShowAllData.Heart * 15 + value) / 16;
	}
	if (value)
		xFitShowAllData.Heart = _avg_(pulse_avg, BUF_SIZE, value);
	//
}

static void prvvHeartRateDataClean(void)
{
	xFitShowAllData.Heart = 0;
	for (uint8_t i = 0; i < BUF_SIZE; i++)
	{
		pulse_data[i] = 0;
		pulse_avg[i] = 72;
	}
	//
}

HeartRateSensor_t *pGetHeartRateSensorHandler(void)
{
	return &xHeartRateSensor;
}



