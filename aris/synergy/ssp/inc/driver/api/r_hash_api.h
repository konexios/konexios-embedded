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
 * File Name    : r_hash_api.h
 * Description  : HASH_Interface
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           HASH_Interface  1.00    Initial Release.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Crypto_API
 * @defgroup HASH_API HASH Algorithm Interface
 *
 * @brief HASH_Interface Hash algorithm APIs
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_HASH_API_H
#define DRV_HASH_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_crypto_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define HASH_API_VERSION_MAJOR (01)
#define HASH_API_VERSION_MINOR (00)

#define HASH_MESSAGE_BLOCK_SIZE_WORDS (16)
#define HASH_MESSAGE_BLOCK_SIZE_BYTES ((HASH_MESSAGE_BLOCK_SIZE_WORDS) * 4)

#define HASH_MAX_DIGEST_SIZE_WORDS        (8)
#define HASH_MAX_DIGEST_SIZE_BYTES        ((HASH_DIGEST_SIZE_WORDS) * 4)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** HASH_Interface configuration structure  */
typedef struct st_hash_cfg
{
    crypto_ctrl_t        * p_crypto_ctrl;      ///< pointer to crypto engine control structure
    crypto_api_t   const * p_crypto_api;       ///< pointer to crypto engine API structure
} hash_cfg_t;

/** HASH_Interface control structure */
typedef struct st_hash_ctrl
{
    uint32_t  msgbuf[HASH_MESSAGE_BLOCK_SIZE_WORDS];     ///< message buffer to be hashed
    uint32_t  hash[HASH_MAX_DIGEST_SIZE_WORDS];          ///< current hash value
    uint64_t  length;                                    ///< 64-bit message length (number of bits)
} hash_ctrl_t;

/** HASH_Interface SCE functions implemented at the HAL layer will follow this API. */
typedef struct st_hash_api
{
    /** HASH_Interface: Initial configuration
     * @pre HASH_Interface: Peripheral clocks and any required output pins should be configured prior
     *                                     to calling this function.
     * @param[in,out] HASH_Interface: p_ctrl   Pointer to control structure. Must be declared by user. Elements set here.
     * @param[in]     HASH_Interface: p_cfg    Pointer to configuration structure. All elements of this
     *structure
     *                                               must be set by user.
     */
    uint32_t (* open)(hash_ctrl_t * const p_ctrl, hash_cfg_t const * const p_cfg);

    /** update hash for the `num_words` words from source buffer `p_source`.
     *
     *  @param[in]     p_source pointer to input message buffer. size must be a multiple of 64-bytes
     *  @param[in]     num_words number of words to be hashed from the buffer `p_source`
     *  @param[in,out] p_dest pointer to the message digest. on input contains initialization value.
     *
     *  @deprecated: This function will be deprecated. The function hashUpdate() should be used instead.
     */
    uint32_t (* updateHash)(const uint32_t * p_source, uint32_t num_words, uint32_t * p_dest);

    /** update hash for the `num_words` words from source buffer `p_source`.
     *
     *  @param[in]     p_ctrl  pointer to hash_ctrl_t control structure.
     *  @param[in]     p_source pointer to input message buffer. size must be a multiple of 64-bytes
     *  @param[in]     num_words number of words to be hashed from the buffer `p_source`
     *  @param[in,out] p_dest pointer to the message digest. on input contains initialization value.
     */
    uint32_t (* hashUpdate)(hash_ctrl_t * const p_ctrl, const uint32_t * p_source, uint32_t num_words, uint32_t * p_dest);

    uint32_t (* close)(hash_ctrl_t * const p_ctrl);

    /** Gets version and stores it in provided pointer p_version.
     * @param[out]    p_version  Code and API version used.
     */
    uint32_t (* versionGet)(ssp_version_t * const p_version);
} hash_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_hash_instance {
    hash_ctrl_t   * p_ctrl ;         ///< Pointer to the control structure for this instance
    hash_cfg_t    const * p_cfg  ;   ///< Pointer to the configuration structure for this instance
    hash_api_t    const * p_api  ;   ///< Pointer to the API structure for this instance
} hash_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

extern const hash_api_t g_sha1_hash_on_sce;
extern const hash_api_t g_sha256_hash_on_sce;

/*******************************************************************************************************************//**
 * @} (end addtogroup HASH_Interface)
 **********************************************************************************************************************/
#endif /* DRV_HASH_API_H */
