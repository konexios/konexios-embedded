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
 * File Name    : r_qspi.h
 * Description  : QSPI HAL module header file
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup QSPI QSPI
 * @brief Driver for the Quad Serial Peripheral Interface (QSPI).
 *
 * This is a driver for the Quad-SPI module (QSPI) which is a memory controller for connecting a serial ROM (non-volatile
 * memory such as a serial flash memory, serial EEPROM, or serial FeRAM) that has an SPI-compatible interface.
 *
 * @section QSPI_SUMMARY Summary
 * Extends @ref QSPI_API.
 * @{
 **********************************************************************************************************************/

#ifndef R_QSPI_H
#define R_QSPI_H

#if defined(__GNUC__) && defined(__ARM_EABI__)
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include <string.h>
#include "r_qspi_cfg.h"
#include "r_qspi_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define QSPI_CODE_VERSION_MAJOR (1)
#define QSPI_CODE_VERSION_MINOR (1)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const qspi_api_t g_qspi_on_qspi;
/** @endcond */

#endif /* R_QSPI_H */

/*******************************************************************************************************************//**
 * @} (end defgroup QSPI)
 **********************************************************************************************************************/
