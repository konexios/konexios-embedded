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
* File Name    : bsp_clocks.h
* Description  : Calls the CGC module to setup the system clocks. Settings for clocks are based on macros in
*                bsp_clock_cfg.h.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_S7G2
 * @defgroup BSP_MCU_CLOCKS Clock Initialization
 *
 * Functions in this file  configure the system clocks based upon the macros in bsp_clock_cfg.h.
 *
 * @{
 **********************************************************************************************************************/

/** @} (end degroup BSP_MCU_CLOCKS) */

#ifndef BSP_CLOCKS_H_
#define BSP_CLOCKS_H_

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
/* Public functions defined in bsp.h */
void     bsp_clock_init(void);      // Used internally by BSP
uint32_t bsp_cpu_clock_get(void);   // Used internally by BSP

#endif /* BSP_CLOCKS_H_ */


