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
 * File Name    : r_cgc_cfg_ref.h
 * Description  : Configuration reference file for the CGC module.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup CGC
 * @defgroup CGC_CONFIG Build Time Configurations
 * @{
 **********************************************************************************************************************/

#ifndef R_CGC_CFG_H
#define R_CGC_CFG_H



/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Configuration Options
 **********************************************************************************************************************/
/** SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */

/** Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting.
 * Setting to 1 includes parameter checking; 0 compiles out parameter checking.
 */
#define CGC_CFG_PARAM_CHECKING_ENABLE (1)

/**
 *  Set CGC_CFG_MAIN_OSC_CLOCK_SOURCE to 0 if a resonator, or crystal, is used.
 *  Set CGC_CFG_MAIN_OSC_CLOCK_SOURCE to 1 if an external oscillator input is used.
 */
#define CGC_CFG_MAIN_OSC_CLOCK_SOURCE (0)

/**
 * Set CGC_CFG_MAIN_OSC_WAIT to one of the values below. It should be at least as long
 * as the main clock stabilization time.
 * This delay will be configured only if CGC_CFG_MAIN_OSC_CLOCK_SOURCE is set to 0 to
 * indicate that a resonator/ crystal is used.
 *
 * Set the main clock oscillation stabilization time to longer than or equal to the
 * stabilization time recommended by the oscillator manufacturer.
 *
 * - 0 0 0 0 0: Wait time = 3 cycles
 * - 0 0 0 0 1: Wait time = 35 cycles
 * - 0 0 0 1 0: Wait time = 67 cycles
 * - 0 0 0 1 1: Wait time = 131 cycles
 * - 0 0 1 0 0: Wait time = 259 cycles
 * - 0 0 1 0 1: Wait time = 547 cycles
 * - 0 0 1 1 0: Wait time = 1059 cycles
 * - 0 0 1 1 1: Wait time = 2147 cycles
 * - 0 1 0 0 0: Wait time = 4291 cycles
 * - 0 1 0 0 1: Wait time = 8163 cycles
 *
 *  Default is set to maximum time.
 */
#define CGC_CFG_MAIN_OSC_WAIT (0x09)

/**
 * Set CGC_CFG_SUBCLOCK_DRIVE to one of the values below
 * based on the crystal parameters.
 * - 0: Standard (12.5 pf)
 * - 1: Middle (4.4 pf)
 */

#define CGC_CFG_SUBCLOCK_DRIVE (0x00)
#endif /* R_CGC_CFG_H */

/*******************************************************************************************************************//**
 * @} (end defgroup CGC_CONFIG)
 **********************************************************************************************************************/
