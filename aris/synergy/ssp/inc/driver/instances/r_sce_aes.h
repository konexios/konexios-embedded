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
* File Name    : r_sce_aes.h
* Description  : SCE_AES
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           28.01.2015 SCE_AES  1.00    Initial Release.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @ingroup SCE
 * @addtogroup SCE_AES
 * @{
***********************************************************************************************************************/

#ifndef R_SCE_AES_H
#define R_SCE_AES_H

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/

#include "sf_crypto_err.h"
#include "r_crypto_api.h"
#include "r_aes_api.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SCE_AES_CODE_VERSION_MAJOR   (01)
#define SCE_AES_CODE_VERSION_MINOR   (00)

#define SIZE_GCMTAG_BITS    (128)
#define SIZE_GCMTAG_BYTES   ( (SIZE_GCMTAG_BITS) / 8)
#define SIZE_GCMTAG_WORDS   ( (SIZE_GCMTAG_BITS) / 32)

#define SIZE_AES_BLOCK_BITS  (128)
#define SIZE_AES_BLOCK_BYTES ((SIZE_AES_BLOCK_BITS) / 8)
#define SIZE_AES_BLOCK_WORDS ((SIZE_AES_BLOCK_BITS) / 32)

#define SIZE_AES_128BIT_KEYLEN_BITS  (128)
#define SIZE_AES_128BIT_KEYLEN_BYTES ((SIZE_AES_128BIT_KEYLEN_BITS) / 8)
#define SIZE_AES_128BIT_KEYLEN_WORDS ((SIZE_AES_128BIT_KEYLEN_BITS) / 32)

#define SIZE_AES_192BIT_KEYLEN_BITS  (192)
#define SIZE_AES_192BIT_KEYLEN_BYTES ((SIZE_AES_192BIT_KEYLEN_BITS) / 8)
#define SIZE_AES_192BIT_KEYLEN_WORDS ((SIZE_AES_192BIT_KEYLEN_BITS) / 32)

#define SIZE_AES_256BIT_KEYLEN_BITS  (256)
#define SIZE_AES_256BIT_KEYLEN_BYTES ((SIZE_AES_256BIT_KEYLEN_BITS) / 8)
#define SIZE_AES_256BIT_KEYLEN_WORDS ((SIZE_AES_256BIT_KEYLEN_BITS) / 32)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/

uint32_t R_SCE_AES_Open(aes_ctrl_t * const p_ctrl, aes_cfg_t const * const p_cfg);

uint32_t R_SCE_AES_VersionGet(ssp_version_t * const p_version);

uint32_t R_SCE_AES_Close(aes_ctrl_t * const p_ctrl);

#endif // R_SCE_AES_H

/*******************************************************************************************************************//**
 * @} (end addtogroup SCE_AES)
***********************************************************************************************************************/
