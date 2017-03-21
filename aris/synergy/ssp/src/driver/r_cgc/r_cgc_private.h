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
 * File Name    : r_cgc_private.h
 * Description  : Private header file for the CGC module.
 **********************************************************************************************************************/

#ifndef R_CGC_PRIVATE_H
#define R_CGC_PRIVATE_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_clock_cfg.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if BSP_MCU_MOCKED
#define BSP_MCU_GROUP_S7G2
#endif
/** Timeout to wait for register to be written */
#define MAX_REGISTER_WAIT_COUNT (0xFFFF)
/** Number of subclock cycles to wait after subcclock has been stopped */
#define SUBCLOCK_DELAY          (5)

/** Divisor to use to obtain time for 1 tick in us from iclk */
#define RELOAD_COUNT_FOR_1US   1000000

#ifdef BSP_MCU_GROUP_S7G2
/** The main oscillator drive value is based upon the oscillator frequency selected in the configuration */
#if (BSP_CFG_XTAL_HZ > (19999999))
#define CGC_MAINCLOCK_DRIVE (0x00)
#elif (BSP_CFG_XTAL_HZ > (15999999)) && (BSP_CFG_XTAL_HZ < (20000000))
#define CGC_MAINCLOCK_DRIVE (0x01)
#elif (BSP_CFG_XTAL_HZ > (7999999)) && (BSP_CFG_XTAL_HZ < (16000000))
#define CGC_MAINCLOCK_DRIVE (0x02)
#else
#define CGC_MAINCLOCK_DRIVE (0x03)
#endif
#endif

#if (defined BSP_MCU_GROUP_S3A7 || defined BSP_MCU_GROUP_S124 || defined BSP_MCU_GROUP_S128)
/** The main oscillator drive value is based upon the oscillator frequency selected in the configuration */
#if (BSP_CFG_XTAL_HZ > (9999999))
#define CGC_MAINCLOCK_DRIVE (0x00)
#else
#define CGC_MAINCLOCK_DRIVE (0x01)
#endif
#endif



/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#endif /* R_CGC_PRIVATE_H */
