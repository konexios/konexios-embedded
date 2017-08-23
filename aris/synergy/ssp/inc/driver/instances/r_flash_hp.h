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
* File Name    : r_flash_hp.h
* Description  : HLD Interface for the High-Performance Flash peripheral on SC32 MCUs.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#ifndef R_FLASH_HP_H
#define R_FLASH_HP_H

#include "bsp_api.h"

/* This will generate a build error if this file is included and the target MCU used is NOT one of the following. */
#if defined(BSP_MCU_GROUP_S7G2)

#include "r_flash_api.h"
#include "r_flash_cfg.h"
/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup FLASH_HP High-performance Flash
 * @brief Driver for the High-performance Flash Memory (S7G2 only).
 *
 * This module supports the Flash interface for the High Performance FLASH peripheral.
 * @{
 **********************************************************************************************************************/


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define FLASH_HP_CODE_VERSION_MAJOR   (1)
#define FLASH_HP_CODE_VERSION_MINOR   (1)

/* If Code Flash programming is enabled, then all API functions must execute out of RAM. */
#if (FLASH_CFG_PARAM_CODE_FLASH_PROGRAMMING_ENABLE == 1)
#if defined(__ICCARM__)
#pragma section=".code_in_ram"
#endif
#define PLACE_IN_RAM_SECTION        BSP_PLACE_IN_SECTION(".code_in_ram")
#else
#define PLACE_IN_RAM_SECTION
#endif


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const flash_api_t g_flash_on_flash_hp;
/** @endcond */



/*******************************************************************************************************************//**
 * @} (end defgroup FLASH_HP)
***********************************************************************************************************************/
#else
#error "r_flash_hp is not a supported module for this board type."
#endif

#endif // R_FLASH_HP_H

