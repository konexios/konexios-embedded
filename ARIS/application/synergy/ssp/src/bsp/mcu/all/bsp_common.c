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
* File Name    : bsp_common.c
* Description  : Implements functions common to all BSPs. Example: VersionGet() function.
***********************************************************************************************************************/


/***********************************************************************************************************************
 *
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#if defined(__ICCARM__)
#define WEAK_ERROR_ATTRIBUTE
#pragma weak ssp_error_log                            = ssp_error_log_internal
#elif defined(__GNUC__)
#define WEAK_ERROR_ATTRIBUTE       __attribute__ ((weak, alias("ssp_error_log_internal")))
#endif

#define SSP_SECTION_VERSION ".version"

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/
#if ((1 == BSP_CFG_ERROR_LOG) || (1 == BSP_CFG_ASSERT))
/** Prototype of function called before errors are returned in SSP code if BSP_CFG_LOG_ERRORS is set to 1.  This
 * prototype sets the weak association of this function to an internal example implementation. */
void ssp_error_log(ssp_err_t err, const char * module, int32_t line) WEAK_ERROR_ATTRIBUTE;
#endif

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
/** SSP pack version structure. */
BSP_DONT_REMOVE const ssp_pack_version_t g_ssp_version BSP_PLACE_IN_SECTION(SSP_SECTION_VERSION) =
{
    .major = SSP_VERSION_MAJOR,
    .minor = SSP_VERSION_MINOR,
    .build = SSP_VERSION_BUILD,
    .patch = SSP_VERSION_PATCH
};

/** Public SSP version name. */
BSP_DONT_REMOVE const uint8_t g_ssp_version_string[] BSP_PLACE_IN_SECTION(SSP_SECTION_VERSION) =
        SSP_VERSION_STRING;

/** Unique SSP version ID. */
BSP_DONT_REMOVE const uint8_t g_ssp_version_build_string[] BSP_PLACE_IN_SECTION(SSP_SECTION_VERSION) =
        SSP_VERSION_BUILD_STRING;

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_COMMON
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Set BSP version based on compile time macros.
 *
 * @param[out] p_version Memory address to return version information to.
 *
 * @retval SSP_SUCCESS Version information stored.
 **********************************************************************************************************************/
ssp_err_t R_BSP_VersionGet (ssp_version_t * p_version)
{
    p_version->api_version_major  = BSP_API_VERSION_MAJOR;
    p_version->api_version_minor  = BSP_API_VERSION_MINOR;
    p_version->code_version_major = BSP_CODE_VERSION_MAJOR;
    p_version->code_version_minor = BSP_CODE_VERSION_MINOR;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Set SSP version based on compile time macros.
 *
 * @param[out] p_version Memory address to return version information to.
 *
 * @retval SSP_SUCCESS Version information stored.
 **********************************************************************************************************************/
ssp_err_t R_SSP_VersionGet (ssp_pack_version_t * const p_version)
{
    *p_version = g_ssp_version;

    return SSP_SUCCESS;
}

#if ((1 == BSP_CFG_ERROR_LOG) || (1 == BSP_CFG_ASSERT))
/*******************************************************************************************************************//**
 * @brief      Default error logger function, used only if ssp_error_log is not defined in the user application.
 *
 * @param[in]  err     The error code encountered.
 * @param[in]  module  The module name in which the error code was encountered.
 * @param[in]  line    The line number at which the error code was encountered.
 **********************************************************************************************************************/
void ssp_error_log_internal(ssp_err_t err, const char * module, int32_t line)
{
    /** Do nothing. */
}
#endif

/** @} (end addtogroup BSP_MCU_COMMON) */
