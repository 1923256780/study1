/*
 * @Author: your name
 * @Date: 2021-07-28 11:38:23
 * @LastEditTime: 2024-04-07 11:23:15
 * @LastEditors: LHB
 * @Description: In User Settings Edit
 * @FilePath:
 */

#ifndef __SystemConfig_h__
#define __SystemConfig_h__

#include "stdint.h"
#include <stdbool.h>
#include "FitShow.h"
// #include "gpio.h"

/* GBK encode */

// #define SYS_CMD_TEST

#define HeartChannel false
	
#define ConnectStart false

#define FTMS_SumUpdateChannel 1//1??0??

#define Only_FTMS 0  //0: fs+ftms   1:ftms   2:other

#define SleepTime 4//HB_临时注释 min
#define Save_Voltage 21 //HB_临时注释 安全值


#ifdef SYS_CMD_TEST
 FitShow_t* pGetTestFitShowHandler(void);
#endif

typedef void(*Callback)(void);

typedef enum RequstSourceList
{
    FitShow_AppConfig,
    FitShow_DeviceConfig,
} RequstSourceList_e;

typedef enum DataAcquisitionModeList
{
    FITSHOW_DEVICE_PARAM_DATA_ACQ_PULSE_MODE = 0,
    FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE,
} DataAcquisitionModeList_e;

typedef enum CaloriesCalculationMethodList
{
    FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_DISTANCE = 0,
    FITSHOW_DEVICE_PARAM_CALORIES_BASED_ON_COUNT,
} CaloriesCalculationMethodList_e;

typedef enum Fitshow_GPIO_StateList
{
    FITSHOW_DEVICE_PARAM_GPIO_ENABLE,
    FITSHOW_DEVICE_PARAM_GPIO_DISABLE,
} Fitshow_GPIO_StateList_e;

typedef enum Fitshow_GPIO_DirectList
{
    FITSHOW_DEVICE_PARAM_GPIO_INPUT,
    FITSHOW_DEVICE_PARAM_GPIO_OUTPUT,
    FITSHOW_DEVICE_PARAM_GPIO_Float,
} Fitshow_GPIO_DirectList_e;

typedef enum Fitshow_GPIO_PullTypeList
{
    FITSHOW_DEVICE_PARAM_GPIO_PULL_UP,
    FITSHOW_DEVICE_PARAM_GPIO_PULL_DOWN,
    FITSHOW_DEVICE_PARAM_GPIO_PULL_NONE,
} Fitshow_GPIO_PullTypeList_e;

typedef enum Fitshow_GPIO_InterruptModeList
{
    FITSHOW_DEVICE_PARAM_GPIO_INT_RISING_EDGE,
    FITSHOW_DEVICE_PARAM_GPIO_INT_FALLING_EDGE,
    FITSHOW_DEVICE_PARAM_GPIO_INT_HIGH_LEVEL,
    FITSHOW_DEVICE_PARAM_GPIO_INT_LOW_LEVEL,
} Fitshow_GPIO_InterruptModeList_e;

typedef enum Fitshow_GPIO_OutputModeList
{
    FITSHOW_DEVICE_PARAM_GPIO_OUTPUT_HIGH_LEVEL,
    FITSHOW_DEVICE_PARAM_GPIO_OUTPUT_LOW_LEVEL,
} Fitshow_GPIO_OutputModeList_e;

typedef enum FitshowCountAcquireModeList
{
    FITSHOW_DEVICE_PARAM_SINGLE_PULSE_MODE = 0x00,
    FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE = 0x10,
    FITSHOW_DEVICE_PARAM_DOUBLE_PULSE_WITH_ENCODER_MODE_2,
    FITSHOW_DEVICE_PARAM_LONG_SHORT_WAVE_MODE = 0x20,
    FITSHOW_DEVICE_PARAM_LONG_SHORT_WAVE_MODE_2,
} FitshowCountAcquireModeList_e;

typedef enum Fun_GPIO_ConfigList
{
    SPEED_PIN_A = 0x01 << 0,
    SPEED_PIN_B = 0x01 << 1,
    HEART_PIN = 0x01 << 2,
    APP_LINK_PIN = 0x01 << 3,
} Fun_GPIO_ConfigList_e;

typedef enum DeviceParamConfigCommandList
{
    FITSHOW_DEVICE_PARAM_DATA_ACQ_MODE_READ = 1,
    FITSHOW_DEVICE_PARAM_DATA_ACQ_MODE_WRITE,
    FITSHOW_DEVICE_PARAM_FUN_GPIO_LIST_READ,
    FITSHOW_DEVICE_PARAM_FUN_GPIO_LIST_WRITE,
    FITSHOW_DEVICE_PARAM_FUN_GPIO_PARAM_READ,
    FITSHOW_DEVICE_PARAM_FUN_GPIO_PARAM_WRITE,
    FITSHOW_DEVICE_PARAM_WHEEL_DIAMETER_READ,
    FITSHOW_DEVICE_PARAM_WHEEL_DIAMETER_WRITE,
    FITSHOW_DEVICE_PARAM_ONE_PULSE_DISTANCE_READ,
    FITSHOW_DEVICE_PARAM_ONE_PULSE_DISTANCE_WRITE,
    FITSHOW_DEVICE_PARAM_CALORIE_CALC_MODE_READ,
    FITSHOW_DEVICE_PARAM_CALORIE_CALC_MODE_WRITE,
    FITSHOW_DEVICE_PARAM_CALORIE_RATE_READ,
    FITSHOW_DEVICE_PARAM_CALORIE_RATE_WRITE,
    FITSHOW_DEVICE_PARAM_COUNT_ACQ_MODE_READ,
    FITSHOW_DEVICE_PARAM_COUNT_ACQ_MODE_WRITE,
    FITSHOW_DEVICE_PARAM_COUNT_PER_PULSE_READ,
    FITSHOW_DEVICE_PARAM_COUNT_PER_PULSE_WRITE,
    FITSHOW_DEVICE_PARAM_COUNT_DOWN_VALUE_READ,
    FITSHOW_DEVICE_PARAM_COUNT_DOWN_VALUE_WRITE,
    FITSHOW_DEVICE_PARAM_UNIT_READ,
    FITSHOW_DEVICE_PARAM_UNIT_WRITE,
} DeviceParamConfigCommandList_e;

typedef enum FitShowSaveType
{
    FitShowSaveType_BLE_INFO,
    FitShowSaveType_SystemConfig,
} FitShowSaveType_e;


typedef enum FitShowConfigCommand
{
    FITSHOW_CMD_CONFIG				= 0x60,		//系统配置
    FITSHOW_CODE_SAVE               = 0,
	FITSHOW_CODE_CFG_BLE_NAME       = 1,		//设置模块名称（BLE）
	FITSHOW_CODE_CFG_MODEL			,			//设置机型代码
	FITSHOW_CODE_CFG_PARAM			,			//配置参数
	FITSHOW_CODE_CFG_BT_NAME        ,			//配置蓝牙名称（蓝牙音频）

	FITSHOW_CODE_CFG_DEL_BLE_NAME   = 6,
	FITSHOW_CODE_CFG_DEL_MODEL		,
	FITSHOW_CODE_CFG_DEL_PARAM		,
	FITSHOW_CODE_CFG_DEL_BT_NAME    ,

	FITSHOW_CODE_CFG_REBOOT			= 10,		//重启

	FITSHOW_CODE_CFG_MANU_NAME		,
	FITSHOW_CODE_CFG_SN				,
	FITSHOW_CODE_CFG_MODEL_NAME		,
	FITSHOW_CODE_CFG_SYS_ID			,

	FITSHOW_CODE_GET_CFG_MANU_NAME	,
	FITSHOW_CODE_GET_SN				,
	FITSHOW_CODE_GET_MODEL_NAME		,
	FITSHOW_CODE_GET_SYS_ID			,
	
	FITSHOW_CODE_CFG_GET_MAC        = 0x20,

    FITSHOW_CODE_CFG_DEVICE_PARAM   = 0X30,


	FITSHOW_CODE_CFG_CHECK   		= 96,		//校验模块
	
	FITSHOW_CODE_CFG_BN				= 0x7f,
} FitShowConfigCommand_e;

// typedef struct BLE_SystemConfigBase
// {
//     char *pManufacturerName;
//     char *pModelName;
//     char *pSerialNumber;
//     char *pHardWareRevision;
//     char *pSoftWareRevision;
//     char *pFirmWareRevision;
//     char *pSystemID;
//     char *pBLE_Name;
//     // uint8_t* pBLE_NameLength;
//     char *pBluetoothName;
//     uint8_t *pMAC_Address;
// } BLE_SystemConfigBase_t;



typedef struct DeviceInformation
{
    uint16_t usSUM_Check;      
    char arrManufacturerName[20];
    char arrModelName[20];
    char arrSerialNumber[20];
    char arrHardWareRevision[20];
    char arrSoftWareRevision[20];
    char arrFirmWareRevision[20];
    char arrSystemID[20];
    char arrBLE_Name[30];
    char arrBluetooth_Name[20];
    char arrMAC_Address[6];

    // unsigned char Model[2];
    // unsigned char Factory[2];
    // unsigned char Type[2];
} DeviceInformation_t; 

typedef struct FitShowBaseInformation
{
		uint16_t usSUM_Check;   
		char FitShow_code[5];  
	
    uint16_t usModel;
    uint16_t usBrand;
    uint8_t ucType;
} FitShowBaseInformation_t;

typedef struct GPIO_Base
{
    uint8_t Pin;
    Fitshow_GPIO_DirectList_e DirectionType;
    Fitshow_GPIO_PullTypeList_e PullType;
    Fitshow_GPIO_InterruptModeList_e InterruptMode;
} GPIO_Base_t;

typedef struct PulseSensorPinConfig
{
    bool isEnable;
    GPIO_Base_t xGPIO;
    uint16_t usMaxInterval;
    uint16_t usMinInterval;
} PulseSensorPinConfig_t;

typedef struct PulseCalcParam
{
    uint8_t ucPulsePerCount;
    FitshowCountAcquireModeList_e CountAcquisitionMode;
} PulseCalcParam_t;

typedef struct PulseSensor
{
    PulseSensorPinConfig_t xPulsetSensorPin;
    PulseCalcParam_t xPulseSensorCalcParam;
} PulseSensor_t;

typedef struct SpeedPulseSensor
{
    PulseSensor_t xBaseSpeedSensor;
    GPIO_Base_t xAddition_GPIO;
    uint16_t DistancePerPulse;
    uint16_t CaloriesRate;
		uint16_t Speed_Rate ;
		bool CaloriesRate_is_Change;
    CaloriesCalculationMethodList_e CaloriesCalculationMethod;
} SpeedPulseSensor_t;


typedef struct AppLinkSignal
{
    bool isEnable;
    GPIO_Base_t xAppLinkSignalPin;
		GPIO_Base_t xAppLinkSignalPin2;
    // Fitshow_GPIO_OutputModeList_e OutputSignal;
} AppLinkSignal_t;

typedef struct SystemConfigInformation
{
    uint16_t usSUM_Check;                              // check
    DataAcquisitionModeList_e eDataAcquisitionMode;    // data acq mode
    SpeedPulseSensor_t xSpeedPulseSensor;              // Speed Sensor
    PulseSensor_t xHeartPulseSensor;                   // Heart Sensor
    AppLinkSignal_t xAppLink;                          // app Link Signal
    uint8_t ucUnit;                                    // unit
    uint8_t ucCountDownValue;                          // Count down value
    uint16_t usFunction_GPIO_List;
    uint16_t Model;
    uint16_t Factory;
    uint16_t Type;
} SystemConfigInformation_t;

typedef struct FitShowSystemConfig
{
    void(*Init)(void);
    void(*Config)(RequstSourceList_e RequstSource, uint8_t *pData, uint8_t len);
    SystemConfigInformation_t *pSystemConfigInformation;
    DeviceInformation_t *pDeviceInformation; // BLE Information
    uint8_t DeviceInformationWriteFlag;
    uint8_t SystemConfigWriteFlag;
} FitShowSystemConfig_t;

typedef struct Device_UartMode_RunStopStatue
{
    uint8_t RunStopStatue;
} Device_UartMode_RunStopStatue_t;

FitShowSystemConfig_t *pGetFitShowSystemConfig(void);
// void vSystemConfigInit(void);
void vResetFunctionRegister(Callback cb);
uint16_t usSumUp(uint8_t *pData, uint8_t len);


//================================================================================================================================================//HB-注释
typedef enum ReadType
{
    OLD_ReadType,
    NEWforFTMS_Base_ReadType,
} ReadType_e;

DeviceInformation_t *get_Init_deviceBLE_Information(void);
void prvucFitShowDataSerialization(uint8_t Command, uint8_t CommandCode, DataBuff_t *pData);

extern uint8_t IO_or_Uart_Mode;
void prvvFitShowSetting(RequstSourceList_e RequstSource, uint8_t *pData, uint8_t size);
char Get_FitShowCodeInfo(void) ;

char Get_BLEInformation_NVDS(void);
//------------------------------------------------------------------------------------------------------------------------------------------------//HB-注释
static void prvvSystemConfigInit(void);

#endif



//================================================================================================================================================//HB-注释
