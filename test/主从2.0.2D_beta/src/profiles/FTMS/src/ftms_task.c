/**
 ****************************************************************************************
 *
 * @file   fff0s_task.c
 *
 * @brief FFF0 Server Role Task Implementation.
 *
 * Copyright (C) Beken 2009-2015
 *
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

// #if (BLE_FTMS_SERVER)
#if (1)
#include "gap.h"
#include "gattc_task.h"
#include "attm.h"
#include "atts.h"
#include "co_utils.h"
#include "uart.h"
#include "prf_utils.h"
#include "ftms.h"
#include "ftms_task.h"

#include "DataFormat.h"

static int ftms_TreadmillData_0x2ACD_Notify_req_handler(ke_msg_id_t const msgid,
                                                        struct DataPack const *param,
                                                        ke_task_id_t const dest_id,
                                                        ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Treadmill_Data_0x2ACD_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}
static int ftms_CrossTrainerData_0x2ACE_Notify_req_handler(ke_msg_id_t const msgid,
                                                           struct DataPack const *param,
                                                           ke_task_id_t const dest_id,
                                                           ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);

    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Cross_Trainer_Data_0x2ACE_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}
static int ftms_StepClimberData_0x2ACF_Notify_req_handler(ke_msg_id_t const msgid,
                                                          struct DataPack const *param,
                                                          ke_task_id_t const dest_id,
                                                          ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Step_Climber_Data_0x2ACF_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}
static int ftms_StairClimberData_0x2AD0_Notify_req_handler(ke_msg_id_t const msgid,
                                                           struct DataPack const *param,
                                                           ke_task_id_t const dest_id,
                                                           ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Stair_Climber_Data_0x2AD0_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}
static int ftms_RowerData_0x2AD1_Notify_req_handler(ke_msg_id_t const msgid,
                                                    struct DataPack const *param,
                                                    ke_task_id_t const dest_id,
                                                    ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Rower_Data_0x2AD1_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}
static int ftms_IndoorBikeData_0x2AD2_Notify_req_handler(ke_msg_id_t const msgid,
                                                         struct DataPack const *param,
                                                         ke_task_id_t const dest_id,
                                                         ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Indoor_Bike_Data_0x2AD2_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}
static int ftms_TrainingStatus_0x2AD3_Notify_req_handler(ke_msg_id_t const msgid,
                                                         struct DataPack const *param,
                                                         ke_task_id_t const dest_id,
                                                         ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Training_Status_0x2AD3_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}
static int ftms_MachineStatus_0x2ADA_Notify_req_handler(ke_msg_id_t const msgid,
                                                        struct DataPack const *param,
                                                        ke_task_id_t const dest_id,
                                                        ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(src_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByNotify(conidx,pftms_env, FTMS_IDX_Fitness_Machine_Status_0x2ADA_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}

static int ftms_ControlPointResponse_0x2AD9_Indicate_req_handler(ke_msg_id_t const msgid,
                                                                 struct DataPack const *param,
                                                                 ke_task_id_t const dest_id,
                                                                 ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);
    // check state of the task
    if (state == FTMS_IDLE)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);

        // put task in a busy state
        ke_state_set(dest_id, FTMS_BUSY);
        ftms_SendDataByIndicate(pftms_env, FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_VAL, param);
        ke_state_set(dest_id, FTMS_IDLE);
        msg_status = KE_MSG_CONSUMED;
    }
    return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the attribute info request message.
 *
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_att_info_req_ind_handler(ke_msg_id_t const msgid,
                                          struct gattc_att_info_req_ind *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    if (ke_state_get(dest_id) == FTMS_IDLE)
    {
        // Get the address of the environment
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
        uint16_t att_idx = ucGetFTMS_Index(param->handle);
        struct gattc_att_info_cfm *cfm;

        //Send write response
        cfm = KE_MSG_ALLOC(GATTC_ATT_INFO_CFM, src_id, dest_id, gattc_att_info_cfm);
        cfm->handle = param->handle;

        switch (att_idx)
        {
        case FTMS_IDX_Treadmill_Data_0x2ACD_NTF_CFG:
        case FTMS_IDX_Cross_Trainer_Data_0x2ACE_NTF_CFG:
        case FTMS_IDX_Step_Climber_Data_0x2ACF_NTF_CFG:
        case FTMS_IDX_Stair_Climber_Data_0x2AD0_NTF_CFG:
        case FTMS_IDX_Rower_Data_0x2AD1_NTF_CFG:
        case FTMS_IDX_Indoor_Bike_Data_0x2AD2_NTF_CFG:
        case FTMS_IDX_Training_Status_0x2AD3_NTF_CFG:
        case FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_IND_CFG:
        case FTMS_IDX_Fitness_Machine_Status_0x2ADA_NTF_CFG:
            // CCC attribute length = 2
            cfm->length = 2;
            cfm->status = GAP_ERR_NO_ERROR;
            break;
        case FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_VAL:
            // force length to zero to reject any write starting from something != 0
            cfm->length = 0;
            cfm->status = GAP_ERR_NO_ERROR;
            break;
        default:
            cfm->length = 0;
            cfm->status = ATT_ERR_WRITE_NOT_PERMITTED;
            break;
        }
        ke_msg_send(cfm);
    }
    return (KE_MSG_CONSUMED);
}

static int gattc_write_req_ind_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const *param,
                                       ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_write_cfm *cfm;
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
    uint8_t att_idx = ucGetFTMS_Index(param->handle);
    // uint8_t Character;
    uint8_t conidx = KE_IDX_GET(src_id);
    // retrieve handle information
    uint8_t status = GAP_ERR_NO_ERROR;

    uint8_t FlagUpDataNotifyIndicate = false;

    // If the attribute has been found, status is GAP_ERR_NO_ERROR
    if (status == GAP_ERR_NO_ERROR)
    {
        ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
        // Extract value before check
        // Only update configuration if value for stop or notification enable
        switch (att_idx)
        {
        case FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_IND_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_IND)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_IndicateFlag_Fitness_Machine_Control_Point_0x2AD9;
            }
            else if (ntf_cfg == PRF_CLI_STOP_NTFIND)
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_IndicateFlag_Fitness_Machine_Control_Point_0x2AD9);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Treadmill_Data_0x2ACD_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Treadmill_Data_0x2ACD;
            }
            else if (ntf_cfg == PRF_CLI_STOP_NTFIND)
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Treadmill_Data_0x2ACD);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Cross_Trainer_Data_0x2ACE_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Cross_Trainer_Data_0x2ACE;
            }
            else
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Cross_Trainer_Data_0x2ACE);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Step_Climber_Data_0x2ACF_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Step_Climber_Data_0x2ACF;
            }
            else
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Step_Climber_Data_0x2ACF);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Stair_Climber_Data_0x2AD0_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Stair_Climber_Data_0x2AD0;
            }
            else
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Stair_Climber_Data_0x2AD0);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Rower_Data_0x2AD1_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Rower_Data_0x2AD1;
            }
            else
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Rower_Data_0x2AD1);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Indoor_Bike_Data_0x2AD2_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Indoor_Bike_Data_0x2AD2;
            }
            else
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Indoor_Bike_Data_0x2AD2);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Training_Status_0x2AD3_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Cross_Trainer_Data_0x2ACE;
            }
            else
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Cross_Trainer_Data_0x2ACE);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Fitness_Machine_Status_0x2ADA_NTF_CFG:
        {
            uint16_t ntf_cfg = co_read16p(&param->value[0]);
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                pftms_env->NotifyIndicateConfigList |= FTMS_NotifyFlag_Fitness_Machine_Status_0x2ADA;
            }
            else
            {
                pftms_env->NotifyIndicateConfigList &= (~FTMS_NotifyFlag_Fitness_Machine_Status_0x2ADA);
            }
            FlagUpDataNotifyIndicate = true;
            break;
        }
        case FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_VAL:
        {
			
            FTMS_ControlPointWriteData_t *pCrtl = KE_MSG_ALLOC(FTMS_Control_Point_0x2AD9_Write,
                                                               prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                               prf_src_task_get(&(pftms_env->prf_env), conidx),
                                                               FTMS_ControlPointWriteData);
            pCrtl->op_code = param->value[0];
            memcpy(pCrtl->para, &(param->value[1]), (param->length) - 1);
            pCrtl->length = (param->length) - 1;
            ke_msg_send(pCrtl);
            break;
        }
        default:
            status = PRF_APP_ERROR;
        }
    }

    if (FlagUpDataNotifyIndicate == true)
    {
        FTMS_NotifyIndicateConfig_t *pFlag = KE_MSG_ALLOC(FTMS_DATA_NTF_OR_IND_CFG,
                                                          prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                          prf_src_task_get(&(pftms_env->prf_env), conidx),
                                                          FTMS_NotifyIndicateConfig);
        pFlag->NTF_Config = pftms_env->NotifyIndicateConfigList;
        pFlag->Att_Index = att_idx;
        pFlag->Con_Index = 0;
        ke_msg_send(pFlag);
    }

    //Send write response
    cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, src_id, dest_id, gattc_write_cfm);
    cfm->handle = param->handle;
    cfm->status = status;
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

static int gattc_read_req_ind_handler(ke_msg_id_t const msgid, struct gattc_read_req_ind const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_read_cfm *cfm;

    // uint8_t conidx = KE_IDX_GET(src_id);
    // retrieve handle information
    // uint16_t length = 0;

    // uint8_t *value;
/*
{
    // profile environment
    prf_env_t prf_env;
    // Service Attribute Start Handle
    uint16_t start_hdl;
    // FTMS machine type
    FTMS_TYPE_e DeviceType;
    // Fitness Machine Features
    FitnessMachineFeature_t Feature;
		{
				uint32_t FitnessMachineFeatures;
				uint32_t TargetSettingFeatures;
		}
    //Fitness Machine Feature State
    TrainingStatusList_e DeviceState;
    // FTMS Parameter Range
    SupportedInclinationRange_t *pInclinationRange;
    SupportedResistanceRange_t *pResistanceRange;
    SupportedHeartRateRange_t *pHeartRateRange;
    SupportedPowerRange_t *pPowerRange;
    SupportedSpeedRange_t *pSpeedRange;
    TrainingStatusData_t xTrainingStatus;
			{
					uint8_t Flag;
					uint8_t Status;
					uint8_t StatusString[MAX_BUFF_LEN];
			}

 
    uint32_t NotifyIndicateConfigList;
    // uint32_t NotifyStatusList;
    uint32_t EnableCharList;
    ke_state_t state[FTMS_IDX_MAX];
    uint8_t conidx;
}
*/
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
		uart_printf("pftms_env->Feature:%x \r\n",pftms_env->Feature);
		
    uint8_t att_idx = ucGetFTMS_Index(param->handle);
    //test
    uart_printf("%s->%s->%d: Start handle = %d, handle = %d, att_idx = %d \r\n", __FILE__, __func__, __LINE__, pftms_env->start_hdl, param->handle, att_idx);
    //Send Read Response
    uint8_t status = ATT_ERR_NO_ERROR;

    /**************************************************/
    // If the attribute has been found, status is GAP_ERR_NO_ERROR
    if (status == GAP_ERR_NO_ERROR)
    {
        // read notification information
        switch (att_idx)
        {
        case FTMS_IDX_Feature_0x2ACC_VAL:
        {
					DataPack_t xDataPack;
					memset(&xDataPack, 0, sizeof(DataPack_t));
					FTMS_FeatureDataPackSerialization(&xDataPack, &pftms_env->Feature);
					uart_printf("pftms_env->Feature:%x \r\n",pftms_env->Feature);
					cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, xDataPack.Len);
					memcpy(&cfm->value[0], &xDataPack.Para[0], xDataPack.Len);
					cfm->length = xDataPack.Len;
				
					
					break;
        }
        case FTMS_IDX_Training_Status_0x2AD3_VAL:
        {
					//Updata the status
					DataPack_t xDataPack;
					memset(&xDataPack, 0, sizeof(DataPack_t));
					pftms_env->xTrainingStatus.Flag = TrainingStatusString | TrainingStatus;//������Ҫѡ���Ƿ���Ҫstring
					pftms_env->xTrainingStatus.Status = 0x02;//�����豸��ǰ״�������� TBD ��������NTF���ʱ��ͬ�����¸�����
					memcpy(pftms_env->xTrainingStatus.StatusString, "TobeDefine", strlen("TobeDefine"));//�����豸��ǰ״�������� TBD
					FTMS_TrainingStatusDataPackSerialization(&xDataPack, &pftms_env->xTrainingStatus);

					cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, xDataPack.Len);
					memcpy(&cfm->value[0], &xDataPack.Para[0], xDataPack.Len);
					cfm->length = xDataPack.Len;
					
            /**/
            break;
        }
        case FTMS_IDX_Speed_Range_0x2AD4_VAL:
        {
            DataPack_t xDataPack;
            memset(&xDataPack, 0, sizeof(DataPack_t));
            FTMS_SupportedSpeedRangeDataPackSerialization(&xDataPack, pftms_env->pSpeedRange);
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, xDataPack.Len);
            memcpy(&cfm->value[0], &xDataPack.Para[0], xDataPack.Len);
            cfm->length = xDataPack.Len;
            break;
        }
        case FTMS_IDX_Inclination_Range_0x2AD5_VAL:
        {
            DataPack_t xDataPack;
            memset(&xDataPack, 0, sizeof(DataPack_t));
            FTMS_SupportedInclinationRangeDataPackSerialization(&xDataPack, pftms_env->pInclinationRange);
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, xDataPack.Len);
            memcpy(&cfm->value[0], &xDataPack.Para[0], xDataPack.Len);
            cfm->length = xDataPack.Len;
            break;
        }
        case FTMS_IDX_Resistance_Level_Range_0x2AD6_VAL:
        {
            DataPack_t xDataPack;
            memset(&xDataPack, 0, sizeof(DataPack_t));
            FTMS_SupportedResistanceRangeDataPackSerialization(&xDataPack, pftms_env->pResistanceRange);
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, xDataPack.Len);
            memcpy(&cfm->value[0], &xDataPack.Para[0], xDataPack.Len);
            cfm->length = xDataPack.Len;
            break;
        }
        case FTMS_IDX_Heart_Rate_Range_0x2AD7_VAL:
        {
            DataPack_t xDataPack;
            memset(&xDataPack, 0, sizeof(DataPack_t));
            FTMS_SupportedHeartRateRangeDataPackSerialization(&xDataPack, pftms_env->pHeartRateRange);
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, xDataPack.Len);
            memcpy(&cfm->value[0], &xDataPack.Para[0], xDataPack.Len);
            cfm->length = xDataPack.Len;
            break;
        }
        case FTMS_IDX_Power_Range_0x2AD8_VAL:
        {
            DataPack_t xDataPack;
            memset(&xDataPack, 0, sizeof(DataPack_t));
            FTMS_SupportedPowerRangeDataPackSerialization(&xDataPack, pftms_env->pPowerRange);
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, xDataPack.Len);
            memcpy(&cfm->value[0], &xDataPack.Para[0], xDataPack.Len);
            cfm->length = xDataPack.Len;
            break;
        }
        case FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_IND_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));
            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_IndicateFlag_Fitness_Machine_Control_Point_0x2AD9) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        case FTMS_IDX_Treadmill_Data_0x2ACD_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));
            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Treadmill_Data_0x2ACD) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        case FTMS_IDX_Cross_Trainer_Data_0x2ACE_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));
            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Cross_Trainer_Data_0x2ACE) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        case FTMS_IDX_Step_Climber_Data_0x2ACF_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));
            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Step_Climber_Data_0x2ACF) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        case FTMS_IDX_Stair_Climber_Data_0x2AD0_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));
            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Stair_Climber_Data_0x2AD0) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        case FTMS_IDX_Rower_Data_0x2AD1_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));
            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Rower_Data_0x2AD1) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        case FTMS_IDX_Indoor_Bike_Data_0x2AD2_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));

            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Indoor_Bike_Data_0x2AD2) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            //test
            uart_printf("%s->%s->%d: cfm->value = %d ", __FILE__, __func__, __LINE__, *(uint16_t *)cfm->value);
            break;
        case FTMS_IDX_Training_Status_0x2AD3_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));
            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Training_Status_0x2AD3) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        case FTMS_IDX_Fitness_Machine_Status_0x2ADA_NTF_CFG:
            cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, sizeof(uint16_t));

            cfm->length = sizeof(uint16_t);
            co_write16((uint16_t *)cfm->value, (pftms_env->NotifyIndicateConfigList & FTMS_NotifyFlag_Fitness_Machine_Status_0x2ADA) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
            break;
        default:
            cfm = KE_MSG_ALLOC(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm);
            cfm->length = 0;
            status = ATT_ERR_REQUEST_NOT_SUPPORTED;
            break;
        }
        if (cfm != NULL)
        {
            cfm->handle = param->handle;
            cfm->status = status;
            ke_msg_send(cfm);
        }
    }
    return (KE_MSG_CONSUMED);
}

static int gattc_cmp_evt_handler(ke_msg_id_t const msgid,
                                 struct gattc_cmp_evt const *param,
                                 ke_task_id_t const dest_id,
                                 ke_task_id_t const src_id)
{
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
    uint8_t conidx = KE_IDX_GET(src_id);
    if (param->operation == GATTC_NOTIFY)
    {
        // uint8_t conidx = KE_IDX_GET(src_id);
        switch (param->seq_num)
        {
        case FTMS_IDX_Treadmill_Data_0x2ACD_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Treadmill_Data_0x2ACD_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        case FTMS_IDX_Cross_Trainer_Data_0x2ACE_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Cross_Trainer_Data_0x2ACE_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        case FTMS_IDX_Step_Climber_Data_0x2ACF_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Step_Climber_Data_0x2ACF_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        case FTMS_IDX_Stair_Climber_Data_0x2AD0_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Stair_Climber_Data_0x2AD0_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        case FTMS_IDX_Rower_Data_0x2AD1_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Rower_Data_0x2AD1_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        case FTMS_IDX_Indoor_Bike_Data_0x2AD2_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Indoor_Bike_Data_0x2AD2_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        case FTMS_IDX_Training_Status_0x2AD3_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Training_Status_0x2AD3_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        case FTMS_IDX_Fitness_Machine_Status_0x2ADA_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_MachineStatus_0x2ADA_Notify_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        }
    }
    if (param->operation == GATTC_INDICATE)
    {
        switch (param->seq_num)
        {
        case FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_VAL:
        {
            ftms_notify_rsp_t *rsp = KE_MSG_ALLOC(FTMS_Control_Point_0x2AD9_Indicate_RSP,
                                                  prf_dst_task_get(&(pftms_env->prf_env), conidx),
                                                  dest_id,
                                                  ftms_notify_rsp);
            rsp->status = param->status;
            ke_msg_send(rsp);
            break;
        }
        }
    }
    ke_state_set(dest_id, ke_state_get(dest_id) & (~FTMS_BUSY));

    return (KE_MSG_CONSUMED);
}

/// Default State handlers definition
KE_MSG_HANDLER_TAB(ftms)
    {
        {GATTC_ATT_INFO_REQ_IND, (ke_msg_func_t)gattc_att_info_req_ind_handler},
        {GATTC_WRITE_REQ_IND, (ke_msg_func_t)gattc_write_req_ind_handler},
        {GATTC_READ_REQ_IND, (ke_msg_func_t)gattc_read_req_ind_handler},
        {GATTC_CMP_EVT, (ke_msg_func_t)gattc_cmp_evt_handler},

        {FTMS_Treadmill_Data_0x2ACD_Notify, (ke_msg_func_t)ftms_TreadmillData_0x2ACD_Notify_req_handler},
        {FTMS_Cross_Trainer_Data_0x2ACE_Notify, (ke_msg_func_t)ftms_CrossTrainerData_0x2ACE_Notify_req_handler},
        {FTMS_Step_Climber_Data_0x2ACF_Notify, (ke_msg_func_t)ftms_StepClimberData_0x2ACF_Notify_req_handler},
        {FTMS_Stair_Climber_Data_0x2AD0_Notify, (ke_msg_func_t)ftms_StairClimberData_0x2AD0_Notify_req_handler},
        {FTMS_Rower_Data_0x2AD1_Notify, (ke_msg_func_t)ftms_RowerData_0x2AD1_Notify_req_handler},
        {FTMS_Indoor_Bike_Data_0x2AD2_Notify, (ke_msg_func_t)ftms_IndoorBikeData_0x2AD2_Notify_req_handler},
        {FTMS_Training_Status_0x2AD3_Notify, (ke_msg_func_t)ftms_TrainingStatus_0x2AD3_Notify_req_handler},
        {FTMS_MachineStatus_0x2ADA_Notify, (ke_msg_func_t)ftms_MachineStatus_0x2ADA_Notify_req_handler},

        {FTMS_Control_Point_0x2AD9_Indicate, (ke_msg_func_t)ftms_ControlPointResponse_0x2AD9_Indicate_req_handler},
};

void ftms_task_init(struct ke_task_desc *task_desc)
{
    // Get the address of the environment
    struct ftms_env_tag *srv_env = PRF_ENV_GET(FTMS, ftms);

    task_desc->msg_handler_tab = ftms_msg_handler_tab;
    task_desc->msg_cnt         = ARRAY_LEN(ftms_msg_handler_tab);
    task_desc->state           = srv_env->state;
    task_desc->idx_max         = FTMS_IDX_MAX;
}

#endif /* #if (BLE_FFF0S_SERVER) */
