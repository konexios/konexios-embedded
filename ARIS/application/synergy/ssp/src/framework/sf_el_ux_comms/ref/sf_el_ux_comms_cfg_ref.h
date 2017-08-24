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
* File Name    : sf_el_ux_comms_cfg_ref.h
* Description  : UART on SCI configuration
***********************************************************************************************************************/

#ifndef SF_EL_UX_COMMS_CFG_H
#define SF_EL_UX_COMMS_CFG_H

/*******************************************************************************************************************//**
 * @ingroup SF_EL_UX_COMMS
 * @defgroup SF_EL_UX_COMMS_CONFIG Build Time Configurations
 * @{
***********************************************************************************************************************/

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Specify whether to include code for API parameter checking. Valid settings include:
 *    - BSP_CFG_PARAM_CHECKING_ENABLE : Utilizes the system default setting from bsp_cfg.h
 *    - 1 : Includes parameter checking
 *    - 0 : Compiles out parameter checking
 */
#define SF_EL_UX_COMMS_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/** Specify the size of memory to allocate for USBX.  Size of 65536 is recommended for device applications.
 */
#define SF_EL_UX_COMMS_CFG_USB_MEMORY_SIZE_BYTES (65536)

/** Specify the size of memory to allocate for a read input buffer. */
#define SF_EL_UX_COMMS_CFG_BUFFER_MAX_LENGTH     (128)

/*******************************************************************************************************************//**
 * @} (end defgroup SF_EL_UX_COMMS_CONFIG)
***********************************************************************************************************************/

#endif /* SF_EL_UX_COMMS_CFG_H */
