/**
 * @Author: 苏良�?
 * @Date: 2021-03-27 03:01:17
 * @LastEditTime: 2021-04-12 00:58:52
 * @LastEditors: 苏良�?
 * @Description: 
 * @FilePath: \BK3431Q\SDK\sdk\FitShow\FitShow.h
 * @喵了个咪
 */

#ifndef __FITSHOW_H__
#define __FITSHOW_H__

#include "stdint.h"
#include "DataFormat.h"

#define FITSHOW_MAX_DATA_LEN 30
#define StartCode 0x02
#define EndCode 0x03

#define StartCode_forFTMS_base 0x22
#define EndCode_forFTMS_base 0x23

typedef enum UUIDList
{
	UUIDList_FFF0S,
	UUIDList_FFF1,
	UUIDList_FFF2,

	UUIDList_FTMS,
	UUIDList_FitnessMachineControlPoint,

} UUIDList_e;

typedef DataPack_t DataBuff_t;

typedef struct NewProtocolDataPack
{
	uint16_t usAddress;
	DataPack_t xDataPack;
} NewProtocolDataPack_t;


typedef enum NewProtocolCommandList
{
	eNPCMD_EnvWrite,
	eNPCMD_EnvRead,
} NewProtocolCommandList_e;

typedef enum SYSTEM_COMMAND
{
	SYS_CMD = 0x60,
	SYS_CFG_BLE_NAME = 1,
	SYS_CFG_DEVICE_TYPE,
	SYS_CFG_DEVICE_PARA,
	SYS_CFG_BT_NAME,

	SYS_DEL_BLE_NAME = 6,
	SYS_DEL_DEVICE_TYPE,
	SYS_DEL_DEVICE_PARA,
	SYS_DEL_BT_NAME,

	SYS_CFG_REBOOT,

	SYS_GET_MAC,

	SYS_GET_MANUFACTURER,



	SYS_CFG_DEVICE_PARAM = 0X30,

} SYSTEM_COMMAND_e;
typedef enum DEVICE_MODEL
{
	DEVICE_INFO = 0x50,
	DEVICE_INFO_MODEL = 0,
	DEVICE_INFO_DATE,
	DEVICE_INFO_SPEED,
	DEVICE_INFO_INCLINE,
} DEVICE_MODEL_e;

typedef enum DEVICE_MODEL_FTMS_BASE
{
	DEVICE_FUNCTION_CODE_FTMS_BASE_Feature = 0x02,
	DEVICE_FUNCTION_CODE_FTMS_BASE_Target = 0x03,
	DEVICE_INFO_FTMS_BASE_Need = 0x01,
	DEVICE_INFO_MODEL_FTMS_BASE = 0x01,
	
	
/*	
	DEVICE_INFO_DATE_FTMS_BASE,
	DEVICE_INFO_SPEED_FTMS_BASE,
	DEVICE_INFO_INCLINE_FTMS_BASE,
*/
} DEVICE_MODEL_FTMS_BASE_e;

typedef enum DEVICE_REPORT
{
	DEVICE_BaseInfo = 0x01,
	DEVICE_FeatureInfo = 0x02,
	DEVICE_TargetInfo = 0x03,
	
	DEVICE_MachineStatue = 0x04,
	DEVICE_ControlReq = 0x05,
	
	DEVICE_Info_TREADMILL 		= 0x60,
	DEVICE_Info_CrossTrainer 	= 0x61,
	DEVICE_Info_StepClimber	 	= 0x62,
	DEVICE_Info_StairClimber 	= 0x63,
	DEVICE_Info_Rower 				= 0x64,
	DEVICE_Info_IndoorBike	 	= 0x65,
	
} DEVICE_REPORT_e;

typedef enum FitShowConfig
{
	FitShowConfig_Unit_Imperial = (1 << 0),
	FitShowConfig_PauseSupport = (1 << 1),
} FitShowConfig_e;







typedef enum FitShowControlSwitchList
{
	ResistanceControl_Switch = (1 << 0),
	InclineControl_Switch = (1 << 1),
} FitShowControlSwitchList_e;


typedef struct FitShow
{
	void (*Init)(void);
	void (*DataSerialization)(uint8_t Command, uint8_t CommandCode, DataBuff_t *pData);
	void (*DataSerializationFTMSbase)(uint8_t FunctionCode,uint8_t Command, uint8_t CommandCode,char *Value, DataBuff_t *pData);
	
	void (*ResponseDataParser)(uint8_t *pData, uint8_t len); 
	void (*ReportDataParser)(uint8_t *pData, uint8_t len);//New add
	void (*CommandParser)(uint8_t *pData, uint8_t len);

	void (*SendToBle)(uint8_t UUID, uint8_t *pData, uint8_t size);
	void (*ReceivedFromBleCallBack)(uint8_t UUID, uint8_t *pData, uint8_t size);
	void (*SendToUart)(uint8_t *pData, uint8_t size);
	void (*ReceivedFromUartCallBack)(uint8_t *pData, uint8_t size);
	void (*EventTrigger)(uint8_t Event);
	void (*NewProtocolPaser)(uint8_t *pData, uint8_t len);
} FitShow_t;


// void vFitShowDataFormat(uint8_t *pData, uint8_t ucLength);
FitShow_t *pGetFitShowHandler(void);

#endif
