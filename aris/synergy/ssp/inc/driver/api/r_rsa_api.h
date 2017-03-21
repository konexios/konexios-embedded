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
 * File Name    : r_rsa_api.h
 * Description  : RSA_Interface
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           29.01.2015 RSA_Interface  1.00    Initial Release.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Crypto_API
 * @defgroup RSA_API RSA Interface
 *
 * @brief RSA cryptographic functions for signature generation, verification, encryption and decryption
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_RSA_API_H
#define DRV_RSA_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_crypto_api.h"
#include "r_rsa_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RSA_API_VERSION_MAJOR (01)
#define RSA_API_VERSION_MINOR (00)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** RSA Interface control structure */
typedef struct st_rsa_ctrl
{
    crypto_ctrl_t       * p_crypto_ctrl;    //!< pointer to crypto engine control structure
    crypto_api_t const  * p_crypto_api;     //!< pointer to crypto engine API
    uint32_t              stage_num;        //!< processing stage
} rsa_ctrl_t;

/** RSA Interface configuration structure. User must fill in these values before invoking the open() function */
typedef struct st_rsa_cfg
{
    crypto_api_t const  * p_crypto_api; //!< pointer to crypto engine api
} rsa_cfg_t;

/** RSA_Interface SCE functions implemented at the HAL layer will follow this API. */
typedef struct st_rsa_api
{
    /** RSA module open function. Must be called before performing any encrypt/decrypt or
     *  sign/verify operations.
     *
     * @param[in,out] p_ctrl pointer to control structure for the RSA interface. Must be declared by user.
     *                       Elements are set here.
     * @param[in]     p_cfg  pointer to control structure for the RSA configuration. All elemenets of this structure
     *                       must be set by user.
     *
     */
    uint32_t (* open)(rsa_ctrl_t * const p_ctrl, rsa_cfg_t const * const p_cfg);

    /** @brief Encrypt source data from `p_source` using an RSA public key from `p_key` and write the results
     *  to destination buffer `p_dest`.
     *
     * @param[in]  *p_ctrl     pointer to control structure for RSA interface
     * @param[in]  *p_key      pointer to the RSA plain-text public key.
     * @param[in]  *p_domain   unused parameter for RSA encryption.
     * @param[in]   num_words  data buffer size in words. Each word is 4-bytes.
     * @param[in]  *p_source   source data buffer to be encrypted.
     * @param[out] *p_dest     destination data buffer, encryption result will be stored here.
     */
    uint32_t (* encrypt)(rsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                         uint32_t * p_source, uint32_t * p_dest);

    /** @brief Decrypt source data from `p_source` using an RSA private key from `p_key` and write the results
     *  to destination buffer `p_dest`.
     *
     * @param[in]  *p_ctrl     pointer to control structure for RSA interface
     * @param[in]  *p_key      pointer to RSA plain-text private key.
     * @param[in]  *p_domain   unused parameter for RSA encryption.
     * @param[in]  num_words   data buffer size in words. Each word is 4-bytes.
     * @param[in]  *p_source   input data buffer to be decrypted.
     * @param[out] *p_dest     output destination data buffer, decryption result will be stored here.
     */
    uint32_t (* decrypt)(rsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                         uint32_t * p_source, uint32_t * p_dest);

    /** @brief Decrypt source data from `p_source` using an RSA private key from `p_key` and write the results
     *  to destination buffer `p_dest`. RSA private key data is specified in CRT format.
     *
     * @param[in]  *p_key      pointer to RSA plain-text private key in CRT format.
     * @param[in]  *p_domain   unused parameter for RSA encryption.
     * @param[in]   num_words  data buffer size in words. Each word is 4-bytes.
     * @param[in]  *p_source   input data buffer to be decrypted.
     * @param[out] *p_dest     output destination data buffer, decryption result will be stored here.
     */
    uint32_t (* decryptCrt)(rsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                            uint32_t * p_source, uint32_t * p_dest);

    /** @brief Verify signature given in buffer `p_signature` using the RSA public key `p_key` for the given
     *  padded message hash from buffer `p_padded_hash`.
     *
     * @param[in] *p_key        pointer to the RSA plain-text public key.
     * @param[in] *p_domain     this parameter is not required for the RSA encryption functionality
     * @param[in] num_words     data buffer size in words. Each word is 4-bytes.
     * @param[in] *p_signature  signature data that needs to be verified
     * @param[in] *p_paddedHash padded hash value of the input message buffer
     */
    uint32_t (* verify)(rsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                        uint32_t * p_signature, uint32_t * p_padded_hash);

    /** @brief Generate signature for the given padded hash buffer `p_padded_hash` using the RSA private key
     *  `p_key`. Write the results to the buffer `p_dest`.
     *
     * @param[in] *p_ctrl           pointer to control structure for RSA interface
     * @param[in]  *p_key           pointer to RSA private key
     * @param[in]  *p_domain        unused parameter
     * @param[in]  num_words        data buffer size in words. Each word is 4-bytes. multiples of 4
     * @param[in]  *p_padded_hash   padded hash for the input message for which an RSA signature is desired
     * @param[out] *p_dest          generated signature data will be written here.
     */
    uint32_t (* sign)(rsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                      uint32_t * p_padded_hash, uint32_t * p_dest);

    /** @brief Generate signature for the given padded hash buffer `p_padded_hash` using the RSA private key
     *  `p_key`. RSA private key `p_key` is assumed to be in CRT format. Write the results to the buffer `p_dest`.
     *
     * @param[in] *p_ctrl           pointer to control structure for RSA interface
     * @param[in]  *p_key           pointer to RSA private key
     * @param[in]  *p_domain        unused parameter
     * @param[in]  num_words        data buffer size in words. Each word is 4-bytes. multiples of 4
     * @param[in]  *p_padded_hash   padded hash for the input message for which an RSA signature is desired
     * @param[out] *p_dest          generated signature data will be written here.
     */
    uint32_t (* signCrt)(rsa_ctrl_t * const p_ctrl, const uint32_t * p_key, const uint32_t * p_domain, uint32_t num_words,
                         uint32_t * p_padded_hash, uint32_t * p_dest);

    /** Close the RSA module.
     *  @param[in] p_ctrl pointer to the control structure
     */
    uint32_t (* close)(rsa_ctrl_t * const p_ctrl);

    /** Gets version and stores it in provided pointer p_version.
     * @param[out]    p_version  Code and API version used.
     */
    uint32_t (* versionGet)(ssp_version_t * const p_version);
} rsa_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_rsa_instance {
    rsa_ctrl_t   * p_ctrl ;         ///< Pointer to the control structure for this instance
    rsa_cfg_t    const * p_cfg  ;   ///< Pointer to the configuration structure for this instance
    rsa_api_t    const * p_api  ;   ///< Pointer to the API structure for this instance
} rsa_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern const crypto_api_t g_sce_crypto_api;

extern const rsa_api_t    g_rsa1024_on_sce;
extern const rsa_api_t    g_rsa2048_on_sce;

#endif /* DRV_RSA_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RSA_API)
 **********************************************************************************************************************/
