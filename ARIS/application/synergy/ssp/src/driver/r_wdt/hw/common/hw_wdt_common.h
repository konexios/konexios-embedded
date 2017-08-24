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
 * File Name    : hw_wdt_common.h
 * Description  : WDT HAL low-level functions to interface with hardware registers.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup WDT
 * @{
 **********************************************************************************************************************/
#ifndef HW_WDT_COMMON_H
#define HW_WDT_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Writes H'00 then H'FF to WDTRR register
 *
 **********************************************************************************************************************/
__STATIC_INLINE void HW_WDT_Refresh (void)
{
    R_WDT->WDTRR = 0x00;
    R_WDT->WDTRR = 0xFF;
}

/*******************************************************************************************************************//**
 * Writes configuration data to WDTCR.
 * @param[in] config   Value to write to WDTCR
 **********************************************************************************************************************/
__STATIC_INLINE void HW_WDT_WDTCRWrite (uint16_t cfg)
{
    R_WDT->WDTCR = cfg;
}

/*******************************************************************************************************************//**
 * Reads configuration from WDTCR.
 * @param[out] WDTCR value
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_WDT_WDTCRRead (void)
{
    return R_WDT->WDTCR;
}

/*******************************************************************************************************************//**
 * Writes reset request configuration value to WDTRCR.
 * @param[in] reset_request   Value to write to WDTRCR
 **********************************************************************************************************************/
__STATIC_INLINE void HW_WDT_WDTRCRWrite (wdt_reset_control_t reset_request)
{
    R_WDT->WDTRCR = reset_request;
}

/*******************************************************************************************************************//**
 * Reads reset mode from WDTRCR.
 * @param[out] WDTRCR value
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_WDT_WDTRCRRead (void)
{
    return R_WDT->WDTRCR;
}

/*******************************************************************************************************************//**
 * Writes stop control configuration value to WDTCSTPR.
 * @param[in] stop_value   Value to write to WDTCSTPR
 **********************************************************************************************************************/
__STATIC_INLINE void HW_WDT_WDTCSTPRWrite (wdt_stop_control_t stop_value)
{
    R_WDT->WDTCSTPR = stop_value;
}

/*******************************************************************************************************************//**
 * Reads stop mode from WDTCSTPR.
 * @param[out] WDTCSTPR value
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_WDT_WDTCSTPRRead (void)
{
    return R_WDT->WDTCSTPR;
}

/*******************************************************************************************************************//**
 * Reads WDTSR register.
 * @retval Value of WDTSR (WDT StatusRegister)
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_WDT_WDTSRRead (void)
{
    return R_WDT->WDTSR;
}

/*******************************************************************************************************************//**
 * Writes to WDTSR register.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_WDT_WDTSRWrite (uint16_t value)
{
    R_WDT->WDTSR = value;
}

/*******************************************************************************************************************//**
 * Enables WDT underflow/refresh error interrupt. This interrupt will generate an NMI.
 **********************************************************************************************************************/
__STATIC_INLINE void HW_WDT_InterruptEnable (void)
{
    R_ICU->NMIER_b.WDTEN = 1;
}
#endif ///< HW_WDT_COMMON_H

/*******************************************************************************************************************//**
 * @} (end addtogroup WDT)
 **********************************************************************************************************************/
