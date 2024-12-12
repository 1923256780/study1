#include "rwip_config.h"

#include <string.h>
#include "ke_task.h"                 // Kernel
#include "app_ftms.h"
#include "ftms.h"
#include "app.h"      // Application Definitions
#include "app_task.h" // application task definitions
#include "co_bt.h"
#include "prf_types.h" // Profile common types definition
#include "arch.h"      // Platform Definitions
#include "prf.h"
#include "ke_timer.h"
#include "uart.h"
#include "rf.h" 

#include "ftms_task.h"
#include "DataFormat.h"
#include "gattc_task.h"
#include "ftms.h"
// #include "Device.h"
#include "prf_utils.h"
#include "co_utils.h"



#include "SystemConfig.h"




// #if (BLE_FTMS_SERVER)
#if 1
static app_FTMS_Handler_t xApp_FTMS_Handler;
struct app_ftms_env_tag app_ftms_env;
extern uint8_t FTMS_Uart_ReadType,APP_Disconnect_ID;

static uint8_t Training_Mode = 0x04;

extern FitShow_t *pFitShow;
extern  Device_Report_FeaturesFiedList_t xDevice_Report_FeaturesFiedList;

static void SetTargetResistanceLevel_Function(uint8_t Level);
static void SetTargetPower_Function(int16_t Watt);
static void SetTargetInclination_Function(uint8_t Percent);
static void SetTargetSpeed_Function(uint16_t Value);

static void StartOrResume_Function(void);
static void Stop_Function(void);
static void Pause_Function(void);

uint16_t Temp_for_Ctl_Speed,	Temp_for_Ctl_Inclination;
uint8_t Temp_for_Ctl_ResLevel, Temp_for_Device_Type;


char DataForMachineStatueUpdate[15]={};
	
uint8_t HeartRate_NewCh[2];

app_FTMS_Handler_t xApp_FTMS_Handler = 
{
	
	.StartOrResume = StartOrResume_Function,
	.Stop = Stop_Function,
	.Pause = Pause_Function,
	.SetTargetResistanceLevel = SetTargetResistanceLevel_Function,
	.SetTargetPower = SetTargetPower_Function,
	.SetTargetInclination = SetTargetInclination_Function,
	.SetTargetSpeed = SetTargetSpeed_Function, 
	
};

enum {
    Training_Mode_Power = 0x05,
    Training_Mode_ResLevel = 0x04,
};

app_FTMS_Handler_t *pGetApp_FTMS_Handler(void)
{
    return &xApp_FTMS_Handler;
}



void app_ftms_init(void)
{
    // Reset the environment
    // memset(&app_ftms_env, 0, sizeof(struct app_ftms_env_tag));
    // app_ftms_env.Type = pFitShowAllData->xDeviceDataTypePara.Type;
}

void app_ftm_add_ftms(void)//Fitness Machine Feature����
{
    struct FTMS_db_cfg *db_cfg;

    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                             TASK_GAPM, TASK_APP,
                                                             gapm_profile_task_add_cmd, sizeof(struct FTMS_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = 0;
    req->prf_task_id = TASK_ID_FTMS;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct FTMS_db_cfg *)req->param;
		uart_printf("db_cfg->DeviceType:%x \r\n",db_cfg->DeviceType);
		
		Temp_for_Device_Type = xFitShowAllData.xDeviceDataTypePara.Type;
    switch (xFitShowAllData.xDeviceDataTypePara.Type)
    {
			
    case TREADMILL:
        db_cfg->DeviceType = TREADMILL_TYPE;
				
        break;
    case CROSS_TRAINER:
        db_cfg->DeviceType = CROSS_TYPE;
        break;
    case ROWER:
        db_cfg->DeviceType = ROWER_TYPE;
        break;
    case INDOORBIKE:
        db_cfg->DeviceType = INDOOR_BIKE_TYPE;
        break;
    // case STEP_CLIMBER:
    //     db_cfg->DeviceType = STEP_TYPE;
    //     break;
    // case STAIR_CLIMBER:
    //     db_cfg->DeviceType = STAIR_TYPE;

    //     break;
    }
    db_cfg->EnableCharList = 0;
		if(FTMS_Uart_ReadType == OLD_ReadType )//
		{
			 db_cfg->EnableCharList |= FTMS_CharFlag_Feature_0x2ACC_CHAR |
                              FTMS_CharFlag_Training_Status_0x2AD3_CHAR |
                              FTMS_CharFlag_Fitness_Machine_Status_0x2ADA_CHAR;
			db_cfg->EnableCharList |= FTMS_CharFlag_Fitness_Machine_Control_Point_0x2AD9_CHAR;
			    if (xFitShowAllData.xSpeedRange.Maximum > xFitShowAllData.xSpeedRange.Minimum)
			{
					db_cfg->EnableCharList |= FTMS_CharFlag_Speed_Range_0x2AD4_CHAR;
					db_cfg->pSpeedRange = &xFitShowAllData.xSpeedRange;
			}
			if (xFitShowAllData.xHeartRateRange.Maximum > xFitShowAllData.xHeartRateRange.Minimum)
			{
					db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesHeartRateMeasurementSupported;
					db_cfg->EnableCharList |= FTMS_CharFlag_Heart_Rate_Range_0x2AD7_CHAR;
					db_cfg->pHeartRateRange = &xFitShowAllData.xHeartRateRange;
			}
			if (xFitShowAllData.xInclinationRange.Maximum > xFitShowAllData.xInclinationRange.Minimum)
			{
					db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesInclinationSupported |FTMS_FeaturesElevationGainSupported;
					db_cfg->EnableCharList |= FTMS_CharFlag_Inclination_Range_0x2AD5_CHAR;
					db_cfg->pInclinationRange = &xFitShowAllData.xInclinationRange;
					db_cfg->pInclinationRange->Maximum = db_cfg->pInclinationRange->Maximum * 10;
					db_cfg->pInclinationRange->Minimum = db_cfg->pInclinationRange->Minimum * 10;
					db_cfg->pInclinationRange->Increment = 10;
			}
			if (xFitShowAllData.xResistanceRange.Maximum > xFitShowAllData.xResistanceRange.Minimum)
			{
			
					db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesResistanceLevelSupported;
					
					db_cfg->EnableCharList |= FTMS_CharFlag_Resistance_Level_Range_0x2AD6_CHAR;
					db_cfg->pResistanceRange = &xFitShowAllData.xResistanceRange;
					db_cfg->pResistanceRange->Maximum = db_cfg->pResistanceRange->Maximum * 10;
					db_cfg->pResistanceRange->Minimum = 10;
					db_cfg->pResistanceRange->Increment = 10;
			}
			if (xFitShowAllData.xPowerRange.Maximum > xFitShowAllData.xPowerRange.Minimum)
			{
			
					db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesPowerMeasurementSupported;
					db_cfg->EnableCharList |= FTMS_CharFlag_Power_Range_0x2AD8_CHAR;
					db_cfg->pPowerRange = &xFitShowAllData.xPowerRange;
					db_cfg->pPowerRange->Maximum = 9999;
					db_cfg->pPowerRange->Minimum = 0;
					db_cfg->pPowerRange->Increment = 10;
			}
		}
	

    switch (db_cfg->DeviceType)//
    {
    case TREADMILL_TYPE:		
			if(FTMS_Uart_ReadType == OLD_ReadType )//
			{
				db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesTotalDistanceSupported |
                                                  FTMS_FeaturesPaceSupported |
                                                  FTMS_FeaturesExpendedEnergySupported |
                                                  FTMS_FeaturesElapsedTimeSupported;

				db_cfg->Feature.TargetSettingFeatures |= FTMS_SettingFeaturesInclinationTargetSettingSupported|
														FTMS_SettingFeaturesSpeedTargetSettingSupported;

        db_cfg->EnableCharList |= FTMS_CharFlag_Treadmill_Data_0x2ACD_CHAR;
			}

        break;
    case CROSS_TYPE:
			if(FTMS_Uart_ReadType == OLD_ReadType )//
			{
				db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesTotalDistanceSupported |
                                                	FTMS_FeaturesStepCountSupported |
													FTMS_FeaturesExpendedEnergySupported |
													/**/FTMS_FeaturesPowerMeasurementSupported |
	                                            	FTMS_FeaturesElapsedTimeSupported;

				db_cfg->Feature.TargetSettingFeatures |= FTMS_SettingFeaturesPowerTargetSettingSupported |
														FTMS_SettingFeaturesInclinationTargetSettingSupported |
//														FTMS_SettingFeaturesIndoorBikeSimulationParametersSupported |
														FTMS_SettingFeaturesResistanceTargetSettingSupported;
#if FTMS_SumUpdateChannel == 1
        		db_cfg->EnableCharList |= FTMS_CharFlag_Cross_Trainer_Data_0x2ACE_CHAR;
				db_cfg->EnableCharList |= FTMS_CharFlag_Indoor_Bike_Data_0x2AD2_CHAR;
				db_cfg->EnableCharList |= FTMS_CharFlag_Rower_Data_0x2AD1_CHAR;
#else
				db_cfg->EnableCharList |= FTMS_CharFlag_Cross_Trainer_Data_0x2ACE_CHAR;
#endif
			}

        
        break;
    case ROWER_TYPE:
			if(FTMS_Uart_ReadType == OLD_ReadType )//
			{
				db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesCadenceSupported |
                                                  FTMS_FeaturesTotalDistanceSupported |
                                                  FTMS_FeaturesExpendedEnergySupported |
                                                  FTMS_FeaturesElapsedTimeSupported;

				db_cfg->Feature.TargetSettingFeatures |= FTMS_SettingFeaturesInclinationTargetSettingSupported |
														FTMS_SettingFeaturesPowerTargetSettingSupported |
														
														FTMS_SettingFeaturesResistanceTargetSettingSupported;
        
#if FTMS_SumUpdateChannel == 1
        		db_cfg->EnableCharList |= FTMS_CharFlag_Cross_Trainer_Data_0x2ACE_CHAR;
				db_cfg->EnableCharList |= FTMS_CharFlag_Indoor_Bike_Data_0x2AD2_CHAR;
				db_cfg->EnableCharList |= FTMS_CharFlag_Rower_Data_0x2AD1_CHAR;
#else
				db_cfg->EnableCharList |= FTMS_CharFlag_Rower_Data_0x2AD1_CHAR;
#endif
			}

        
        break;
    case INDOOR_BIKE_TYPE:
			if(FTMS_Uart_ReadType == OLD_ReadType )//
			{
				db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesCadenceSupported |
                                                  FTMS_FeaturesTotalDistanceSupported |
                                                //   FTMS_FeaturesStrideCountSupported |
                                                //   FTMS_FeaturesAverageSpeedSupported | 
                                                  FTMS_FeaturesExpendedEnergySupported |
                                                  FTMS_FeaturesHeartRateMeasurementSupported |
                                                  FTMS_FeaturesElapsedTimeSupported;
				db_cfg->Feature.TargetSettingFeatures |= FTMS_SettingFeaturesInclinationTargetSettingSupported |
														FTMS_SettingFeaturesPowerTargetSettingSupported |
//														FTMS_SettingFeaturesIndoorBikeSimulationParametersSupported |
														FTMS_SettingFeaturesResistanceTargetSettingSupported;
#if FTMS_SumUpdateChannel == 1
        		db_cfg->EnableCharList |= FTMS_CharFlag_Cross_Trainer_Data_0x2ACE_CHAR;
				db_cfg->EnableCharList |= FTMS_CharFlag_Indoor_Bike_Data_0x2AD2_CHAR;
				db_cfg->EnableCharList |= FTMS_CharFlag_Rower_Data_0x2AD1_CHAR;
#else
				db_cfg->EnableCharList |= FTMS_CharFlag_Indoor_Bike_Data_0x2AD2_CHAR;
#endif
       ;
			}

        break;
    case STEP_TYPE:
			if(FTMS_Uart_ReadType == OLD_ReadType )//
			{
				db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesStepCountSupported |
                                                  FTMS_FeaturesElevationGainSupported |
                                                  FTMS_FeaturesExpendedEnergySupported |
                                                  FTMS_FeaturesElapsedTimeSupported;
        		db_cfg->EnableCharList |= FTMS_CharFlag_Step_Climber_Data_0x2ACF_CHAR;
			}

        
        break;
    case STAIR_TYPE:
			if(FTMS_Uart_ReadType == OLD_ReadType )//
			{
				db_cfg->Feature.FitnessMachineFeatures |= FTMS_FeaturesStepCountSupported |
                                                  FTMS_FeaturesElevationGainSupported |
                                                  FTMS_FeaturesExpendedEnergySupported |
                                                  FTMS_FeaturesElapsedTimeSupported;
        db_cfg->EnableCharList |= FTMS_CharFlag_Stair_Climber_Data_0x2AD0_CHAR;
			}

        
        break;
    }
    // app_ftms_env.Type = db_cfg->DeviceType;

    // Send the message
    ke_msg_send(req);
}

static int app_FTMS_msg_dflt_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    return KE_MSG_CONSUMED;
}

void app_FitShow_To_FTMS_Treadmill_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag)
{
	if(FTMS_Uart_ReadType == OLD_ReadType )//
	{
		TreadmillData_t xTreadmillData;
    DataPack_t xDataPack;
    memset(&xTreadmillData, 0, sizeof(TreadmillData_t));
    memset(&xDataPack, 0, sizeof(DataPack_t));
    xTreadmillData.Flag = FTMS_Flag;

    xTreadmillData.InstantaneousSpeed = pFitShowAllData->Speed * 10;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xTreadmillData.InstantaneousSpeed = xTreadmillData.InstantaneousSpeed * 161 /100;
		}
		
		Temp_for_Ctl_Speed = pFitShowAllData->Speed;
    /**/
    xTreadmillData.TotalDistance = pFitShowAllData->Distance;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xTreadmillData.TotalDistance = xTreadmillData.TotalDistance * 161 /100;
		}
    /**/
    xTreadmillData.TotalEnergy = pFitShowAllData->Calories;
    xTreadmillData.EnergyPerHour = 0;
    xTreadmillData.EnergyPerMinute = 0;
    /**/
    xTreadmillData.ElapsedTime = pFitShowAllData->Time;
    /**/
    xTreadmillData.HeartRate = pFitShowAllData->Heart;
		/**/
		HeartRate_NewCh[0] = 0x00;
		HeartRate_NewCh[1] = xTreadmillData.HeartRate;
		// app_fee4_send_ntf(0x00,2,HeartRate_NewCh);
    /**/
    xTreadmillData.Inclination = pFitShowAllData->Incline*10;
	Temp_for_Ctl_Inclination = pFitShowAllData->Incline*10;
    xTreadmillData.RampAngleSetting = 0x7FFF;
    /**/
    xTreadmillData.PositiveElevationGain = 0;
    xTreadmillData.NegativeElevationGain = 0;
    /**/
    FTMS_TreadmillDataPackSerialization(&xDataPack, &xTreadmillData);
    if (xDataPack.Len <= MAX_BUFF_LEN)
    {
        DataPack_t *req = KE_MSG_ALLOC(FTMS_Treadmill_Data_0x2ACD_Notify,
                                       prf_get_task_from_id(TASK_ID_FTMS),
                                       KE_BUILD_ID(TASK_APP, conidx),
                                       DataPack);
        req->Len = xDataPack.Len;
        memcpy(req->Para, xDataPack.Para, xDataPack.Len);
        ke_msg_send(req);
    }
	}
}

static int app_FTMS_Treadmill_Data_Notify_Response(ke_msg_id_t const msgid,
                                                   struct ftms_notify_rsp const *param,
                                                   ke_task_id_t const dest_id,
                                                   ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}

void app_FitShow_To_FTMS_Cross_Trainer_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag) //  
{
	if(FTMS_Uart_ReadType == OLD_ReadType )//
	{
    DataPack_t xDataPack;
    CrossTrainerData_t xCrossTrainerData;
    memset(&xCrossTrainerData, 0, sizeof(CrossTrainerData_t));
    memset(&xDataPack, 0, sizeof(DataPack_t));
    xCrossTrainerData.Flag = FTMS_Flag;

    xCrossTrainerData.InstantaneousSpeed = pFitShowAllData->Speed;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xCrossTrainerData.InstantaneousSpeed = xCrossTrainerData.InstantaneousSpeed * 161 /100;
		}
		Temp_for_Ctl_Speed = pFitShowAllData->Speed;

    xCrossTrainerData.TotalDistance = pFitShowAllData->Distance;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xCrossTrainerData.TotalDistance = xCrossTrainerData.TotalDistance * 161 /100;
		}

    xCrossTrainerData.InstantaneousPower = pFitShowAllData->Power / 10;
    
    xCrossTrainerData.ResistanceLevel = pFitShowAllData->Resistance;
		
		Temp_for_Ctl_ResLevel = pFitShowAllData->Resistance;

    xCrossTrainerData.StepPerMinute = pFitShowAllData->Freq;

    xCrossTrainerData.AverageStepRate = pFitShowAllData->Count / pFitShowAllData->Time;

    xCrossTrainerData.HeartRate = pFitShowAllData->Heart;
		/**/
		HeartRate_NewCh[0] = 0x00;
		HeartRate_NewCh[1] = xCrossTrainerData.HeartRate;
		// app_fee4_send_ntf(0x00,2,HeartRate_NewCh);

    xCrossTrainerData.TotalEnergy = pFitShowAllData->Calories/10;
    xCrossTrainerData.EnergyPerHour = 0;
    xCrossTrainerData.EnergyPerMinute = 0;

    xCrossTrainerData.ElapsedTime = pFitShowAllData->Time;

    FTMS_CrossTrainerDataPackSerialization(&xDataPack, &xCrossTrainerData);
    if (xDataPack.Len <= MAX_BUFF_LEN)
    {
		ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        DataPack_t *req = KE_MSG_ALLOC(FTMS_Cross_Trainer_Data_0x2ACE_Notify,
                                       prf_get_task_from_id(TASK_ID_FTMS),
									   KE_BUILD_ID(TASK_APP, conidx),
                                       DataPack);
        req->Len = xDataPack.Len;
        memcpy(req->Para, xDataPack.Para, xDataPack.Len);
        ke_msg_send(req);
	
    }
	}
	
}
static int app_FTMS_Cross_Trainer_Data_Notify_Response(ke_msg_id_t const msgid,
                                                       struct ftms_notify_rsp const *param,
                                                       ke_task_id_t const dest_id,
                                                       ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}
void app_FitShow_To_FTMS_Step_Climber_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag)
{
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
    DataPack_t xDataPack;
    memset(&xDataPack, 0, sizeof(DataPack_t));
}
static int app_FTMS_Step_Climber_Data_Notify_Response(ke_msg_id_t const msgid,
                                                      struct ftms_notify_rsp const *param,
                                                      ke_task_id_t const dest_id,
                                                      ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}
void app_FitShow_To_FTMS_Stair_Climber_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag)
{
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
    DataPack_t xDataPack;
    memset(&xDataPack, 0, sizeof(DataPack_t));
}
static int app_FTMS_Stair_Climber_Data_Notify_Response(ke_msg_id_t const msgid,
                                                       struct ftms_notify_rsp const *param,
                                                       ke_task_id_t const dest_id,
                                                       ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}
void app_FitShow_To_FTMS_Rower_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag)
{
	if(FTMS_Uart_ReadType == OLD_ReadType )//
	{
    DataPack_t xDataPack;
    RowerData_t xRowerData;
    memset(&xRowerData, 0, sizeof(RowerData_t));
    memset(&xDataPack, 0, sizeof(DataPack_t));

    xRowerData.Flag = FTMS_Flag;

    xRowerData.StrokeRate = (pFitShowAllData->Freq << 1);

    xRowerData.HeartRate = pFitShowAllData->Heart;
		/**/
		HeartRate_NewCh[0] = 0x00;
		HeartRate_NewCh[1] = xRowerData.HeartRate;
		// app_fee4_send_ntf(0x00,2,HeartRate_NewCh);

    xRowerData.ResistanceLevel = pFitShowAllData->Resistance;
		
		Temp_for_Ctl_ResLevel = pFitShowAllData->Resistance;

    xRowerData.InstantaneousPace =  180000 / pFitShowAllData->Speed;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xRowerData.InstantaneousPace = xRowerData.InstantaneousPace * 161 /100;
		}
		Temp_for_Ctl_Speed = pFitShowAllData->Speed;

    xRowerData.InstantaneousPower = xFitShowAllData.Power / 10;

    xRowerData.StrokeCount = pFitShowAllData->Count;

    xRowerData.TotalDistance = pFitShowAllData->Distance;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xRowerData.TotalDistance = xRowerData.TotalDistance * 161 /100;
		}
  
    xRowerData.TotalEnergy = pFitShowAllData->Calories/10;
    xRowerData.EnergyPerHour = 0;
    xRowerData.EnergyPerMinute = 0;

    xRowerData.ElapsedTime = pFitShowAllData->Time;

    FTMS_RowerDataPackSerialization(&xDataPack, &xRowerData);



    if (xDataPack.Len <= MAX_BUFF_LEN)
    {
        DataPack_t *req = KE_MSG_ALLOC(FTMS_Rower_Data_0x2AD1_Notify,
                                       prf_get_task_from_id(TASK_ID_FTMS),
                                       KE_BUILD_ID(TASK_APP, conidx),
                                       DataPack);
        req->Len = xDataPack.Len;
        memcpy(req->Para, xDataPack.Para, xDataPack.Len);
        ke_msg_send(req);
    }
	}
	
}
static int app_FTMS_Rower_Data_Notify_Response(ke_msg_id_t const msgid,
                                               struct ftms_notify_rsp const *param,
                                               ke_task_id_t const dest_id,
                                               ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}
void app_FitShow_To_FTMS_Indoor_Bike_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag)
{
	if(FTMS_Uart_ReadType == OLD_ReadType )//
	{		
    DataPack_t xDataPack;
    IndoorBikeData_t xIndoorBikeData;
    memset(&xIndoorBikeData, 0, sizeof(IndoorBikeData_t));
    memset(&xDataPack, 0, sizeof(DataPack_t));
    /* More Data */
    //
    xIndoorBikeData.Flag = FTMS_Flag;
    xIndoorBikeData.InstantaneousSpeed = pFitShowAllData->Speed;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xIndoorBikeData.InstantaneousSpeed = xIndoorBikeData.InstantaneousSpeed * 161 /100;
		}
		Temp_for_Ctl_Speed = pFitShowAllData->Speed;

    xIndoorBikeData.InstantaneousCadence = pFitShowAllData->Freq << 1;

    // xIndoorBikeData.AverageCadence = pFitShowAllData->Freq;

    xIndoorBikeData.TotalDistance = pFitShowAllData->Distance;
		if (xFitShowAllData.xDeviceDataTypePara.Unit == Imperial)//
		{
			xIndoorBikeData.TotalDistance = xIndoorBikeData.TotalDistance * 161 /100;
		}

    xIndoorBikeData.ResistanceLevel = pFitShowAllData->Resistance;
		
		Temp_for_Ctl_ResLevel = pFitShowAllData->Resistance;

    xIndoorBikeData.InstantaneousPower = pFitShowAllData->Power / 10;

	//
	
    xIndoorBikeData.TotalEnergy = pFitShowAllData->Calories/10;
    xIndoorBikeData.EnergyPerHour = 0;
    xIndoorBikeData.EnergyPerMinute = 0;

    xIndoorBikeData.HeartRate = pFitShowAllData->Heart;
		
		/**/
		HeartRate_NewCh[0] = 0x00;
		HeartRate_NewCh[1] = xIndoorBikeData.HeartRate;
		// app_fee4_send_ntf(0x00,2,HeartRate_NewCh);
		
    xIndoorBikeData.ElapsedTime = pFitShowAllData->Time;

    FTMS_IndoorBikeDataPackSerialization(&xDataPack, &xIndoorBikeData);
    if (xDataPack.Len <= MAX_BUFF_LEN)
    {
        DataPack_t *req = KE_MSG_ALLOC(FTMS_Indoor_Bike_Data_0x2AD2_Notify,
                                       prf_get_task_from_id(TASK_ID_FTMS),
                                       KE_BUILD_ID(TASK_APP, conidx),
                                       DataPack);
        req->Len = xDataPack.Len;
        memcpy(req->Para, xDataPack.Para, xDataPack.Len);
        ke_msg_send(req);
    }
	}
	
}
static int app_FTMS_Indoor_Bike_Data_Notify_Response(ke_msg_id_t const msgid,
                                                     struct ftms_notify_rsp const *param,
                                                     ke_task_id_t const dest_id,
                                                     ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}
void app_FitShow_To_FTMS_Training_Status_Notify_handler(uint8_t conidx,uint8_t FTMS_StatusFlag, uint8_t FTMS_TrainingStatus, const char *str)// 
{
    DataPack_t xDataPack;
    TrainingStatusData_t xTrainingStatusData;
    memset(&xDataPack, 0, sizeof(DataPack_t));
    memset(&xTrainingStatusData, 0, sizeof(TrainingStatusData_t));

    xTrainingStatusData.Flag = FTMS_StatusFlag;
		
	
    xTrainingStatusData.Status = FTMS_TrainingStatus;

	
    memcpy(&xTrainingStatusData.StatusString[0], str, strlen(str));
	
	
    // FTMS_TrainingStatusDataPackSerialization(&xDataPack, &xTrainingStatusData);
    if (xDataPack.Len <= MAX_BUFF_LEN)
    {
        DataPack_t *req = KE_MSG_ALLOC(FTMS_Training_Status_0x2AD3_Notify,
                                       prf_get_task_from_id(TASK_ID_FTMS),
                                       KE_BUILD_ID(TASK_APP, conidx),
                                       DataPack);
        req->Len = xDataPack.Len;
        memcpy(req->Para, xDataPack.Para, xDataPack.Len);
        ke_msg_send(req);
    }
}
static int app_FTMS_Training_Status_Notify_Response(ke_msg_id_t const msgid,
                                                    struct ftms_notify_rsp const *param,
                                                    ke_task_id_t const dest_id,
                                                    ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}

//--> 
static int app_FTMS_Control_Point_Write_CallBack(ke_msg_id_t const msgid,
                                                 FTMS_ControlPointWriteData_t const *param,
                                                 ke_task_id_t const dest_id,
                                                 ke_task_id_t const src_id)
{
		DataBuff_t xDataPack;
		memset(&xDataPack, 0, sizeof(DataBuff_t));
		char ControlTargetValue[10];
		memset(&ControlTargetValue, 0, sizeof(ControlTargetValue));
	
	uint16_t value_res = 0;
    uint8_t value_resLast = 0;

	uint16_t PercentageInc = 0;
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

    FitnessMachineControlPointList_e RequestCode = (FitnessMachineControlPointList_e)param->op_code;
    FitnessMachineControlPointResultList_e ResultCode = (FitnessMachineControlPointResultList_e)0x00;

    uint16_t Value = 0;



    switch (RequestCode)
    {
    case RequestControl://
				//
				//       
				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_Success;
				}
				
        break;
    case FitnessMachineControlReset://
				// =
        /* code */

				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					/* code */ 
				}
				
        break;
    case SetTargetSpeed://
			
				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					Value = co_read16p(&param->para[0]);
					// if (Value > pftms_env->pSpeedRange->Maximum ||
					// 		Value < pftms_env->pSpeedRange->Minimum)
					if(0)
					{
							ResultCode = ResultCode_InvalidParameter;
					}
					else
					{
							ResultCode = ResultCode_Success;
							SetTargetSpeed_Function(Value);//HB ������֮��ĵ�byte���ݷ���
					}
				}
				
        

        /* code */
        break;
    case SetTargetInclination://
			

				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					Value = co_read16p(&param->para[0]);
		

					// if (Value > pftms_env->pInclinationRange->Maximum ||
					// 	Value < pftms_env->pInclinationRange->Minimum)
					if(0)
					{
						ResultCode = ResultCode_InvalidParameter;
					}
					else
					{
						ResultCode = ResultCode_Success;
						// xApp_FTMS_Handler.SetTargetInclination((int16_t)Value);//HB ������֮��ĵ�byte���ݷ���
						PercentageInc = Value;	
						uart_printf("--:%d,0x%02x\r\n",PercentageInc,PercentageInc);//HB		
						if (PercentageInc > 0xFF00)
						{
							value_resLast = 256 - (256 - (PercentageInc & 0x00FF)) / 10;					
						}
						else
						{
							value_resLast = (PercentageInc & 0x00FF) / 10;
						}
				
						SetTargetInclination_Function(value_resLast);
					}
				}
				
        /* code */
        break;
    case SetTargetResistanceLevel://(2 bytes)
				Training_Mode = Training_Mode_ResLevel;
			
				if (Training_Mode != Training_Mode_ResLevel)
				{
					
					break;
				}
				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					Value = param->para[0];
					// if (Value > pftms_env->pResistanceRange->Maximum ||
					// 		Value < pftms_env->pResistanceRange->Minimum)
					if(0)
					{
							ResultCode = ResultCode_InvalidParameter;
					}
					else
					{
							ResultCode = ResultCode_Success;
						
							if(pftms_env->pResistanceRange->Maximum){
								if(pftms_env->pResistanceRange->Maximum == 0xff)
									SetTargetResistanceLevel_Function((uint8_t)Value);//HB
								else{
									value_resLast = (Value*pftms_env->pResistanceRange->Maximum/10)/0xff;
									value_resLast = Value/10;
									SetTargetResistanceLevel_Function(value_resLast);//HB 
								}
							}
							else{
								SetTargetResistanceLevel_Function((uint8_t)Value);//HB 
								
							}
							
					}
				}
				
        /* code */
        break;
    case SetTargetPower://
				Training_Mode = Training_Mode_Power;
			
				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					Value = co_read16p(&param->para[0]);
					// if (Value > pftms_env->pPowerRange->Maximum ||
					// 		Value < pftms_env->pPowerRange->Minimum)
					if(0)
					{
							ResultCode = ResultCode_InvalidParameter;
					}
					else
					{
							ResultCode = ResultCode_Success;
							
							if (Training_Mode == Training_Mode_Power)
							{
								SetTargetPower_Function((int16_t)Value);//HB ������֮��ĵ�byte���ݷ���
								
							}
							
					}
				}
				
        /* code */
        break;
    case SetTargetHeartRate://
        
				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
        break;
    case StartorResume://
        ResultCode = ResultCode_Success;
	
        xApp_FTMS_Handler.StartOrResume();

        break;

    case StoporPause://
    {
			//
			memset(DataForMachineStatueUpdate,0,15);	
			ControlTargetValue[0] = param->para[0];
			strcpy(DataForMachineStatueUpdate,ControlTargetValue);
			
        switch (param->para[0])
        {
        case StopOrPause_Stop:
            ResultCode = ResultCode_Success;
			
            xApp_FTMS_Handler.Stop();
			
            break;
        case StopOrPause_Pause:
            ResultCode = ResultCode_Success;
            xApp_FTMS_Handler.Pause();//
            break;
        default:
            ResultCode = ResultCode_RFU;
            break;
        }

        break;
    }
    case SetTargetedExpendedEnergy://
        
				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedNumberofSteps://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedNumberofStrides://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedDistance://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedTrainingTime://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedTimeinTwoHeartRateZones://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedTimeinThreeHeartRateZones://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedTimeinFiveHeartRateZones://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetIndoorBikeSimulationParameters://
        if(FTMS_Uart_ReadType == OLD_ReadType )//HB ������֮��ĵ�byte���ݷ���
		{
			ResultCode = ResultCode_Success;

			Training_Mode = Training_Mode_ResLevel;
			if (Training_Mode != Training_Mode_ResLevel)
			{
				break;
			}			
	
			// Value = param->para[0]; 
			if(param->para[3] >= 0xf0)
			{
				value_res =(0xffff - ((param->para[3] << 8) | param->para[2]))*3  / 5 ;//HB 4500=>732   732				
				if(value_res > 128){value_res = 127;}
				value_res = 128 - value_res;
				uart_printf("-value_res-:%d,0x%02x\r\n",value_res,value_res);				
			}
			else
			{
				value_res =128 + ((param->para[3] << 8) | param->para[2])*2  / 6 ;//HB 4500=>768   768
				if(value_res > 255){value_res = 255;}
				uart_printf("-value_res-:%d,0x%02x\r\n",value_res,value_res);	
			}
			value_resLast = value_res & 0x00ff;

			if(pftms_env->pResistanceRange->Maximum)
			{
				if(pftms_env->pResistanceRange->Maximum == 0xff){SetTargetResistanceLevel_Function(value_resLast);}
				else
				{
					value_resLast = (value_resLast*pftms_env->pResistanceRange->Maximum/10)/0xff;
					SetTargetResistanceLevel_Function(value_resLast);
				}

			}
			else
			SetTargetResistanceLevel_Function(value_resLast);

			uart_printf("-value_resLast-LEVEL======:%d,0x%02x\r\n",value_resLast,value_resLast);	
		}
		
        break;
    case SetWheelCircumference://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
			
        break;
    case SpinDownControl://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    case SetTargetedCadence://
        if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
			
        break;
    default://
				if(FTMS_Uart_ReadType == OLD_ReadType )//
				{
					ResultCode = ResultCode_OpCodeNotSupported;
				}
				
        break;
    }

		if(FTMS_Uart_ReadType == OLD_ReadType )//
		{
			app_FTMS_ControlPointResponse_Indicate_handler(APP_Disconnect_ID,RequestCode, ResultCode);//
		}
		
		
    return KE_MSG_CONSUMED;
}

int app_FTMS_ControlPointResponse_Indicate_handler(uint8_t conidx,FitnessMachineControlPointList_e RequstCode, FitnessMachineControlPointResultList_e ResultCode)
{
	
    // Allocate the message
    DataPack_t *pDataPack = KE_MSG_ALLOC(FTMS_Control_Point_0x2AD9_Indicate,
                                        prf_get_task_from_id(TASK_ID_FTMS),
                                        TASK_APP,
                                        DataPack);

    FitnessMachineControlPointResponse_t xControlPointResponse;
    // memset(&xDataPack, 0, sizeof(DataPack_t));
    memset(&xControlPointResponse, 0, sizeof(FitnessMachineControlPointResponse_t));
    xControlPointResponse.ResponseCode = ResponseCode;//
    
    xControlPointResponse.ResultCode = ResultCode;
		xControlPointResponse.RequestCode = RequstCode;//op code

    // Fill in the parameter structure
    FTMS_ControlPointResponseDataPackSerialization(pDataPack, &xControlPointResponse);
	
	
		

    // Send the message
    ke_msg_send(pDataPack);

    return KE_MSG_CONSUMED;
}
static int app_FTMS_ControlPointResponse_Indicate_Response(ke_msg_id_t const msgid,
                                                        	struct ftms_notify_rsp const *param,
                                                        	ke_task_id_t const dest_id,
                                                           	ke_task_id_t const src_id)
{	
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}

void app_FitShow_To_FTMS_MachineStatus_Notify_handler(uint8_t conidx,FitnessMachineStatusList_e OpCode, uint8_t *pData, uint8_t len)
{
    FitnessMachineStatus_t xFitnessMachineStatus;
    DataPack_t xDataPack;
    memset(&xDataPack, 0, sizeof(DataPack_t));
    memset(&xFitnessMachineStatus, 0, sizeof(FitnessMachineStatus_t));
    xFitnessMachineStatus.OpCode = OpCode;
    memcpy(&xFitnessMachineStatus.Para[0], pData, len);
				
	
		///////////////////////////////////////////////////////////// FTMS_TrainingStatusDataPackSerialization
		// FTMS_FitnessMachineStatusDataPackSerialization(&xDataPack,&xFitnessMachineStatus);
	
	
		
		if (xDataPack.Len <= MAX_BUFF_LEN)
    {
        DataPack_t *req = KE_MSG_ALLOC(FTMS_MachineStatus_0x2ADA_Notify,
                                       prf_get_task_from_id(TASK_ID_FTMS),
                                       KE_BUILD_ID(TASK_APP, conidx),
                                       DataPack);
        req->Len = xDataPack.Len;
        memcpy(req->Para, xDataPack.Para, xDataPack.Len);
        ke_msg_send(req);
    }    
 
    
}

static void StartOrResume_Function(void)
{
	uint8_t index = 0;
	uint16_t fcs = 0;
	uint8_t Data[20];
	
	if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
	
		if(Temp_for_Device_Type == TREADMILL)
		{
				Data[index++] = 0x02;
				Data[index++] = 0x53;
				Data[index++] = 0x01;

				Data[index++] = 0x00;
				Data[index++] = 0x00;
				Data[index++] = 0x00;
				Data[index++] = 0x00;
			
				Data[index++] = 0x00;
				Data[index++] = 0x00;
				Data[index++] = 0x00;
				Data[index++] = 0x00;
				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);				
		}
		else
		{
				Data[index++] = 0x02;
				Data[index++] = 0x44;
				Data[index++] = 0x02;

				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	
		}

	}
			
}
static void Stop_Function(void)
{
	uint8_t index = 0;
	uint16_t fcs = 0;
	uint8_t Data[20];
	if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
		if(Temp_for_Device_Type == TREADMILL)
		{
				Data[index++] = 0x02;
				Data[index++] = 0x53;
				Data[index++] = 0x03;

				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);				
		}
		else
		{
				Data[index++] = 0x02;
				Data[index++] = 0x44;
				Data[index++] = 0x04;

				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	
		}

	}
}
static void Pause_Function(void)
{
	uint8_t index = 0;
	uint16_t fcs = 0;
	uint8_t Data[20];
	
	if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
		if(Temp_for_Device_Type == TREADMILL)
		{
				Data[index++] = 0x02;
				Data[index++] = 0x53;
				Data[index++] = 0x0A;

				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);				
		}
		else
		{
				Data[index++] = 0x02;
				Data[index++] = 0x44;
				Data[index++] = 0x03;

				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	
		}

	}
}

static void SetTargetResistanceLevel_Function(uint8_t Level)
{
	uint8_t index = 0;
	uint16_t fcs = 0;
	uint8_t Data[20];
	
	if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
		if(Temp_for_Device_Type == TREADMILL)
		{
	
		}
		else
		{
				Data[index++] = 0x02;
				Data[index++] = 0x44;
				Data[index++] = 0x05;
	
				Data[index++] = Level;
				Data[index++] = Temp_for_Ctl_Inclination;
			
				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	

			
		}

	}	
}
static void SetTargetPower_Function(int16_t Watt)
{
	
	uint8_t index = 0;
	uint16_t fcs = 0;
	uint8_t Data[20];
	
	
	if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
		if(Temp_for_Device_Type == TREADMILL)
		{
	
		}
		else
		{
			
				Data[index++] = 0x02;
				Data[index++] = 0x44;
				Data[index++] = 0x0B;
	
			Data[index++] = 0x00;
			Data[index++] = 0x00;
			Data[index++] = 0x00;
			Data[index++] = 0x00;
			
				Data[index++] = 0x30;//HB mode
			
			Data[index++] = 0x00;
			
				Data[index++] = Watt & 0x00ff;
				Data[index++] = Watt >> 8;
			
				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	
			
		}

	}	
}
static void SetTargetInclination_Function(uint8_t Percent)
{
	uint8_t index = 0;
	uint16_t fcs = 0;
	uint8_t Data[20];
	
	if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
		if(Temp_for_Device_Type == TREADMILL)
		{
				Data[index++] = 0x02;
				Data[index++] = 0x53;
				Data[index++] = 0x02;
	
//				Data[index++] = (Temp_for_Ctl_Speed>>8) & 0x00FF;
				Data[index++] =  (Temp_for_Ctl_Speed) & 0x00FF;
				Data[index++] =  (Percent) & 0x00FF;
			
				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	
		}
		else
		{
			
				Data[index++] = 0x02;
				Data[index++] = 0x44;
				Data[index++] = 0x05;
	
				Data[index++] = Temp_for_Ctl_ResLevel;
				Data[index++] = (Percent) & 0x00FF;
			
				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	
			
		}

	}	
}
static void SetTargetSpeed_Function(uint16_t Value)
{
	uint8_t index = 0;
	uint16_t fcs = 0;
	uint8_t Data[20];
	
	if(IO_or_Uart_Mode == FITSHOW_DEVICE_PARAM_DATA_ACQ_UART_MODE)
	{
		if(Temp_for_Device_Type == TREADMILL)
		{
				Data[index++] = 0x02;
				Data[index++] = 0x53;
				Data[index++] = 0x02;
	
//				Data[index++] = (Temp_for_Ctl_Speed>>8) & 0x00FF;
				Data[index++] =  (Value) & 0x00FF;
				Data[index++] =  (Temp_for_Ctl_Inclination) & 0x00FF;
			
				for(uint8_t i=1;i<index;i++)
				{
					fcs ^=Data[i];
				}
				
				Data[index++] = fcs;
				Data[index++] = 0x03;
				BLE_SendToUart0(&Data[0], index);	
		}
		else
		{
			
			
		}

	}	

}
static int app_FTMS_MachineStatus_Notify_Response(ke_msg_id_t const msgid,
                                                  struct ftms_notify_rsp const *param,
                                                  ke_task_id_t const dest_id,
                                                  ke_task_id_t const src_id)
{
    if (param->status == GAP_ERR_NO_ERROR)
    {
        app_ftms_env.send_status = 0;
    }
    return KE_MSG_CONSUMED;
}

// Default State handlers definition
const struct ke_msg_handler app_ftm_msg_handler_list[] =
    {
        // Note: first message is latest message checked by kernel so default is put on top.
        {KE_MSG_DEFAULT_HANDLER, (ke_msg_func_t)app_FTMS_msg_dflt_handler},
        /******/
        {FTMS_Treadmill_Data_0x2ACD_Notify_RSP, (ke_msg_func_t)(app_FTMS_Treadmill_Data_Notify_Response)},
        /******/
        {FTMS_Cross_Trainer_Data_0x2ACE_Notify_RSP, (ke_msg_func_t)(app_FTMS_Cross_Trainer_Data_Notify_Response)},
        /******/
        {FTMS_Step_Climber_Data_0x2ACF_Notify_RSP, (ke_msg_func_t)(app_FTMS_Step_Climber_Data_Notify_Response)},
        /******/
        {FTMS_Stair_Climber_Data_0x2AD0_Notify_RSP, (ke_msg_func_t)(app_FTMS_Stair_Climber_Data_Notify_Response)},
        /******/
        {FTMS_Rower_Data_0x2AD1_Notify_RSP, (ke_msg_func_t)(app_FTMS_Rower_Data_Notify_Response)},
        /******/
        {FTMS_Indoor_Bike_Data_0x2AD2_Notify_RSP, (ke_msg_func_t)(app_FTMS_Indoor_Bike_Data_Notify_Response)},
        /******/
        {FTMS_Training_Status_0x2AD3_Notify_RSP, (ke_msg_func_t)(app_FTMS_Training_Status_Notify_Response)},
        /******/
        {FTMS_Control_Point_0x2AD9_Write, (ke_msg_func_t)(app_FTMS_Control_Point_Write_CallBack)},
        {FTMS_Control_Point_0x2AD9_Indicate_RSP, (ke_msg_func_t)(app_FTMS_ControlPointResponse_Indicate_Response)},
        /******/
        {FTMS_MachineStatus_0x2ADA_Notify_RSP, (ke_msg_func_t)(app_FTMS_MachineStatus_Notify_Response)},
};

const struct app_subtask_handlers app_ftms_handler = APP_HANDLERS(app_ftm);
#endif
