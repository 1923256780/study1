#ifndef _FTMS_TASK_H_
#define _FTMS_TASK_H_

#include "rwprf_config.h"
#include <stdint.h>
#include "rwip_task.h" // Task definitions
#include "DataFormat.h"
#if (BLE_FTMS_SERVER)
/*
 * DEFINES
 ****************************************************************************************
 */
#define FTMS_IDX_MAX 0x01

enum FTMS_msg_id
{
    FTMS_Server_0x1826 = TASK_FIRST_MSG(TASK_ID_FTMS),
    /******/
    // FTMS_Feature_0x2ACC_Read,
    /******/
    FTMS_Treadmill_Data_0x2ACD_Notify,
    FTMS_Treadmill_Data_0x2ACD_Notify_RSP,
    /******/
    FTMS_Cross_Trainer_Data_0x2ACE_Notify,
    FTMS_Cross_Trainer_Data_0x2ACE_Notify_RSP,
    /******/
    FTMS_Step_Climber_Data_0x2ACF_Notify,
    FTMS_Step_Climber_Data_0x2ACF_Notify_RSP,
    /******/
    FTMS_Stair_Climber_Data_0x2AD0_Notify,
    FTMS_Stair_Climber_Data_0x2AD0_Notify_RSP,
    /******/
    FTMS_Rower_Data_0x2AD1_Notify,
    FTMS_Rower_Data_0x2AD1_Notify_RSP,
    /******/
    FTMS_Indoor_Bike_Data_0x2AD2_Notify,
    FTMS_Indoor_Bike_Data_0x2AD2_Notify_RSP,
    /******/
    FTMS_Training_Status_0x2AD3_Notify,
    FTMS_Training_Status_0x2AD3_Notify_RSP,
    // FTMS_Training_Status_0x2AD3_Read,
    /******/
    // FTMS_Speed_Range_0x2AD4_Read,
    /******/
    // FTMS_Inclination_Range_0x2AD5_Read,
    /******/
    // FTMS_Resistance_Level_Range_0x2AD6_Read,
    /******/
    // FTMS_Heart_Rate_Range_0x2AD7_Read,
    /******/
    // FTMS_Power_Range_0x2AD8_Read,
    /******/
    FTMS_Control_Point_0x2AD9_Write,
    FTMS_Control_Point_0x2AD9_Indicate,
    FTMS_Control_Point_0x2AD9_Indicate_RSP,
    /******/
    FTMS_MachineStatus_0x2ADA_Notify,
    FTMS_MachineStatus_0x2ADA_Notify_RSP,

    FTMS_DATA_NTF_OR_IND_CFG,
};

enum FTMS_features
{
    FTMS_Feature_0x2ACC_Read_FEATURE,

    FTMS_Treadmill_Data_0x2ACD_NTF_SUP_FEATURE,
    FTMS_Treadmill_Data_0x2ACD_NTF_NOT_SUP_FEATURE,

    FTMS_Cross_Trainer_Data_0x2ACE_NTF_SUP_FEATURE,
    FTMS_Cross_Trainer_Data_0x2ACE_NTF_NOT_SUP_FEATURE,

    FTMS_Step_Climber_Data_0x2ACF_NTF_SUP_FEATURE,
    FTMS_Step_Climber_Data_0x2ACF_NTF_NOT_SUP_FEATURE,

    FTMS_Stair_Climber_Data_0x2AD0_NTF_SUP_FEATURE,
    FTMS_Stair_Climber_Data_0x2AD0_NTF_NOT_SUP_FEATURE,

    FTMS_Rower_Data_0x2AD1_NTF_SUP_FEATURE,
    FTMS_Rower_Data_0x2AD1_NTF_NOT_SUP_FEATURE,

    FTMS_Indoor_Bike_Data_0x2AD2_NTF_SUP_FEATURE,
    FTMS_Indoor_Bike_Data_0x2AD2_NTF_NOT_SUP_FEATURE,

    FTMS_Training_Status_0x2AD3_NTF_SUP_FEATURE,
    FTMS_Training_Status_0x2AD3_NTF_NOT_SUP_FEATURE,
    FTMS_Training_Status_0x2AD3_Read_FEATURE,

    FTMS_Speed_Range_0x2AD4_Read_FEATURE,

    FTMS_Inclination_Range_0x2AD5_Read_FEATURE,

    FTMS_Resistance_Level_Range_0x2AD6_Read_FEATURE,

    FTMS_Heart_Rate_Range_0x2AD7_Read_FEATURE,

    FTMS_Power_Range_0x2AD8_Read_FEATURE,

    FTMS_Control_Point_0x2AD9_Write_FEATURE,
    FTMS_Control_Point_0x2AD9_Indicate_FEATURE,

    FTMS_Status_0x2ADA_NTF_SUP_FEATURE,
    FTMS_Status_0x2ADA_NOT_NTF_SUP_FEATURE,
};

typedef struct FTMS_db_cfg
{
    FTMS_TYPE_e DeviceType;
    uint32_t EnableCharList;
    FitnessMachineFeature_t Feature;
    SupportedInclinationRange_t *pInclinationRange;
    SupportedResistanceRange_t *pResistanceRange;
    SupportedHeartRateRange_t *pHeartRateRange;
    SupportedPowerRange_t *pPowerRange;
    SupportedSpeedRange_t *pSpeedRange;

	
} FTMS_db_cfg_t;

typedef struct FTMS_enable_req
{
    uint8_t para[20];
} FTMS_enable_req_t;



typedef struct FTMS_ControlPointWriteData
{
    uint8_t op_code;
    uint8_t para[20];
    uint8_t length;
} FTMS_ControlPointWriteData_t;
typedef struct FTMS_ControlPointIndicateData
{
    uint8_t ResponseCode;
    uint8_t RequestCode;
    uint8_t ResultCode;
    uint8_t ResponseParameter[17];
    uint8_t length;
    uint8_t conidx;
} FTMS_ControlPointIndicateData_t;

typedef struct FTMS_CommonData
{
    uint8_t conidx;
    uint8_t length;
    uint8_t Data[20];
} FTMS_CommonData_t;

typedef struct ftms_notify_rsp
{
    ///status
    uint8_t status;
} ftms_notify_rsp_t;

typedef struct FTMS_NotifyIndicateConfig
{
    uint8_t Con_Index;
    uint8_t Att_Index;
    uint32_t NTF_Config;
} FTMS_NotifyIndicateConfig_t;
extern const struct ke_state_handler FTMS_default_handler;
#endif // BLE_FFF0S_SERVER

#endif /* _FTMS_TASK_H_ */
