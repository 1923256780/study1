/**
 ****************************************************************************************
 *
 * @file fff0s.h
 *
 * @brief Header file - FFF0 Service Server Role
 *
 * Copyright (C) beken 2019-2022
 *
 *
 ****************************************************************************************
 */
#ifndef _FFF0S_H_
#define _FFF0S_H_

/**
 ****************************************************************************************
 * @addtogroup  FFF0 'Profile' Server
 * @ingroup FFF0
 * @brief FFF0 'Profile' Server
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"
#include "rwprf_config.h"

#if (BLE_FFF0S_SERVER)

#include "fff0s_task.h"
#include "atts.h"
#include "prf_types.h"
#include "prf.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define FFF0S_CFG_FLAG_MANDATORY_MASK       (0x3FFF)

enum
{		
		ATT_USER_SERVER_FFF0 						= ATT_UUID_16(0xFFF0),	// service 
		// ATT_USER_SERVER_CHAR_FFF1                   = ATT_UUID_16(0xFFF1), // read		
		ATT_USER_SERVER_CHAR_FFF2					= ATT_UUID_16(0xFFF2),// write cmd
		// ATT_USER_SERVER_CHAR_FFF3					= ATT_UUID_16(0xFFF3),// write req
        ATT_USER_SERVER_CHAR_FFF4					= ATT_UUID_16(0xFFF1), // ntf
        // ATT_USER_SERVER_CHAR_FFF5					= ATT_UUID_16(0xFFF5), // ind
		
};

/// FFF0S Service Attributes Indexes
enum
{
	FFF0S_IDX_SVC,

    // FFF0S_IDX_FFF1_VAL_CHAR,
	// FFF0S_IDX_FFF1_VAL_VAL,
    // FFF0S_IDX_FFF1_USER_DESC,
    
	FFF0S_IDX_FFF2_VAL_CHAR,
	FFF0S_IDX_FFF2_VAL_VAL,

    // FFF0S_IDX_FFF3_VAL_CHAR,
	// FFF0S_IDX_FFF3_VAL_VAL,
    
	FFF0S_IDX_FFF4_VAL_CHAR,
	FFF0S_IDX_FFF4_VAL_VAL,
	FFF0S_IDX_FFF4_VAL_NTF_CFG,

	// FFF0S_IDX_FFF5_VAL_CHAR,
	// FFF0S_IDX_FFF5_VAL_VAL,
	// FFF0S_IDX_FFF5_VAL_IND_CFG,

	FFF0S_IDX_NB,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// FFF0 'Profile' Server environment variable
struct fff0s_env_tag
{
    /// profile environment
    prf_env_t prf_env;
   
    /// On-going operation
    struct ke_msg * operation;   
    /// List of values set by application
    struct co_list values;
    /// FFF0 Services Start Handle
    uint16_t start_hdl;
    /// value of the Fffx
    // uint8_t fff1_val[FFF0_CHAR_DATA_LEN];
    uint8_t fff2_val[FFF0_CHAR_DATA_LEN];
    // uint8_t fff3_val[FFF0_CHAR_DATA_LEN];
    uint8_t fff4_val[FFF0_CHAR_DATA_LEN];
    // uint8_t fff5_val[FFF0_CHAR_DATA_LEN];
    uint8_t fff1_desc[FFF0_CHAR_DATA_LEN];
    uint8_t fff1_desc_len;
    /// BASS task state
    ke_state_t state[FFF0S_IDX_MAX];
    /// Notification configuration of pffr devices.
    uint16_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Notification configuration of pffr devices.
    uint16_t ind_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;

};



/**
 ****************************************************************************************
 * @brief Retrieve fff0 service profile interface
 *
 * @return fff0 service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* fff0s_prf_itf_get(void);

uint16_t fff0s_get_att_handle(uint8_t att_idx);

uint8_t  fff0s_get_att_idx(uint16_t handle, uint8_t *att_idx);

void fff0s_notify_fff4_val(uint8_t conidx,struct fff0s_env_tag* fff0s_env, struct fff0s_fff45_val_upd_req const *param);

// void fff0s_indicate_fff5_val(uint8_t conidx,struct fff0s_env_tag* fff0s_env, struct fff0s_fff45_val_upd_req const *param);
void fff0s_task_init(struct ke_task_desc *task_desc);

#endif /* #if (BLE_FFF0S_SERVERs) */



#endif /*  _FFF0_H_ */



