
#include <string.h>
#include "co_utils.h"
#include "SystemConfig.h"
#include "Device.h"
#include "nvds.h"
#include "app.h"
#include "gpio.h"

#define IO_TYPE_OTHERS_ENABLE 0// 0:default  1:custom

uint8_t FTMS_Uart_ReadType = OLD_ReadType;
extern DeviceInformation_t *Init_deviceBLE_Information;
FitShowBaseInformation_t Init_FactaryInformation;

uint8_t flash_write_BLEname[20];
// uint8_t flash_read_BLEname[20];

static SystemConfigInformation_t xSystemConfigInformation = 
{	 
/*0*/    .eDataAcquisitionMode = FITSHOW_DEVICE_PARAM_DATA_ACQ_PULSE_MODE,       
   // -1   FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE  FITSHOW_DEVICE_PARAM_DATA_ACQ_PULSE_MODE
#if (IO_TYPE_OTHERS_ENABLE == 0)	
    .xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.Pin = 0x01,                                                      // 0   -�?03
#else
		.xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.Pin = 0x03,
#endif
    .xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.DirectionType = (Fitshow_GPIO_DirectList_e)INPUT,
/*1*/		.xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.PullType = (Fitshow_GPIO_PullTypeList_e)PULL_HIGH,               // 1 
/*1*/	    .xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.xGPIO.InterruptMode = (Fitshow_GPIO_InterruptModeList_e)RISE_EDGE,				//																																																						
/*2*/     .xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.usMaxInterval = 65535,                                                 // 2.1
/*2*/    .xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.usMinInterval = 20,                                                    // 2.2   
    .xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.isEnable = true,

    .xSpeedPulseSensor.xAddition_GPIO.Pin = 0x02,
    .xSpeedPulseSensor.xAddition_GPIO.DirectionType = (Fitshow_GPIO_DirectList_e)INPUT,
/*3*/    .xSpeedPulseSensor.xAddition_GPIO.InterruptMode = (Fitshow_GPIO_InterruptModeList_e)RISE_EDGE,
/*4*/    .xSpeedPulseSensor.xAddition_GPIO.PullType = (Fitshow_GPIO_PullTypeList_e)PULL_HIGH,                                        // 3  senserB

#if (IO_TYPE_OTHERS_ENABLE == 0)	
    .xHeartPulseSensor.xPulsetSensorPin.xGPIO.Pin = 0x00,  //  -�?04                                                    // 0   -�?03
#else
		.xHeartPulseSensor.xPulsetSensorPin.xGPIO.Pin = 0x04,  //  -�?04
#endif
    .xHeartPulseSensor.xPulsetSensorPin.xGPIO.DirectionType = (Fitshow_GPIO_DirectList_e)INPUT,
/*4*/    .xHeartPulseSensor.xPulsetSensorPin.xGPIO.PullType = (Fitshow_GPIO_PullTypeList_e)PULL_LOW,                                 // 4
/*4*/    .xHeartPulseSensor.xPulsetSensorPin.xGPIO.InterruptMode = (Fitshow_GPIO_InterruptModeList_e)RISE_EDGE,
    .xHeartPulseSensor.xPulsetSensorPin.usMaxInterval = 4000,//
    .xHeartPulseSensor.xPulsetSensorPin.usMinInterval = 20,//
    .xHeartPulseSensor.xPulsetSensorPin.isEnable = false,

		// APP
    .xAppLink.xAppLinkSignalPin.DirectionType = (Fitshow_GPIO_DirectList_e)OUTPUT,
    .xAppLink.xAppLinkSignalPin.Pin = 0x11,                                                                                     // 5
    .xAppLink.xAppLinkSignalPin.PullType = (Fitshow_GPIO_PullTypeList_e)PULL_NONE,
    .xAppLink.xAppLinkSignalPin.InterruptMode = (Fitshow_GPIO_InterruptModeList_e)VOLTAGE_HIGHT,                                  // 6
		// APP2
    .xAppLink.xAppLinkSignalPin2.DirectionType = (Fitshow_GPIO_DirectList_e)OUTPUT,
    .xAppLink.xAppLinkSignalPin2.Pin = 0x12,                                                                                     // 5
    .xAppLink.xAppLinkSignalPin2.PullType = (Fitshow_GPIO_PullTypeList_e)PULL_NONE,
    .xAppLink.xAppLinkSignalPin2.InterruptMode = (Fitshow_GPIO_InterruptModeList_e)VOLTAGE_LOW,                                  // 6		
    // .xAppLink.OutputSignal = VOLTAGE_HIGHT,
    .xAppLink.isEnable = true,

    .ucCountDownValue = 3,//
/*6*/    .ucUnit = Metric,        //  Metric   Imperial                                                                    // 6.1
/*7*/    .xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.CountAcquisitionMode = FITSHOW_DEVICE_PARAM_SINGLE_PULSE_MODE,    //   // 7  

/*8*/    .xSpeedPulseSensor.CaloriesCalculationMethod = FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_DISTANCE, //                // 8  

/*8*/    .xSpeedPulseSensor.DistancePerPulse =  1200, // Circumference unit is mm                                                                           // 9
		.xSpeedPulseSensor.Speed_Rate = 100,  //don't change it
		.xSpeedPulseSensor.CaloriesRate_is_Change = 0,
/*9*/    .xSpeedPulseSensor.CaloriesRate = 150,  //  unit is0.1kCal/km when distance; otherwise, unit is 1Cal/cnt ,                       // 10
/*10*/    .xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount = 45,  // How many pulses to calculate a count,  unit is 0.1        �?1-10=>1�?11=1.1�?             // 11
/*11*/    .Type = 3,          // Type                         // 12
/*12*/    .Factory = 202,      // Factory                          // 13
/*13*/    .Model = 500,     // Model                    // 14
};

static DeviceInformation_t xDeviceInformation = 
{
    .arrManufacturerName = "FitShow",
    .arrModelName = "FS-BLE-V3X",
    .arrSerialNumber = "FS240329001",
    .arrHardWareRevision = "V1.0.1F",
    .arrSoftWareRevision = "V2.0.1D",
    .arrFirmWareRevision = "LowCode.23.10",
    .arrSystemID = "231011",
    .arrBLE_Name = "™FS-xxxxxx"//
};

static FitShowSystemConfig_t xFitShowSystemConfig =
{
        .Init = prvvSystemConfigInit,
        .Config = prvvFitShowSetting,
        .pSystemConfigInformation = &xSystemConfigInformation,
        .pDeviceInformation = &xDeviceInformation,
};

FitShowSystemConfig_t *pGetFitShowSystemConfig(void)
{
    return &xFitShowSystemConfig;
}

static void prvvSystemConfigInit(void)
{
// #ifdef SYS_CMD_TEST
//     pFitShow = pGetTestFitShowHandler();
// #else
//     pFitShow = pGetFitShowHandler();
// #endif
//     xFitShowSystemConfig.Config = prvvFitShowSetting;

//     // pTimerList = pGetDeviceTimerList();
//     // pSystemWriteDelayTimer = (xSoftTimer_t *)SoftTimerCreat(pTimerList, TimerID_SystemWriteDelay, 10, 0, prvvFitShowSaveData);
//     // SoftTimerPause(pSystemWriteDelayTimer);

//     nvds_tag_len_t Len = sizeof(SystemConfigInformation_t);
//     uint8_t data[sizeof(SystemConfigInformation_t)];
//     memset(data, 0, sizeof(SystemConfigInformation_t));

//     uint8_t ret = nvds_get(NVDE_TAG_FS_SYSTEM_DATA, &Len, data);
//     if (ret != NVDS_OK)
//     {
      
//         return;
//     }
//     SystemConfigInformation_t *pSystem_Buff = (SystemConfigInformation_t *)data;
//     uint16_t CheckValue = 0x0000;
//     CheckValue = usSumUp((uint8_t*)&pSystem_Buff->eDataAcquisitionMode, sizeof(SystemConfigInformation_t) - 2);
    
//     if (CheckValue != pSystem_Buff->usSUM_Check)
//     {
     
//         return;
//     }
//     /**/
//     memcpy(&xSystemConfigInformation, pSystem_Buff, Len);
    
 
    
//     if (xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulsetSensorPin.isEnable == true)
//     {
//         xFitShowSystemConfig.pSystemConfigInformation->usFunction_GPIO_List |= SPEED_PIN_A;
//     }
//     if (xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.CountAcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE || \
//         xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.CountAcquisitionMode == FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2)
//     {
//         xFitShowSystemConfig.pSystemConfigInformation->usFunction_GPIO_List |= SPEED_PIN_B;
//     }
//     if (xFitShowSystemConfig.pSystemConfigInformation->xHeartPulseSensor.xPulsetSensorPin.isEnable == true)
//     {
//         xFitShowSystemConfig.pSystemConfigInformation->usFunction_GPIO_List |= HEART_PIN;
//     }
//     if (xFitShowSystemConfig.pSystemConfigInformation->xAppLink.isEnable == true)
//     {
//         xFitShowSystemConfig.pSystemConfigInformation->usFunction_GPIO_List |= APP_LINK_PIN;
//     }
    
}

DeviceInformation_t *get_Init_deviceBLE_Information(void)
{
    return &xDeviceInformation;
}


uint16_t usSumUp(uint8_t *pData, uint8_t len)
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < len; i++)
    {
        sum += pData[i];
    }
    return sum;
}

// static void prvvSystemConfigClear(void)
// {
//     uint8_t ret = 0;
			
//     ret = nvds_del(NVDE_TAG_FS_SYSTEM_DATA);
//     uart_printf("prvvDeviceInformationSave nvds_del return %d \r\n", ret);
// }

static void prvvDeviceInformationClear(void)
{
    uint8_t ret = 0;
			
    ret = nvds_del(NVDE_TAG_DIS_DATA);
		 uart_printf("prvvDeviceInformationSave nvds_del return %d \r\n", ret);
}

static void prvvSystemConfigSave(void)
{
    // uint8_t ret = 0;
    // xSystemConfigInformation.usSUM_Check = usSumUp((uint8_t*)&xSystemConfigInformation.eDataAcquisitionMode, sizeof(SystemConfigInformation_t) - 2);
    // ret = nvds_put(NVDE_TAG_FS_SYSTEM_DATA, sizeof(SystemConfigInformation_t), (uint8_t *)&xSystemConfigInformation);
    // uart_printf("prvvDeviceInformationSave nvds_put return %d \r\n", ret);
}

static void prvvDeviceInformationSave(void)
{
    uint8_t ret = 0;
    xDeviceInformation.usSUM_Check = usSumUp((uint8_t*)&xDeviceInformation.arrManufacturerName[0], sizeof(DeviceInformation_t) - 2);
    ret = nvds_put(NVDE_TAG_DIS_DATA, sizeof(DeviceInformation_t), (uint8_t *)&xDeviceInformation);
    uart_printf("prvvDeviceInformationSave nvds_put return %d \r\n", ret);
}

static FitShowBaseInformation_t xDeviceNewData = 
{
    .usModel = 0,
		.usBrand = 0,
		.ucType = 0,
};

static void prvvFitShowConfig_Read(RequstSourceList_e RequstSource, uint8_t *pData, uint8_t size)
{
    DataBuff_t xDataPack;
    memset(&xDataPack, 0, sizeof(DataBuff_t));
    uint8_t ucIndex = 3;

    co_write8(&xDataPack.Para[xDataPack.Len], pData[3]);
    ++xDataPack.Len;

    switch (pData[ucIndex++])
    {
    case FITSHOW_DEVICE_PARAM_DATA_ACQ_MODE_READ:
        co_write8(&xDataPack.Para[xDataPack.Len], IO_or_Uart_Mode);
        ++xDataPack.Len;
        break;
    // case FITSHOW_DEVICE_PARAM_FUN_GPIO_LIST_READ:
    //     co_write16p(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->usFunction_GPIO_List);
    //     xDataPack.Len += 2;
    //     break;
    // case FITSHOW_DEVICE_PARAM_FUN_GPIO_PARAM_READ:
    // {
    //     uint16_t usFun_GPIO = 0;
    //     usFun_GPIO = co_read16p(&pData[ucIndex]);
    //     ucIndex += 2;
    //     // co_write16p(&xDataPack.Para[xDataPack.Len], usFun_GPIO);
    //     // xDataPack.Len += 2;
    //     prvvGetFitShowFun_GPIO_Param(&xDataPack, (Fun_GPIO_ConfigList_e)usFun_GPIO);
    // }
    // break;
    // // case FITSHOW_DEVICE_PARAM_WHEEL_DIAMETER_READ:
    // //     /* code */
    // //     break;
    // case FITSHOW_DEVICE_PARAM_ONE_PULSE_DISTANCE_READ:
    //     co_write16p(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.DistancePerPulse);
    //     xDataPack.Len += 2;
    //     break;
    // case FITSHOW_DEVICE_PARAM_CALORIE_CALC_MODE_READ:
    //     co_write8(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.CaloriesCalculationMethod);
    //     ++xDataPack.Len;
    //     break;
    // case FITSHOW_DEVICE_PARAM_CALORIE_RATE_READ:
    //     co_write16p(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.CaloriesRate);
    //     xDataPack.Len += 2;
    //     break;
    // case FITSHOW_DEVICE_PARAM_COUNT_ACQ_MODE_READ:
    //     co_write8(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.CountAcquisitionMode);
    //     ++xDataPack.Len;
    //     break;
    // case FITSHOW_DEVICE_PARAM_COUNT_PER_PULSE_READ:
    //     co_write8(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->xSpeedPulseSensor.xBaseSpeedSensor.xPulseSensorCalcParam.ucPulsePerCount);
    //     ++xDataPack.Len;
    //     break;
    // case FITSHOW_DEVICE_PARAM_COUNT_DOWN_VALUE_READ:
    //     co_write8(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->ucCountDownValue);
    //     ++xDataPack.Len;
    //     break;
    // case FITSHOW_DEVICE_PARAM_UNIT_READ:      
    //     co_write8(&xDataPack.Para[xDataPack.Len], xFitShowSystemConfig.pSystemConfigInformation->ucUnit);
    //     ++xDataPack.Len;
    //     break;
    default:
        break;
    }
   
    if(FTMS_Uart_ReadType == OLD_ReadType )
    {
            prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, SYS_CFG_DEVICE_PARAM, &xDataPack);//
    }
						
    switch (RequstSource)
    {
    case FitShow_DeviceConfig:
        // pFitShow->SendToUart(xDataPack.Para, xDataPack.Len);
        BLE_SendToUart0(xDataPack.Para, xDataPack.Len);
        break;
    case FitShow_AppConfig:
        // pFitShow->SendToBle(UUIDList_FFF1, xDataPack.Para, xDataPack.Len);
        BLE_SEND_DATA(xDataPack.Para, xDataPack.Len);
        break;
    default:
        break;
    }
}

static void prvvDeviceFitShowCodeSave(void)
{
    uint8_t ret = 0;
    
		xDeviceNewData.usBrand = Init_FactaryInformation.usBrand;
		xDeviceNewData.ucType = Init_FactaryInformation.ucType	;
		xDeviceNewData.usModel = Init_FactaryInformation.usModel	;
	
    xDeviceNewData.usSUM_Check = usSumUp((uint8_t*)&xDeviceNewData.FitShow_code[0], sizeof(FitShowBaseInformation_t) - 2);
    ret = nvds_put(NVDE_TAG_FSCODE_DATA, sizeof(FitShowBaseInformation_t), (uint8_t *)&xDeviceNewData);
    uart_printf("prvvDeviceFitShowCodeSave nvds_put return %d \r\n", ret);
}

static void prvvSystemConfig(RequstSourceList_e RequstSource, uint8_t *pData, uint8_t size)
{
    switch (pData[3] % 2)
    {
    case 0:
        // prvvFitShowConfig_Write(RequstSource, pData, size);
        break;
    case 1:
        prvvFitShowConfig_Read(RequstSource, pData, size);
        break;
    }
}

void prvvFitShowSetting(RequstSourceList_e RequstSource, uint8_t *pData, uint8_t size)
{
    DataBuff_t xDataPack;
    memset(&xDataPack, 0, sizeof(DataBuff_t));
    uint8_t ucIndex = 2;

		
    switch (pData[ucIndex++])
    {
    case FITSHOW_CODE_SAVE:
    {
        switch (pData[ucIndex++])
        {
        case FitShowSaveType_BLE_INFO:
            prvvDeviceInformationSave();
            break;
        case FitShowSaveType_SystemConfig:
            prvvSystemConfigSave();
        break;
        default:
            break;
        }

        co_write16p(&xDataPack.Para[xDataPack.Len], 0x0000);
        xDataPack.Len += 2;


				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_SAVE, &xDataPack);//
				}
							
        // return;
        break;
    }
        case FITSHOW_CODE_CFG_BLE_NAME://

        
        memset(&Init_deviceBLE_Information->arrBLE_Name[0], 0, 20);
        memcpy(&Init_deviceBLE_Information->arrBLE_Name[0], &pData[ucIndex], size - 5);

        // memset(&xFitShowSystemConfig.pDeviceInformation->arrBLE_Name[0], 0, 20);
        // memcpy(&xFitShowSystemConfig.pDeviceInformation->arrBLE_Name[0], &pData[ucIndex], size - 5);
				
				
        // memcpy(&flash_write_BLEname[0], &pData[ucIndex], size - 5);
        
        
				
        ucIndex += (size - 5);
        co_write16p(&xDataPack.Para[xDataPack.Len], 0x0000);
        xDataPack.Len += 2;
		
        if(FTMS_Uart_ReadType == OLD_ReadType )
        {
            prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_BLE_NAME, &xDataPack);//
        }
							
        // prprvvDeviceInformationSave(); // abo
        break;
    case SYS_CFG_BT_NAME:
        co_write16p(&xDataPack.Para[xDataPack.Len], 0xffff);
        xDataPack.Len += 2;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, SYS_CFG_BT_NAME, &xDataPack);//
				}
									
        break;
    case FITSHOW_CODE_CFG_MODEL://2
    {
        Init_FactaryInformation.usModel = co_read16p(&pData[ucIndex]);
        // xFitShowSystemConfig.pSystemConfigInformation->Model = co_read16p(&pData[ucIndex]);
        ucIndex += 2;
        uint16_t usTypeAndFactory = co_read16p(&pData[ucIndex]);
        ucIndex += 2;
        Init_FactaryInformation.usBrand = usTypeAndFactory & 0x0fff;
    
        // xFitShowSystemConfig.pSystemConfigInformation->Factory = usTypeAndFactory & 0x0fff;
        Init_FactaryInformation.ucType = (usTypeAndFactory >> 12) & 0x0f;
        // xFitShowSystemConfig.pSystemConfigInformation->Type = (usTypeAndFactory >> 12) & 0x0f;
        uart_printf("--Init_FactaryInformation.ucType:%d,%d\r\n",Init_FactaryInformation.ucType);//HB-??
        
        co_write16p(&xDataPack.Para[xDataPack.Len], 0x0000);
        xDataPack.Len += 2;
        
        if(FTMS_Uart_ReadType == OLD_ReadType )
        {
            prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_MODEL, &xDataPack);//
        }
            
        prvvDeviceFitShowCodeSave();
        break;
    }
    case SYS_CFG_DEVICE_PARA:
        // co_write16p(&xDataPack.Para[xDataPack.Len], 0xffff);
        // xDataPack.Len += 2;
        
		// 		if(FTMS_Uart_ReadType == OLD_ReadType )
		// 		{
		// 			prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, SYS_CFG_DEVICE_PARA, &xDataPack);//
		// 		}
					
        break;

    case FITSHOW_CODE_CFG_DEL_BLE_NAME:
        co_write16p(&xDataPack.Para[xDataPack.Len], 0xffff);
        xDataPack.Len += 2;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_DEL_BLE_NAME, &xDataPack);//
				}
										
        break;
    case FITSHOW_CODE_CFG_DEL_MODEL:
        co_write16p(&xDataPack.Para[xDataPack.Len], 0xffff);
        xDataPack.Len += 2;
		
				prvvDeviceInformationClear();
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_DEL_MODEL, &xDataPack);//
				}
						
        break;
    case FITSHOW_CODE_CFG_DEL_PARAM:
        // co_write16p(&xDataPack.Para[xDataPack.Len], 0xffff);
        // xDataPack.Len += 2;
        
				// prvvSystemConfigClear();
		// 		if(FTMS_Uart_ReadType == OLD_ReadType )
		// 		{
		// 			prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_DEL_PARAM, &xDataPack);//
		// 		}
						
        break;
    case FITSHOW_CODE_CFG_DEL_BT_NAME:
        co_write16p(&xDataPack.Para[xDataPack.Len], 0xffff);
        xDataPack.Len += 2;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_DEL_BT_NAME, &xDataPack);//
				}
							
        break;
    case FITSHOW_CODE_CFG_REBOOT:
    //    while(1){
    //     if(xFitShowSystemConfig.SystemConfigWriteFlag == false && xFitShowSystemConfig.SystemConfigWriteFlag == false)
    //     {
    //         uart_printf("-rbeak-line:%d\r\n",__LINE__);//HB-??
            
    //         break;
    //     }
    //    };
         reset_mcu();
        break;

    case FITSHOW_CODE_CFG_MANU_NAME:
    {
        uint8_t ucDataLen = size - 5;
        
        memset(&Init_deviceBLE_Information->arrManufacturerName[0], 0, 20);
        memcpy(&Init_deviceBLE_Information->arrManufacturerName[0], &pData[ucIndex], ucDataLen);
        // memset(&xFitShowSystemConfig.pDeviceInformation->arrManufacturerName[0], 0, 20);
        // memcpy(&xFitShowSystemConfig.pDeviceInformation->arrManufacturerName[0], &pData[ucIndex], ucDataLen);
        ucIndex += ucDataLen;
        xDataPack.Para[xDataPack.Len] = 0x00;
        ++xDataPack.Len;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_MANU_NAME, &xDataPack);//
				}
							
        // prprvvDeviceInformationSave(); // about Device information
        break;
    }
    case FITSHOW_CODE_CFG_SN:
    {
        uint8_t ucDataLen = size - 5;
        memset(&Init_deviceBLE_Information->arrSerialNumber[0], 0, 20);
        memcpy(&Init_deviceBLE_Information->arrSerialNumber[0], &pData[ucIndex], ucDataLen);
        // memset(&xFitShowSystemConfig.pDeviceInformation->arrSerialNumber[0], 0, 20);
        // memcpy(&xFitShowSystemConfig.pDeviceInformation->arrSerialNumber[0], &pData[ucIndex], ucDataLen);
        ucIndex += ucDataLen;
        xDataPack.Para[xDataPack.Len] = 0x00;
        ++xDataPack.Len;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_SN, &xDataPack);//
				}
								
        // prprvvDeviceInformationSave(); // about Device information
        break;
    }
    case FITSHOW_CODE_CFG_MODEL_NAME:
    {
        uint8_t ucDataLen = size - 5;
        memset(&Init_deviceBLE_Information->arrModelName[0], 0, 20);
        memcpy(&Init_deviceBLE_Information->arrModelName[0], &pData[ucIndex], ucDataLen);
        // memset(&xFitShowSystemConfig.pDeviceInformation->arrModelName[0], 0, 20);
        // memcpy(&xFitShowSystemConfig.pDeviceInformation->arrModelName[0], &pData[ucIndex], ucDataLen);
        ucIndex += ucDataLen;
        xDataPack.Para[xDataPack.Len] = 0x00;
        ++xDataPack.Len;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_MODEL_NAME, &xDataPack);//
				}
							
        // prprvvDeviceInformationSave(); // about Device 
        break;
    }
    case FITSHOW_CODE_CFG_SYS_ID:
    {
        uint8_t ucDataLen = size - 5;
        memset(&Init_deviceBLE_Information->arrSystemID[0], 0, 20);
        memcpy(&Init_deviceBLE_Information->arrSystemID[0], &pData[ucIndex], ucDataLen);
        // memset(&xFitShowSystemConfig.pDeviceInformation->arrSystemID[0], 0, 20);
        // memcpy(&xFitShowSystemConfig.pDeviceInformation->arrSystemID[0], &pData[ucIndex], ucDataLen);
        ucIndex += ucDataLen;
        xDataPack.Para[xDataPack.Len] = 0x00;
        ++xDataPack.Len;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_SYS_ID, &xDataPack);//
				}
							
        // prprvvDeviceInformationSave(); // about Device information
        break;
    }
    case FITSHOW_CODE_GET_CFG_MANU_NAME:
    {
        uint8_t ucManufacturerNameLen = 0;
        ucManufacturerNameLen = strlen(Init_deviceBLE_Information->arrManufacturerName);
        memcpy(&xDataPack.Para[xDataPack.Len], &Init_deviceBLE_Information->arrManufacturerName[0], ucManufacturerNameLen);
        xDataPack.Len += ucManufacturerNameLen;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_GET_CFG_MANU_NAME, &xDataPack);//
				}
							
        break;
    }
    case FITSHOW_CODE_GET_SN:
    {
        uint8_t ucSN_LEN = 0;
        ucSN_LEN = strlen(Init_deviceBLE_Information->arrSerialNumber);
        memcpy(&xDataPack.Para[xDataPack.Len], &Init_deviceBLE_Information->arrSerialNumber[0], ucSN_LEN);
        xDataPack.Len += ucSN_LEN;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_GET_SN, &xDataPack);//
				}
							
        break;
    }
    case FITSHOW_CODE_GET_MODEL_NAME:
    {
        uint8_t ucMODEL_NAME_LEN = 0;
        ucMODEL_NAME_LEN = strlen(Init_deviceBLE_Information->arrModelName);
        memcpy(&xDataPack.Para[xDataPack.Len], &Init_deviceBLE_Information->arrModelName[0], ucMODEL_NAME_LEN);
        xDataPack.Len += ucMODEL_NAME_LEN;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_GET_MODEL_NAME, &xDataPack);//
				}
								
        break;
    }
    case FITSHOW_CODE_GET_SYS_ID:
    {
        uint8_t ucSYS_ID_LEN = 0;
        ucSYS_ID_LEN = strlen(Init_deviceBLE_Information->arrSystemID);
        memcpy(&xDataPack.Para[xDataPack.Len], &Init_deviceBLE_Information->arrSystemID[0], ucSYS_ID_LEN);
        xDataPack.Len += ucSYS_ID_LEN;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_GET_SYS_ID, &xDataPack);//
				}
							
        break;
    }
    case FITSHOW_CODE_CFG_GET_MAC:
    {
        uint8_t ucMAC_LEN = 6;
        memcpy(&xDataPack.Para[xDataPack.Len], &Init_deviceBLE_Information->arrMAC_Address[0], ucMAC_LEN);
        xDataPack.Len += ucMAC_LEN;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, FITSHOW_CODE_CFG_GET_MAC, &xDataPack);//
				}
								
        break;
    }
    case FITSHOW_CODE_CFG_DEVICE_PARAM:
		// 	uart_printf("----------FITSHOW_CODE_CFG_DEVICE_PARAM:0x%x---------------\r\n",FITSHOW_CODE_CFG_DEVICE_PARAM);
        prvvSystemConfig(RequstSource, pData, size);
        return; //  in function send data to requst source
        // break;

    case FITSHOW_CODE_CFG_BN:

        break;
    default:
        co_write16p(&xDataPack.Para[xDataPack.Len], 0xffff);
        xDataPack.Len += 2;
        
				if(FTMS_Uart_ReadType == OLD_ReadType )
				{
					prvucFitShowDataSerialization(FITSHOW_CMD_CONFIG, pData[2], &xDataPack);//
				}
							
        break;
    }
    switch (RequstSource)
    {
    case FitShow_DeviceConfig:
        // pFitShow->SendToUart(xDataPack.Para, xDataPack.Len);
        BLE_SendToUart0(xDataPack.Para, xDataPack.Len);
        break;
    case FitShow_AppConfig:
        // pFitShow->SendToBle(UUIDList_FFF1, xDataPack.Para, xDataPack.Len);
        BLE_SEND_DATA(xDataPack.Para, xDataPack.Len);
        break;
    default:
        break;
    }
}
//================================================================================================================================================//HB-注释

void prvucFitShowDataSerialization(uint8_t Command, uint8_t CommandCode, DataBuff_t *pData)
{
	if(FTMS_Uart_ReadType == OLD_ReadType ){
    uint8_t FitShowPack[FITSHOW_MAX_DATA_LEN];
    uint8_t index = 0;
    FitShowPack[index++] = StartCode;
    FitShowPack[index++] = Command;
    FitShowPack[index++] = CommandCode;
    memcpy(&FitShowPack[index], pData->Para, pData->Len);
    index += pData->Len;
    uint8_t FCS = 0;
    for (uint8_t i = 1; i < index; i++)
    {
        FCS ^= FitShowPack[i];
    }
    FitShowPack[index++] = FCS;
    FitShowPack[index++] = EndCode;
    memcpy(pData->Para, FitShowPack, index);
    pData->Len = index;
	}
	
}

uint8_t Read_Code_type = 0;
uint16_t Read_Code_Brand = 0,Read_Code_Model = 0;
char Get_FitShowCodeInfo(void) 
{
	char res = 1;
	nvds_tag_len_t Len = sizeof(FitShowBaseInformation_t);
	uint8_t data[sizeof(FitShowBaseInformation_t)];
	memset(data, 0, sizeof(FitShowBaseInformation_t));
	
	uint8_t ret = nvds_get(NVDE_TAG_FSCODE_DATA, &Len, data);
	
    
	FitShowBaseInformation_t *pSystem_Buff = (FitShowBaseInformation_t *)data;
	
	uint16_t CheckValue = 0x0000;
	CheckValue = usSumUp((uint8_t*)&pSystem_Buff->FitShow_code, sizeof(FitShowBaseInformation_t) - 2);
    uart_printf("--:%d,%d\r\n",CheckValue,pSystem_Buff->usSUM_Check);//HB
    
	if (CheckValue != pSystem_Buff->usSUM_Check)
	{      
			return res;
	}
	
	if((pSystem_Buff->ucType | pSystem_Buff->usBrand | pSystem_Buff->usModel)>0)
	{
     
		Read_Code_type = pSystem_Buff->ucType;
		Read_Code_Brand = pSystem_Buff->usBrand;
		Read_Code_Model = pSystem_Buff->usModel;
		res = 0;
	}

	return res;
}

char Get_BLEInformation_NVDS(void)
{
    char res = 1;
	nvds_tag_len_t Len = sizeof(DeviceInformation_t);
	uint8_t data[sizeof(DeviceInformation_t)];
	memset(data, 0, sizeof(DeviceInformation_t));
	
	uint8_t ret = nvds_get(NVDE_TAG_DIS_DATA, &Len, data);
	
	DeviceInformation_t *pSystem_Buff = (DeviceInformation_t *)data;
	
	uint16_t CheckValue = 0x0000;
	CheckValue = usSumUp((uint8_t*)&pSystem_Buff->arrManufacturerName, sizeof(DeviceInformation_t) - 2);
    
	if (CheckValue != pSystem_Buff->usSUM_Check)
	{      
			return res;
	}

    if(CheckValue == 0 && pSystem_Buff->usSUM_Check == 0){
        res = 1; 
        return res;
        }

    memcpy(&Init_deviceBLE_Information->arrBLE_Name[0],&pSystem_Buff->arrBLE_Name[0],sizeof(pSystem_Buff->arrBLE_Name));
    memcpy(&Init_deviceBLE_Information->arrFirmWareRevision[0],&pSystem_Buff->arrFirmWareRevision[0],sizeof(pSystem_Buff->arrFirmWareRevision));
    memcpy(&Init_deviceBLE_Information->arrHardWareRevision[0],&pSystem_Buff->arrHardWareRevision[0],sizeof(pSystem_Buff->arrHardWareRevision));
    memcpy(&Init_deviceBLE_Information->arrMAC_Address[0],&pSystem_Buff->arrMAC_Address[0],sizeof(pSystem_Buff->arrMAC_Address));
    memcpy(&Init_deviceBLE_Information->arrManufacturerName[0],&pSystem_Buff->arrManufacturerName[0],sizeof(pSystem_Buff->arrManufacturerName));
    memcpy(&Init_deviceBLE_Information->arrModelName[0],&pSystem_Buff->arrModelName[0],sizeof(pSystem_Buff->arrModelName));
    memcpy(&Init_deviceBLE_Information->arrSerialNumber[0],&pSystem_Buff->arrSerialNumber[0],sizeof(pSystem_Buff->arrSerialNumber));
    memcpy(&Init_deviceBLE_Information->arrSoftWareRevision[0],&pSystem_Buff->arrSoftWareRevision[0],sizeof(pSystem_Buff->arrSoftWareRevision));
    memcpy(&Init_deviceBLE_Information->arrSystemID[0],&pSystem_Buff->arrSystemID[0],sizeof(pSystem_Buff->arrSystemID));

    res = 0;

	return res;
}
