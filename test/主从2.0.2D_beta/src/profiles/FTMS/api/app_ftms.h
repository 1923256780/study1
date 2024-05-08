

#ifndef __APP_FTMS_H_
#define __APP_FTMS_H_

#include "rwip_config.h" // SW configuration 

#include "stdint.h"  // Standard Integer Definition
#include "ke_task.h" // Kernel Task Definition

#include "Device.h"
// #if (BLE_FTMS_SERVER)
#if (1)
#define UpdatePeriod 100
struct app_ftms_env_tag
{
    // Connection handle
    uint8_t conidx;

    uint8_t send_status;
    FTMS_TYPE_e Type;
};

typedef struct app_FTMS_Handler
{
    void (*StartOrResume)(void);
    void (*Stop)(void);
    void (*Pause)(void);
    void (*SetTargetResistanceLevel)(uint8_t Level);
    void (*SetTargetPower)(int16_t Watt);
    void (*SetTargetInclination)(uint8_t Percent);
    void (*SetTargetSpeed)(uint16_t Value);
} app_FTMS_Handler_t;
 

extern struct app_ftms_env_tag app_ftms_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_ftms_handler;

void app_ftms_init(void);

void app_ftm_add_ftms(void);

void app_ftms_enable_prf(uint8_t conidx);

void app_FitShow_To_FTMS_Treadmill_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag);
void app_FitShow_To_FTMS_Indoor_Bike_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag);
void app_FitShow_To_FTMS_Rower_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag);
void app_FitShow_To_FTMS_Cross_Trainer_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag);
void app_FitShow_To_FTMS_Step_Climber_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag);
void app_FitShow_To_FTMS_Stair_Climber_Data_Notify_handler(uint8_t conidx,FitShowAllData_t *pFitShowAllData, uint16_t FTMS_Flag);

void app_FitShow_To_FTMS_Training_Status_Notify_handler(uint8_t conidx,uint8_t FTMS_StatusFlag, uint8_t FTMS_TrainingStatus, const char *str);
int app_FTMS_ControlPointResponse_Indicate_handler(uint8_t conidx,FitnessMachineControlPointList_e RequstCode, FitnessMachineControlPointResultList_e ResultCode);
void app_FitShow_To_FTMS_MachineStatus_Notify_handler(uint8_t conidx,FitnessMachineStatusList_e OpCode, uint8_t *pData, uint8_t len);

//================================================================================================================================================//HB-注释


app_FTMS_Handler_t *pGetApp_FTMS_Handler(void);
#endif
#endif
