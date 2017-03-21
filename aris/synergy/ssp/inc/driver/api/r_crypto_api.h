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
 * File Name    : r_crypto_api.h
 * Description  : Crypto_Interface
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           Crypto_Interface  1.00    Initial Release.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup Crypto_API Crypto Interface
 *
 * @brief Cryptographic algorithm APIs for encryption/decryption, signing/verification, and hashing.
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_CRYPTO_API_H
#define DRV_CRYPTO_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define CRYPTO_API_VERSION_MAJOR (01)
#define CRYPTO_API_VERSION_MINOR (00)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Crypto_Interface Add API definitions required by user here. */
typedef struct st_crypto_ctrl
{
    uint32_t  state; //!< indicates state of the SCE/SCE-Lite driver e.g whether it is initialized
    uint32_t  cb_data;
	void (*p_sce_long_plg_start_callback)(void);
	void (*p_sce_long_plg_end_callback)(void);
} crypto_ctrl_t;

/** Crypto engine configuration parameters */
typedef struct st_crypto_cfg
{
	void (*p_sce_long_plg_start_callback)(void);

    /** Callback provided when a ISR occurs.  Set to NULL for no CPU interrupt. */
	void (*p_sce_long_plg_end_callback)(void);
} crypto_cfg_t;

/** Crypto_Interface SCE functions implemented at the HAL layer will follow this API. */
typedef struct st_crypto_api
{
    /** Open crypto module using the given configuration
     * @param[in,out] p_ctrl pointer to control structure. Must be declared by user. Elements set here.
     * @param[in]     p_cfg  pointer to configuration structure. All elements of this structure must be set by user
     */
    uint32_t (* open)(crypto_ctrl_t * const p_ctrl, crypto_cfg_t const * const p_cfg);

    /** Close the crypto interface module for the given control structure p_ctrl
     * @param[in] p_ctrl pointer to control structure
     */
    uint32_t (* close)(crypto_ctrl_t * const p_ctrl);

    /** Get status of SCE initialization
     * @param[out] p_status initialization status of SCE module will be written to the memory pointed to by p_status
     */
    uint32_t (* statusGet)(uint32_t * p_status);

    /** Gets version and stores it in provided pointer p_version.
     * @param[out]    p_version  Code and API version used.
     */
    uint32_t (* versionGet)(ssp_version_t * const p_version);
} crypto_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_crypto_instance {
    crypto_ctrl_t   * p_ctrl ;         ///< Pointer to the control structure for this instance
    crypto_cfg_t    const * p_cfg  ;   ///< Pointer to the configuration structure for this instance
    crypto_api_t    const * p_api  ;   ///< Pointer to the API structure for this instance
} crypto_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure. */
extern const crypto_api_t g_sce_crypto_api;
/** @endcond */

#endif /* DRV_CRYPTO_API_H */

/*******************************************************************************************************************//**
 * @} (end defgroup Crypto_API)
 **********************************************************************************************************************/
