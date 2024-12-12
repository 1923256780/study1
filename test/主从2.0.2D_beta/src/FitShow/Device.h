/*
 * @Author: LHB
 * @Date: 2023-05-29 11:12:49
 * @LastEditors: LHB
 * @LastEditTime: 2023-09-01 14:19:53
 * @Description: 
 * 1
 */
#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "stdint.h"
#include "DataFormat.h"

#ifndef uint8_t 
#define uint8_t unsigned char
#endif

#ifndef uint16_t 
#define uint16_t unsigned short
#endif

#ifndef uint32_t 
#define uint32_t unsigned int
#endif


typedef struct FitShowDeviceTypePara
{
    uint16_t Model;
    uint16_t Factory : 12;
    uint16_t Type : 4;
    uint16_t DistancePerPulse;
    uint16_t CaloriesRate;
	
	
    // uint16_t CaloriesRateBaseOn;
		
    uint8_t PulsePerCount;
    uint8_t Unit;
} FitShowDeviceTypePara_t;

typedef struct ProgramData
{
    uint16_t Data[32];
    uint8_t CurrentIndex;
    uint8_t IndexMax;
} ProgramData_t;

typedef struct FitShowSportInformation
{
    uint32_t UserID;
    uint8_t Height;
    uint8_t Weight;
    uint8_t Age;
    uint8_t Gender;
    uint32_t SportID;
    uint8_t Mode;
    uint8_t Segments;
    uint16_t Goal;
    ProgramData_t *pProgramData;
} FitShowSportInformation_t;

typedef struct FitShowDeviceTimeStamp
{
    uint16_t Year;
    uint16_t Mouth;
    uint16_t Day;
    uint16_t Week;
    uint16_t Hour;
    uint16_t Minute;
    uint16_t Second;
} FitShowDeviceTimeStamp_t;

typedef struct FitShowAllData
{
    /**/
    FitShowDeviceTypePara_t xDeviceDataTypePara;
    /**/
    uint8_t DeviceConfig;
    /**/
    FitShowDeviceTimeStamp_t xTimeStamp;
    /**/
    FitShowSportInformation_t xSportInformation;
    /**/

    uint8_t StartCountDownValue;
    uint8_t RealStartCountDownValue;
    uint32_t TotalCount;
    uint8_t ucErrorCode;
    uint8_t DisableCode;
    /**/
    uint8_t State;
    uint16_t Speed;
		uint16_t AverageSpeed;
		uint8_t InstantaneousPace;     //bit 5   km/m
    uint8_t AveragePace;     //bit 6    km/m
    uint16_t TargetSpeed;
    uint8_t Resistance;
    uint8_t TargetResistance;
    uint16_t Freq;
    uint8_t Heart;
    uint16_t Power;
    uint16_t TargetPower;
    uint8_t Incline;
    uint8_t TargetIncline;
		uint8_t MetabolicEquivalent;
		uint16_t RemainingTime;     //bit 11
    int16_t ForceonBelt;     //bit 12
    int16_t PowerOutput;     //bit 12
    /**/
    uint16_t Time;
    uint32_t Distance;
    uint16_t Calories;
    uint16_t Count;
    /*setting switch*/
    uint8_t DeviceFunctionSwitch;

    /**/
    SupportedSpeedRange_t xSpeedRange;
    SupportedInclinationRange_t xInclinationRange;
    SupportedResistanceRange_t xResistanceRange;
    SupportedPowerRange_t xPowerRange;
    SupportedHeartRateRange_t xHeartRateRange;
    /**/
} FitShowAllData_t;

typedef enum DeviceTypeList
{
    TREADMILL = 0,
    CROSS_TRAINER = 1,
    INDOORBIKE = 2,
    ROWER = 3,
    SPINNING,
    WALK_TREADMILL = 5,
    Mechanical_Treadmill = 10,

    FS_Rope = 6,
    FS_FasciaGun,
    FS_AbdomenWheel,
    FS_FatThrowingMachine,
    FS_Field = 11,
    FS_JUMP,
} DeviceTypeList_e;


typedef enum UnitDimension
{
    Metric,
    Imperial,
} UnitDimension_e;

uint16_t usGetDeviceModel(void);
uint16_t usGetDeviceFactory(void);
uint16_t usGetDeviceType(void);
//================================================================================================================================================//HB-注释
enum UartInitStep
{
    // Step_,
    Step_FactryInformation,
    Step_ResRange,
    Step_SpeedRange,
    Step_InclinationRange,
    Step_Complete,
    Step_OutTime,
    Step_OutTime_Idle,
};

enum FC_CMD_Code
{
    FC_CMD_Code_SysInfo = 0x50, 
        sunCMD_Code_SysInfo_model = 0,
        sunCMD_Code_SysInfo_speed = 2,
        sunCMD_Code_SysInfo_incline = 3,
        sunCMD_Code_SysInfo_total = 4,

    FC_CMD_Code_SysInfo2 = 0x41,
        sunCMD_Code_SysInfo2_Res = 0x02,

    FC_CMD_Code_SysSta = 0x51,
        sunCMD_Code_SysSta_NORMAL = 0,
        sunCMD_Code_SysSta_END ,
        sunCMD_Code_SysSta_START ,
        sunCMD_Code_SysSta_RUNNING ,
        sunCMD_Code_SysSta_STOPPING ,
        sunCMD_Code_SysSta_ERROR ,
        sunCMD_Code_SysSta_DISABLE ,
        sunCMD_Code_SysSta_PAUSED = 0X0A,

    FC_CMD_Code_DevSta = 0x42,
        sunCMD_Code_DevSta_NORMAL = 0,
        sunCMD_Code_DevSta_START	 ,
        sunCMD_Code_DevSta_RUNNING ,
        sunCMD_Code_DevSta_PAUSED ,
        sunCMD_Code_DevSta_SLEEP = 0x14,
        sunCMD_Code_DevSta_ERROR = 0x15,

    FC_CMD_Code_DevSta2 = 0x43,
        sunCMD_Code_DevSta2 = 1,

    FC_CMD_Code_Config = 0x60,

    FC_CMD_Code_DeviceCtrl = 0x44,
        sunCMD_Code_DeviceCtrl_Name = 0xc5,
};

typedef struct {
	uint16_t		crc;
	uint16_t 	model;			// �豸�ͺ�
	uint16_t 	brand	:12;	// Ʒ�Ʊ�ʶ
	uint16_t 	type	:4;		// �豸����

} dev_info_t;

typedef struct {
	uint8_t		config;			// ��������
	uint8_t 		min_speed;		// ����ٶ�
	uint16_t 		max_speed;		// ����ٶ�
	signed char  	min_incline;	// ����¶�
	signed char  	max_incline;	// ����¶�
	uint8_t 		min_level;		// �������
	uint8_t 		max_level;		// �������
	uint8_t      Inch_enable;
} dev_param_t;

typedef struct {
	dev_info_t	info;
	dev_param_t	param;

} device_t;

typedef struct {
	uint8_t		state;			// �豸״̬

	uint16_t		runtime;		// �˶�ʱ��
	uint32_t		distance;		// �˶�����
	uint16_t		calories;		// �˶�����
	uint16_t		count;			// �˶�����

	uint16_t		speed;			// �ٶ�
	signed char		incline;		// �¶�
	uint8_t		level;			// ����
	uint8_t		heart;			// ����
	uint16_t		freq;			// Ƶ��
	uint16_t		power;			// ���ģ����������أ�

	//cross
	// uint16_t     InstantaneousSpeed;    //

	// uint8_t      CrossTrainerDataHeartRate;
	// uint16_t     CrossTrainerDataStepCount_perMin;
	// uint16_t     CrossTrainerDataStepCount_Rate;
	// uint16_t     CrossTrainerDataInstantaneousPower;
	// uint16_t      CrossTrainerDataResistanceLevel;//ԭ��Ϊu8
	// uint16_t     CrossTrainerDataExpendedEnergy_all;
	// uint16_t     CrossTrainerDataExpendedEnergy_Hour;
	// uint8_t     CrossTrainerDataExpendedEnergy_Min;
	// uint16_t     CrossTrainerDataElapsedTime;

} sport_t;

extern FitShowAllData_t xFitShowAllData;

void DeviceInit(void);
void delay(uint32_t Delaycnt);

void BLE_SendToUart0(uint8_t *data,uint16_t len);
void BLE_SendToUart2(uint8_t *data,uint16_t len);

void DeviceLoop(void);
void send_fomat_uart0(uint8_t *data,uint8_t size,uint8_t typeChoose);
void send_fomat_fff1(uint8_t *data,uint8_t size,uint8_t typeChoose);
void send_fomat_fff2_toSlave(uint8_t *data,uint8_t size,uint8_t typeChoose);
void BLE_SEND_DATA(uint8_t * data, uint8_t size);
void BLE_REC_DATA(uint8_t * data, uint8_t size);
void Slave_Disconnect_CB(void);
void Slave_Connect_CB(void);
void Master_Disconnect_CB(void);
void Master_Connect_CB(void);
void Advertising_Start(void);
void Advertising_Stop(void);
void Scanning_Stop(void);
void Scanning_Start(void);

void Update_ConnectFlag(void);

void fitshow_protocol_forDevice(uint8_t * data, uint8_t size);
void fitshow_protocol_forBoxSlave(uint8_t * data, uint8_t size);
void fitshow_protocol_forHeartSlave(uint8_t * data, uint8_t size);
void fitshow_config_CB(uint8_t *data, uint8_t size);
void reset_mcu(void);
void sleep_ctrl(void);
void get_nvds_device_TypeInfo(void);

void ClearSportInfoOnly(void);

#endif
