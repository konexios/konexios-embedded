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
 * File Name    : hw_wdt_private.h
 * Description  : Watchdog Timer (WDT) low level driver API
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup WDT
 * @{
 **********************************************************************************************************************/

#ifndef HW_WDT_PRIVATE_H
#define HW_WDT_PRIVATE_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_wdt.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/
__STATIC_INLINE void     HW_WDT_Refresh (void);

__STATIC_INLINE void     HW_WDT_WDTCRWrite (uint16_t cfg);

__STATIC_INLINE uint16_t HW_WDT_WDTCRRead (void);

__STATIC_INLINE void     HW_WDT_WDTRCRWrite (wdt_reset_control_t reset_request);

__STATIC_INLINE uint8_t  HW_WDT_WDTRCRRead (void);

__STATIC_INLINE void     HW_WDT_WDTCSTPRWrite (wdt_stop_control_t stop_value);

__STATIC_INLINE uint8_t  HW_WDT_WDTCSTPRRead (void);

__STATIC_INLINE uint16_t HW_WDT_WDTSRRead (void);

__STATIC_INLINE void     HW_WDT_WDTSRWrite (uint16_t value);

__STATIC_INLINE void     HW_WDT_InterruptEnable (void);

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "common/hw_wdt_common.h"
#endif ///< HW_WDT_PRIVATE_H

/*******************************************************************************************************************//**
 * @} (end addtogroup WDT)
 **********************************************************************************************************************/
