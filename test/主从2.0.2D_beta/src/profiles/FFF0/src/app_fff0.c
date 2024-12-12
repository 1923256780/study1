/**
 ****************************************************************************************
 *
 * @file app_fff0.c
 *
 * @brief fff0 Application Module entry point
 *
 * @auth  gang.cheng
 *
 * @date  2020.03.17
 *
 * Copyright (C) Beken 2019-2022
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration
#include "Device.h"
//#if (BLE_APP_FFF0S)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>
#include "ke_task.h"                 // Kernel
#include "app_fff0.h"              // Battery Application Module Definitions
#include "app.h"                   // Application Definitions
#include "app_task.h"              // application task definitions
#include "fff0s_task.h"            // health thermometer functions
#include "co_bt.h"
#include "co_utils.h"
#include "prf_types.h"             // Profile common types definition
#include "arch.h"                  // Platform Definitions
#include "prf.h"
#include "fff0s.h"
#include "ke_timer.h"
#include "uart.h"
#include "prf_utils.h"
#include "user_config.h"


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// fff0 Application Module Environment Structure
struct app_fff0_env_tag app_fff0_env;

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


void app_fff0s_init(void)
{

    // Reset the environment
    memset(&app_fff0_env, 0, sizeof(struct app_fff0_env_tag));
}

void app_fff0_add_fff0s(void)
{

   struct fff0s_db_cfg *db_cfg;
		
   struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct fff0s_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl =   0;
    req->prf_task_id = TASK_ID_FFF0S;
    req->app_task = TASK_APP;
    req->start_hdl = 0; //req->start_hdl = 0; dynamically allocated

	 
    // Set parameters
    db_cfg = (struct fff0s_db_cfg* ) req->param;
   
    db_cfg->fff1_desc_len = strlen("fff1_only_read");
   
    memcpy(db_cfg->fff1_desc,"fff1_only_read",strlen("fff1_only_read"));
	 
    // Sending of notifications is supported
    // Send the message
    ke_msg_send(req);
}


void app_fff4_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf)
{
    if( ke_state_get(prf_get_task_from_id(TASK_ID_FFF0S))==FFF0S_IDLE ) 
    {
        struct fff0s_env_tag* fff0s_env = PRF_ENV_GET(FFF0S, fff0s);
   
        // if(fff0s_env->ntf_cfg[conidx] == PRF_CLI_START_NTF)
        {
            // Allocate the message
            struct fff0s_fff45_val_upd_req * req = KE_MSG_ALLOC(FFF0S_FFF4_VALUE_UPD_REQ,
                                                                prf_get_task_from_id(TASK_ID_FFF0S),
                                                                KE_BUILD_ID(TASK_APP, conidx),
                                                                fff0s_fff45_val_upd_req);
            // Fill in the parameter structure	
            req->length = len;
        	memcpy(req->value, buf, len);

            // Send the message
            ke_msg_send(req);
        }
    }
}

// void app_fff5_send_ind(uint8_t conidx,uint16_t len,uint8_t* buf)
// {
//     if( ke_state_get(prf_get_task_from_id(TASK_ID_FFF0S))==FFF0S_IDLE )
//     {
//         struct fff0s_env_tag* fff0s_env = PRF_ENV_GET(FFF0S, fff0s);

//         if(fff0s_env->ind_cfg[conidx] == PRF_CLI_START_IND)
//         {
//             // Allocate the message
//             struct fff0s_fff45_val_upd_req * req = KE_MSG_ALLOC(FFF0S_FFF5_VALUE_UPD_REQ,
//                                                                 prf_get_task_from_id(TASK_ID_FFF0S),
//                                                                 KE_BUILD_ID(TASK_APP, conidx),
//                                                                 fff0s_fff45_val_upd_req);
//             // Fill in the parameter structure	
//             req->length = len;
//         	memcpy(req->value, buf, len);

//             // Send the message
//             ke_msg_send(req);
//         }
//     }
// }



static int fff0s_fff4_val_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                               struct fff0s_fff4_val_ntf_cfg_ind const *param,
                                               ke_task_id_t const dest_id,
                                               ke_task_id_t const src_id)
{
	uart_printf("fff4->param->ntf_cfg = %x\r\n",param->ntf_cfg);
	if(param->ntf_cfg == PRF_CLI_STOP_NTFIND)
	{
		//ke_timer_clear(FFF0S_FFF1_LEVEL_PERIOD_NTF,dest_id);
	}else
	{
		//ke_timer_set(FFF0S_FFF1_LEVEL_PERIOD_NTF,dest_id , 100);
	}
    
    return (KE_MSG_CONSUMED);
}

// static int fff0s_fff5_val_ind_cfg_ind_handler(ke_msg_id_t const msgid,
//                                                struct fff0s_fff5_val_ind_cfg_ind const *param,
//                                                ke_task_id_t const dest_id,
//                                                ke_task_id_t const src_id)
// {
// 	uart_printf("fff5->param->ind_cfg = %x\r\n",param->ind_cfg);
// 	if(param->ind_cfg == PRF_CLI_STOP_NTFIND)
// 	{
// 		//ke_timer_clear(FFF0S_FFF3_LEVEL_PERIOD_NTF,dest_id);
// 	}else
// 	{
// 		//ke_timer_set(FFF0S_FFF3_LEVEL_PERIOD_NTF,dest_id , 100);
// 	}
    
//     return (KE_MSG_CONSUMED);
// }
static int fff4_val_upd_rsp_handler(ke_msg_id_t const msgid,
                                      struct fff0s_fff45_val_upd_rsp const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
	//uart_printf("%s,status:%x\r\n", __func__,param->status);
	
	if(param->status == GAP_ERR_NO_ERROR)
	{
	    ke_state_set(src_id, FFF0S_IDLE);	
	}
	
    return (KE_MSG_CONSUMED);
}

// static int fff5_val_upd_rsp_handler(ke_msg_id_t const msgid,
//                                       struct fff0s_fff45_val_upd_rsp const *param,
//                                       ke_task_id_t const dest_id,
//                                       ke_task_id_t const src_id)
// {
// 	uart_printf("%s,status:%x\r\n", __func__,param->status);
	
// 	if(param->status == GAP_ERR_NO_ERROR)
// 	{
// 	    ke_state_set(src_id, FFF0S_IDLE);	
// 	}
	
//     return (KE_MSG_CONSUMED);
// }


/**
 ****************************************************************************************
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_fff0_msg_dflt_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
	uart_printf("%s\r\n", __func__);
	
    // Drop the message
    return (KE_MSG_CONSUMED);
}


static int fff2_writer_cmd_handler(ke_msg_id_t const msgid,
                                     struct fff0s_fff23_writer_ind *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    // uint8_t buf[20];

    BLE_REC_DATA(param->value, param->length);
    // Drop the message
	// uart_printf("FFF2 conidx:%d,length:%d,param->value = 0x ",param->conidx,param->length);
	
	// for(uint16_t i = 0;i < param->length;i++)
	// {
	// 	uart_printf("%02x ",param->value[i]);
	// }
    
	// uart_printf("\r\n");
    // for(uint8_t i = 0;i < 20;i++)
	// {
	// 	buf[i] = i;
	// }
    
    // if(param->value[0] == 0x55)
    // {
    //     app_fff4_send_ntf(param->conidx,20,buf);
    // }
    // if(param->value[0] == 0x66)
    // {
    //     // app_fff5_send_ind(param->conidx,20,buf);
    // }
#if 0    
    if(param->value[0] == 0xAA)
    {
        struct gapc_conn_param conn_param;
        conn_param.intv_min = param->value[1];
        conn_param.intv_max = param->value[1] + 2;
        conn_param.latency  = 0;
        conn_param.time_out = 200;

        appm_update_param(&conn_param);
    }
#endif		
    return (KE_MSG_CONSUMED);
}

// static int fff3_writer_req_handler(ke_msg_id_t const msgid,
//                                      struct fff0s_fff23_writer_ind *param,
//                                      ke_task_id_t const dest_id,
//                                      ke_task_id_t const src_id)
// {
//     // Drop the message
// 	uart_printf("FFF3 conidx:%d,length:%d,param->value = 0x ",param->conidx,param->length);
	
// 	for(uint16_t i = 0;i < param->length;i++)
// 	{
// 		uart_printf("%02x ",param->value[i]);
// 	}
// 	uart_printf("\r\n");
		
//     return (KE_MSG_CONSUMED);
// }



/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_fff0_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_fff0_msg_dflt_handler},
    {FFF0S_FFF4_VALUE_NTF_CFG_IND,  (ke_msg_func_t)fff0s_fff4_val_ntf_cfg_ind_handler},
    // {FFF0S_FFF5_VALUE_IND_CFG_IND,  (ke_msg_func_t)fff0s_fff5_val_ind_cfg_ind_handler},
    {FFF0S_FFF4_VALUE_UPD_RSP,      (ke_msg_func_t)fff4_val_upd_rsp_handler},
    // {FFF0S_FFF5_VALUE_UPD_RSP,      (ke_msg_func_t)fff5_val_upd_rsp_handler},
    {FFF0S_FFF2_WRITER_CMD_IND,		(ke_msg_func_t)fff2_writer_cmd_handler},
    // {FFF0S_FFF3_WRITER_REQ_IND,		(ke_msg_func_t)fff3_writer_req_handler},
    
};

const struct app_subtask_handlers app_fff0_handler = APP_HANDLERS(app_fff0);

//#endif

