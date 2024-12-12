#include "PulseSystemConfig.h"





//  全局变量声明

Pin_Interrupt_Level_e PinA_Interrupt_Level;
Pin_Interrupt_Level_e PinB_Interrupt_Level;
Pulse_Time_Stamp_t Pulse_Time_Stamp;
System_Para_Config_t System_Para_Config;

//init()
//{
//case singlePulse
//  int_mode
//case FITSHOW_DEVICE_PARAM_LONG_SHORT_WAVE_MODE:  
//  int_mode
//  pLongShortWaveSensor = pGetLongShortWaveHandler();  
//case FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2: 
//  pDoublePulseEncoderCheck = pGetPulseEncoderHandler();  
//	pDoublePulseEncoderCheck->Init();  
//xSpeedSensor.EncoderModeAcquire = prvvEncoderModePulseAcquire; //双脉冲处理函数
//	int_mode	
//}
//static void prvvSpeedSensorStart(void)  
//{
//	
//// 初始化时间差为0  
//    xSpeedSensor.ulTimeDiff = 0;  
//  
//    // 获取当前系统时间戳，作为速度传感器的前一次时间戳  
//    xSpeedSensor.ulPreTimeStamp = ulGetDeviceSysTick();  

//	 // 判断传感器使用的脉冲通道数量  
//    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)  
//		{
//			gpio_config
//		gpio_set_int_mode
//		// 启用Pin_A引脚唤醒功能  
//        REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);
//			// 设置双脉冲编码器检查实例的前一次启动脉冲时间戳  
//        pDoublePulseEncoderCheck->ucPreStartStrokeTimeStamp = ulGetDeviceSysTick();
//		}
//		else  
//    {   
//        gpio_config
//        gpio_set_int_mode 
//        // 启用Pin引脚唤醒功能  
//        REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);  
//    }  
//}

//static void prvvResetData(void)  
//{  
//    // 重置脉冲数量  
//    xSpeedSensor.xPulseSensorBase.NumberOfPulse = 0;  
//    // 重置计数数量  
//    xSpeedSensor.NumberOfCount = 0;  
//} 

//static void prvvSpeedSensorStop(void)  
//{  
//    // 禁用速度传感器  
//    xSpeedSensor.ucEnable = false;  
//    // 清除速度传感器的数据  
//    prvvResetData(void) 
//  
//    // 判断传感器使用的脉冲通道数量  
//    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)  
//    {  
//        // 禁用Pin_A引脚的唤醒功能  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);  
//        // 禁用Pin_B引脚的唤醒功能  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_B);  
//    }  
//    else  
//    {  
//        // 禁用Pin引脚的唤醒功能  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);  
//    }  
//}

//static void prvvSpeedSensorPause(void)  
//{  
//    // 禁用速度传感器  
//    xSpeedSensor.ucEnable = false;  
//  
//    // 判断传感器使用的脉冲通道数量  
//    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)  
//    {  
//        // 禁用Pin_A引脚的唤醒功能  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);  
//        // 禁用Pin_B引脚的唤醒功能  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_B);  
//    }  
//    else  
//    {  
//        // 禁用Pin引脚的唤醒功能  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);  
//    }  
//}

////若单感和长短波，则判断最小脉冲间隔，其他不判断
//static void prvvSpeedSensorCalculate(void)  
//{  
////    // 如果速度传感器未启用，则直接返回  
////    if (xSpeedSensor.ucEnable == false)  
////    {  
////        return;  
////    }  
//    // 如果脉冲事件队列中没有事件，则直接返回  
//    if (xPulseEventQueue.ucCount == 0)  
//    {  
//        return;  
//    }  
//    // 根据传感器的采集模式进行处理  
//    if (xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE || \
//        xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2)  
//    {  
//        prvvEncoderModePulseAcquire();
//        // 处理完成后返回  
//        return;  
//    }  
//    else  
//    {  
//        // 如果不是双脉冲加编码器模式，则进行单脉冲模式的处理  
//        // 如果脉冲事件的时间戳小于或等于上一次的时间戳，则直接返回  
//        if (xPulseEvent.TimeStamp <= xSpeedSensor.ulPreTimeStamp)  
//        {  
//            return;   
//        }  
//        // 计算时间差  
//        xSpeedSensor.ulTimeDiff = xPulseEvent.TimeStamp - xSpeedSensor.ulPreTimeStamp;  
//        // 更新上一次的时间戳  
//        xSpeedSensor.ulPreTimeStamp = xPulseEvent.TimeStamp;  
//        // 如果时间差超出最大或最小间隔范围，则直接返回  
//        if ((xSpeedSensor.ulTimeDiff > xSpeedSensor.usMaxInterval) || (xSpeedSensor.ulTimeDiff < xSpeedSensor.usMinInterval))  
//        {  
//            return;  
//        }  
//    }  
//    // 调用速度传感器的计算处理函数  
//    xSpeedSensor.CalcHandler(&xSpeedSensor);  
//}

//// 定义一个用于存储速度的数组，长度为SPEED_BUFF_LEN  
//static uint16_t aSpeed[SPEED_BUFF_LEN];  
//  
//// 定义一个用于存储频率的数组，长度为SPEED_BUFF_LEN  
//static uint16_t aFrequency[SPEED_BUFF_LEN]; 

//// 清除速度传感器速度数据缓冲区的函数  
//static void prvvSpeedSensorSpeedDataBuffClean(void)  
//{  
//    // 将速度数组中的所有元素置为0  
//    memset(aSpeed, 0, sizeof(aSpeed));  
//      
//    // 将频率数组中的所有元素置为0  
//    memset(aFrequency, 0, sizeof(aFrequency));  
//      
//    // 调用重置数据的函数（该函数的具体实现在代码中未给出）  
//    prvvResetData();  
//}

////单感下的数据计算处理
//static void prvvSinglePulseCalc(void *param)  
//{  
//    // 将传入的参数转换为SpeedSensor_t类型指针  
//    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;  
//      
//    // 初始化速度变量  
//    uint16_t usSpeed = 0;  
//      
//    // 根据脉冲传感器配置和系统时间差计算速度  
//    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;  
//      
//    // 如果配置中的单位是英制单位（Imperial）  
//    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//    {  
//        // 将速度从公制转换为英制  
//        usSpeed = usSpeed * 100 / 161;  
//    }  
//      
//    // 如果每个计数的脉冲数大于10  
//	if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)  
//	{  
//		// 根据脉冲数调整速度值  
//		usSpeed = usSpeed * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;  
//	}  
//      
//    // 将计算得到的速度值进行平均处理，并更新全局速度变量  
//    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);  
//      
//    // 根据速度传感器的速度比率调整速度值  
//		xFitShowAllData.Speed = xFitShowAllData.Speed * 100 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.Speed_Rate;  
//      
//    // 初始化频率变量  
//    uint16_t usFreq = 0;  
//      
//    // 根据系统时间差计算频率  
//    usFreq = 60 * 1000 / pSpeedSensor->ulTimeDiff;  
//      
//    // 如果每个计数的脉冲数大于10  
//    if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)  
//    {  
//        // 根据脉冲数调整频率值  
//        usFreq = usFreq * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;  
//    }  
//      
//    // 将计算得到的频率值进行平均处理，并更新全局频率变量  
//    xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);  
//      
//    // 计算功率（这里简单地通过将速度右移一位来实现，即除以2）  
//    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);  
//      
//    // 增加脉冲传感器的脉冲计数  
//    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;  
//      
//    // 调用设备重置休眠定时器的函数  
//		prvvDeviceResetSleepTimer2();  
//		
//    // 根据脉冲数调整脉冲计数，如果每个计数的脉冲数大于10，则进行相应调整  
//		pSpeedSensor->NumberOfCount = (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10) ? \
//																		(pSpeedSensor->xPulseSensorBase.NumberOfPulse * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount) : \
//																		pSpeedSensor->xPulseSensorBase.NumberOfPulse;  
//			
//		// 将调整后的脉冲计数赋值给全局变量  
//		xFitShowAllData.Count = pSpeedSensor->NumberOfCount;  
//			
//		// 初始化距离变量  
//		uint32_t ulDistance = 0;  
//			
//		// 根据脉冲数和每脉冲的距离计算总距离（单位为毫米）  
//		ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / 1000);  
//			
//		// 如果配置中的单位是英制单位（Imperial）  
//		// Calories calcries base on Metric 这里的注释似乎是拼写错误，应该是“Calories calculation based on Metric”  
//		if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//		{  
//				// 将距离从公制转换为英制  
//				ulDistance = ulDistance * 100 / 161;  
//		}  
//			
//		// 如果每个计数的脉冲数大于10  
//		if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)  
//		{  
//				// 根据脉冲数调整距离值  
//				ulDistance = ulDistance * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;  
//		}  
//			
//		// 根据距离值判断是否需要设置高位标志位  
//		if (ulDistance >= 0x8000)  
//		{  
//				// 如果距离大于或等于0x8000，则除以10并设置高位标志位  
//				xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;  
//		}  
//		else  
//		{  
//				// 否则直接赋值  
//				xFitShowAllData.Distance = ulDistance;  
//		}  
//			
//		// 如果卡路里计算是基于距离的  
//		if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesCalculationMethod == FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_DISTANCE)  
//		{  
//				// 计算卡路里值（每0.01千卡对应0.001米）  
//				// 0.1kCal; 这里的注释似乎是说明卡路里值是以0.1千卡为单位的  
//				xFitShowAllData.Calories = ulDistance * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000; // x/(0.01kCal * 0.001m)  
//			
//				// 如果配置中的单位是英制单位（Imperial）  
//				if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//				{  
//						// 将卡路里值从公制转换为英制  
//						xFitShowAllData.Calories = xFitShowAllData.Calories * 161 / 100;  
//				}  
//		}
//    else  
//		{  
//				// 将卡路里值转换为0.1千卡单位  
//				//turn to 0.1kCal;  
//				xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;  
//		}  
//			
//		// 定义并初始化一个用于表示减速周期的变量  
//		uint32_t SlowDownPeriod = 0;  
//			
//		// 计算减速周期，这里假设ulTimeDiff表示速度传感器的时间差  
//		SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;  
//			
//		// 检查减速周期是否超过了5秒（500毫秒）  
//		if (SlowDownPeriod > 500)  
//		{  
//				// 如果超过，则减速周期设置为5秒  
//				SlowDownPeriod = 500; // 5s  
//		}  
//		else  
//		{  
//				// 否则，减速周期设置为250毫秒  
//				SlowDownPeriod = 250;  
//		}  
//			
//		// 设置减速定时器的过期时间  
//		SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);  
//			
//		// 重置减速定时器  
//		SoftTimerReset(pSlowDownTimer);  
//			
//		// 启动减速定时器  
//		SoftTimerStart(pSlowDownTimer);
//}

//uint16_t _avg_(uint16_t *arr, uint16_t len, uint16_t value)
//{
//	uint32_t sum = value;
//	for (uint16_t i = 0; i < len - 1; i++)
//	{
//		arr[i] = arr[i + 1];
//		sum += arr[i];
//	}
//	arr[len - 1] = value;

//	return (sum / len);
//}

//// 定义一个静态函数，用于计算长波短波模式下的数据  
//static void prvvLongShortWaveMode_1_Calc(void *param)  
//{  
//    // 将传入的参数转换为SpeedSensor_t类型的指针  
//    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;  
//      
//    // 定义一个用于存储速度的变量  
//    uint16_t usSpeed = 0;  
//      
//    // 计算速度，单位可能是转/秒或米/秒等，具体取决于DistancePerPulse和ulTimeDiff的含义  
//    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;  
//      
//    // 如果系统配置的单位是英制单位  
//    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//    {  
//        // 将速度转换为英制单位（可能是英里/小时）  
//        usSpeed = usSpeed * 100 / 161;  
//    }  
//      
//    // 使用平均值函数计算当前速度的平均值，并赋值给全局变量xFitShowAllData.Speed  
//    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);  
//      
//    // 计算功率，这里简单地通过右移一位来近似计算（相当于除以2）  
//    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);  
//      
//    // 脉冲数量加一  
//    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;  
//      
//    // 调用函数重置设备的睡眠定时器  
//    prvvDeviceResetSleepTimer2();  
//      
//    // 定义一个用于存储距离的变量  
//    uint32_t ulDistance = 0;  
//      
//    // 计算行驶的总距离  
//    ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse) / 1000;  
//      
//    // 如果系统配置的单位是英制单位  
//    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//    {  
//        // 将距离转换为英制单位（可能是英里）  
//        ulDistance = ulDistance * 100 / 161;  
//    }  
//      
//    // 如果距离大于或等于0x8000（即32768）  
//    if (ulDistance >= 0x8000 )  
//    {  
//        // 将距离除以10并设置高位为1，赋值给全局变量xFitShowAllData.Distance  
//        xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;  
//    }  
//    else  
//    {  
//        // 否则直接赋值  
//        xFitShowAllData.Distance = ulDistance;  
//    }  
//      
//    // 计算消耗的卡路里，注意这里的计算单位已经是0.1千卡  
//    xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * \
//                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse * \
//                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / \
//                                (1000 * 1000);  
//      
//    // 获取当前的系统时间戳  
//    uint32_t TimeStamp = ulGetDeviceSysTick();  
//      
//    // 计算本次计算与前一次计算之间的时间差  
//    uint32_t ulCurTimeDiff = TimeStamp - pLongShortWaveSensor->PreTimeStamp;  
//      
//    // 更新前一次计算的时间戳  
//    pLongShortWaveSensor->PreTimeStamp = TimeStamp;  

//    // 判断当前时间差是否大于前一次的时间差  
//		if (ulCurTimeDiff > pLongShortWaveSensor->PreTimeDiff)  
//		{  
//				// 如果当前时间差与前一次时间差之差大于前一次时间差与变化率的乘积的百分比  ,,ChangeOfRate = 10
//				if ((ulCurTimeDiff - pLongShortWaveSensor->PreTimeDiff) > (pLongShortWaveSensor->PreTimeDiff * pLongShortWaveSensor->ChangeOfRate / 100))  
//				{  
//						// 将预脉冲状态设置为长脉冲类型  
//						pLongShortWaveSensor->PrePulseState = LongPulseType;  
//						// 触发减速事件  
//						pLongShortWaveSensor->EventTrigger(LongShortWave_DecelerationEvent);  
//				}  
//		}  
//		else  
//		{  
//				// 如果前一次时间差与当前时间差之差大于前一次时间差与变化率的乘积的百分比  
//				if ((pLongShortWaveSensor->PreTimeDiff - ulCurTimeDiff) > (pLongShortWaveSensor->PreTimeDiff * pLongShortWaveSensor->ChangeOfRate / 100))  
//				{  
//						// 如果预脉冲状态为长脉冲类型  
//						if (pLongShortWaveSensor->PrePulseState == LongPulseType)  
//						{  
//								// 重新计算时间差  
//								pLongShortWaveSensor->TimeDiff = TimeStamp - pLongShortWaveSensor->FirstAccelerationTimeStamp;  
//								// 更新首次加速时间戳  
//								pLongShortWaveSensor->FirstAccelerationTimeStamp = TimeStamp;  
//						}  
//						// 将预脉冲状态设置为短脉冲类型  
//						pLongShortWaveSensor->PrePulseState = ShortPulseType;  
//						// 触发加速事件  
//						pLongShortWaveSensor->EventTrigger(LongShortWave_AccelerationEvent);  
//				}  
//		}  
//		// 更新前一次的时间差  
//		pLongShortWaveSensor->PreTimeDiff = ulCurTimeDiff;  
//		// 扫描传感器  
//		pLongShortWaveSensor->Scan();  
//		// 如果传感器的标志为1  
//		if (pLongShortWaveSensor->Flag == 1)  
//		{  
//				// 将标志设置为0  
//				pLongShortWaveSensor->Flag = 0;  
//				// 计数加一  
//				pSpeedSensor->NumberOfCount++;  
//				// 更新全局数据中的计数  
//				xFitShowAllData.Count = pSpeedSensor->NumberOfCount;  
//				// 计算频率  
//				uint16_t usFreq = 60 * 1000 / pLongShortWaveSensor->TimeDiff;  
//				// 计算频率的平均值  
//				xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);  
//		}  
//			
//		// 初始化减速周期变量  
//		uint32_t SlowDownPeriod = 0;  
//		// 计算减速周期  
//		SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;  
//		// 如果减速周期大于500毫秒  
//		if (SlowDownPeriod > 500)  
//		{  
//				// 将减速周期设置为500毫秒（即5秒）  
//				SlowDownPeriod = 500; // 5s  
//		}  
//		else  
//		{  
//				// 否则将减速周期设置为250毫秒  
//				SlowDownPeriod = 250;  
//		}  
//		// 设置软定时器的过期时间  
//		SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);  
//		// 重置软定时器  
//		SoftTimerReset(pSlowDownTimer);  
//		// 启动软定时器  
//		SoftTimerStart(pSlowDownTimer);
//}















////减速过程处理
//static void prvvDeviceSlowDown(void)
//{
//	xFitShowAllData.Speed >>= 1;
//	xFitShowAllData.Freq >>= 1;
//	xFitShowAllData.Power >>= 1;
//	if (xFitShowAllData.Speed < 200)
//	{
//		xFitShowAllData.Speed = 0;
//		xFitShowAllData.Freq = 0;
//		xFitShowAllData.Power = 0;
//		SoftTimerPause(pSlowDownTimer);
//	}
//}

//// 计算速度
//float calculateSpeed() {
//    // 计算两次脉冲之间的时间间隔
//    uint32_t timeInterval = /* 计算时间间隔 */;
//    
//    // 计算速度（单位：米/秒）
//    float speed = (WHEEL_CIRCUMFERENCE * PULSE_PER_REVOLUTION) / (timeInterval * TIMER_FREQUENCY);
//    
//    return speed;
//}

//// 移动平均滤波算法
//float movingAverageFilter(float currentSpeed) {
//    static float speedBuffer[5] = {0};  // 缓存数组，存储最近5次速度数据
//    static uint8_t bufferIndex = 0;
//    float sum = 0;
//    
//    // 将当前速度放入缓存数组
//    speedBuffer[bufferIndex] = currentSpeed;
//    bufferIndex = (bufferIndex + 1) % 5;  // 更新缓存数组索引
//    
//    // 计算缓存数组中的数据总和
//    for (int i = 0; i < 5; i++) {
//        sum += speedBuffer[i];
//    }
//    
//    // 计算平均速度
//    float averageSpeed = sum / 5;
//    
//    return averageSpeed;
//}






