/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : sf_el_ux_comms.h
 * Description  : UART on USBX driver
 **********************************************************************************************************************/

#ifndef SF_EL_UX_COMMS_H
#define SF_EL_UX_COMMS_H

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_EL_UX_COMMS USB Communication Framework
 * @brief RTOS-integrated USBX CDC ACM device implementation
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_comms_api.h"
#include "sf_el_ux_comms_cfg.h"
#include "ux_api.h"
#include "ux_device_class_cdc_acm.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_EL_UX_COMMS_CODE_VERSION_MAJOR (1)
#define SF_EL_UX_COMMS_CODE_VERSION_MINOR (2)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** USBX CDC ACM device communications driver configuration */
typedef struct st_sf_el_ux_comms_on_comms_ctrl
{
    uint32_t  open;
    TX_MUTEX  mutex[2];
    UX_SLAVE_CLASS_CDC_ACM * p_cdc;
    uint32_t  leftover_length;
    uint32_t  index;
    uint8_t   memory[SF_EL_UX_COMMS_CFG_USB_MEMORY_SIZE_BYTES];
    uint8_t   rx_memory[SF_EL_UX_COMMS_CFG_BUFFER_MAX_LENGTH];
} sf_el_ux_comms_on_comms_ctrl_t;

/** USBX CDC ACM device communications driver configuration */
typedef struct st_sf_el_ux_comms_on_comms_cfg
{
    sf_el_ux_comms_on_comms_ctrl_t * p_ctrl; ///< Memory allocated for USBX CDC ACM device control block
} sf_el_ux_comms_on_comms_cfg_t;

/**********************************************************************************************************************
 *
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const sf_comms_api_t g_sf_el_ux_comms_on_sf_comms;
/** @endcond */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_EL_UX_COMMS )
 **********************************************************************************************************************/

#endif // SF_EL_UX_COMMS_H
