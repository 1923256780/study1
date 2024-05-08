
#ifndef __DATAFORMAT_H__
#define __DATAFORMAT_H__

#include "stdint.h"
#include "stdbool.h"

#define MAX_BUFF_LEN 20

typedef enum FTMS_TYPE
{
    TREADMILL_TYPE = (0x01 << 0),
    CROSS_TYPE = (0x01 << 1),
    STEP_TYPE = (0x01 << 2), 
    STAIR_TYPE = (0x01 << 3),
    ROWER_TYPE = (0x01 << 4),
    INDOOR_BIKE_TYPE = (0x01 << 5),
} FTMS_TYPE_e;

typedef enum//�豸��������ʾ���ɼ�֧��
{
    FTMS_FeaturesAverageSpeedSupported = (1 << 0),
    FTMS_FeaturesCadenceSupported = (1 << 1),
    FTMS_FeaturesTotalDistanceSupported = (1 << 2),
    FTMS_FeaturesInclinationSupported = (1 << 3),
    FTMS_FeaturesElevationGainSupported = (1 << 4),
    FTMS_FeaturesPaceSupported = (1 << 5),
    FTMS_FeaturesStepCountSupported = (1 << 6),
    FTMS_FeaturesResistanceLevelSupported = (1 << 7),
    FTMS_FeaturesStrideCountSupported = (1 << 8),
    FTMS_FeaturesExpendedEnergySupported = (1 << 9),
    FTMS_FeaturesHeartRateMeasurementSupported = (1 << 10),
    FTMS_FeaturesMetabolicEquivalentSupported = (1 << 11),
    FTMS_FeaturesElapsedTimeSupported = (1 << 12),
    FTMS_FeaturesRemainingTimeSupported = (1 << 13),
    FTMS_FeaturesPowerMeasurementSupported = (1 << 14),
    FTMS_FeaturesForceonBeltandPowerOutputSupported = (1 << 15),
    FTMS_FeaturesUserDataRetentionSupported = (1 << 16),
} FitnessMachineFeaturesFieldList_e;

typedef enum TargetSettingFeaturesFieldList//�豸�����ơ�����֧��
{
    FTMS_SettingFeaturesSpeedTargetSettingSupported = (1 << 0),
    FTMS_SettingFeaturesInclinationTargetSettingSupported = (1 << 1),
    FTMS_SettingFeaturesResistanceTargetSettingSupported = (1 << 2),
    FTMS_SettingFeaturesPowerTargetSettingSupported = (1 << 3),
    FTMS_SettingFeaturesHeartRateTargetSettingSupported = (1 << 4),
    FTMS_SettingFeaturesTargetedExpendedEnergyConfigurationSupported = (1 << 5),
    FTMS_SettingFeaturesTargetedStepNumberConfigurationSupported = (1 << 6),
    FTMS_SettingFeaturesTargetedStrideNumberConfigurationSupported = (1 << 7),
    FTMS_SettingFeaturesTargetedDistanceConfigurationSupported = (1 << 8),
    FTMS_SettingFeaturesTargetedTrainingTimeConfigurationSupported = (1 << 9),
    FTMS_SettingFeaturesTargetedTimeinTwoHeartRateZonesConfigurationSupported = (1 << 10),
    FTMS_SettingFeaturesTargetedTimeinThreeHeartRateZonesConfigurationSupported = (1 << 11),
    FTMS_SettingFeaturesTargetedTimeinFiveHeartRateZonesConfigurationSupported = (1 << 12),
    FTMS_SettingFeaturesIndoorBikeSimulationParametersSupported = (1 << 13),
    FTMS_SettingFeaturesWheelCircumferenceConfigurationSupported = (1 << 14),
    FTMS_SettingFeaturesSpinDownControlSupported = (1 << 15),
    FTMS_SettingFeaturesTargetedCadenceConfigurationSupported = (1 << 16),
} TargetSettingFeaturesFieldList_e;

typedef enum Device_Power_Type
{
	Device_Power_Type_No_Power = 0x01,
	Device_Power_Type_Need_Power = 0x02,
}Device_Power_Type_e;

typedef enum Device_Report_MetricOrImperial
{
	Device_Report_Metric = 0x01,
	Device_Report_Imperial = 0x02,
}Device_Report_MetricOrImperial_e;

typedef enum DeviceReportFitnessMachineFeatureList
{
	Report_SpeedTargetSettingSupported 															= 0x00000001 << 0,
	Report_InclinationTargetSettingSupported 												= 0x00000001 << 1,
	Report_ResistanceTargetSettingSupported	 												= 0x00000001 << 2,
	Report_PowerTargetSettingSupported 															= 0x00000001 << 3,
	Report_HeartRateTargetSettingSupported 													= 0x00000001 << 4,
	Report_TargetedExpendedEnergyConfigurationSupported 						= 0x00000001 << 5,
	Report_TargetedStepNumberConfigurationSupported						 			= 0x00000001 << 6,
	Report_TargetedStrideNumberConfigurationSupported						 		= 0x00000001 << 7,
	Report_TargetedDistanceConfigurationSupported 									= 0x00000001 << 8,
	Report_TargetedTrainingTimeConfigurationSupported 							= 0x00000001 << 9,
	Report_TargetedTimeinTwoHeartRateZonesConfigurationSupported 		= 0x00000001 << 10,
	Report_TargetedTimeinThreeHeartRateZonesConfigurationSupported 	= 0x00000001 << 11,
	Report_TargetedTimeinFiveHeartRateZonesConfigurationSupported 	= 0x00000001 << 12,
	Report_IndoorBikeSimulationParametersSupported 									= 0x00000001 << 13,
	Report_WheelCircumferenceConfigurationSupported 								= 0x00000001 << 14,
	Report_SpinDownControlSupported							 										= 0x00000001 << 15,
	Report_TargetedCadenceConfigurationSupported 										= 0x00000001 << 16,
	
	
	
	//-----------------------RFU
	Report_RFU = 0x00000001 << 17,
	
}DeviceReportFitnessMachineFeatureList_e;

typedef struct TrainingStatusData
{
    uint8_t Flag;
    uint8_t Status;
    uint8_t StatusString[MAX_BUFF_LEN];
} TrainingStatusData_t;
typedef struct TreadmillData
{
    uint16_t Flag;
    uint16_t InstantaneousSpeed;
    uint16_t AverageSpeed;
    uint32_t TotalDistance;// : 24;
    int16_t Inclination;
    int16_t RampAngleSetting;
    uint16_t PositiveElevationGain;
    uint16_t NegativeElevationGain;
    uint16_t InstantaneousPace;//*****8-16
    uint16_t AveragePace;//*****8-16
    uint16_t TotalEnergy;
    uint16_t EnergyPerHour;
    uint8_t EnergyPerMinute;
    uint8_t HeartRate;
    uint8_t MetabolicEquivalent;
    uint16_t ElapsedTime;
    uint16_t RemainingTime;
    int16_t ForceonBelt;
    int16_t PowerOutput;
} TreadmillData_t;

typedef struct CrossTrainerData
{
    uint32_t Flag;
    uint16_t InstantaneousSpeed;
    uint16_t AverageSpeed;
    uint32_t TotalDistance : 24;
    uint16_t StepPerMinute;
    uint16_t AverageStepRate;
    uint16_t StrideCount;
    
    uint16_t PositiveElevationGain;
    uint16_t NegativeElevationGain;
	
		int16_t Inclination;
    int16_t RampAngleSetting;
	
    int8_t ResistanceLevel;
    int16_t InstantaneousPower;
    int16_t AveragePower;
    uint16_t TotalEnergy;
    uint16_t EnergyPerHour;
    uint8_t EnergyPerMinute;
    uint8_t HeartRate;
    uint8_t MetabolicEquivalent;
    uint16_t ElapsedTime;
    uint16_t RemainingTime;
		
		bool Direction;
} CrossTrainerData_t;

typedef struct StepClimberData
{
    uint16_t Flag;
    uint16_t Floors;
    uint16_t StepCount;
    uint16_t StepPerMinute;
    uint16_t AverageStepRate;
    uint16_t PositiveElevationGain;
		uint16_t TotalEnergy;
    uint16_t EnergyPerHour;
    uint8_t EnergyPerMinute;
    uint8_t HeartRate;
    uint8_t MetabolicEquivalent;
    uint16_t ElapsedTime;
    uint16_t RemainingTime;
} StepClimberData_t;

typedef struct StairClimberData
{
    uint16_t Flag;
    uint16_t Floors;
    uint16_t StepPerMinute;
    uint16_t AverageStepRate;
    uint16_t PositiveElevationGain;
    uint16_t StrideCount;
    uint16_t TotalEnergy;
    uint16_t EnergyPerHour;
    uint8_t EnergyPerMinute;
    uint8_t HeartRate;
    uint8_t MetabolicEquivalent;
    uint16_t ElapsedTime;
    uint16_t RemainingTime;
} StairClimberData_t;

typedef struct RowerData
{
    uint16_t Flag;
    uint8_t StrokeRate;
    uint16_t StrokeCount;
    uint8_t AverageStrokeRate;
    uint32_t TotalDistance : 24;
    uint16_t InstantaneousPace;
    uint16_t AveragePace;
    int16_t InstantaneousPower;
    int16_t AveragePower;
    int8_t ResistanceLevel;
    uint16_t TotalEnergy;
    uint16_t EnergyPerHour;
    uint8_t EnergyPerMinute;
    uint8_t HeartRate;
    uint8_t MetabolicEquivalent;
    uint16_t ElapsedTime;
    uint16_t RemainingTime;
} RowerData_t;

typedef struct IndoorBikeData
{
    uint16_t Flag;
    uint16_t InstantaneousSpeed;
    uint16_t AverageSpeed;
    uint16_t InstantaneousCadence;
    uint16_t AverageCadence;
    uint32_t TotalDistance : 24;
    int8_t ResistanceLevel;
    int16_t InstantaneousPower;
    int16_t AveragePower;
    uint16_t TotalEnergy;
    uint16_t EnergyPerHour;
    uint8_t EnergyPerMinute;
    uint8_t HeartRate;
		uint16_t HeartRate_New;
	
    uint8_t MetabolicEquivalent;
    uint16_t ElapsedTime;
    uint16_t RemainingTime;
} IndoorBikeData_t;

typedef struct Device_Report_FeaturesFiedList
{
	uint8_t Report_Setting_Statue;//��ʼ����־λ
	
	TargetSettingFeaturesFieldList_e Device_Report_Control;
	FitnessMachineFeaturesFieldList_e Device_Report_Features;
	
	uint8_t  DeviceReportPowerType;
	uint16_t DeviceReportDeviceType;
	uint16_t DeviceReportPlantCode;
	uint16_t DeviceReportMachineCode;
	uint32_t DeviceReportFitnessMachineFeature;
	uint32_t DeviceReportTargetSettingFeature;
	
	uint16_t DeviceReport_MinSpeed;
	uint16_t DeviceReport_MaxSpeed;
	uint16_t DeviceReport_MinIncrementSpeed;	
	
	uint16_t DeviceReport_MinInclination;
	uint16_t DeviceReport_MaxInclination;
	uint16_t DeviceReport_MinIncrementInclination;	
	
	uint16_t DeviceReport_MinResistance;
	uint16_t DeviceReport_MaxResistance;
	uint16_t DeviceReport_MinIncrementResistance;	

	uint16_t DeviceReport_MinPower;
	uint16_t DeviceReport_MaxPower;
	uint16_t DeviceReport_MinIncrementPower;	

	uint16_t DeviceReport_MinHeartRate;
	uint16_t DeviceReport_MaxHeartRate;
	uint16_t DeviceReport_MinIncrementHeartRate;	
	
	uint16_t AppSetDevice_Speed_Target;
	uint16_t AppSetDevice_Inclination_Target;
	uint16_t AppSetDevice_Resistance_Target;
	uint16_t AppSetDevice_Power_Target;
	uint16_t AppSetDevice_HeartRate_Target;
	uint16_t AppSetDevice_ExpendedEnergy_Target;
	uint16_t AppSetDevice_NumberOfSteps_Target;
	uint16_t AppSetDevice_NumberOfStrides_Target;
	uint32_t AppSetDevice_Distance_Target;//3 Byte
	uint16_t AppSetDevice_TrainingTime_Target;
	uint16_t AppSetDevice_TimeInTwoHeartRateZones_Target;//4 Byte
	uint16_t AppSetDevice_TimeInThreeHeartRateZones_Target;//6 Byte
	uint16_t AppSetDevice_TimeInFiveHeartRateZones_Target;//10 Byte
	
	uint16_t AppSetDevice_IndoorBikeSimulationParameters;//6 Byte
	uint16_t AppSetDevice_WheelCircumference;
	uint8_t AppSetDevice_SpinDownControl;//1 Byte
	
	uint16_t AppSetDevice_Cadence_Target;
//Time
	uint16_t AppSetDevice_FeaturesElapsedTime;
	uint16_t AppSetDevice_RemainingTimeTime;

//Training Statues
	uint8_t  AppSetDevice_TrainingStatues;
	TrainingStatusData_t DeviceReport_TrainingStatue;
//FTMS_Flag for read Data
	uint32_t FitnessMachineDataGet_Flag;

//Model get FTMS device data
	TreadmillData_t 			xTreadmillData;
	CrossTrainerData_t 		xCrossTrainerData;
	StepClimberData_t 		xStepClimberData;
	StairClimberData_t 		xStairClimberData;
	RowerData_t 					xRowerData;
	IndoorBikeData_t 			xIndoorBikeData;
}Device_Report_FeaturesFiedList_t;

typedef enum TreadmillDataList
{
    TreadmillDataMoreData = (1 << 0),
    TreadmillDataAverageSpeed = (1 << 1),
    TreadmillDataTotalDistance = (1 << 2),
    TreadmillDataInclinationAndRampAngleSetting = (1 << 3),
    TreadmillDataElevationGain = (1 << 4),
    TreadmillDataInstantaneousPace = (1 << 5),
    TreadmillDataAveragePace = (1 << 6),
    TreadmillDataExpendedEnergy = (1 << 7),
    TreadmillDataHeartRate = (1 << 8),
    TreadmillDataMetabolicEquivalent = (1 << 9),
    TreadmillDataElapsedTime = (1 << 10),
    TreadmillDataRemainingTime = (1 << 11),
    TreadmillDataForceonBeltandPowerOutput = (1 << 12),
} TreadmillDataList_e;

typedef enum CrossTrainerDataList
{
    CrossTrainerDataMoreData = (1 << 0),  
    CrossTrainerDataAverageSpeed = (1 << 1),
    CrossTrainerDataTotalDistance = (1 << 2),
    CrossTrainerDataStepCount = (1 << 3),
    CrossTrainerDataStrideCount = (1 << 4),
    CrossTrainerDataElevationGain = (1 << 5),
    CrossTrainerDataInclinationandRampAngleSetting = (1 << 6),
    CrossTrainerDataResistanceLevel = (1 << 7),
    CrossTrainerDataInstantaneousPower = (1 << 8),
    CrossTrainerDataAveragePower = (1 << 9),
    CrossTrainerDataExpendedEnergy = (1 << 10),
    CrossTrainerDataHeartRate = (1 << 11),
    CrossTrainerDataMetabolicEquivalent = (1 << 12),
    CrossTrainerDataElapsedTime = (1 << 13),
    CrossTrainerDataRemainingTime = (1 << 14),
    CrossTrainerDataMovementDirection = (1 << 15),
} CrossTrainerDataList_e;

typedef enum StepClimberDataList
{
    StepClimberDataMoreData = (1 << 0),
    StepClimberDataStepperMinute = (1 << 1),
    StepClimberDataAverageStepRate = (1 << 2),
    StepClimberDataPositiveElevationGain = (1 << 3),
    StepClimberDataExpendedEnergy = (1 << 4),
    StepClimberDataHeartRate = (1 << 5),
    StepClimberDataMetabolicEquivalent = (1 << 6),
    StepClimberDataElapsedTime = (1 << 7),
    StepClimberDataRemainingTime = (1 << 8),
} StepClimberDataList_e;

typedef enum StairClimberDataList
{
    StairClimberDataMoreData = (1 << 0),
    StairClimberDataStepperMinute = (1 << 1),
    StairClimberDataAverageStepRate = (1 << 2),
    StairClimberDataPositiveElevationGain = (1 << 3),
    StairClimberDataStrideCount = (1 << 4),
    StairClimberDataExpendedEnergy = (1 << 5),
    StairClimberDataHeartRate = (1 << 6),
    StairClimberDataMetabolicEquivalent = (1 << 7),
    StairClimberDataElapsedTime = (1 << 8),
    StairClimberDataRemainingTime = (1 << 9),
} StairClimberDataList_e;

typedef enum RowDataList
{
    RowerDataMoreData = (1 << 0),
    RowerDataAverageStroke = (1 << 1),
    RowerDataTotalDistance = (1 << 2),
    RowerDataInstantaneousPace = (1 << 3),
    RowerDataAveragePace = (1 << 4),
    RowerDataInstantaneousPower = (1 << 5),
    RowerDataAveragePower = (1 << 6),
    RowerDataResistanceLevel = (1 << 7),
    RowerDataExpendedEnergy = (1 << 8),
    RowerDataHeartRate = (1 << 9),
    RowerDataMetabolicEquivalent = (1 << 10),
    RowerDataElapsedTime = (1 << 11),
    RowerDataRemainingTime = (1 << 12),
} RowDataList_e;

typedef enum IndoorBikeDataList
{
    IndoorBikeDataMoreData = (1 << 0),
    IndoorBikeDataAverageSpeed = (1 << 1),
    IndoorBikeDataInstantaneousCadence = (1 << 2),
    IndoorBikeDataAverageCadence = (1 << 3),
    IndoorBikeDataTotalDistance = (1 << 4),
    IndoorBikeDataResistanceLevel = (1 << 5),
    IndoorBikeDataInstantaneousPower = (1 << 6),
    IndoorBikeDataAveragePower = (1 << 7),
    IndoorBikeDataExpendedEnergy = (1 << 8),
    IndoorBikeDataHeartRate = (1 << 9),
    IndoorBikeDataMetabolicEquivalent = (1 << 10),
    IndoorBikeDataElapsedTime = (1 << 11),
    IndoorBikeDataRemainingTime = (1 << 12),
} IndoorBikeDataList_e;

typedef enum TrainingStatusFlagList
{
    TrainingStatus = (1 << 0),
    TrainingStatusString = (1 << 1),
} TrainingStatusFlagList_e;

typedef enum TrainingStatusList
{
    Other = 0x00,
    Idle = 0x01,
    WarmingUp = 0x02,
    LowIntensityInterval = 0x03,
    HighIntensityInterval = 0x04,
    RecoveryInterval = 0x05,
    Isometric = 0x06,
    HeartRateControl = 0x07,
    FitnessTest = 0x08,
    SpeedOutsideOfControlRegionLow = 0x09,  //(increase speed to return to controllable region)
    SpeedOutsideOfControlRegionHigh = 0x0A, //(decrease speed to return to controllable region)
    CoolDown = 0x0B,
    WattControl = 0x0C,
    ManualModeQuickStart = 0x0D,
    Pre_Workout = 0x0E,
    Post_Workout = 0x0F,
} TrainingStatusList_e;

typedef enum FitnessMachineControlPointList
{
    RequestControl,
    FitnessMachineControlReset,
    SetTargetSpeed,
    SetTargetInclination,
    SetTargetResistanceLevel,
    SetTargetPower,
    SetTargetHeartRate,
    StartorResume,
    StoporPause,
    SetTargetedExpendedEnergy,
    SetTargetedNumberofSteps,
    SetTargetedNumberofStrides,
    SetTargetedDistance,
    SetTargetedTrainingTime,
    SetTargetedTimeinTwoHeartRateZones,
    SetTargetedTimeinThreeHeartRateZones,
    SetTargetedTimeinFiveHeartRateZones,
    SetIndoorBikeSimulationParameters,
    SetWheelCircumference,
    SpinDownControl,
    SetTargetedCadence,
		
		RFU_Line, 
    ResponseCode = 0x80,
} FitnessMachineControlPointList_e;

typedef enum FitnessMachineControlPointResultList
{
    ResultCode_RFU = 0,
    ResultCode_Success,
    ResultCode_OpCodeNotSupported,
    ResultCode_InvalidParameter,
    ResultCode_OperationFailed,
    ResultCode_ControlNotPermitted,
} FitnessMachineControlPointResultList_e;

typedef enum StoporPause
{
    StopOrPause_Stop = 0x01,
    StopOrPause_Pause,
} StoporPause_e;

typedef enum DeviceUartModeStatueList
{
    DeviceStatue_Stop,
    DeviceStatue_Pause,
		DeviceStatue_StartOrResume,
} DeviceUartModeStatueList_e;

typedef enum SpinDownControl
{
    SpinDownControl_Start = 0x01,
    SpinDownControl_Ignore,
} SpinDownControl_e;

typedef enum SpinDownOpCode
{
    SpinDownOpCode_SpinDownRequested = 0x01,
    SpinDownOpCode_Success,
    SpinDownOpCode_Error,
    SpinDownOpCode_StopPedaling,
} SpinDownOpCode_e;

typedef enum FitnessMachineStatusList
{ 
    FitnessMachineStatusReset = 0x01, 
    FitnessMachineStoppedorPausedbytheUser,
    FitnessMachineStoppedbySafetyKey, 
    FitnessMachineStartedorResumedbytheUser,
    TargetSpeedChanged,
    TargetInclineChanged,
    TargetResistanceLevelChanged,
    TargetPowerChanged,
    TargetHeartRateChanged,
    TargetedExpendedEnergyChanged,
    TargetedNumberofStepsChanged,
    TargetedNumberofStridesChanged,
    TargetedDistanceChanged,
    TargetedTrainingTimeChanged,
    TargetedTimeinTwoHeartRateZonesChanged,
    TargetedTimeinThreeHeartRateZonesChanged,
    TargetedTimeinFiveHeartRateZonesChanged,
    IndoorBikeSimulationParametersChanged,
    WheelCircumferenceChanged,
    SpinDownStatus,
    TargetedCadenceChanged,
		TraingStatueChanged = 0xA1,
    ControlPermissionLost = 0xFF,
} FitnessMachineStatusList_e;

typedef struct FitnessMachineFeature
{
    uint32_t FitnessMachineFeatures;
    uint32_t TargetSettingFeatures;
} FitnessMachineFeature_t;




typedef struct SupportedSpeedRange
{
    uint16_t Minimum;
    uint16_t Maximum;
    uint16_t Increment;
} SupportedSpeedRange_t;

typedef struct SupportedInclinationRange
{
    int16_t Minimum;
    int16_t Maximum;
    uint16_t Increment;
} SupportedInclinationRange_t;

typedef struct SupportedResistanceRange
{
    uint16_t Minimum;
    uint16_t Maximum;
    uint16_t Increment;
} SupportedResistanceRange_t;

typedef struct SupportedPowerRange
{
    int16_t Minimum;
    int16_t Maximum;
    uint16_t Increment;
} SupportedPowerRange_t;

typedef struct SupportedHeartRateRange
{
    uint8_t Minimum;
    uint8_t Maximum;
    uint8_t Increment;
} SupportedHeartRateRange_t;
typedef struct FitnessMachineControlPoint
{
    uint8_t op_code;
    uint8_t Para[20];
} FitnessMachineControlPoint_t;

typedef struct FitnessMachineControlPointResponse
{
    uint8_t ResponseCode;
    uint8_t RequestCode;
    uint8_t ResultCode;
	
		//Spin Down Control feeback
		uint16_t TargetSpeedLowOrHigh;
	
    uint8_t Para[20];
} FitnessMachineControlPointResponse_t;

typedef struct FitnessMachineStatus
{
    uint8_t OpCode;
    uint8_t Para[20];
} FitnessMachineStatus_t;

typedef struct DataPack
{
    uint8_t Para[40];
    uint8_t Len;
} DataPack_t;

void FTMS_FeatureDataPackSerialization(DataPack_t *pDataPack, FitnessMachineFeature_t *pFeature);
void FTMS_TreadmillDataPackSerialization(DataPack_t *pDataPack, TreadmillData_t *pTreadmillData);
void FTMS_CrossTrainerDataPackSerialization(DataPack_t *pDataPack, CrossTrainerData_t *pCrossTrainerData);
void FTMS_StepClimberDataPackSerialization(DataPack_t *pDataPack, StepClimberData_t *pStepClimberData);
void FTMS_StairClimberDataPackSerialization(DataPack_t *pDataPack, StairClimberData_t *pStairClimberData);
void FTMS_RowerDataPackSerialization(DataPack_t *pDataPack, RowerData_t *pRowData);
void FTMS_IndoorBikeDataPackSerialization(DataPack_t *pDataPack, IndoorBikeData_t *pIndoorBikeData);
void FTMS_TrainingStatusDataPackSerialization(DataPack_t *pDataPack, TrainingStatusData_t *pTrainingStatusData);
void FTMS_SupportedSpeedRangeDataPackSerialization(DataPack_t *pDataPack, SupportedSpeedRange_t *pData);
void FTMS_SupportedInclinationRangeDataPackSerialization(DataPack_t *pDataPack, SupportedInclinationRange_t *pData);
void FTMS_SupportedResistanceRangeDataPackSerialization(DataPack_t *pDataPack, SupportedResistanceRange_t *pData);
void FTMS_SupportedPowerRangeDataPackSerialization(DataPack_t *pDataPack, SupportedPowerRange_t *pData);
void FTMS_SupportedHeartRateRangeDataPackSerialization(DataPack_t *pDataPack, SupportedHeartRateRange_t *pData);
void FTMS_ControlPointResponseDataPackSerialization(DataPack_t *pDataPack, FitnessMachineControlPointResponse_t *pResponseData);
void FTMS_FitnessMachineStatusDataPackSerialization(DataPack_t *pDataPack, FitnessMachineStatus_t *pFitnessMachineStatus);

#endif
