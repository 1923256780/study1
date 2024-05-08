/**
 ****************************************************************************************
 *
 * @file fff0s.c
 *
 * @brief fff0 Server Implementation.
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_FFF0S_SERVER)
#include "attm.h"
#include "fff0s.h"
#include "fff0s_task.h"
#include "prf_utils.h"
#include "prf.h"
#include "ke_mem.h"

#include "uart.h"



/*
 * FFF0 ATTRIBUTES DEFINITION
 ****************************************************************************************
 */
 
/// Full FFF0 Database Description - Used to add attributes into the database
const struct attm_desc fff0_att_db[FFF0S_IDX_NB] =
{
    // FFF0 Service Declaration
    [FFF0S_IDX_SVC]            =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

    
	// fff2 value Characteristic Declaration
	[FFF0S_IDX_FFF2_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // fff2 Level Characteristic Value
    [FFF0S_IDX_FFF2_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_FFF2, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), FFF0_CHAR_DATA_LEN * sizeof(uint8_t)},


	
	// fff4 Level Characteristic Declaration
	[FFF0S_IDX_FFF4_VAL_CHAR]  =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // fff4 Level Characteristic Value
    [FFF0S_IDX_FFF4_VAL_VAL]   =   {ATT_USER_SERVER_CHAR_FFF4, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE), FFF0_CHAR_DATA_LEN * sizeof(uint8_t)},

	// fff1 Level Characteristic - Client Characteristic Configuration Descriptor
	[FFF0S_IDX_FFF4_VAL_NTF_CFG] = {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    

    
	
};/// Macro used to retrieve permission value from access and rights on attribute.


static uint8_t fff0s_init (struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task, uint8_t sec_lvl,  struct fff0s_db_cfg* params)
{
    uint16_t shdl;
    struct fff0s_env_tag* fff0s_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    
    //-------------------- allocate memory required for the profile  ---------------------
    fff0s_env = (struct fff0s_env_tag* ) ke_malloc(sizeof(struct fff0s_env_tag), KE_MEM_ATT_DB);
    memset(fff0s_env, 0 , sizeof(struct fff0s_env_tag));

    // Service content flag
    uint16_t cfg_flag = FFF0S_CFG_FLAG_MANDATORY_MASK;

    // Save database configuration
    fff0s_env->features |= (params->features) ;
    
    fff0s_env->fff1_desc_len = params->fff1_desc_len;
    memcpy(fff0s_env->fff1_desc,params->fff1_desc,params->fff1_desc_len);
    shdl = *start_hdl;

    //Create FFF0 in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(&(shdl), ATT_USER_SERVER_FFF0, (uint8_t *)&cfg_flag,
            FFF0S_IDX_NB, NULL, env->task, &fff0_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));

	//Set optional permissions
    if (status == GAP_ERR_NO_ERROR)
    {
        //Set optional permissions
       // if((params->features & 0x01) == FFF0_FFF4_VAL_NTF_SUP)
        {
            // Battery Level characteristic value permissions
            uint16_t perm = PERM(NTF, ENABLE);//PERM(RD, ENABLE) | 

            attm_att_set_permission(shdl + FFF0S_IDX_FFF4_VAL_VAL, perm, 0);
        }
		// //if((params->features & 0x02) == FFF0_FFF3_LVL_NTF_SUP)
		// {
		// 	uint16_t perm = PERM(IND, ENABLE);//PERM(RD, ENABLE) | 

        //     attm_att_set_permission(shdl + FFF0S_IDX_FFF5_VAL_VAL, perm, 0);
		// }
    }
	
    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {

        // allocate  required environment variable
        env->env = (prf_env_t*) fff0s_env;
        *start_hdl = shdl;
        fff0s_env->start_hdl = *start_hdl;
        fff0s_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        fff0s_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_FFF0S;
        env->desc.idx_max           = FFF0S_IDX_MAX;
        env->desc.state             = fff0s_env->state;
        
        fff0s_task_init(&(env->desc));
        co_list_init(&(fff0s_env->values));



        // service is ready, go into an Idle state
        ke_state_set(env->task, FFF0S_IDLE);
    }
    else if(fff0s_env != NULL)
    {
        ke_free(fff0s_env);
    }
     
    return (status);
}


static void fff0s_destroy(struct prf_task_env* env)
{
    struct fff0s_env_tag* fff0s_env = (struct fff0s_env_tag*) env->env;

    // clear on-going operation
    if(fff0s_env->operation != NULL)
    {
        ke_free(fff0s_env->operation);
    }

    // frff profile environment variables
    env->env = NULL;
    ke_free(fff0s_env);
}

static void fff0s_create(struct prf_task_env* env, uint8_t conidx)
{
    struct fff0s_env_tag* fff0s_env = (struct fff0s_env_tag*) env->env;
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when pffr device is connected
    fff0s_env->ntf_cfg[conidx] = 0;
}


static void fff0s_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct fff0s_env_tag* fff0s_env = (struct fff0s_env_tag*) env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when pffr device is disconnected
    fff0s_env->ntf_cfg[conidx] = 0;
}

void fff0s_notify_fff4_val(uint8_t conidx,struct fff0s_env_tag* fff0s_env, struct fff0s_fff45_val_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(fff0s_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

    // Fill in the parameter structure
    val->operation = GATTC_NOTIFY;
    val->handle = fff0s_get_att_handle(FFF0S_IDX_FFF4_VAL_VAL);

    // pack measured value in database
    val->length = param->length;
	memcpy(&val->value[0],&param->value[0],param->length);
    // send notification to pffr device
    ke_msg_send(val);
}


// void fff0s_indicate_fff5_val(uint8_t conidx,struct fff0s_env_tag* fff0s_env, struct fff0s_fff45_val_upd_req const *param)
// {
//     // Allocate the GATT notification message
//     struct gattc_send_evt_cmd *val = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
//             KE_BUILD_ID(TASK_GATTC, conidx), prf_src_task_get(&(fff0s_env->prf_env),0),
//             gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

//     // Fill in the parameter structure
//     val->operation = GATTC_INDICATE;
//     val->handle = fff0s_get_att_handle(FFF0S_IDX_FFF5_VAL_VAL);
//     // pack measured value in database
//     val->length = param->length;
// 	memcpy(&val->value[0],&param->value[0],param->length);
//     // send notification to pffr device
//     ke_msg_send(val);
// }



/// BASS Task interface required by profile manager
const struct prf_task_cbs fff0s_itf =
{
        (prf_init_fnct) fff0s_init,
        fff0s_destroy,
        fff0s_create,
        fff0s_cleanup,
};


const struct prf_task_cbs* fff0s_prf_itf_get(void)
{
   return &fff0s_itf;
}


uint16_t fff0s_get_att_handle( uint8_t att_idx)
{
		
    struct fff0s_env_tag *fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
    uint16_t handle = ATT_INVALID_HDL;
   
    handle = fff0s_env->start_hdl;

    // increment index according to expected index
    if(att_idx <= FFF0S_IDX_FFF4_VAL_NTF_CFG)
    {
        handle += att_idx;
    }
	else
    {
        handle = ATT_INVALID_HDL;
    }
    

    return handle;
}

uint8_t fff0s_get_att_idx(uint16_t handle, uint8_t *att_idx)
{
    struct fff0s_env_tag* fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
    uint16_t hdl_cursor1 = fff0s_env->start_hdl;	
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
    if(handle <= (hdl_cursor1 + FFF0S_IDX_FFF4_VAL_NTF_CFG))
    {
        *att_idx = handle -hdl_cursor1;
        status = GAP_ERR_NO_ERROR;
        
    }
    
    return (status);
}


#endif // (BLE_FFF0S_SERVER)


 
