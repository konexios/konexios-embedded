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
 * File Name    : hw_qspi_private.h
 * Description  : Quad Serial Peripheral Interface (QSPI) Module hardware private header file.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup QSPI
 * @{
 **********************************************************************************************************************/

#ifndef HW_QSPI_PRIVATE_H
#define HW_QSPI_PRIVATE_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_qspi.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_QSPI_PowerOn (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_QSPI_PowerOff (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_QSPI_BANK_SELECT (uint32_t bank) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_QSPI_DIRECT_COMMUNICATION_ENTER (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_QSPI_DIRECT_COMMUNICATION_ENTER (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t HW_QSPI_BYTE_READ (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_QSPI_BYTE_WRITE (uint8_t byte) INLINE_ATTRIBUTE;

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Select implementation based on BSP here */
#include "common/hw_qspi_common.h"
#endif /* HW_QSPI_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup QSPI)
 **********************************************************************************************************************/
