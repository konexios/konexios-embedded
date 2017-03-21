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

/***********************************************************************************************************************
 * File Name     : r_rspi_cfg_ref.h
 * Description   : Configures the RSPI drivers
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @ingroup RSPI
 * @{
 **********************************************************************************************************************/

#ifndef R_RSPI_CFG_H
#define R_RSPI_CFG_H

/***********************************************************************************************************************
 * Configuration Options
 **********************************************************************************************************************/

/* Checking of arguments passed to RSPI API functions can be enable or disabled.
 * Disabling argument checking is provided for systems that absolutely require faster and smaller code.
 * By default the module is configured to use the setting of the system-wide BSP_CFG_PARAM_CHECKING_ENABLE macro.
 * This can be overridden for the local module by redefining RSPI_CFG_PARAM_CHECKING_ENABLE.
 * To control parameter checking locally, set RSPI_CFG_PARAM_CHECKING_ENABLE to 1 to enable it,
 * otherwise set to 0 skip checking.
 */
/* Fixed data that is transmitted during receive-only operations. Change as needed. */
#define RSPI_DUMMY_TXDATA (0xFFFFFFFF)

/* Enable the RSPI channels to use in this build. (0) = not used. (1) = used. */
#define RSPI_CFG_USE_CH0 (1)
#define RSPI_CFG_USE_CH1 (0)

/* Set interrupt priority levels for each channel present.
 * Priority is shared by all interrupt sources in a channel.
 * Values must be in the range 0 (interrupt disabled) to 15 (highest)*/
#define RSPI_IR_PRIORITY_CHAN0 (3)
#define RSPI_IR_PRIORITY_CHAN1 (3)

/** Specify whether to include code for API parameter checking. Valid settings include:
 *    - BSP_CFG_PARAM_CHECKING_ENABLE : Utilizes the system default setting from bsp_cfg.h
 *    - 1 : Includes parameter checking
 *    - 0 : Compiles out parameter checking
 */
#define RSPI_CFG_PARAM_CHECKING_ENABLE (1)
#endif /* R_RSPI_CFG_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RSPI)
 **********************************************************************************************************************/
