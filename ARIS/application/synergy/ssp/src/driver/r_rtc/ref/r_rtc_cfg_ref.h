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
* File Name    : r_rtc_cfg_ref.h
* Description  : Real-Time Clock (RTC) Module configuration header file.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup RTC
 * @defgroup RTC_CFG Build Time Configurations
 * @{
 **********************************************************************************************************************/

#ifndef R_RTC_CFG_H
#define R_RTC_CFG_H

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/** SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/** Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/** Setting to 1 includes parameter checking; 0 compiles out parameter checking */
#define RTC_CFG_PARAM_CHECKING_ENABLE   (1)

/* SPECIFY IF WANT TO CALCULATE DAY OF YEAR FOR R_RTC_Read(); 1-yes, 0=no (compiles out code) */
#define RTC_CFG_CALCULATE_YDAY          (1)

#endif /* R_RTC_CFG_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RTC_CONFIG)
***********************************************************************************************************************/
