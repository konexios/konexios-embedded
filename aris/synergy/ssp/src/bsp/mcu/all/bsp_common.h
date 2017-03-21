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
* File Name    : bsp_common.h
* Description  : Contains common includes, typedefs, and macros used by the BSP.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_Interface
 * @defgroup BSP_MCU_COMMON Common BSP Code
 * @brief Code common to all BSPs.
 *
 * Implements functions that are common to all BSPs.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_COMMON_H_
#define BSP_COMMON_H_

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* C99 includes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
/* Different compiler support. */
#include "../../inc/ssp_common_api.h"
#include "bsp_compiler_support.h"
/* Build time error checking. */
#include "bsp_error_checking.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Used to signify that an ELC event is not able to be used as an interrupt. */
#define BSP_IRQ_DISABLED            (0xFFFFFFFF)

/* Version of this module's code and API. */
#define BSP_CODE_VERSION_MAJOR      (1)
#define BSP_CODE_VERSION_MINOR      (1)
#define BSP_API_VERSION_MAJOR       (1)
#define BSP_API_VERSION_MINOR       (0)

#if (0 == BSP_CFG_RTOS || (defined(__GNUC__) && !defined(__ARM_EABI__)) || (defined(__ICCARM__) && !defined(__ARM_EABI__)))
#define SF_CONTEXT_SAVE
#define SF_CONTEXT_RESTORE
#elif 1 == BSP_CFG_RTOS
#define SF_CONTEXT_SAVE    tx_isr_start(1);
#define SF_CONTEXT_RESTORE tx_isr_end(1);
void  tx_isr_start(unsigned long isr_id);
void  tx_isr_end(unsigned long isr_id);
#endif

/** Function call to insert before returning assertion error. */
#if (1 == BSP_CFG_ASSERT)
#define SSP_ASSERT_FAIL                         SSP_ERROR_LOG(SSP_ERR_ASSERTION, __FILE__, __LINE__);
#else
#define SSP_ASSERT_FAIL
#endif

/** This function is called before returning an error code. To stop on a runtime error, define ssp_error_log in
 * user code and do required debugging (breakpoints, stack dump, etc) in this function.*/
#if (1 == BSP_CFG_ERROR_LOG)
#ifndef SSP_ERROR_LOG
#define SSP_ERROR_LOG(err, module, version)     SSP_PARAMETER_NOT_USED((version));          \
	                                            ssp_error_log((err), (module), __LINE__);
#endif
#else
#define SSP_ERROR_LOG(err, module, version)
#endif

/** Default assertion calls ::SSP_ASSERT_FAIL if condition "a" is false. Used to identify incorrect use of API's in SSP
 * functions. */
#if (2 == BSP_CFG_ASSERT)
#define SSP_ASSERT(a)   assert(a)
#else
#define SSP_ASSERT(a)                 \
    {                                 \
        if ((a))                      \
        {                             \
            (void) 0; /* Do nothing */\
        }                             \
        else                          \
        {                             \
            SSP_ASSERT_FAIL;          \
            return SSP_ERR_ASSERTION; \
        }                             \
    }
#endif // ifndef SSP_ASSERT

/** All SSP error codes are returned using this macro. Calls ::SSP_ERROR_LOG function if condition "a" is false. Used
 * to identify runtime errors in SSP functions. */
#define SSP_ERROR_RETURN(a, err, module, version) \
    {                                             \
        if ((a))                                  \
        {                                         \
            (void) 0; /* Do nothing */            \
        }                                         \
        else                                      \
        {                                         \
            SSP_ERROR_LOG((err), (module), (version));  \
            return (err);                         \
        }                                         \
    }

/** @} (end defgroup BSP_MCU_COMMON) */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/** Different warm start entry locations in the BSP. */
typedef enum e_bsp_warm_start_event
{
    BSP_WARM_START_PRE_C = 0,   ///< Called almost immediately after reset. No C runtime environment, clocks, or IRQs.
    BSP_WARM_START_POST_C       ///< Called after clocks and C runtime environment have been setup
} bsp_warm_start_event_t;


/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
#if ((1 == BSP_CFG_ERROR_LOG) || (1 == BSP_CFG_ASSERT))
/** Prototype of function called before errors are returned in SSP code if BSP_CFG_LOG_ERRORS is set to 1. */
void ssp_error_log(ssp_err_t err, const char * module, int32_t line);
#endif


#endif /* BSP_COMMON_H_ */
