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
 * Copyright (C) Beken 2020-2022
 *
 *
 ****************************************************************************************
 */
#ifndef APP_FFF0_H_
#define APP_FFF0_H_
/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief fff0 Application Module entry point
 *
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration


#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"         // Kernel Task Definition

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// fff0s Application Module Environment Structure
struct app_fff0_env_tag
{
    /// Connection handle
    uint8_t conidx;
    /// Current Battery Level
    uint8_t fff1_lvl;
	uint8_t fff3_lvl;
};
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// fff0s Application environment
extern struct app_fff0_env_tag app_fff0_env;

/// Table of message handlers
extern const struct app_subtask_handlers app_fff0_handler;

/*
 * FUNCTIONS DECLARATION 
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * fff0s Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize fff0s Application Module
 ****************************************************************************************
 */
void app_fff0s_init(void);
/**
 ****************************************************************************************
 * @brief Add a fff0 Service instance in the DB
 ****************************************************************************************
 */
void app_fff0_add_fff0s(void);
/**
 ****************************************************************************************
 * @brief Enable the fff0 Service
 ****************************************************************************************
 */
void app_fff4_send_ntf(uint8_t conidx,uint16_t len,uint8_t* buf);
/**
 ****************************************************************************************
 * @brief Send a fff5  value
 ****************************************************************************************
 */
// void app_fff5_send_ind(uint8_t conidx,uint16_t len,uint8_t* buf);


#endif // APP_FFF0_H_
