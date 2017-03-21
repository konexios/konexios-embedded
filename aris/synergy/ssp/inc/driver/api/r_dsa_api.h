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
 * File Name    : r_dsa_api.h
 * Description  : DSA_Interface
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           28.01.2015 DSA_Interface  1.00    Initial Release.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Crypto_API
 * @defgroup DSA_API DSA Interface
 *
 * @brief DSA (Digital Signature Algorithm) signature generation and verification APIs
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_DSA_API_H
#define DRV_DSA_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_crypto_api.h"
#include "r_dsa_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DSA_API_VERSION_MAJOR (01)
#define DSA_API_VERSION_MINOR (00)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** DSA Interface control structure */
typedef struct st_dsa_ctrl
{
    crypto_ctrl_t       * p_crypto_ctrl; //!< pointer to crypto engine control structure
    crypto_api_t const  * p_crypto_api;  //!< pointer to crypto engine API
} dsa_ctrl_t;

/** DSA Interface configuration structure. User must fill in these values before invoking the open() function */
typedef struct st_dsa_cfg
{
    crypto_api_t const  * p_crypto_api; //!< pointer to crypto engine api
} dsa_cfg_t;

/** DSA_Interface SCE functions implemented at the HAL layer will follow this API. */
typedef struct st_dsa_api
{
    /** DSA module open function. Must be called before performing any encrypt/decrypt operations.
     *
     * @param[in,out] p_ctrl pointer to control structure for the DSA interface. Must be declared by user.
     *                       Elements are set here.
     * @param[in]     p_cfg  pointer to control structure for the DSA configuration. All elemenets of this structure
     *                       must be set by user.
     *
     */
    uint32_t (* open)(dsa_ctrl_t * const p_ctrl, dsa_cfg_t const * const p_cfg);

    /** @brief DSA signature verification using given DSA public key.
     *
     * Verify DSA signature from buffer `p_signature` using the given DSA public key `p_key` with domain parameters
     * from `p_domain` for the input message hash `p_paddedHash`
     *
     * @param[in] *p_key pointer to the DSA plain-text key.
     * @param[in] *p_domain pointer to DSA domain parameters.
     * @param[in] num_words data buffer size in words. Each word is 4-bytes. multiples of 4
     * @param[in] *p_signature signature data buffer to be verified
     * @param[in] *p_paddedHash padded hash of the input message
     *
     * @deprecated This function is deprecated. The function hashVerify should be used instead.
     */
    uint32_t (* verify)(const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                        uint32_t * p_signature, uint32_t * p_paddedHash);

    /** @brief DSA signature verification using given DSA public key.
     *
     * Verify DSA signature from buffer `p_signature` using the given DSA public key `p_key` with domain parameters
     * from `p_domain` for the input message hash `p_paddedHash`
     *
     * @param[in] *p_ctrl pointer to DSA control structure
     * @param[in] *p_key pointer to the DSA plain-text key.
     * @param[in] *p_domain pointer to DSA domain parameters.
     * @param[in] num_words data buffer size in words. Each word is 4-bytes. multiples of 4
     * @param[in] *p_signature signature data buffer to be verified
     * @param[in] *p_paddedHash padded hash of the input message
     */
    uint32_t (* hashVerify)(dsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                        uint32_t * p_signature, uint32_t * p_paddedHash);

    /** @brief DSA Signature generation using DSA private key.
     *
     * Generate signature for the buffer `p_paddedHash` with the given DSA private key `p_key` for the
     * domain parameters `p_domain`. The result will be written to the buffer `p_dest`
     *
     * @param[in] *p_key DSA plain text private key
     * @param[in] *p_domain pointer to DSA domain parameters.
     * @param[in] num_words data buffer size in words. Each word is 4-bytes. multiples of 4
     * @param[in] *p_paddedHash input data buffer
     * @param[out] *p_dest output data buffer
     *
     * @deprecated This function is deprecated. The function hashSign should be used instead.
     *
     */
    uint32_t (* sign)(const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                      uint32_t * p_paddedHash, uint32_t * p_dest);


    /** @brief DSA Signature generation using DSA private key.
     *
     * Generate signature for the buffer `p_paddedHash` with the given DSA private key `p_key` for the
     * domain parameters `p_domain`. The result will be written to the buffer `p_dest`
     *
     * @param[in] *p_ctrl pointer to control structure
     * @param[in] *p_key DSA plain text private key
     * @param[in] *p_domain pointer to DSA domain parameters.
     * @param[in] num_words data buffer size in words. Each word is 4-bytes. multiples of 4
     * @param[in] *p_paddedHash input data buffer
     * @param[out] *p_dest output data buffer
     */
    uint32_t (* hashSign)(dsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                      uint32_t * p_paddedHash, uint32_t * p_dest);

    /** Close the DSA module.
     *  @param[in] p_ctrl pointer to the control structure
     */
    uint32_t (* close)(dsa_ctrl_t * const p_ctrl);

    /** Gets version and stores it in provided pointer p_version.
     * @param[out]    p_version  Code and API version used.
     */
    uint32_t (* versionGet)(ssp_version_t * const p_version);
} dsa_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_dsa_instance {
    dsa_ctrl_t   * p_ctrl ;         ///< Pointer to the control structure for this instance
    dsa_cfg_t    const * p_cfg  ;   ///< Pointer to the configuration structure for this instance
    dsa_api_t    const * p_api  ;   ///< Pointer to the API structure for this instance
} dsa_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern const crypto_api_t g_sce_crypto_api;

extern const dsa_api_t    g_dsa1024_160_on_sce;
extern const dsa_api_t    g_dsa2048_224_on_sce;
extern const dsa_api_t    g_dsa2048_256_on_sce;

#endif /* DRV_DSA_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup DSA_API)
 **********************************************************************************************************************/
