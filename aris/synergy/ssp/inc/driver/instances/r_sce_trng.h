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
* File Name    : r_sce_trng.h
* Description  : SCE_TRNG
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           28.01.2015 SCE_TRNG  1.00    Initial Release.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @ingroup SCE
 * @addtogroup SCE_TRNG
 * @{
***********************************************************************************************************************/

#ifndef R_SCE_TRNG_H
#define R_SCE_TRNG_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "r_crypto_api.h"
#include "r_trng_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SCE_TRNG_CODE_VERSION_MAJOR   (01)
#define SCE_TRNG_CODE_VERSION_MINOR   (00)


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** SCE_TRNG example: extern const r_sce_t g_<interface>_on_sce; */
/** @cond INC_HEADER_DEFS_SEC */
extern const crypto_api_t g_sce_crypto_api;
extern const trng_api_t g_trng_on_sce;
/** @endcond */

/**********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/

extern uint32_t R_SCE_TRNG_Open(trng_ctrl_t * const p_ctrl, trng_cfg_t const * const p_cfg);

extern uint32_t R_SCE_TRNG_VersionGet(ssp_version_t * const p_version);

extern uint32_t R_SCE_TRNG_Read(trng_ctrl_t * const p_ctrl, uint32_t * const p_dest, uint32_t nwords);

extern uint32_t R_SCE_TRNG_Close(trng_ctrl_t * const p_ctrl);

#endif // R_SCE_TRNG_H

/*******************************************************************************************************************//**
 * @} (end addtogroup SCE_TRNG)
***********************************************************************************************************************/
