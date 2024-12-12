
#include "rwip_config.h"

#if (BLE_FTMS_SERVER)
#include "attm.h"
#include "att.h"
#include "ftms.h"
#include "ftms_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"

#include "uart.h"
#include "Device.h"

const struct attm_desc ftms_att_db[FTMS_IDX_NB] =
    {
        [FTMS_IDX_SVC] = {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

        //0x2ACC
        [FTMS_IDX_Feature_0x2ACC_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Feature_0x2ACC_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Feature, PERM(RD, ENABLE), PERM(RI, ENABLE), FTMS_Feature_0x2ACC_LEN},

        //0x2AD4
        [FTMS_IDX_Speed_Range_0x2AD4_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Speed_Range_0x2AD4_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Speed_Range, PERM(RD, ENABLE), PERM(RI, ENABLE), FTMS_Speed_Range_0x2AD4_LEN},

        //0x2AD5
        [FTMS_IDX_Inclination_Range_0x2AD5_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Inclination_Range_0x2AD5_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Inclination_Range, PERM(RD, ENABLE), PERM(RI, ENABLE), FTMS_Inclination_Range_0x2AD5_LEN},

        //0x2AD6
        [FTMS_IDX_Resistance_Level_Range_0x2AD6_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Resistance_Level_Range_0x2AD6_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Resistance_Level_Range, PERM(RD, ENABLE), PERM(RI, ENABLE), FTMS_Resistance_Level_Range_0x2AD6_LEN},

        //0x2AD7
        [FTMS_IDX_Heart_Rate_Range_0x2AD7_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Heart_Rate_Range_0x2AD7_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Heart_Rate_Range, PERM(RD, ENABLE), PERM(RI, ENABLE), FTMS_Heart_Rate_Range_0x2AD7_LEN},

        //0x2AD8
        [FTMS_IDX_Power_Range_0x2AD8_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Power_Range_0x2AD8_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Power_Range, PERM(RD, ENABLE), PERM(RI, ENABLE), FTMS_Power_Range_0x2AD8_LEN},

        //0x2AD9
        [FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Control_Point, PERM(WRITE_REQ, ENABLE) | PERM(IND, ENABLE), PERM(RI, ENABLE), FTMS_Control_Point_0x2AD9_LEN},
        [FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_IND_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2ACD Treadmill_Data
        [FTMS_IDX_Treadmill_Data_0x2ACD_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Treadmill_Data_0x2ACD_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Treadmill_Data, PERM(NTF, ENABLE), PERM(RI, ENABLE), FTMS_Treadmill_Data_0x2ACD_LEN},
        [FTMS_IDX_Treadmill_Data_0x2ACD_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2ACE  Cross_Trainer_Data
        [FTMS_IDX_Cross_Trainer_Data_0x2ACE_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Cross_Trainer_Data_0x2ACE_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Cross_Trainer_Data, PERM(NTF, ENABLE), PERM(RI, ENABLE), FTMS_Cross_Trainer_Data_0x2ACE_LEN},
        [FTMS_IDX_Cross_Trainer_Data_0x2ACE_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2ACF
        [FTMS_IDX_Step_Climber_Data_0x2ACF_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Step_Climber_Data_0x2ACF_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Step_Climber_Data, PERM(NTF, ENABLE), PERM(RI, ENABLE), FTMS_Step_Climber_Data_0x2ACF_LEN},
        [FTMS_IDX_Step_Climber_Data_0x2ACF_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2AD0
        [FTMS_IDX_Stair_Climber_Data_0x2AD0_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Stair_Climber_Data_0x2AD0_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Stair_Climber_Data, PERM(NTF, ENABLE), PERM(RI, ENABLE), FTMS_Stair_Climber_Data_0x2AD0_LEN},
        [FTMS_IDX_Stair_Climber_Data_0x2AD0_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2AD1
        [FTMS_IDX_Rower_Data_0x2AD1_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Rower_Data_0x2AD1_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Rower_Data, PERM(NTF, ENABLE), PERM(RI, ENABLE), FTMS_Rower_Data_0x2AD1_LEN},
        [FTMS_IDX_Rower_Data_0x2AD1_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2AD2
        [FTMS_IDX_Indoor_Bike_Data_0x2AD2_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Indoor_Bike_Data_0x2AD2_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Indoor_Bike_Data, PERM(NTF, ENABLE), PERM(RI, ENABLE), FTMS_Indoor_Bike_Data_0x2AD2_LEN},
        [FTMS_IDX_Indoor_Bike_Data_0x2AD2_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2AD3
        [FTMS_IDX_Training_Status_0x2AD3_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Training_Status_0x2AD3_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Training_Status, PERM(NTF, ENABLE) | PERM(RD, ENABLE), PERM(RI, ENABLE), FTMS_Training_Status_0x2AD3_LEN},
        [FTMS_IDX_Training_Status_0x2AD3_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},

        //0x2ADA
        [FTMS_IDX_Fitness_Machine_Status_0x2ADA_CHAR] = {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
        [FTMS_IDX_Fitness_Machine_Status_0x2ADA_VAL] = {ATT_USER_SERVER_CHAR_FTMS_Status, PERM(NTF, ENABLE), PERM(RI, ENABLE), FTMS_Status_0x2ADA_LEN},
        [FTMS_IDX_Fitness_Machine_Status_0x2ADA_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},
};

static uint8_t ftms_init(struct prf_task_env *env, uint16_t *start_hdl, uint16_t app_task, uint8_t sec_lvl, struct FTMS_db_cfg *params)
{
    uint16_t shdl;
    ftms_env_tag_t *pftms_env = NULL;

    pftms_env = (ftms_env_tag_t *)ke_malloc(sizeof(ftms_env_tag_t), KE_MEM_ATT_DB);
    memset(pftms_env, 0, sizeof(ftms_env_tag_t));

    pftms_env->pPowerRange = params->pPowerRange;
    pftms_env->pSpeedRange = params->pSpeedRange;
    pftms_env->pInclinationRange = params->pInclinationRange;
    pftms_env->pResistanceRange = params->pResistanceRange;
    pftms_env->pHeartRateRange = params->pHeartRateRange;

    uint8_t cfg_flag[5] = {0};
    memset(cfg_flag, 0, sizeof(cfg_flag));
    cfg_flag[0] = (1 << FTMS_IDX_SVC);
    uint32_t MaskFlag = 0x01;
    uart_printf("%s->%s->%d: 0x%08x, 0x%08x \r\n", __FILE__, __func__, __LINE__, params->EnableCharList, params->Feature.FitnessMachineFeatures);
    pftms_env->EnableCharList = params->EnableCharList;
    pftms_env->Feature.FitnessMachineFeatures = params->Feature.FitnessMachineFeatures;
    pftms_env->Feature.TargetSettingFeatures = params->Feature.TargetSettingFeatures;
    for (uint8_t i = FTMS_IDX_Feature_0x2ACC_CHAR; i < FTMS_IDX_NB;)
    {
        if ((params->EnableCharList & MaskFlag) == MaskFlag)
        {
            cfg_flag[i / 8] |= (1 << (i % 8));
            i++;
            cfg_flag[i / 8] |= (1 << (i % 8));
            i++;
            if (i > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR)
            {
                cfg_flag[i / 8] |= (1 << (i % 8));
                i++;
            }
        }
        else
        {
            i += 2;
            if (i > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR)
            {
                i++;
            }
        }
        MaskFlag = MaskFlag << 1;
    }
    

    shdl = *start_hdl;
    uint8_t status = attm_svc_create_db(&shdl,
                                        ATT_USER_SERVER_FTMS,
                                        (uint8_t *)cfg_flag,
                                        FTMS_IDX_NB,
                                        NULL,
                                        env->task,
                                        &ftms_att_db[0],
                                        (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));
    if (status == ATT_ERR_NO_ERROR)
    {
        env->env = (prf_env_t *)pftms_env;
        *start_hdl = shdl;
        pftms_env->start_hdl = *start_hdl;
        // pftms_env->Feature = cfg_flag;

        pftms_env->prf_env.app_task = app_task | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        pftms_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id = TASK_ID_FTMS;
        env->desc.idx_max = FTMS_IDX_MAX;
        env->desc.state = pftms_env->state;

        pftms_env->NotifyIndicateConfigList = 0;

        ftms_task_init(&(env->desc));

        ke_state_set(env->task, FTMS_IDLE);
    }

    return status;
}

void ftms_SendDataByNotify(uint8_t conidx,ftms_env_tag_t *pftms_env, uint8_t AttIndex, const DataPack_t *pDataPack)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *req = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
                                                      KE_BUILD_ID(TASK_GATTC, conidx),
                                                      prf_src_task_get(&(pftms_env->prf_env), 0),
                                                      gattc_send_evt_cmd,
                                                      pDataPack->Len);
    // Fill in the parameter structure
    req->operation = GATTC_NOTIFY;
    req->handle = ucGetFTMS_Handle(AttIndex);
    // save current to seq_num
    req->seq_num = AttIndex;
    // pack measured value in database
    req->length = pDataPack->Len;
    memcpy(&req->value[0], &pDataPack->Para[0], pDataPack->Len);
    // send notification to peer device
    ke_msg_send(req);
}

void ftms_SendDataByIndicate(ftms_env_tag_t *pftms_env, uint8_t AttIndex, const DataPack_t *pDataPack)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *req = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
                                                      KE_BUILD_ID(TASK_GATTC, 0),
                                                      prf_src_task_get(&(pftms_env->prf_env), 0),
                                                      gattc_send_evt_cmd,
                                                      pDataPack->Len);
    // Fill in the parameter structure
    req->operation = GATTC_INDICATE;
    req->handle = ucGetFTMS_Handle(AttIndex);
    // save current to seq_num
    req->seq_num = AttIndex;
    // pack measured value in database
    req->length = pDataPack->Len;
    memcpy(&req->value[0], &pDataPack->Para[0], pDataPack->Len);
    // send indicate to peer device
    ke_msg_send(req);
}

static void ftms_destroy(struct prf_task_env *env)
{
    ftms_env_tag_t *ftms_env = (ftms_env_tag_t *)env->env;
    // if(ftms_env->operation != NULL)
    // {
    //     ke_free(ftms_env->operation);
    // }
    env->env = NULL;
    ke_free(ftms_env);
}

static void ftms_create(struct prf_task_env *env, uint8_t conidx)
{
    ftms_env_tag_t *ftms_env = (ftms_env_tag_t *)env->env;
    ftms_env->NotifyIndicateConfigList = 0x00; 
}

static void ftms_cleanup(struct prf_task_env *env, uint8_t conidx, uint8_t reason)
{
}

const struct prf_task_cbs ftms_itf =
    {
        (prf_init_fnct)ftms_init,
        ftms_destroy,
        ftms_create,
        ftms_cleanup,
};

const struct prf_task_cbs *ftms_prf_itf_get(void)
{
    return &ftms_itf;
}

uint8_t ucGetFTMS_Index(uint16_t handle)
{
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
    uint32_t MaskFlag = 0x01;
    uint8_t att_index = 0;
    handle = handle - pftms_env->start_hdl;
    for (uint8_t i = FTMS_IDX_Feature_0x2ACC_CHAR; i < FTMS_IDX_NB;)
    {
        if ((pftms_env->EnableCharList & MaskFlag) == MaskFlag)
        {
            if (handle > i)
            {
                i++;
            }
            else
            {
                att_index = i;
                break;
            }
            if (handle > i)
            {
                i++;
            }
            else
            {
                att_index = i;
                break;
            }
            if (i > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR)
            {
                if (handle > i)
                {
                    i++;
                }
                else
                {
                    att_index = i;
                    break;
                }
            }
        }
        else
        {
            i += 2;
            handle += 2;
            if (i > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR)
            {
                i++;
                handle++;
            }
        }
        MaskFlag = MaskFlag << 1;
    }
    return att_index;
}
uint16_t ucGetFTMS_Handle(uint8_t Index) //14
{
    ftms_env_tag_t *pftms_env = PRF_ENV_GET(FTMS, ftms);
    uint32_t MaskFlag = 0x01;
    uint8_t CurrentHandle = pftms_env->start_hdl; // FTMS_IDX_SVC
    for (uint8_t i = FTMS_IDX_Feature_0x2ACC_CHAR; i < FTMS_IDX_NB;)
    {
        if ((pftms_env->EnableCharList & MaskFlag) == MaskFlag)
        {
            if (Index >= i)
            {
                i++;
                CurrentHandle++;
            }
            else
            {
                return CurrentHandle;
            }
            if (Index >= i)
            {
                i++;
                CurrentHandle++;
            }
            else
            {
                return CurrentHandle;
            }
            if (i > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR)
            {
                if (Index >= i)
                {
                    i++;
                    CurrentHandle++;
                }
                else
                {
                    return CurrentHandle;
                }
            }
        }
        else
        {
            i += 2;
            if (i > FTMS_IDX_Fitness_Machine_Control_Point_0x2AD9_CHAR)
            {
                i++;
            }
        }
        MaskFlag = MaskFlag << 1;
    }
    return pftms_env->start_hdl;
}

#endif // (BLE_FTMS_SERVER)
