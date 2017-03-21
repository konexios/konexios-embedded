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
 * File Name    : r_wdt_private.h
 * Description  : Watchdog Timer (WDT) private macros.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup WDT
 * @{
 **********************************************************************************************************************/

#ifndef R_WDT_PRIVATE_H
#define R_WDT_PRIVATE_H

#include "hw/hw_wdt_private.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define WDT_PRV_OSF0_AUTO_START_MASK     (0x00020000L)
#define WDT_PRV_OSF0_NMI_REQUEST_MASK    (0x10000000L)
#define WDT_PRV_WDTSR_COUNTER_MASK       (0x00003FFF)
#define WDT_PRV_WDTSR_FLAGS_MASK       	 (0x0000C000)
#define WDT_PRV_WDTCR_TIMEOUT_MASK       (0x0003)
#define WDT_PRV_WDTCR_CLOCK_DIVIDER_MASK (0x00F0)
#define WDT_PRV_WDTCR_WINDOW_END_MASK    (0x0300)
#define WDT_PRV_WDTCR_WINDOW_START_MASK  (0x3000)
#define WDT_PRV_OFS0_TIMEOUT_MASK        (0x000C0000L)
#define WDT_PRV_OFS0_CLOCK_DIVIDER_MASK  (0x00F00000L)
#define WDT_PRV_OFS0_WINDOW_END_MASK     (0x03000000L)
#define WDT_PRV_OFS0_WINDOW_START_MASK   (0x0C000000L)
#define WDT_PRV_OFS0_RESET_CONTROL_MASK  (0x00000080L)
#define WDT_PRV_OFS0_STOP_CONTROL_MASK   (0x00000080L)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#endif ///< R_WDT_PRIVATE_H

/*******************************************************************************************************************//**
 * @} (end addtogroup WDT)
 **********************************************************************************************************************/
