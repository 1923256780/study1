#include "PulseSystemConfig.h"





//  ȫ�ֱ�������

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
//xSpeedSensor.EncoderModeAcquire = prvvEncoderModePulseAcquire; //˫���崦����
//	int_mode	
//}
//static void prvvSpeedSensorStart(void)  
//{
//	
//// ��ʼ��ʱ���Ϊ0  
//    xSpeedSensor.ulTimeDiff = 0;  
//  
//    // ��ȡ��ǰϵͳʱ�������Ϊ�ٶȴ�������ǰһ��ʱ���  
//    xSpeedSensor.ulPreTimeStamp = ulGetDeviceSysTick();  

//	 // �жϴ�����ʹ�õ�����ͨ������  
//    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)  
//		{
//			gpio_config
//		gpio_set_int_mode
//		// ����Pin_A���Ż��ѹ���  
//        REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);
//			// ����˫������������ʵ����ǰһ����������ʱ���  
//        pDoublePulseEncoderCheck->ucPreStartStrokeTimeStamp = ulGetDeviceSysTick();
//		}
//		else  
//    {   
//        gpio_config
//        gpio_set_int_mode 
//        // ����Pin���Ż��ѹ���  
//        REG_GPIO_WUATOD_ENABLE |= FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);  
//    }  
//}

//static void prvvResetData(void)  
//{  
//    // ������������  
//    xSpeedSensor.xPulseSensorBase.NumberOfPulse = 0;  
//    // ���ü�������  
//    xSpeedSensor.NumberOfCount = 0;  
//} 

//static void prvvSpeedSensorStop(void)  
//{  
//    // �����ٶȴ�����  
//    xSpeedSensor.ucEnable = false;  
//    // ����ٶȴ�����������  
//    prvvResetData(void) 
//  
//    // �жϴ�����ʹ�õ�����ͨ������  
//    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)  
//    {  
//        // ����Pin_A���ŵĻ��ѹ���  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);  
//        // ����Pin_B���ŵĻ��ѹ���  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_B);  
//    }  
//    else  
//    {  
//        // ����Pin���ŵĻ��ѹ���  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);  
//    }  
//}

//static void prvvSpeedSensorPause(void)  
//{  
//    // �����ٶȴ�����  
//    xSpeedSensor.ucEnable = false;  
//  
//    // �жϴ�����ʹ�õ�����ͨ������  
//    if (xSpeedSensor.xPulseSensorBase.ucNumberOfChannels == 2)  
//    {  
//        // ����Pin_A���ŵĻ��ѹ���  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_A);  
//        // ����Pin_B���ŵĻ��ѹ���  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pDoublePulseAcquisition->Pin_B);  
//    }  
//    else  
//    {  
//        // ����Pin���ŵĻ��ѹ���  
//        REG_GPIO_WUATOD_ENABLE &= ~FLAG_GPIO_IN_32Bit(pSinglePulseAcquisition->Pin);  
//    }  
//}

////�����кͳ��̲������ж���С���������������ж�
//static void prvvSpeedSensorCalculate(void)  
//{  
////    // ����ٶȴ�����δ���ã���ֱ�ӷ���  
////    if (xSpeedSensor.ucEnable == false)  
////    {  
////        return;  
////    }  
//    // ��������¼�������û���¼�����ֱ�ӷ���  
//    if (xPulseEventQueue.ucCount == 0)  
//    {  
//        return;  
//    }  
//    // ���ݴ������Ĳɼ�ģʽ���д���  
//    if (xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE || \
//        xSpeedSensor.AcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2)  
//    {  
//        prvvEncoderModePulseAcquire();
//        // ������ɺ󷵻�  
//        return;  
//    }  
//    else  
//    {  
//        // �������˫����ӱ�����ģʽ������е�����ģʽ�Ĵ���  
//        // ��������¼���ʱ���С�ڻ������һ�ε�ʱ�������ֱ�ӷ���  
//        if (xPulseEvent.TimeStamp <= xSpeedSensor.ulPreTimeStamp)  
//        {  
//            return;   
//        }  
//        // ����ʱ���  
//        xSpeedSensor.ulTimeDiff = xPulseEvent.TimeStamp - xSpeedSensor.ulPreTimeStamp;  
//        // ������һ�ε�ʱ���  
//        xSpeedSensor.ulPreTimeStamp = xPulseEvent.TimeStamp;  
//        // ���ʱ����������С�����Χ����ֱ�ӷ���  
//        if ((xSpeedSensor.ulTimeDiff > xSpeedSensor.usMaxInterval) || (xSpeedSensor.ulTimeDiff < xSpeedSensor.usMinInterval))  
//        {  
//            return;  
//        }  
//    }  
//    // �����ٶȴ������ļ��㴦����  
//    xSpeedSensor.CalcHandler(&xSpeedSensor);  
//}

//// ����һ�����ڴ洢�ٶȵ����飬����ΪSPEED_BUFF_LEN  
//static uint16_t aSpeed[SPEED_BUFF_LEN];  
//  
//// ����һ�����ڴ洢Ƶ�ʵ����飬����ΪSPEED_BUFF_LEN  
//static uint16_t aFrequency[SPEED_BUFF_LEN]; 

//// ����ٶȴ������ٶ����ݻ������ĺ���  
//static void prvvSpeedSensorSpeedDataBuffClean(void)  
//{  
//    // ���ٶ������е�����Ԫ����Ϊ0  
//    memset(aSpeed, 0, sizeof(aSpeed));  
//      
//    // ��Ƶ�������е�����Ԫ����Ϊ0  
//    memset(aFrequency, 0, sizeof(aFrequency));  
//      
//    // �����������ݵĺ������ú����ľ���ʵ���ڴ�����δ������  
//    prvvResetData();  
//}

////�����µ����ݼ��㴦��
//static void prvvSinglePulseCalc(void *param)  
//{  
//    // ������Ĳ���ת��ΪSpeedSensor_t����ָ��  
//    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;  
//      
//    // ��ʼ���ٶȱ���  
//    uint16_t usSpeed = 0;  
//      
//    // �������崫�������ú�ϵͳʱ�������ٶ�  
//    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;  
//      
//    // ��������еĵ�λ��Ӣ�Ƶ�λ��Imperial��  
//    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//    {  
//        // ���ٶȴӹ���ת��ΪӢ��  
//        usSpeed = usSpeed * 100 / 161;  
//    }  
//      
//    // ���ÿ������������������10  
//	if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)  
//	{  
//		// ���������������ٶ�ֵ  
//		usSpeed = usSpeed * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;  
//	}  
//      
//    // ������õ����ٶ�ֵ����ƽ������������ȫ���ٶȱ���  
//    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);  
//      
//    // �����ٶȴ��������ٶȱ��ʵ����ٶ�ֵ  
//		xFitShowAllData.Speed = xFitShowAllData.Speed * 100 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.Speed_Rate;  
//      
//    // ��ʼ��Ƶ�ʱ���  
//    uint16_t usFreq = 0;  
//      
//    // ����ϵͳʱ������Ƶ��  
//    usFreq = 60 * 1000 / pSpeedSensor->ulTimeDiff;  
//      
//    // ���ÿ������������������10  
//    if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)  
//    {  
//        // ��������������Ƶ��ֵ  
//        usFreq = usFreq * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;  
//    }  
//      
//    // ������õ���Ƶ��ֵ����ƽ������������ȫ��Ƶ�ʱ���  
//    xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);  
//      
//    // ���㹦�ʣ�����򵥵�ͨ�����ٶ�����һλ��ʵ�֣�������2��  
//    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);  
//      
//    // �������崫�������������  
//    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;  
//      
//    // �����豸�������߶�ʱ���ĺ���  
//		prvvDeviceResetSleepTimer2();  
//		
//    // ��������������������������ÿ������������������10���������Ӧ����  
//		pSpeedSensor->NumberOfCount = (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10) ? \
//																		(pSpeedSensor->xPulseSensorBase.NumberOfPulse * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount) : \
//																		pSpeedSensor->xPulseSensorBase.NumberOfPulse;  
//			
//		// ������������������ֵ��ȫ�ֱ���  
//		xFitShowAllData.Count = pSpeedSensor->NumberOfCount;  
//			
//		// ��ʼ���������  
//		uint32_t ulDistance = 0;  
//			
//		// ������������ÿ����ľ�������ܾ��루��λΪ���ף�  
//		ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / 1000);  
//			
//		// ��������еĵ�λ��Ӣ�Ƶ�λ��Imperial��  
//		// Calories calcries base on Metric �����ע���ƺ���ƴд����Ӧ���ǡ�Calories calculation based on Metric��  
//		if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//		{  
//				// ������ӹ���ת��ΪӢ��  
//				ulDistance = ulDistance * 100 / 161;  
//		}  
//			
//		// ���ÿ������������������10  
//		if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount > 10)  
//		{  
//				// ������������������ֵ  
//				ulDistance = ulDistance * 10 / pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount;  
//		}  
//			
//		// ���ݾ���ֵ�ж��Ƿ���Ҫ���ø�λ��־λ  
//		if (ulDistance >= 0x8000)  
//		{  
//				// ���������ڻ����0x8000�������10�����ø�λ��־λ  
//				xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;  
//		}  
//		else  
//		{  
//				// ����ֱ�Ӹ�ֵ  
//				xFitShowAllData.Distance = ulDistance;  
//		}  
//			
//		// �����·������ǻ��ھ����  
//		if (pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesCalculationMethod == FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_DISTANCE)  
//		{  
//				// ���㿨·��ֵ��ÿ0.01ǧ����Ӧ0.001�ף�  
//				// 0.1kCal; �����ע���ƺ���˵����·��ֵ����0.1ǧ��Ϊ��λ��  
//				xFitShowAllData.Calories = ulDistance * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000; // x/(0.01kCal * 0.001m)  
//			
//				// ��������еĵ�λ��Ӣ�Ƶ�λ��Imperial��  
//				if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//				{  
//						// ����·��ֵ�ӹ���ת��ΪӢ��  
//						xFitShowAllData.Calories = xFitShowAllData.Calories * 161 / 100;  
//				}  
//		}
//    else  
//		{  
//				// ����·��ֵת��Ϊ0.1ǧ����λ  
//				//turn to 0.1kCal;  
//				xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / 1000;  
//		}  
//			
//		// ���岢��ʼ��һ�����ڱ�ʾ�������ڵı���  
//		uint32_t SlowDownPeriod = 0;  
//			
//		// ����������ڣ��������ulTimeDiff��ʾ�ٶȴ�������ʱ���  
//		SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;  
//			
//		// �����������Ƿ񳬹���5�루500���룩  
//		if (SlowDownPeriod > 500)  
//		{  
//				// ����������������������Ϊ5��  
//				SlowDownPeriod = 500; // 5s  
//		}  
//		else  
//		{  
//				// ���򣬼�����������Ϊ250����  
//				SlowDownPeriod = 250;  
//		}  
//			
//		// ���ü��ٶ�ʱ���Ĺ���ʱ��  
//		SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);  
//			
//		// ���ü��ٶ�ʱ��  
//		SoftTimerReset(pSlowDownTimer);  
//			
//		// �������ٶ�ʱ��  
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

//// ����һ����̬���������ڼ��㳤���̲�ģʽ�µ�����  
//static void prvvLongShortWaveMode_1_Calc(void *param)  
//{  
//    // ������Ĳ���ת��ΪSpeedSensor_t���͵�ָ��  
//    SpeedSensor_t *pSpeedSensor = (SpeedSensor_t *)param;  
//      
//    // ����һ�����ڴ洢�ٶȵı���  
//    uint16_t usSpeed = 0;  
//      
//    // �����ٶȣ���λ������ת/�����/��ȣ�����ȡ����DistancePerPulse��ulTimeDiff�ĺ���  
//    usSpeed = 360 * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse / pSpeedSensor->ulTimeDiff;  
//      
//    // ���ϵͳ���õĵ�λ��Ӣ�Ƶ�λ  
//    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//    {  
//        // ���ٶ�ת��ΪӢ�Ƶ�λ��������Ӣ��/Сʱ��  
//        usSpeed = usSpeed * 100 / 161;  
//    }  
//      
//    // ʹ��ƽ��ֵ�������㵱ǰ�ٶȵ�ƽ��ֵ������ֵ��ȫ�ֱ���xFitShowAllData.Speed  
//    xFitShowAllData.Speed = _avg_(aSpeed, sizeof(aSpeed) / sizeof(uint16_t), usSpeed);  
//      
//    // ���㹦�ʣ�����򵥵�ͨ������һλ�����Ƽ��㣨�൱�ڳ���2��  
//    xFitShowAllData.Power = (xFitShowAllData.Speed >> 1);  
//      
//    // ����������һ  
//    pSpeedSensor->xPulseSensorBase.NumberOfPulse++;  
//      
//    // ���ú��������豸��˯�߶�ʱ��  
//    prvvDeviceResetSleepTimer2();  
//      
//    // ����һ�����ڴ洢����ı���  
//    uint32_t ulDistance = 0;  
//      
//    // ������ʻ���ܾ���  
//    ulDistance = (uint32_t)(pSpeedSensor->xPulseSensorBase.NumberOfPulse * pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse) / 1000;  
//      
//    // ���ϵͳ���õĵ�λ��Ӣ�Ƶ�λ  
//    if (pFitShowSystemConfig->pSystemConfigInformation->ucUnit == Imperial)  
//    {  
//        // ������ת��ΪӢ�Ƶ�λ��������Ӣ�  
//        ulDistance = ulDistance * 100 / 161;  
//    }  
//      
//    // ���������ڻ����0x8000����32768��  
//    if (ulDistance >= 0x8000 )  
//    {  
//        // ���������10�����ø�λΪ1����ֵ��ȫ�ֱ���xFitShowAllData.Distance  
//        xFitShowAllData.Distance = (ulDistance / 10) | 0x8000;  
//    }  
//    else  
//    {  
//        // ����ֱ�Ӹ�ֵ  
//        xFitShowAllData.Distance = ulDistance;  
//    }  
//      
//    // �������ĵĿ�·�ע������ļ��㵥λ�Ѿ���0.1ǧ��  
//    xFitShowAllData.Calories = pSpeedSensor->xPulseSensorBase.NumberOfPulse * \
//                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse * \
//                                pFitShowSystemConfig->pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate / \
//                                (1000 * 1000);  
//      
//    // ��ȡ��ǰ��ϵͳʱ���  
//    uint32_t TimeStamp = ulGetDeviceSysTick();  
//      
//    // ���㱾�μ�����ǰһ�μ���֮���ʱ���  
//    uint32_t ulCurTimeDiff = TimeStamp - pLongShortWaveSensor->PreTimeStamp;  
//      
//    // ����ǰһ�μ����ʱ���  
//    pLongShortWaveSensor->PreTimeStamp = TimeStamp;  

//    // �жϵ�ǰʱ����Ƿ����ǰһ�ε�ʱ���  
//		if (ulCurTimeDiff > pLongShortWaveSensor->PreTimeDiff)  
//		{  
//				// �����ǰʱ�����ǰһ��ʱ���֮�����ǰһ��ʱ�����仯�ʵĳ˻��İٷֱ�  ,,ChangeOfRate = 10
//				if ((ulCurTimeDiff - pLongShortWaveSensor->PreTimeDiff) > (pLongShortWaveSensor->PreTimeDiff * pLongShortWaveSensor->ChangeOfRate / 100))  
//				{  
//						// ��Ԥ����״̬����Ϊ����������  
//						pLongShortWaveSensor->PrePulseState = LongPulseType;  
//						// ���������¼�  
//						pLongShortWaveSensor->EventTrigger(LongShortWave_DecelerationEvent);  
//				}  
//		}  
//		else  
//		{  
//				// ���ǰһ��ʱ����뵱ǰʱ���֮�����ǰһ��ʱ�����仯�ʵĳ˻��İٷֱ�  
//				if ((pLongShortWaveSensor->PreTimeDiff - ulCurTimeDiff) > (pLongShortWaveSensor->PreTimeDiff * pLongShortWaveSensor->ChangeOfRate / 100))  
//				{  
//						// ���Ԥ����״̬Ϊ����������  
//						if (pLongShortWaveSensor->PrePulseState == LongPulseType)  
//						{  
//								// ���¼���ʱ���  
//								pLongShortWaveSensor->TimeDiff = TimeStamp - pLongShortWaveSensor->FirstAccelerationTimeStamp;  
//								// �����״μ���ʱ���  
//								pLongShortWaveSensor->FirstAccelerationTimeStamp = TimeStamp;  
//						}  
//						// ��Ԥ����״̬����Ϊ����������  
//						pLongShortWaveSensor->PrePulseState = ShortPulseType;  
//						// ���������¼�  
//						pLongShortWaveSensor->EventTrigger(LongShortWave_AccelerationEvent);  
//				}  
//		}  
//		// ����ǰһ�ε�ʱ���  
//		pLongShortWaveSensor->PreTimeDiff = ulCurTimeDiff;  
//		// ɨ�贫����  
//		pLongShortWaveSensor->Scan();  
//		// ����������ı�־Ϊ1  
//		if (pLongShortWaveSensor->Flag == 1)  
//		{  
//				// ����־����Ϊ0  
//				pLongShortWaveSensor->Flag = 0;  
//				// ������һ  
//				pSpeedSensor->NumberOfCount++;  
//				// ����ȫ�������еļ���  
//				xFitShowAllData.Count = pSpeedSensor->NumberOfCount;  
//				// ����Ƶ��  
//				uint16_t usFreq = 60 * 1000 / pLongShortWaveSensor->TimeDiff;  
//				// ����Ƶ�ʵ�ƽ��ֵ  
//				xFitShowAllData.Freq = _avg_(aFrequency, sizeof(aFrequency) / sizeof(uint16_t), usFreq);  
//		}  
//			
//		// ��ʼ���������ڱ���  
//		uint32_t SlowDownPeriod = 0;  
//		// �����������  
//		SlowDownPeriod = pSpeedSensor->ulTimeDiff / 5;  
//		// ����������ڴ���500����  
//		if (SlowDownPeriod > 500)  
//		{  
//				// ��������������Ϊ500���루��5�룩  
//				SlowDownPeriod = 500; // 5s  
//		}  
//		else  
//		{  
//				// ���򽫼�����������Ϊ250����  
//				SlowDownPeriod = 250;  
//		}  
//		// ������ʱ���Ĺ���ʱ��  
//		SoftTimerSetExpire(pSlowDownTimer, SlowDownPeriod);  
//		// ������ʱ��  
//		SoftTimerReset(pSlowDownTimer);  
//		// ������ʱ��  
//		SoftTimerStart(pSlowDownTimer);
//}















////���ٹ��̴���
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

//// �����ٶ�
//float calculateSpeed() {
//    // ������������֮���ʱ����
//    uint32_t timeInterval = /* ����ʱ���� */;
//    
//    // �����ٶȣ���λ����/�룩
//    float speed = (WHEEL_CIRCUMFERENCE * PULSE_PER_REVOLUTION) / (timeInterval * TIMER_FREQUENCY);
//    
//    return speed;
//}

//// �ƶ�ƽ���˲��㷨
//float movingAverageFilter(float currentSpeed) {
//    static float speedBuffer[5] = {0};  // �������飬�洢���5���ٶ�����
//    static uint8_t bufferIndex = 0;
//    float sum = 0;
//    
//    // ����ǰ�ٶȷ��뻺������
//    speedBuffer[bufferIndex] = currentSpeed;
//    bufferIndex = (bufferIndex + 1) % 5;  // ���»�����������
//    
//    // ���㻺�������е������ܺ�
//    for (int i = 0; i < 5; i++) {
//        sum += speedBuffer[i];
//    }
//    
//    // ����ƽ���ٶ�
//    float averageSpeed = sum / 5;
//    
//    return averageSpeed;
//}






