/*
 * @Author: your name
 * @Date: 2021-09-02 13:56:48
 * @LastEditTime: 2023-05-24 10:53:07
 * @LastEditors: LHB
 * @Description: In User Settings Edit
 * @FilePath: \1.1.1正式 - (调试)Dbeta发货改参数用1.1.1D正式版-流水号 -调试\src\FitShow\SpeedSensor.c
 */

#include "SpeedSensor.h"
#include "PulseSensorBase.h"
#include "SystemConfig.h"
#include "stdio.h"
#include "Device.h"
#include "xSoftTimer.h"
#include "LongShortWave.h"
#include "Regression.h"
#include "FitShowSort.h"

static void prvvSpeedSensorInit(void);
static void prvvSpeedSensorSpeedDataBuffClean(void);
static void prvvSpeedSensorStart(void);
static void prvvSpeedSensorStop(void);
static void prvvSpeedSensorPause(void);
static void prvvSpeedSensorLoop(void);
static void prvvEncoderModePulseAcquire(void *Pin);
static void prvvSpeedSensorPulseEventTrigger(void *param);
static void prvvResetData(void);

static void prvvSinglePulseCalc(void *param);
static void prvvLongShortWaveMode_1_Calc(void *param);
static void prvvLongShortWaveMode_2_Calc(void *param);
static void prvvDoublePulseCalc(void *param);
static void prvvDoublePulseCalc_2(void *param);

static QueueElement_t xPulseEventQueueBuff[10];
static CircleQueue_t xPulseEventQueue;

static FitShowSystemConfig_t *pFitShowSystemConfig;

static LongShortWave_t *pLongShortWaveSensor;
static DoublePulseEncoderCheck_t *pDoublePulseEncoderCheck = NULL;

static uint32_t ulTimeStamp = 0;

static SpeedSensor_t xSpeedSensor =
    {
        .Init = prvvSpeedSensorInit,
        .CleanSpeedBuff = prvvSpeedSensorSpeedDataBuffClean,
};

static void prvvSpeedSensorInit(void)
{
    pFitShowSystemConfig = pGetFitShowSystemConfig();
    vQueueInit(&xPulseEventQueue, xPulseEventQueueBuff, sizeof(xPulseEventQueueBuff) / sizeof(PulseEvent_t));
    xSpeedSensor.Start = prvvSpeedSensorStart;
    xSpeedSensor.Stop = prvvSpeedSensorStop;
    xSpeedSensor.Pause = prvvSpeedSensorPause;
    xSpeedSensor.CalcHandler = prvvSinglePulseCalc;
    xSpeedSensor.Loop = prvvSpeedSensorLoop;
    xSpeedSensor.Trigger = prvvSpeedSensorPulseEventTrigger;
    xSpeedSensor.CleanData = prvvResetData;

    xSpeedSensor.usMaxInterval = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.usMaxInterval;
    xSpeedSensor.usMinInterval = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.usMinInterval;
    xSpeedSensor.xPulseSensorBase.ucNumberOfChannels = 1;
    xSpeedSensor.AcquisitionMode = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.CountAcquisitionMode;

    static SinglePulseAcquisition_t xSinglePulseAcquisition;
    xSpeedSensor.xPulseSensorBase.PulseConfig = &xSinglePulseAcquisition;
    xSinglePulseAcquisition.Pin = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.Pin;
    xSinglePulseAcquisition.Properties.Pull = (Pull_Type)pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.PullType;
    xSinglePulseAcquisition.Properties.IntterruptMode = (int_mode)pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.InterruptMode;
    switch (xSpeedSensor.AcquisitionMode)
    {
    case FITSHOW_DEVICE_PARAM_SINGLE_PULSE_MODE:
        /**
         *     _         _         _         _  
         *  __| |__...__| |__...__| |__...__| |__ 
         */
        if (xSinglePulseAcquisition.Properties.IntterruptMode == RISE_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_HIGHT;
        }
        if (xSinglePulseAcquisition.Properties.IntterruptMode == FALL_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_LOW;
        }
        xSpeedSensor.CalcHandler = prvvSinglePulseCalc;
        break;
    case FITSHOW_DEVICE_PARAM_LONG_SHORT_WAVE_MODE:
        /**
         *     _   _    _      _              
         *  __| |_| |__| |____| |_____________
         *    |.................|             
         *    Gradually increase
         */
        if (xSinglePulseAcquisition.Properties.IntterruptMode == RISE_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_HIGHT;
        }
        if (xSinglePulseAcquisition.Properties.IntterruptMode == FALL_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_LOW;
        }
        xSpeedSensor.CalcHandler = prvvLongShortWaveMode_1_Calc;
        pLongShortWaveSensor = pGetLongShortWaveHandler();
        pLongShortWaveSensor->Init();
        break;
    case FITSHOW_DEVICE_PARAM_LONG_SHORT_WAVE_MODE_2:
    {
        /**
         *     _   _   _   _          _   _   _   _  
         *  __| |_| |_| |_| |________| |_| |_| |_| |___
         *    |.........|T1 |   T2  |..............|
         *                 T2 > 2*T1
         */
        if (xSinglePulseAcquisition.Properties.IntterruptMode == RISE_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_HIGHT;
        }
        if (xSinglePulseAcquisition.Properties.IntterruptMode == FALL_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_LOW;
        }
        xSpeedSensor.CalcHandler = prvvLongShortWaveMode_2_Calc;
        pLongShortWaveSensor = pGetLongShortWaveHandler();
        pLongShortWaveSensor->Init();
        break;
    }
    case FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2:
    {
        /**
         *     ___          ___
         *  __|   |________|   |__      A
         *        ___     ___    
         *  _____|   |___|   |____      B
         */
        xSpeedSensor.xPulseSensorBase.ucNumberOfChannels = 2;
        static DoublePulseAcquisition_t xDoublePulseAcquisition;
        xSpeedSensor.xPulseSensorBase.PulseConfig = &xDoublePulseAcquisition; // Reset PulseConfig
        xDoublePulseAcquisition.Pin_A = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.Pin;
        xDoublePulseAcquisition.Pin_B = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xAddition_GPIO.Pin;
        xDoublePulseAcquisition.Properties.Pull = (Pull_Type)pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.PullType;
        xDoublePulseAcquisition.Properties.IntterruptMode = (int_mode)pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.InterruptMode;
        xSpeedSensor.CalcHandler = prvvDoublePulseCalc_2;
        pDoublePulseEncoderCheck = pGetPulseEncoderHandler();
        pDoublePulseEncoderCheck->Init();
        xSpeedSensor.EncoderModeAcquire = prvvEncoderModePulseAcquire;
        if (xDoublePulseAcquisition.Properties.IntterruptMode == RISE_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_HIGHT;
        }
        if (xDoublePulseAcquisition.Properties.IntterruptMode == FALL_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_LOW;
        }
        
        break;
    }
    case FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE:
    {
        /**
         *     ___          ___
         *  __|   |________|   |__      A
         *        ___     ___    
         *  _____|   |___|   |____      B
         */
        xSpeedSensor.xPulseSensorBase.ucNumberOfChannels = 2;
        static DoublePulseAcquisition_t xDoublePulseAcquisition;
        xSpeedSensor.xPulseSensorBase.PulseConfig = &xDoublePulseAcquisition; // Reset PulseConfig
        xDoublePulseAcquisition.Pin_A = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.Pin;
        xDoublePulseAcquisition.Pin_B = pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xAddition_GPIO.Pin;
        xDoublePulseAcquisition.Properties.Pull = (Pull_Type)pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.PullType;
        xDoublePulseAcquisition.Properties.IntterruptMode = (int_mode)pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.InterruptMode;
        xSpeedSensor.CalcHandler = prvvDoublePulseCalc;
        pDoublePulseEncoderCheck = pGetPulseEncoderHandler();
        pDoublePulseEncoderCheck->Init();
        xSpeedSensor.EncoderModeAcquire = prvvEncoderModePulseAcquire;
        if (xDoublePulseAcquisition.Properties.IntterruptMode == RISE_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_HIGHT;
        }
        if (xDoublePulseAcquisition.Properties.IntterruptMode == FALL_EDGE)
        {
            xSpeedSensor.ucInterruptLevel = VOLTAGE_LOW;
        }
        
        break;
    }
    default:
        break;
    }
}

/**
 * @description: 
 * @param {*}
 * @return {*}
 */
static void prvvSpeedSensorStart(void)
{
    xSpeedSensor.ucEnable = true;
    xSpeedSensor.ulTimeDiff = 0;
    xSpeedSensor.ulPreTimeStamp = ulGetDeviceSysTick();
    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)
    {
        DoublePulseAcquisition_t *pDoublePulseAcquisition = xSpeedSensor.xPulseSensorBase.PulseConfig;
        gpio_config(pDoublePulseAcquisition->Pin_A, INPUT, pDoublePulseAcquisition->Properties.Pull);
        gpio_config(pDoublePulseAcquisition->Pin_B, INPUT, pDoublePulseAcquisition->Properties.Pull);
        gpio_set_int_mode(pDoublePulseAcquisition->Pin_A, pDoublePulseAcquisition->Properties.IntterruptMode);
        gpio_set_int_mode(pDoublePulseAcquisition->Pin_B, pDoublePulseAcquisition->Properties.IntterruptMode);
        REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);
        REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_B);
        pDoublePulseEncoderCheck->ucPreStartStrokeTimeStamp = ulGetDeviceSysTick();
    }
    else
    {
        SinglePulseAcquisition_t *pSinglePulseAcquisition = xSpeedSensor.xPulseSensorBase.PulseConfig;
        gpio_config(pSinglePulseAcquisition->Pin, INPUT, pSinglePulseAcquisition->Properties.Pull);
        gpio_set_int_mode(pSinglePulseAcquisition->Pin, pSinglePulseAcquisition->Properties.IntterruptMode);
        REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);
    }
    
}

static void prvvResetData(void)
{
    xSpeedSensor.xPulseSensorBase.NumberOfPulse = 0;
    xSpeedSensor.NumberOfCount = 0;
}

/**
 * @description: 
 * @param {*}
 * @return {*}
 */
static void prvvSpeedSensorStop(void)
{
    xSpeedSensor.ucEnable = false;
    xSpeedSensor.CleanData();
    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)
    {
        DoublePulseAcquisition_t *pDoublePulseAcquisition = NULL;
        pDoublePulseAcquisition = xSpeedSensor.xPulseSensorBase.PulseConfig;
        gpio_config(pDoublePulseAcquisition->Pin_A, FLOAT, pDoublePulseAcquisition->Properties.Pull);
        gpio_config(pDoublePulseAcquisition->Pin_B, FLOAT, pDoublePulseAcquisition->Properties.Pull);
        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);
        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_B);
    }
    else
    {
        SinglePulseAcquisition_t *pSinglePulseAcquisition = NULL;
        pSinglePulseAcquisition = xSpeedSensor.xPulseSensorBase.PulseConfig;
        gpio_config(pSinglePulseAcquisition->Pin, FLOAT, pSinglePulseAcquisition->Properties.Pull);
        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);
    }
}

static void prvvSpeedSensorPause(void)
{
    xSpeedSensor.ucEnable = false;
    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)
    {
        DoublePulseAcquisition_t *pDoublePulseAcquisition = NULL;
        pDoublePulseAcquisition = xSpeedSensor.xPulseSensorBase.PulseConfig;
        gpio_config(pDoublePulseAcquisition->Pin_A, FLOAT, pDoublePulseAcquisition->Properties.Pull);
        gpio_config(pDoublePulseAcquisition->Pin_B, FLOAT, pDoublePulseAcquisition->Properties.Pull);
        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);
        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_B);
    }
    else
    {
        SinglePulseAcquisition_t *pSinglePulseAcquisition = NULL;
        pSinglePulseAcquisition = xSpeedSensor.xPulseSensorBase.PulseConfig;
        gpio_config(pSinglePulseAcquisition->Pin, FLOAT, pSinglePulseAcquisition->Properties.Pull);
        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);
    }
}

/**
 * @description: 
 * @param {*}
 * @return {*}
 */
static void prvvSpeedSensorLoop(void)
{
    if (xSpeedSensor.ucEnable == false)
    {
        return;
    }
    if (xPulseEventQueue.ucCount == 0)
    {
        return;
    }
    PulseEvent_t xPulseEvent;
    QueueElement_t xElement;
    vQueuePop(&xPulseEventQueue, &xElement);
    xPulseEvent.Pin = xElement.EventID;
    xPulseEvent.TimeStamp = xElement.Event;
    if (xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE || \
        xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2)
    {
        DoublePulseEvent_t xDoublePulseEvent;
        xDoublePulseEvent.ActivePin = xElement.EventID;
        xDoublePulseEvent.AnotherPinStatus = xElement.Event;
        xSpeedSensor.EncoderModeAcquire(&xDoublePulseEvent);
        return;
    }
    else
    {
        if (xPulseEvent.TimeStamp <= xSpeedSensor.ulPreTimeStamp)
        {
            return; 
        }
        xSpeedSensor.ulTimeDiff = xPulseEvent.TimeStamp - xSpeedSensor.ulPreTimeStamp;
        xSpeedSensor.ulPreTimeStamp = xPulseEvent.TimeStamp;
        if ((xSpeedSensor.ulTimeDiff > xSpeedSensor.usMaxInterval) || (xSpeedSensor.ulTimeDiff < xSpeedSensor.usMinInterval))
        {
            return;
        }
    }
    xSpeedSensor.CalcHandler(&xSpeedSensor);
}


/**
 * @description: 当脉冲模式为单感模式，事件ID为gpio，事件为时间戳；
 * 当模式为双感模式时；事件ID为中断的GPIO，事件为另外一个引脚的状态；
 * @param {void} *param
 * @return {*}
 */
static void prvvSpeedSensorPulseEventTrigger(void *param)
{
    if (xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE || \
        xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2)
    {
        DoublePulseEvent_t *pDoublePulseEvent = (DoublePulseEvent_t*)param;
        QueueElement_t xElement;
        xElement.Event = pDoublePulseEvent->AnotherPinStatus;
        xElement.EventID = pDoublePulseEvent->ActivePin;
        vQueuePush(&xPulseEventQueue, xElement);    
    }
    else
    {
        PulseEvent_t *pPulseEvent = (PulseEvent_t*)param;
        QueueElement_t xElement;
        xElement.Event = pPulseEvent->TimeStamp;
        xElement.EventID = pPulseEvent->Pin;
        vQueuePush(&xPulseEventQueue, xElement);
    }
    
    // uart_printf("%s->%s->%d \r\n", __FILE__, __func__, __LINE__);
}


/**
 * double pulse mode
 *     ____          ___
 *  __|    |________|   |__      A
 *        ___     ___    
 *  _____|   |___|   |____      B
 */
static void prvvEncoderModePulseAcquire(void *param)
{
    DoublePulseEvent_t *pDoublePulseEvent = (DoublePulseEvent_t*)param;
    DoublePulseAcquisition_t *pDoublePulseAcquisition = (DoublePulseAcquisition_t *)xSpeedSensor.xPulseSensorBase.PulseConfig;
    if (pDoublePulseEvent->ActivePin == pDoublePulseAcquisition->Pin_A)
    {
        if ((pDoublePulseEncoderCheck->ucState == eEncoderIdleState) && (pDoublePulseEvent->AnotherPinStatus == (!xSpeedSensor.ucInterruptLevel)))
        {
            pDoublePulseEncoderCheck->vExecute(eA_PulseEvent);
        }
        if ((pDoublePulseEncoderCheck->ucState == eEncoderWaittingState) && (pDoublePulseEvent->AnotherPinStatus == (!xSpeedSensor.ucInterruptLevel)))
        {
            pDoublePulseEncoderCheck->vExecute(eA_PulseEvent);
        }
        if ((pDoublePulseEncoderCheck->ucState == eEncoderReversalB_State) && (pDoublePulseEvent->AnotherPinStatus == (xSpeedSensor.ucInterruptLevel)))
        {
            pDoublePulseEncoderCheck->vExecute(eA_PulseEvent);
        }
    }
    else if (pDoublePulseEvent->ActivePin == pDoublePulseAcquisition->Pin_B)
    {
        if ((pDoublePulseEncoderCheck->ucState == eEncoderForwardA_State) && (pDoublePulseEvent->AnotherPinStatus == (xSpeedSensor.ucInterruptLevel)))
        {
            pDoublePulseEncoderCheck->vExecute(eB_PulseEvent);
        }
        if ((pDoublePulseEncoderCheck->ucState == eEncoderWaittingRepeatA_State) && (pDoublePulseEvent->AnotherPinStatus == (xSpeedSensor.ucInterruptLevel)))
        {
            pDoublePulseEncoderCheck->vExecute(eB_PulseEvent);
        }
        if ((pDoublePulseEncoderCheck->ucState == eEncoderWaittingState) && (pDoublePulseEvent->AnotherPinStatus == (!xSpeedSensor.ucInterruptLevel)))
        {
            pDoublePulseEncoderCheck->vExecute(eB_PulseEvent);
        }
        if ((pDoublePulseEncoderCheck->ucState == eEncoderWaittingRepeatA_State) && (pDoublePulseEvent->AnotherPinStatus == (!xSpeedSensor.ucInterruptLevel)))
        {
            pDoublePulseEncoderCheck->vExecute(eB_PulseEvent);
        }
    }    
    if (pDoublePulseEncoderCheck->ucFlag == true)
    {
        ulTimeStamp = ulGetDeviceSysTick();
        xSpeedSensor.ulTimeDiff = ulTimeStamp - xSpeedSensor.ulPreTimeStamp;
        xSpeedSensor.ulPreTimeStamp = ulTimeStamp;
        pDoublePulseEncoderCheck->ucFlag = false;
        xSpeedSensor.CalcHandler(&xSpeedSensor);
    }
}

SpeedSensor_t *pGetSpeedSensorHandler(void)
{
    return &xSpeedSensor;
}

static uint16_t aSpeed[SPEED_BUFF_LEN];
static uint16_t aFrequency[SPEED_BUFF_LEN];

static void prvvSpeedSensorSpeedDataBuffClean(void)
{
    memset(aSpeed, 0, sizeof(aSpeed));
    memset(aFrequency, 0, sizeof(aFrequency));
    prvvResetData();
}


/**
 * @description: 
 * @param {void} *param
 * @return {*}
 */
static void prvvSinglePulseCalc(void *param)
{
    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;
    uint16_t usSpeed = 0;
    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        usSpeed = usSpeed * 100 / 161;
    }
		if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)
		{
			usSpeed = usSpeed * 10 /pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;
		}
    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);
		xFitShowAllData.Speed = xFitShowAllData.Speed * 100 /pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.Speed_Rate;

    uint16_t usFreq = 0;
    usFreq = 60 * 1000 / pSpeedSensor->ulTimeDiff;
    if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)
    {
        usFreq = usFreq * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;
    }
    xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);
    
    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);

    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;
		prvvDeviceResetSleepTimer2();	
		
    pSpeedSensor->NumberOfCount = (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10) ? \
                                    (pSpeedSensor->xPulseSensorBase.NumberOfPulse * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount) : \
                                    pSpeedSensor->xPulseSensorBase.NumberOfPulse;
    xFitShowAllData.Count = pSpeedSensor->NumberOfCount;
    uint32_t ulDistance = 0;
    ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / 1000);
    // 	// Calories calcries base on Metric
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        ulDistance = ulDistance * 100 / 161;
    }
		if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)
		{
			ulDistance = ulDistance * 10 /pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;
		}
    if (ulDistance >= 0x8000)
    {
        xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;
    }
    else
    {
        xFitShowAllData.Distance = ulDistance;
    }
    if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesCalculationMethod == FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_DISTANCE)
    {
        // 0.1kCal;
        xFitShowAllData.Calories = ulDistance *pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;// x/(0.01kCal * 0.001m)
        if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
        {
            xFitShowAllData.Calories = xFitShowAllData.Calories * 161 / 100;
        }
        
    }
    else
    {
        //turn to 0.1kCal;
        xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;
    }
    uint32_t SlowDownPeriod = 0;
    SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;
    if (SlowDownPeriod > 500)
    {
        SlowDownPeriod = 500; // 5s
    }
    else
    {
        SlowDownPeriod = 250;
    }
    SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);
    SoftTimerReset(pSlowDownTimer);
    SoftTimerStart(pSlowDownTimer);
}

/**
 * @description: 
 * @param {void} *param
 * @return {*}
 * 
 */
static void prvvLongShortWaveMode_1_Calc(void *param)
{
    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;
    uint16_t usSpeed = 0;
    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;
	if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        usSpeed = usSpeed * 100 / 161;
    }
    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);

    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);
    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;
		prvvDeviceResetSleepTimer2();	
		
    uint32_t ulDistance = 0;
    ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse) / 1000;
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        ulDistance = ulDistance * 100 / 161;
    }
    if (ulDistance >= 0x8000 )
    {
        xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;
    }
    else
    {
        xFitShowAllData.Distance = ulDistance;
    }
    // 0.1kCal;
    xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * \
                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse * \
                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / \
                                (1000 * 1000);

    uint32_t TimeStamp = ulGetDeviceSysTick();
    uint32_t ulCurTimeDiff = TimeStamp - pLongShortWaveSensor->PreTimeStamp;
    pLongShortWaveSensor->PreTimeStamp = TimeStamp;

    if (ulCurTimeDiff > pLongShortWaveSensor->PreTimeDiff)
    {
        if ((ulCurTimeDiff - pLongShortWaveSensor->PreTimeDiff) > (pLongShortWaveSensor->PreTimeDiff * pLongShortWaveSensor->ChangeOfRate / 100))
        {
            pLongShortWaveSensor->PrePulseState = LongPulseType;
            pLongShortWaveSensor->EventTrigger(LongShortWave_DecelerationEvent);
        }
    }
    else
    {
        if ((pLongShortWaveSensor->PreTimeDiff - ulCurTimeDiff) > (pLongShortWaveSensor->PreTimeDiff * pLongShortWaveSensor->ChangeOfRate / 100))
        {
            if (pLongShortWaveSensor->PrePulseState == LongPulseType)
            {
                pLongShortWaveSensor->TimeDiff = TimeStamp - pLongShortWaveSensor->FirstAccelerationTimeStamp;
                pLongShortWaveSensor->FirstAccelerationTimeStamp = TimeStamp;
            }
            pLongShortWaveSensor->PrePulseState = ShortPulseType;
            pLongShortWaveSensor->EventTrigger(LongShortWave_AccelerationEvent);
        }
    }
    pLongShortWaveSensor->PreTimeDiff = ulCurTimeDiff;
    pLongShortWaveSensor->Scan();
    if (pLongShortWaveSensor->Flag == 1)
    {
        pLongShortWaveSensor->Flag = 0;
        pSpeedSensor->NumberOfCount++;
        xFitShowAllData.Count = pSpeedSensor->NumberOfCount;
        uint16_t usFreq = 60 * 1000 / pLongShortWaveSensor->TimeDiff;
        xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);
    }

    uint32_t SlowDownPeriod = 0;
    SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;
    if (SlowDownPeriod > 500)
    {
        SlowDownPeriod = 500; // 5s
    }
    else
    {
        SlowDownPeriod = 250;
    }
    SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);
    SoftTimerReset(pSlowDownTimer);
    SoftTimerStart(pSlowDownTimer);
}

/**
 * @description: 
 * @param {void} *param
 * @return {*}
 */
static void prvvLongShortWaveMode_2_Calc(void *param)
{
    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;
    uint16_t usSpeed = 0;
    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;
	if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        usSpeed = usSpeed * 100 / 161;
    }
    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);

    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);
    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;
		prvvDeviceResetSleepTimer2();	
		
    uint32_t ulDistance = 0;
    ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse) / 1000;
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        ulDistance = ulDistance * 100 / 161;
    }
    if (ulDistance >= 0x8000 )
    {
        xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;
    }
    else
    {
        xFitShowAllData.Distance = ulDistance;
    }

    xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * \
                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse * \
                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / \
                                (1000 * 1000);
    
    uint32_t TimeStamp = ulGetDeviceSysTick();
    uint32_t ulCurTimeDiff = TimeStamp - pLongShortWaveSensor->PreTimeStamp;
    pLongShortWaveSensor->PreTimeStamp = TimeStamp;
    if (ulCurTimeDiff > (pLongShortWaveSensor->PreTimeDiff << 1))
    {
        pLongShortWaveSensor->PrePulseState = LongPulseType;
        pLongShortWaveSensor->EventTrigger(LongShortWave_DecelerationEvent);
    }
    else
    {
        if (pLongShortWaveSensor->PrePulseState == LongPulseType)
        {
            pLongShortWaveSensor->TimeDiff = TimeStamp - pLongShortWaveSensor->FirstAccelerationTimeStamp;
            pLongShortWaveSensor->FirstAccelerationTimeStamp = TimeStamp;
        }
        pLongShortWaveSensor->PrePulseState = ShortPulseType;
        pLongShortWaveSensor->EventTrigger(LongShortWave_AccelerationEvent);
    }
    pLongShortWaveSensor->PreTimeDiff = ulCurTimeDiff;
    pLongShortWaveSensor->Scan();
    if (pLongShortWaveSensor->Flag == 1)
    {
        pLongShortWaveSensor->Flag = 0;
        pSpeedSensor->NumberOfCount++;
        xFitShowAllData.Count = pSpeedSensor->NumberOfCount;
        uint16_t usFreq =  0;
        usFreq = 60 * 1000 / pLongShortWaveSensor->TimeDiff;
        xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);
    }

    uint32_t SlowDownPeriod = 0;
    SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;
    if (SlowDownPeriod > 500)
    {
        SlowDownPeriod = 500; // 5s
    }
    else
    {
        SlowDownPeriod = 250;
    }
    SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);
    SoftTimerReset(pSlowDownTimer);
    SoftTimerStart(pSlowDownTimer);
}

/**
 * @description: 
 * @param {void} *param
 * @return {*}
 */
static void prvvDoublePulseCalc(void *param)
{
    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;
    if (pDoublePulseEncoderCheck->ucState != eEncoderIdleState)
    {
        uart_printf("%s->%s->%d: status = %d \r\n", __FILE__, __func__, __LINE__, pDoublePulseEncoderCheck->ucState);
        return;
    }
    uart_printf("%s->%s->%d: status = %d \r\n", __FILE__, __func__, __LINE__, pDoublePulseEncoderCheck->ucState);
    uint16_t usSpeed = 0;
    pSpeedSensor->ulTimeDiff = pSpeedSensor->ulPreTimeStamp - pDoublePulseEncoderCheck->ucPreStartStrokeTimeStamp;
    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        usSpeed = usSpeed * 100 / 161;
    }
    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);

    uint16_t usFreq = 0;
    usFreq = 60 * 1000 / pSpeedSensor->ulTimeDiff;
    pDoublePulseEncoderCheck->ucPreStartStrokeTimeStamp = ulGetDeviceSysTick();
    xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);
    
    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);

    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;
		prvvDeviceResetSleepTimer2();	
		
    pSpeedSensor->NumberOfCount = pSpeedSensor->xPulseSensorBase.NumberOfPulse;
    xFitShowAllData.Count = pSpeedSensor->NumberOfCount;
    uint32_t ulDistance = 0;
    ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / 1000);
    // Calories calcries base on Metric
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        ulDistance = ulDistance * 100 / 161;
    }
    if (ulDistance >= 0x8000)
    {
        xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;
    }
    else
    {
        xFitShowAllData.Distance = ulDistance;
    }
    if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesCalculationMethod == FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_DISTANCE)
    {
        xFitShowAllData.Calories = ulDistance * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;   // x/(0.01kCal * 0.001m)
        if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
        {
            xFitShowAllData.Calories = xFitShowAllData.Calories * 161 / 100;
        }
    }
    else
    {
        xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;
    }
    uint32_t SlowDownPeriod = 0;
    SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;
    if (SlowDownPeriod > 500)
    {
        SlowDownPeriod = 500; // 5s
    }
    else
    {
        SlowDownPeriod = 250;
    }
    SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);
    SoftTimerReset(pSlowDownTimer);
    SoftTimerStart(pSlowDownTimer);
}


/**
 * @description: 
 * @param {void} *param
 * @return {*}
 */
static void prvvDoublePulseCalc_2(void *param)
{   
    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;
    if (pDoublePulseEncoderCheck->ucState == eEncoderIdleState)
    {
        pSpeedSensor->NumberOfCount++;
        xFitShowAllData.Count = pSpeedSensor->NumberOfCount;
        uint16_t usFreq = 0;
        usFreq = 60 * 1000 / (pSpeedSensor->ulPreTimeStamp - pDoublePulseEncoderCheck->ucPreStartStrokeTimeStamp);
        xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);
        pDoublePulseEncoderCheck->ucPreStartStrokeTimeStamp = ulGetDeviceSysTick();
        return;
    }
    uint16_t usSpeed = 0;
    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        usSpeed = usSpeed * 100 / 161;
    }
    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);

    // uint16_t usFreq = 0;
    // usFreq = 60 * 1000 / pSpeedSensor->ulTimeDiff;
    // xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);
    
    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);

    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;
		prvvDeviceResetSleepTimer2();	
		
    // pSpeedSensor->NumberOfCount = pSpeedSensor->xPulseSensorBase.NumberOfPulse;
    // xFitShowAllData.Count = pSpeedSensor->NumberOfCount;
    uint32_t ulDistance = 0;
    ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / 1000);
    // Calories calcries base on Metric
    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
    {
        ulDistance = ulDistance * 100 / 161;
    }
    if (ulDistance >= 0x8000)
    {
        xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;
    }
    else
    {
        xFitShowAllData.Distance = ulDistance;
    }
    if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesCalculationMethod == FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_DISTANCE)
    {
        xFitShowAllData.Calories = ulDistance * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;   // x/(0.01kCal * 0.001m)
        if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)
        {
            xFitShowAllData.Calories = xFitShowAllData.Calories * 161 / 100;
        }
    }
    else
    {
        xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;
    }
    uint32_t SlowDownPeriod = 0;
    SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;
    if (SlowDownPeriod > 500)
    {
        SlowDownPeriod = 500; // 5s
    }
    else
    {
        SlowDownPeriod = 250;
    }
    SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);
    SoftTimerReset(pSlowDownTimer);
    SoftTimerStart(pSlowDownTimer);
}







