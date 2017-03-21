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
 * File Name    : r_trng_api.h
 * Description  : TRNG_Interface
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           TRNG_Interface  1.00    Initial Release.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Crypto_API
 * @defgroup RNG_API Random number generation
 *
 * @brief RNG_Interface Random number generation
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_TRNG_API_H
#define DRV_TRNG_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/** Register definitions, common services and error codes. */
#include "bsp_api.h"
#include "r_crypto_api.h"
#include "r_trng_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define TRNG_API_VERSION_MAJOR (01)
#define TRNG_API_VERSION_MINOR (00)

#define TRNG_REGISTER_SIZE_WORDS (4)
#define TRNG_REGISTER_SIZE_BYTES ((TRNG_REGISTER_SIZE_WORDS) * 4)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** TRNG_Interface control structure. */
typedef struct st_trng_ctrl
{
    uint32_t            nattempts;                          //!< number of retries
    crypto_ctrl_t       * p_crypto_ctrl;                    //!< pointer to crypto control structure
    crypto_api_t const  * p_crypto_api;                     //!< pointer to crypto-engine API
    uint32_t            prevbuf[TRNG_REGISTER_SIZE_WORDS];  //!< previous random data
    uint32_t            currbuf[TRNG_REGISTER_SIZE_WORDS];  //!< current random data
} trng_ctrl_t;

/** TRNG interface configuration parameters */
typedef struct st_trng_cfg
{
    crypto_api_t const  * p_crypto_api;  //!< pointer to crypto API
    uint32_t            nattempts;       //!< number of retries when a continuous test failure occurs
} trng_cfg_t;

/** TRNG_Interface SCE functions implemented at the HAL layer will follow this API. */
typedef struct st_trng_api
{
    /** Open the TRNG driver for reading random data from the hardware TRNG module
     * @param[in,out] p_ctrl   Pointer to control structure. Must be declared by user. Elements set here.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of this structure
     *                         must be set by user.
     */
    uint32_t (* open)(trng_ctrl_t * const p_ctrl, trng_cfg_t const * const p_cfg);

    /** Generate `nbytes` of random number bytes and store them in `p_rngbuf` buffer
     *  @param[in]   p_ctrl pointer to trng control structure
     *  @param[out]  p_rngbuf generated random numbers will be stored to the buffer `p_rngbuf`
     *  @param[in]   nbytes number of random bytes to generate
     */
    uint32_t (* read)(trng_ctrl_t * const p_ctrl, uint32_t * const p_rngbuf, uint32_t nwords);

    /** Close the TRNG interface driver
     * @param[in] p_ctrl pointer to trng interface control structure
     */
    uint32_t (* close)(trng_ctrl_t * const p_ctrl);

    /** Gets version and stores it in provided pointer p_version.
     * @param[out]    p_version  Code and API version used.
     */
    uint32_t (* versionGet)(ssp_version_t * const p_version);
} trng_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_trng_instance {
    trng_ctrl_t   * p_ctrl ;         ///< Pointer to the control structure for this instance
    trng_cfg_t    const * p_cfg  ;   ///< Pointer to the configuration structure for this instance
    trng_api_t    const * p_api  ;   ///< Pointer to the API structure for this instance
} trng_instance_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const trng_api_t g_trng_on_sce;
/** @endcond */

#endif /* DRV_TRNG_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RNG_API)
 **********************************************************************************************************************/
