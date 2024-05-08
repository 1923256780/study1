/*
 * @Author: LHB
 * @Date: 2023-05-30 17:10:01
 * @LastEditors: LHB
 * @LastEditTime: 2023-06-02 14:06:49
 * @Description: 
 * 1
 */
#include "DataFormat.h"
#include "string.h"
#include "co_utils.h"

#include "Device.h"

#include "SystemConfig.h"



extern uint8_t FTMS_Uart_ReadType;
/*
	typedef struct FitnessMachineFeature
	{
			uint32_t FitnessMachineFeatures;
			uint32_t TargetSettingFeatures;
	} FitnessMachineFeature_t;
*/
void FTMS_FeatureDataPackSerialization(DataPack_t *pDataPack, FitnessMachineFeature_t *pFeature)
{
	if (pDataPack == NULL) return;
    memcpy(&pDataPack->Para[0], (uint8_t*)pFeature, sizeof(FitnessMachineFeature_t));//->Para
		uart_printf("pFeature:%x \r\n",pFeature->FitnessMachineFeatures);
		uart_printf("pFeature:%x \r\n",pFeature->TargetSettingFeatures);
    pDataPack->Len = sizeof(FitnessMachineFeature_t);
}

void FTMS_TreadmillDataPackSerialization(DataPack_t *pDataPack, TreadmillData_t *pTreadmillData) 
{
	if (pDataPack == NULL || pTreadmillData == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pTreadmillData->Flag);
    index += sizeof(uint16_t);
    if (pTreadmillData->Flag & TreadmillDataMoreData)// 0
    {
        /* code */
    }
    else  
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->InstantaneousSpeed);
        index += sizeof(uint16_t);
    }
    if (pTreadmillData->Flag & TreadmillDataAverageSpeed)// 1
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->AverageSpeed);
        index += sizeof(uint16_t);
    }
    if (pTreadmillData->Flag & TreadmillDataTotalDistance)
    {
        co_write24p(&pDataPack->Para[index], pTreadmillData->TotalDistance);
        index += 3;
    }
    if (pTreadmillData->Flag & TreadmillDataInclinationAndRampAngleSetting)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->Inclination);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pTreadmillData->RampAngleSetting);
        index += sizeof(uint16_t);
    }
		/////////////////////////////////
    if (pTreadmillData->Flag & TreadmillDataElevationGain)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->PositiveElevationGain);
        index += sizeof(int16_t);
        co_write16p(&pDataPack->Para[index], pTreadmillData->NegativeElevationGain);
        index += sizeof(int16_t);
    }
    if (pTreadmillData->Flag & TreadmillDataInstantaneousPace)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->InstantaneousPace);
        index += 2;//  
    }
    if (pTreadmillData->Flag & TreadmillDataAveragePace)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->AveragePace);
        index += 2;//  
    }
    if (pTreadmillData->Flag & TreadmillDataExpendedEnergy)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->TotalEnergy);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pTreadmillData->EnergyPerHour);
        index += sizeof(uint16_t);
        co_write8(&pDataPack->Para[index], pTreadmillData->EnergyPerMinute);
        index += sizeof(uint8_t);
    }
    if (pTreadmillData->Flag & TreadmillDataHeartRate)
    {
        co_write8(&pDataPack->Para[index], pTreadmillData->HeartRate);
        index += sizeof(uint8_t);
    }
    if (pTreadmillData->Flag & TreadmillDataMetabolicEquivalent)
    {
        co_write8(&pDataPack->Para[index], pTreadmillData->MetabolicEquivalent);
        index += sizeof(uint8_t);
    }
    if (pTreadmillData->Flag & TreadmillDataElapsedTime)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->ElapsedTime);
        index += sizeof(uint16_t);
    }
    if (pTreadmillData->Flag & TreadmillDataRemainingTime)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->RemainingTime);
        index += sizeof(uint16_t);
    }
    if (pTreadmillData->Flag & TreadmillDataForceonBeltandPowerOutput)
    {
        co_write16p(&pDataPack->Para[index], pTreadmillData->ForceonBelt);
        index += sizeof(int16_t);
        co_write16p(&pDataPack->Para[index], pTreadmillData->PowerOutput);
        index += sizeof(int16_t);
    }
    pDataPack->Len = index;
		
		
}

void FTMS_CrossTrainerDataPackSerialization(DataPack_t *pDataPack, CrossTrainerData_t *pCrossTrainerData)
{
	if (pDataPack == NULL || pCrossTrainerData == NULL) return;
    uint8_t index = 0;
		if(FTMS_Uart_ReadType == OLD_ReadType )//
		{
			co_write16p(&pDataPack->Para[index], pCrossTrainerData->Flag);
			index += 3;//
		}
		
    if (pCrossTrainerData->Flag & CrossTrainerDataMoreData)
    {
        /* code */    
    }
    else
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->InstantaneousSpeed);
        index += sizeof(uint16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataAverageSpeed)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->AverageSpeed);
        index += sizeof(uint16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataTotalDistance)
    {
        co_write24p(&pDataPack->Para[index], pCrossTrainerData->TotalDistance);
        index += 3;
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataStepCount)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->StepPerMinute);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->AverageStepRate);
        index += sizeof(uint16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataStrideCount)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->StrideCount);
        index += sizeof(uint16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataElevationGain)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->PositiveElevationGain);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->NegativeElevationGain);
        index += sizeof(uint16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataInclinationandRampAngleSetting)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->Inclination);
        index += sizeof(int16_t);
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->RampAngleSetting);
        index += sizeof(int16_t);
    }		
    if (pCrossTrainerData->Flag & CrossTrainerDataResistanceLevel)
    {
        co_write8(&pDataPack->Para[index], pCrossTrainerData->ResistanceLevel);
        index += 1;//  
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataInstantaneousPower)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->InstantaneousPower);
        index += sizeof(int16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataAveragePower)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->AveragePower);
        index += sizeof(int16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataExpendedEnergy)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->TotalEnergy);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->EnergyPerHour);
        index += sizeof(uint16_t);
        co_write8(&pDataPack->Para[index], pCrossTrainerData->EnergyPerMinute);
        index += sizeof(uint8_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataHeartRate)
    {
        co_write8(&pDataPack->Para[index], pCrossTrainerData->HeartRate);
        index += sizeof(uint8_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataMetabolicEquivalent)
    {
        co_write8(&pDataPack->Para[index], pCrossTrainerData->MetabolicEquivalent);
        index += sizeof(uint8_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataElapsedTime)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->ElapsedTime);
        index += sizeof(uint16_t);
    }
    if (pCrossTrainerData->Flag & CrossTrainerDataRemainingTime)
    {
        co_write16p(&pDataPack->Para[index], pCrossTrainerData->RemainingTime);
        index += sizeof(uint16_t);
    }
    pDataPack->Len = index;
		
	
}
void FTMS_StepClimberDataPackSerialization(DataPack_t *pDataPack, StepClimberData_t *pStepClimberData)
{
	if (pDataPack == NULL || pStepClimberData == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pStepClimberData->Flag);
    index += sizeof(uint16_t);
    if (pStepClimberData->Flag & StepClimberDataMoreData)
    {
        /* code */    
    }
    else
    {
        co_write16p(&pDataPack->Para[index], pStepClimberData->Floors);
        index += sizeof(uint16_t);
				co_write16p(&pDataPack->Para[index], pStepClimberData->StepCount);
        index += sizeof(uint16_t);
    }
    if (pStepClimberData->Flag & StepClimberDataStepperMinute)
    {
        co_write16p(&pDataPack->Para[index], pStepClimberData->StepPerMinute);
        index += sizeof(uint16_t);
    }
    if (pStepClimberData->Flag & StepClimberDataAverageStepRate)
    {
        co_write16p(&pDataPack->Para[index], pStepClimberData->AverageStepRate);
        index += 2;
    }
    if (pStepClimberData->Flag & StepClimberDataPositiveElevationGain)
    {
        co_write16p(&pDataPack->Para[index], pStepClimberData->PositiveElevationGain);
        index += sizeof(uint16_t);    
    }
    if (pStepClimberData->Flag & StepClimberDataExpendedEnergy)
    {
        co_write16p(&pDataPack->Para[index], pStepClimberData->TotalEnergy);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pStepClimberData->EnergyPerHour);
        index += sizeof(uint16_t);
        co_write8(&pDataPack->Para[index], pStepClimberData->EnergyPerMinute);
        index += sizeof(uint8_t);
    }
    if (pStepClimberData->Flag & StepClimberDataHeartRate)
    {
        co_write8(&pDataPack->Para[index], pStepClimberData->HeartRate);
        index += sizeof(uint8_t);
    }
    if (pStepClimberData->Flag & StepClimberDataMetabolicEquivalent)
    {
        co_write8(&pDataPack->Para[index], pStepClimberData->MetabolicEquivalent);
        index += sizeof(uint8_t);
    }
    if (pStepClimberData->Flag & StepClimberDataElapsedTime)
    {
        co_write16p(&pDataPack->Para[index], pStepClimberData->ElapsedTime);
        index += sizeof(uint16_t);
    }
    if (pStepClimberData->Flag & StepClimberDataRemainingTime)
    {
        co_write16p(&pDataPack->Para[index], pStepClimberData->RemainingTime);
        index += sizeof(uint16_t);
    }
    pDataPack->Len = index;
		
}
void FTMS_StairClimberDataPackSerialization(DataPack_t *pDataPack, StairClimberData_t *pStairClimberData)
{
		if (pDataPack == NULL || pStairClimberData == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pStairClimberData->Flag);
    index += sizeof(uint16_t);
    if (pStairClimberData->Flag & StairClimberDataMoreData)
    {
        /* code */    
    }
    else
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->Floors);
        index += sizeof(uint16_t);				
    }
    if (pStairClimberData->Flag & StairClimberDataStepperMinute)
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->StepPerMinute);
        index += sizeof(uint16_t);
    }
    if (pStairClimberData->Flag & StairClimberDataAverageStepRate)
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->AverageStepRate);
        index += 2;
    }
		
		if (pStairClimberData->Flag & StairClimberDataPositiveElevationGain)
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->PositiveElevationGain);
        index += sizeof(uint16_t);    
    }
		if (pStairClimberData->Flag & StairClimberDataStrideCount)
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->AverageStepRate);
        index += 2;
    }
    
    if (pStairClimberData->Flag & StairClimberDataExpendedEnergy)
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->TotalEnergy);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pStairClimberData->EnergyPerHour);
        index += sizeof(uint16_t);
        co_write8(&pDataPack->Para[index], pStairClimberData->EnergyPerMinute);
        index += sizeof(uint8_t);
    }
    if (pStairClimberData->Flag & StairClimberDataHeartRate)
    {
        co_write8(&pDataPack->Para[index], pStairClimberData->HeartRate);
        index += sizeof(uint8_t);
    }
    if (pStairClimberData->Flag & StairClimberDataMetabolicEquivalent)
    {
        co_write8(&pDataPack->Para[index], pStairClimberData->MetabolicEquivalent);
        index += sizeof(uint8_t);
    }
    if (pStairClimberData->Flag & StairClimberDataElapsedTime)
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->ElapsedTime);
        index += sizeof(uint16_t);
    }
    if (pStairClimberData->Flag & StairClimberDataRemainingTime)
    {
        co_write16p(&pDataPack->Para[index], pStairClimberData->RemainingTime);
        index += sizeof(uint16_t);
    }
    pDataPack->Len = index;
		
}
void FTMS_RowerDataPackSerialization(DataPack_t *pDataPack, RowerData_t *pRowerData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pRowerData->Flag);
    index += sizeof(uint16_t);
    if (pRowerData->Flag & RowerDataMoreData)
    {
        /* code */
    }
    else
    {
        co_write8(&pDataPack->Para[index], pRowerData->StrokeRate);
        index += sizeof(uint8_t);
        co_write16p(&pDataPack->Para[index], pRowerData->StrokeCount);
        index += sizeof(uint16_t);
    }
    if (pRowerData->Flag & RowerDataAverageStroke)
    {
        co_write8(&pDataPack->Para[index], pRowerData->AverageStrokeRate);
        index += sizeof(uint8_t);
    }
    if (pRowerData->Flag & RowerDataTotalDistance)
    {
        co_write24p(&pDataPack->Para[index], pRowerData->TotalDistance);
        index += 3;
    }
    if (pRowerData->Flag & RowerDataInstantaneousPace)
    {
        co_write16p(&pDataPack->Para[index], pRowerData->InstantaneousPace);
        index += sizeof(uint16_t);
    }
    if (pRowerData->Flag & RowerDataAveragePace)
    {
        co_write16p(&pDataPack->Para[index], pRowerData->AveragePace);
        index += sizeof(uint16_t);
    }
    if (pRowerData->Flag & RowerDataInstantaneousPower)
    {
        co_write16p(&pDataPack->Para[index], pRowerData->InstantaneousPower);
        index += sizeof(int16_t);
    }
    if (pRowerData->Flag & RowerDataAveragePower)
    {
        co_write16p(&pDataPack->Para[index], pRowerData->AveragePace);
        index += sizeof(int16_t);
    }
    if (pRowerData->Flag & RowerDataResistanceLevel)
    {
        co_write8(&pDataPack->Para[index], pRowerData->ResistanceLevel);
        index += sizeof(int8_t);
			
    }
    if (pRowerData->Flag & RowerDataExpendedEnergy)
    {
        co_write16p(&pDataPack->Para[index], pRowerData->TotalEnergy);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pRowerData->EnergyPerHour);
        index += sizeof(uint16_t);
        co_write8(&pDataPack->Para[index], pRowerData->EnergyPerMinute);
        index += sizeof(uint8_t);
    }
    if (pRowerData->Flag & RowerDataHeartRate)
    {
        co_write8(&pDataPack->Para[index], pRowerData->HeartRate);
        index += sizeof(uint8_t);
    }
    if (pRowerData->Flag & RowerDataMetabolicEquivalent)
    {
        co_write8(&pDataPack->Para[index], pRowerData->MetabolicEquivalent);
        index += sizeof(uint8_t);
    }
    if (pRowerData->Flag & RowerDataElapsedTime)
    {
        co_write16p(&pDataPack->Para[index], pRowerData->ElapsedTime);
        index += sizeof(uint16_t);
    }
    if (pRowerData->Flag & RowerDataRemainingTime)
    {
        co_write16p(&pDataPack->Para[index], pRowerData->RemainingTime);
        index += sizeof(uint16_t);
    }
    pDataPack->Len = index;
		
}
void FTMS_IndoorBikeDataPackSerialization(DataPack_t *pDataPack, IndoorBikeData_t *pIndoorBikeData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pIndoorBikeData->Flag);
    index += sizeof(uint16_t);
    if (pIndoorBikeData->Flag & RowerDataMoreData)
    {
        /* code */
    }
    else
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->InstantaneousSpeed);
        index += sizeof(uint16_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataAverageSpeed)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->AverageSpeed);
        index += sizeof(uint16_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataInstantaneousCadence)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->InstantaneousCadence);
        index += sizeof(uint16_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataAverageCadence)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->AverageCadence);
        index += sizeof(uint16_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataTotalDistance)
    {
        co_write24p(&pDataPack->Para[index], pIndoorBikeData->TotalDistance);
        index += 3;
    }
		if(FTMS_Uart_ReadType == OLD_ReadType ){
			if (pIndoorBikeData->Flag & IndoorBikeDataResistanceLevel)
			{
					co_write16p(&pDataPack->Para[index], pIndoorBikeData->ResistanceLevel);
					index += sizeof(int16_t);
			}
		}
		
    if (pIndoorBikeData->Flag & IndoorBikeDataInstantaneousPower)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->InstantaneousPower);
        index += sizeof(int16_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataAveragePower)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->AveragePower);
        index += sizeof(int16_t);
    }
    if (pIndoorBikeData->Flag & RowerDataExpendedEnergy)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->TotalEnergy);
        index += sizeof(uint16_t);
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->EnergyPerHour);
        index += sizeof(uint16_t);
        co_write8(&pDataPack->Para[index], pIndoorBikeData->EnergyPerMinute);
        index += sizeof(uint8_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataHeartRate)
    {
        co_write8(&pDataPack->Para[index], pIndoorBikeData->HeartRate);
        index += sizeof(uint8_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataMetabolicEquivalent)
    {
        co_write8(&pDataPack->Para[index], pIndoorBikeData->MetabolicEquivalent);
        index += sizeof(uint8_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataElapsedTime)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->ElapsedTime);
        index += sizeof(uint16_t);
    }
    if (pIndoorBikeData->Flag & IndoorBikeDataRemainingTime)
    {
        co_write16p(&pDataPack->Para[index], pIndoorBikeData->RemainingTime);
        index += sizeof(uint16_t);
    }
    pDataPack->Len = index;
		
}

void FTMS_TrainingStatusDataPackSerialization(DataPack_t *pDataPack, TrainingStatusData_t *pTrainingStatusData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write8(&pDataPack->Para[index], pTrainingStatusData->Flag);
    index += sizeof(uint8_t);
    if (pTrainingStatusData->Flag & TrainingStatus)
    {
        co_write8(&pDataPack->Para[index], pTrainingStatusData->Status);
				index += sizeof(uint8_t);
    }
    if (pTrainingStatusData->Flag & TrainingStatusString)
    {
        memcpy(&pDataPack->Para[index], pTrainingStatusData->StatusString, strlen((char*)pTrainingStatusData->StatusString));
				index += strlen((char*)pTrainingStatusData->StatusString);
    }
    pDataPack->Len = index;
		
		
}

void FTMS_SupportedSpeedRangeDataPackSerialization(DataPack_t *pDataPack, SupportedSpeedRange_t *pData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pData->Minimum);
    index += 2;
    co_write16p(&pDataPack->Para[index], pData->Maximum);
    index += 2;
    co_write16p(&pDataPack->Para[index], pData->Increment);
    index += 2;
    pDataPack->Len = index;
}

void FTMS_SupportedInclinationRangeDataPackSerialization(DataPack_t *pDataPack, SupportedInclinationRange_t *pData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pData->Minimum);
    index += 2;
    co_write16p(&pDataPack->Para[index], pData->Maximum);
    index += 2;
    co_write16p(&pDataPack->Para[index], pData->Increment);
    index += 2;
    pDataPack->Len = index;
}

void FTMS_SupportedResistanceRangeDataPackSerialization(DataPack_t *pDataPack, SupportedResistanceRange_t *pData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pData->Minimum);
    index += sizeof(uint16_t);
    co_write16p(&pDataPack->Para[index], pData->Maximum);
    index += sizeof(uint16_t);
    co_write16p(&pDataPack->Para[index], pData->Increment);
    index += sizeof(uint16_t);
    pDataPack->Len = index;
}

void FTMS_SupportedPowerRangeDataPackSerialization(DataPack_t *pDataPack, SupportedPowerRange_t *pData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write16p(&pDataPack->Para[index], pData->Minimum);
    index += 2;
    co_write16p(&pDataPack->Para[index], pData->Maximum);
    index += 2;
    co_write16p(&pDataPack->Para[index], pData->Increment);
    index += 2;
    pDataPack->Len = index;
}
void FTMS_SupportedHeartRateRangeDataPackSerialization(DataPack_t *pDataPack, SupportedHeartRateRange_t *pData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write8(&pDataPack->Para[index++], pData->Minimum);
		index += sizeof(uint8_t);
    co_write8(&pDataPack->Para[index++], pData->Maximum);
		index += sizeof(uint8_t);
    co_write8(&pDataPack->Para[index++], pData->Increment);
		index += sizeof(uint8_t);
    pDataPack->Len = index;
}
void FTMS_ControlPointResponseDataPackSerialization(DataPack_t *pDataPack, FitnessMachineControlPointResponse_t *pResponseData)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    pDataPack->Para[index++] = pResponseData->ResponseCode;
    pDataPack->Para[index++] = pResponseData->RequestCode;
    pDataPack->Para[index++] = pResponseData->ResultCode;
    pDataPack->Len = index;
}

void FTMS_FitnessMachineStatusDataPackSerialization(DataPack_t *pDataPack, FitnessMachineStatus_t *pFitnessMachineStatus)
{
	if (pDataPack == NULL) return;
    uint8_t index = 0;
    co_write8(&pDataPack->Para[index], pFitnessMachineStatus->OpCode);
    index += sizeof(uint8_t);
    switch (pFitnessMachineStatus->OpCode)
    {
    case FitnessMachineStatusReset:
        
				if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				
        break;
    case FitnessMachineStoppedorPausedbytheUser:
				if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					co_write8(&pDataPack->Para[index], *(pFitnessMachineStatus->Para));
				}
				
        break;
    case FitnessMachineStoppedbySafetyKey:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				
        break;
    case FitnessMachineStartedorResumedbytheUser:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				
        break;
    case TargetSpeedChanged:
        memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(uint16_t));
        index += sizeof(uint16_t);
        break;
    case TargetInclineChanged: 
        memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
        index += sizeof(int16_t);
        break;
    case TargetResistanceLevelChanged://
        
				if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(uint8_t));
        index += sizeof(uint8_t);
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
    case TargetPowerChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case TargetHeartRateChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int8_t));
				// 	index += sizeof(int8_t);
				// }
        break;
		case TargetedExpendedEnergyChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case TargetedNumberofStepsChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case TargetedNumberofStridesChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case TargetedDistanceChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case TargetedTrainingTimeChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case TargetedTimeinTwoHeartRateZonesChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case TargetedTimeinThreeHeartRateZonesChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], 6);
				// 	index += 6;
				// }
        break;
		case TargetedTimeinFiveHeartRateZonesChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], 10);
				// 	index += 10;
				// }
        break;
		case IndoorBikeSimulationParametersChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], 6);
				// 	index += 6;
				// }
        break;
		case WheelCircumferenceChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
		case SpinDownStatus://TBD
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int8_t));
				// 	index += sizeof(int8_t);
				// }
        break;
		case TargetedCadenceChanged:
        if(FTMS_Uart_ReadType == OLD_ReadType )//
        {
					/* code */
				}
				// else if(FTMS_Uart_ReadType == NEWforFTMS_Base_ReadType)// 
				// {
				// 	memcpy(&pDataPack->Para[index], &pFitnessMachineStatus->Para[0], sizeof(int16_t));
				// 	index += sizeof(int16_t);
				// }
        break;
											
    default:
        break;
    }
    pDataPack->Len = index;
}





