/**
 ****************************************************************************************
 *
 * @file FTMS.h
 *
 * @brief Header file - FTM Service Server Role
 *
 *
 ****************************************************************************************
 */
#ifndef _FTMS_H_
#define _FTMS_H_

/**
 ****************************************************************************************
 * @addtogroup  FTMS 'Profile' Server
 * @ingroup FTM
 * @brief FTM 'Profile' Server
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"
#include "rwprf_config.h"

#if (BLE_FTMS_SERVER)

#include "ftms_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"
#include "DataFormat.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define FTMS_Feature_0x2ACC_LEN (8)
#define FTMS_Treadmill_Data_0x2ACD_LEN (20)
#define FTMS_Cross_Trainer_Data_0x2ACE_LEN (20)
#define FTMS_Step_Climber_Data_0x2ACF_LEN (20)
#define FTMS_Stair_Climber_Data_0x2AD0_LEN (20)
#define FTMS_Rower_Data_0x2AD1_LEN (20)
#define FTMS_Indoor_Bike_Data_0x2AD2_LEN (20)
#define FTMS_Training_Status_0x2AD3_LEN (5)
#define FTMS_Speed_Range_0x2AD4_LEN (6)
#define FTMS_Inclination_Range_0x2AD5_LEN (6)
#define FTMS_Resistance_Level_Range_0x2AD6_LEN (6)
#define FTMS_Heart_Rate_Range_0x2AD7_LEN (3)
#define FTMS_Power_Range_0x2AD8_LEN (6)
#define FTMS_Control_Point_0x2AD9_LEN (20)
#define FTMS_Status_0x2ADA_LEN (5)

#define FTMS_NTF_MAX_BUFF_LEN (20)
#define FTMS_MAX_BUFF_LEN (20)

#define FTMS_FEATURE_0x2ACC_MANDATORY_SUPPORT (0x1024) // Elapsed Time Supported === Pace Supported === Total Distance Supported

#define FTMS_IS_FEATURE_SUPPORTED(features, flag) ((features & flag) == flag)

#define FTMS_HANDLE(idx)                                                                            \
    (pftms_env->start_hdl + (idx)) -                                                                \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Speed_Range_0x2AD4_CHAR) &&                   \
          ((idx) > FTMS_IDX_Speed_Range_0x2AD4_CHAR))                                               \
             ? (2)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Inclination_Range_0x2AD5_CHAR) &&             \
          ((idx) > FTMS_IDX_Inclination_Range_0x2AD5_CHAR))                                         \
             ? (2)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Resistance_Level_Range_0x2AD6_CHAR) &&        \
          ((idx) > FTMS_IDX_Resistance_Level_Range_0x2AD6_CHAR))                                    \
             ? (2)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Heart_Rate_Range_0x2AD7_CHAR) &&              \
          ((idx) > FTMS_IDX_Heart_Rate_Range_0x2AD7_CHAR))                                          \
             ? (2)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Power_Range_0x2AD8_CHAR) &&                   \
          ((idx) > FTMS_IDX_Power_Range_0x2AD8_CHAR))                                               \
             ? (2)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Fitness_Machine_Control_Point_0x2AD9_CHAR) && \
          ((idx) > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR))                             \
             ? (2)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Treadmill_Data_0x2ACD_CHAR) &&                \
          ((idx) > FTMS_IDX_Treadmill_Data_0x2ACD_CHAR))                                            \
             ? (3)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Cross_Trainer_Data_0x2ACE_CHAR) &&            \
          ((idx) > FTMS_IDX_Cross_Trainer_Data_0x2ACE_CHAR))                                        \
             ? (3)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Step_Climber_Data_0x2ACF_CHAR) &&             \
          ((idx) > FTMS_IDX_Step_Climber_Data_0x2ACF_CHAR))                                         \
             ? (3)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Stair_Climber_Data_0x2AD0_CHAR) &&            \
          ((idx) > FTMS_IDX_Stair_Climber_Data_0x2AD0_CHAR))                                        \
             ? (3)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Rower_Data_0x2AD1_CHAR) &&                    \
          ((idx) > FTMS_IDX_Rower_Data_0x2AD1_CHAR))                                                \
             ? (3)                                                                                  \
             : (0)) -                                                                               \
        ((!(pftms_env->EnableCharList & FTMS_CharFlag_Indoor_Bike_Data_0x2AD2_CHAR) &&              \
          ((idx) > FTMS_IDX_Indoor_Bike_Data_0x2AD2_CHAR))                                          \
             ? (3)                                                                                  \
             : (0))

// Get database attribute index
#define FTMS_IDX(hdl)                                                                            \
    ((hdl - pftms_env->start_hdl) +                                                              \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Speed_Range_0x2AD4_CHAR) &&                   \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Speed_Range_0x2AD4_CHAR))                        \
          ? (2)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Inclination_Range_0x2AD5_CHAR) &&             \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Inclination_Range_0x2AD5_CHAR))                  \
          ? (2)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Resistance_Level_Range_0x2AD6_CHAR) &&        \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Resistance_Level_Range_0x2AD6_CHAR))             \
          ? (2)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Heart_Rate_Range_0x2AD7_CHAR) &&              \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Heart_Rate_Range_0x2AD7_CHAR))                   \
          ? (2)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Power_Range_0x2AD8_CHAR) &&                   \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Power_Range_0x2AD8_CHAR))                        \
          ? (2)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Fitness_Machine_Control_Point_0x2AD9_CHAR) && \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR))      \
          ? (2)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Treadmill_Data_0x2ACD_CHAR) &&                \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Treadmill_Data_0x2ACD_CHAR))                     \
          ? (3)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Cross_Trainer_Data_0x2ACE_CHAR) &&            \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Cross_Trainer_Data_0x2ACE_CHAR))                 \
          ? (3)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Step_Climber_Data_0x2ACF_CHAR) &&             \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Step_Climber_Data_0x2ACF_CHAR))                  \
          ? (3)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Stair_Climber_Data_0x2AD0_CHAR) &&            \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Stair_Climber_Data_0x2AD0_CHAR))                 \
          ? (3)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Rower_Data_0x2AD1_CHAR) &&                    \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Rower_Data_0x2AD1_CHAR))                         \
          ? (3)                                                                                  \
          : (0)) +                                                                               \
     ((!(pftms_env->EnableCharList & FTMS_CharFlag_Indoor_Bike_Data_0x2AD2_CHAR) &&              \
       ((hdl - pftms_env->start_hdl) > FTMS_IDX_Indoor_Bike_Data_0x2AD2_CHAR))                   \
          ? (3)                                                                                  \
          : (0)))

enum FTMS_state
{
    // Idle state
    FTMS_IDLE,

    // Busy state
    FTMS_BUSY,

    // Number of defined states.
    FTMS_STATE_MAX
};

enum
{
    ATT_USER_SERVER_FTMS = ATT_UUID_16(0x1826),

    ATT_USER_SERVER_CHAR_FTMS_Feature = ATT_UUID_16(0x2ACC),

    ATT_USER_SERVER_CHAR_FTMS_Treadmill_Data = ATT_UUID_16(0x2ACD),

    ATT_USER_SERVER_CHAR_FTMS_Cross_Trainer_Data = ATT_UUID_16(0x2ACE),

    ATT_USER_SERVER_CHAR_FTMS_Step_Climber_Data = ATT_UUID_16(0x2ACF),

    ATT_USER_SERVER_CHAR_FTMS_Stair_Climber_Data = ATT_UUID_16(0x2AD0),

    ATT_USER_SERVER_CHAR_FTMS_Rower_Data = ATT_UUID_16(0x2AD1),

    ATT_USER_SERVER_CHAR_FTMS_Indoor_Bike_Data = ATT_UUID_16(0x2AD2),

    ATT_USER_SERVER_CHAR_FTMS_Training_Status = ATT_UUID_16(0x2AD3),

    ATT_USER_SERVER_CHAR_FTMS_Speed_Range = ATT_UUID_16(0x2AD4),

    ATT_USER_SERVER_CHAR_FTMS_Inclination_Range = ATT_UUID_16(0x2AD5),

    ATT_USER_SERVER_CHAR_FTMS_Resistance_Level_Range = ATT_UUID_16(0x2AD6),

    ATT_USER_SERVER_CHAR_FTMS_Heart_Rate_Range = ATT_UUID_16(0x2AD7),

    ATT_USER_SERVER_CHAR_FTMS_Power_Range = ATT_UUID_16(0x2AD8),

    ATT_USER_SERVER_CHAR_FTMS_Control_Point = ATT_UUID_16(0x2AD9),

    ATT_USER_SERVER_CHAR_FTMS_Status = ATT_UUID_16(0x2ADA),
};

enum FTMS_ATT_List
{
    FTMS_IDX_SVC,

    FTMS_IDX_Feature_0x2ACC_CHAR,
    FTMS_IDX_Feature_0x2ACC_VAL,

    FTMS_IDX_Speed_Range_0x2AD4_CHAR,
    FTMS_IDX_Speed_Range_0x2AD4_VAL,

    FTMS_IDX_Inclination_Range_0x2AD5_CHAR,
    FTMS_IDX_Inclination_Range_0x2AD5_VAL,

    FTMS_IDX_Resistance_Level_Range_0x2AD6_CHAR,
    FTMS_IDX_Resistance_Level_Range_0x2AD6_VAL,

    FTMS_IDX_Heart_Rate_Range_0x2AD7_CHAR,
    FTMS_IDX_Heart_Rate_Range_0x2AD7_VAL,

    FTMS_IDX_Power_Range_0x2AD8_CHAR,
    FTMS_IDX_Power_Range_0x2AD8_VAL,

    FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR,
    FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_VAL,
    FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_IND_CFG,

    FTMS_IDX_Treadmill_Data_0x2ACD_CHAR,
    FTMS_IDX_Treadmill_Data_0x2ACD_VAL,
    FTMS_IDX_Treadmill_Data_0x2ACD_NTF_CFG,

    FTMS_IDX_Cross_Trainer_Data_0x2ACE_CHAR,
    FTMS_IDX_Cross_Trainer_Data_0x2ACE_VAL,
    FTMS_IDX_Cross_Trainer_Data_0x2ACE_NTF_CFG,

    FTMS_IDX_Step_Climber_Data_0x2ACF_CHAR,
    FTMS_IDX_Step_Climber_Data_0x2ACF_VAL,
    FTMS_IDX_Step_Climber_Data_0x2ACF_NTF_CFG,

    FTMS_IDX_Stair_Climber_Data_0x2AD0_CHAR,
    FTMS_IDX_Stair_Climber_Data_0x2AD0_VAL,
    FTMS_IDX_Stair_Climber_Data_0x2AD0_NTF_CFG,

    FTMS_IDX_Rower_Data_0x2AD1_CHAR,
    FTMS_IDX_Rower_Data_0x2AD1_VAL,
    FTMS_IDX_Rower_Data_0x2AD1_NTF_CFG,

    FTMS_IDX_Indoor_Bike_Data_0x2AD2_CHAR,
    FTMS_IDX_Indoor_Bike_Data_0x2AD2_VAL,
    FTMS_IDX_Indoor_Bike_Data_0x2AD2_NTF_CFG,

    FTMS_IDX_Training_Status_0x2AD3_CHAR,
    FTMS_IDX_Training_Status_0x2AD3_VAL,
    FTMS_IDX_Training_Status_0x2AD3_NTF_CFG,

    FTMS_IDX_Fitness_Machine_Status_0x2ADA_CHAR,
    FTMS_IDX_Fitness_Machine_Status_0x2ADA_VAL,
    FTMS_IDX_Fitness_Machine_Status_0x2ADA_NTF_CFG,

    FTMS_IDX_NB,
};

typedef enum FTMS_EnableCharList
{
    FTMS_CharFlag_Feature_0x2ACC_CHAR = (1 << 0),
    FTMS_CharFlag_Speed_Range_0x2AD4_CHAR = (1 << 1),
    FTMS_CharFlag_Inclination_Range_0x2AD5_CHAR = (1 << 2),
    FTMS_CharFlag_Resistance_Level_Range_0x2AD6_CHAR = (1 << 3),
    FTMS_CharFlag_Heart_Rate_Range_0x2AD7_CHAR = (1 << 4),
    FTMS_CharFlag_Power_Range_0x2AD8_CHAR = (1 << 5),
    FTMS_CharFlag_Fitness_Machine_Control_Point_0x2AD9_CHAR = (1 << 6),
    FTMS_CharFlag_Treadmill_Data_0x2ACD_CHAR = (1 << 7),
    FTMS_CharFlag_Cross_Trainer_Data_0x2ACE_CHAR = (1 << 8),
    FTMS_CharFlag_Step_Climber_Data_0x2ACF_CHAR = (1 << 9),
    FTMS_CharFlag_Stair_Climber_Data_0x2AD0_CHAR = (1 << 10),
    FTMS_CharFlag_Rower_Data_0x2AD1_CHAR = (1 << 11),
    FTMS_CharFlag_Indoor_Bike_Data_0x2AD2_CHAR = (1 << 12),
    FTMS_CharFlag_Training_Status_0x2AD3_CHAR = (1 << 13),
    FTMS_CharFlag_Fitness_Machine_Status_0x2ADA_CHAR = (1 << 14),
} FTMS_EnableCharList_e;

typedef enum FTMS_NotifyIndicateList
{
    FTMS_NotifyFlag_Treadmill_Data_0x2ACD = (1 << 0),
    FTMS_NotifyFlag_Cross_Trainer_Data_0x2ACE = (1 << 1),
    FTMS_NotifyFlag_Step_Climber_Data_0x2ACF = (1 << 2),
    FTMS_NotifyFlag_Stair_Climber_Data_0x2AD0 = (1 << 3),
    FTMS_NotifyFlag_Rower_Data_0x2AD1 = (1 << 4),
    FTMS_NotifyFlag_Indoor_Bike_Data_0x2AD2 = (1 << 5),
    FTMS_NotifyFlag_Training_Status_0x2AD3 = (1 << 6),
    FTMS_NotifyFlag_Fitness_Machine_Status_0x2ADA = (1 << 7),
    FTMS_IndicateFlag_Fitness_Machine_Control_Point_0x2AD9 = (1 << 8),
} FTMS_NotifyIndicateList_e;
/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
// FTMS 'Profile' Server environment variable
typedef struct ftms_env_tag
{
    // profile environment
    prf_env_t prf_env;
    // Service Attribute Start Handle
    /// On-going operation
    struct ke_msg * operation;
    /// FFF0 Services Start Handle
    struct co_list values;

    uint16_t start_hdl;
    // FTMS machine type
    FTMS_TYPE_e DeviceType;
    // Fitness Machine Features
    FitnessMachineFeature_t Feature;
    //Fitness Machine Feature State
    TrainingStatusList_e DeviceState;
    // FTMS Parameter Range
    SupportedInclinationRange_t *pInclinationRange;
    SupportedResistanceRange_t *pResistanceRange;
    SupportedHeartRateRange_t *pHeartRateRange; 
    SupportedPowerRange_t *pPowerRange;
    SupportedSpeedRange_t *pSpeedRange;
    TrainingStatusData_t xTrainingStatus;

    /**/
    uint32_t NotifyIndicateConfigList;
    // uint32_t NotifyStatusList;
    uint32_t EnableCharList;
    ke_state_t state[FTMS_IDX_MAX];
    uint8_t conidx;
} ftms_env_tag_t;

/**
 ****************************************************************************************
 * @brief Retrieve FTM service profile interface
 *
 * @return FTM service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs *ftms_prf_itf_get(void);

uint8_t ucGetFTMS_Index(uint16_t handle);
uint16_t ucGetFTMS_Handle(uint8_t Index);

void ftms_SendDataByIndicate(ftms_env_tag_t *pftms_env, uint8_t AttIndex, const DataPack_t *pDataPack);
void ftms_SendDataByNotify(uint8_t conidx,ftms_env_tag_t *pftms_env, uint8_t AttIndex, const DataPack_t *pDataPack);

void ftms_task_init(struct ke_task_desc *task_desc);
#endif /* #if (BLE_ftm_SERVER) */

#endif /*  _FTMS_H_ */
