/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * Unless otherwise expressly agreed in writing between Renesas and you: 1) you may not use, copy, modify, distribute,
 * display, or perform the contents; 2) you may not use any name or mark of Renesas for advertising or publicity
 * purposes or in connection with your use of the contents; 3) RENESAS MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE
 * SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND
 * NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR CONSEQUENTIAL DAMAGES,
 * INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF CONTRACT OR TORT, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents included in this file may
 * be subject to different terms.
 **********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_sce.h
* Description  : Secure crypto engine control APIs
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
***********************************************************************************************************************/


#ifndef R_SCE_H
#define R_SCE_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"

#include "r_sce_cfg.h"
#include "r_crypto_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SCE_CODE_VERSION_MAJOR   (01)
#define SCE_CODE_VERSION_MINOR   (00)

#define SCE_API_VERSION_MAJOR    (01)
#define SCE_API_VERSION_MINOR    (00)

#define SCE_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define SCE_MAX(x, y) (((x) > (y)) ? (x) : (y))


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** Crypto API interface on SCE */
/** @cond INC_HEADER_DEFS_SEC */
extern const crypto_api_t g_sce_crypto_api;
/** @endcond  */

/*******************************************************************************************************************//**
 * @addtogroup SCE
 * @{
***********************************************************************************************************************/

/**********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/

extern uint32_t R_SCE_Open(crypto_ctrl_t      * const p_ctrl,
                            crypto_cfg_t const * const p_cfg);

extern uint32_t R_SCE_VersionGet(ssp_version_t * const p_version);

extern uint32_t R_SCE_StatusGet(uint32_t *p_status);

extern uint32_t R_SCE_Close(crypto_ctrl_t * const p_ctrl);

#endif // R_SCE_H

/*******************************************************************************************************************//**
 * @} (end addtogroup SCE)
***********************************************************************************************************************/
