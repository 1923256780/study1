/**
 ****************************************************************************************
 *
 * @file fff0s_task.h
 *
 * @brief Header file - Battery Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */


#ifndef _FFF0S_TASK_H_
#define _FFF0S_TASK_H_


#include "rwprf_config.h"
#if (BLE_FFF0S_SERVER)
#include <stdint.h>
#include "rwip_task.h" // Task definitions
/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of FFF0 Server task instances
#define FFF0S_IDX_MAX     0x01
///Maximal number of FFF0 that can be added in the DB

#define  FFF0_CHAR_DATA_LEN  628

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// Possible states of the FFF0S task
enum fff0s_state
{
    /// Idle state
    FFF0S_IDLE,
    /// busy state
    FFF0S_BUSY,
    /// Number of defined states.
    FFF0S_STATE_MAX
};

/// Messages for FFF0 Server
enum fff0s_msg_id
{
    /// Start the FFF0 Server - at connection used to restore bond data
	FFF0S_CREATE_DB_REQ   = TASK_FIRST_MSG(TASK_ID_FFF0S),
	
    // FFF0S_FFF1_VALUE_READ_IND,
    FFF0S_FFF2_WRITER_CMD_IND,
	// FFF0S_FFF3_WRITER_REQ_IND,

    
    /// FFF1 char Value Update Request
    FFF0S_FFF4_VALUE_UPD_REQ,
    /// Inform APP the FFF4 value has bffn send to stack
    FFF0S_FFF4_VALUE_UPD_RSP,
    /// Inform APP that FFF1 Level Notification Configuration has bffn changed - use to update bond data
    FFF0S_FFF4_VALUE_NTF_CFG_IND,

	// /// FFF5 Level Value Update Request
    // FFF0S_FFF5_VALUE_UPD_REQ,
    // /// Inform APP if FFF1 Level value has bffn notified or not
    // FFF0S_FFF5_VALUE_UPD_RSP,

	// FFF0S_FFF5_VALUE_IND_CFG_IND,
	


};

/// Features Flag Masks
enum fff0s_features
{
    /// FFFX  Characteristic doesn't support notifications
    FFF0_FFFX_VLUAE_NTF_NOT_SUP,
    /// FFF4 Value Characteristic support notifications
    FFF0_FFF4_VAL_NTF_SUP,
    /// FFF5 Value Characteristic support indication
    // FFF0_FFF5_VAL_IND_SUP,
};


/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct fff0s_db_cfg
{
    /// Number of FFF0 to add
    uint8_t fff0_nb;
    /// Features of each FFF0 instance
    uint8_t features;
    
    uint16_t fff1_desc_len;
    uint8_t fff1_desc[FFF0_CHAR_DATA_LEN];
};

/// Parameters of the @ref FFF0S_CREATE_DB_REQ message
struct fff0s_enable_req
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration
    uint16_t  ntf_cfg;

};


///Parameters of the @ref FFF0S_FFFx_VALUE_UPD_REQ message
struct fff0s_fff45_val_upd_req 
{
    ///  instance
    uint8_t conidx;
	
	uint16_t length;
    /// char value
    uint8_t value[FFF0_CHAR_DATA_LEN];
};


///Parameters of the @ref FFF0S_FFF4_VALUE_UPD_RSP message
struct fff0s_fff45_val_upd_rsp
{
    /// connection index
    uint8_t  conidx;
    ///status
    uint8_t status;
};

///Parameters of the @ref FFF0S_FFF4_VALUE_NTF_CFG_IND message
struct fff0s_fff4_val_ntf_cfg_ind
{
    /// connection index
    uint8_t  conidx;
    ///Notification Configuration
    uint16_t  ntf_cfg;
};


///Parameters of the @ref FFF0S_FFF5_VALUE_IND_CFG_IND message
// struct fff0s_fff5_val_ind_cfg_ind
// {
//     /// connection index
//     uint8_t  conidx;
//     ///Ind Configuration
//     uint16_t  ind_cfg;
// };

/// Parameters of the @ref FFF0S_FFF2_WRITER_CMD_IND message
struct fff0s_fff23_writer_ind
{
  /// Connection index
    uint8_t conidx;
    uint16_t length;
    ///  value
    uint8_t value[FFF0_CHAR_DATA_LEN];

};


/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler fff0s_default_handler;
#endif // BLE_FFF0S_SERVER


#endif /* _FFF0S_TASK_H_ */

