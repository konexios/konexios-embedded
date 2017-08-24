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
* File Name    : r_sce_hash.h
* Description  : SCE_SHA1
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           SCE_SHA1  1.00    Initial Release.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
***********************************************************************************************************************/

#ifndef R_SCE_SHA1_H
#define R_SCE_SHA1_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "r_crypto_api.h"
#include "r_hash_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SCE_SHA1_CODE_VERSION_MAJOR   (01)
#define SCE_SHA1_CODE_VERSION_MINOR   (00)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** SCE_SHA1 Definitions specific to this driver (typically extensions to an interface). */

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/
/** SCE_SHA1 example: extern const r_sce_t g_<interface>_on_sce; */
/** @cond INC_HEADER_DEFS_SEC */
extern const crypto_api_t g_sce_crypto_api;
extern const hash_api_t   g_sha1_hash_on_sce;
/** @endcond */

/*******************************************************************************************************************//**
 * @ingroup SCE
 * @addtogroup SCE_HASH
 * @{
***********************************************************************************************************************/

/**********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/

extern uint32_t R_SCE_SHA1_Open(hash_ctrl_t * const p_ctrl, hash_cfg_t const * const p_cfg);

extern uint32_t R_SCE_SHA1_VersionGet(ssp_version_t * const p_version);

extern uint32_t  R_SCE_SHA1_UpdateHash(const uint32_t *p_msg, uint32_t imaxcnt, uint32_t *p_digest);

extern uint32_t R_SCE_SHA1_HashUpdate(hash_ctrl_t * const p_ctrl, const uint32_t * p_msg, uint32_t num_words, uint32_t * p_digest);

extern uint32_t R_SCE_SHA1_Close(hash_ctrl_t * const p_ctrl);

#endif // R_SCE_SHA1_H

/*******************************************************************************************************************//**
 * @} (end addtogroup SCE_SHA1)
***********************************************************************************************************************/
