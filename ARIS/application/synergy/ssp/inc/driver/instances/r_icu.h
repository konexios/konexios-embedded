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
* File Name    : r_icu.h
* Description  : Prototypes of ICU functions used to implement external interrupt interfaces.
***********************************************************************************************************************/

#ifndef R_ICU_H
#define R_ICU_H

/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup ICU ICU
 * @brief Driver for the Interrupt Controller Unit (ICU) External pin interrupts function.
 *
 * @section ICU_SUMMARY Summary
 * Extends @ref EXTERNAL_IRQ_API.
 *
 * This module implements the @ref EXTERNAL_IRQ_API using the external input pins in the Interrupt Controller Unit (ICU).
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"
#include "r_external_irq_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define ICU_CODE_VERSION_MAJOR   (1)
#define ICU_CODE_VERSION_MINOR   (1)

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const external_irq_api_t g_external_irq_on_icu;
/** @endcond */

/*******************************************************************************************************************//**
 * @} (end defgroup ICU)
 **********************************************************************************************************************/

#endif // R_ICU_H
