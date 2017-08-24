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
* File Name    : bsp_api.h
* Description  : BSP API header file. This is the only file you need to include for BSP use.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @defgroup BSP_Interface Board Support Package
 * @brief Common BSP includes.
 *
 * The BSP is responsible for getting the MCU from reset to the user application (i.e. the main function). Before
 * reaching the user application the BSP sets up the stacks, heap, clocks, interrupts, and C
 * runtime environment. The BSP is configurable to allow users to modify the process to meet design requirements.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_API_H
#define BSP_API_H

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/** The BSP configuration determines which board is used. */
#include "bsp_cfg.h"

/*******************************************************************************************************************//**
 * @defgroup BSP_Boards Supported Boards
 * @brief Supported boards in this version of the BSP
 *
 * The BSP provides support for multiple boards. The board BSP that will be built with the project is chosen based upon
 * the bsp_cfg.h file used. You can find reference bsp_cfg.h files for your board inside of each
 * ssp/src/bsp/board/"board"/ref folder.
 *
 * @{
***********************************************************************************************************************/
#if defined(BSP_BOARD_S7G2_PE_HMI1)
#include "../../src/bsp/board/s7g2_pe_hmi1/bsp.h"
#endif

#if defined(BSP_BOARD_ARIS)
#include "../../src/bsp/board/aris/bsp.h"
#endif

#if defined(BSP_BOARD_S7G2_SK)
#include "../../src/bsp/board/s7g2_sk/bsp.h"
#endif

#if defined(BSP_BOARD_S3A7_DK)
#include "../../src/bsp/board/s3a7_dk/bsp.h"
#endif

#if defined(BSP_BOARD_S7G2_USER)
#include "../../src/bsp/board/s7g2_user/bsp.h"
#endif

#if defined(BSP_BOARD_S3A7_USER)
#include "../../src/bsp/board/s3a7_user/bsp.h"
#endif

#if defined(BSP_BOARD_S124_DK)
#include "../../src/bsp/board/s124_dk/bsp.h"
#endif

#if defined(BSP_BOARD_S124_USER)
#include "../../src/bsp/board/s124_user/bsp.h"
#endif


/** @} (end defgroup BSP_Boards) */

/*******************************************************************************************************************//**
 * @defgroup BSP_MCUs Supported MCUs
 * @brief Supported MCUs in this version of the BSP
 *
 * The BSP has code specific to a MCU and a board. The code that is specific to a MCU can be shared amongst
 * boards that use the MCU.
 *
 * @{
***********************************************************************************************************************/
ssp_err_t   R_SSP_VersionGet(ssp_pack_version_t * const p_version);         /// Common to all MCU's

/** @} (end defgroup BSP_MCUs) */

/** @} (end defgroup BSP_Interface) */

#endif /* BSP_API_H */
