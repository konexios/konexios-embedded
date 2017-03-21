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
 * File Name    : r_icu.c
 * Description  : ICU functions used to implement various interrupt interfaces.
 **********************************************************************************************************************/



/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_icu.h"
#include "r_icu_cfg.h"
#include "hw/hw_icu_private.h"
#include "r_icu_private_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Macro for error logger. */
#ifndef ICU_ERROR_RETURN
#define ICU_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_icu_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Array of callback functions and contexts */
static external_irq_ctrl_t * gp_ctrls[ICU_BUTTON_MAX_NUM_CHANNELS] =
{
    NULL
};

#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t g_icu_version =
{
    .api_version_minor  = EXTERNAL_IRQ_API_VERSION_MINOR,
    .api_version_major  = EXTERNAL_IRQ_API_VERSION_MAJOR,
    .code_version_major = ICU_CODE_VERSION_MAJOR,
    .code_version_minor = ICU_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/** Name of module used by error logger macro */
static const char g_module_name[] = "icu";

/**********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/* ICU BUTTON Driver  */
const external_irq_api_t g_external_irq_on_icu =
{
    .open          = R_ICU_ExternalIrqOpen,
    .enable        = R_ICU_ExternalIrqEnable,
    .disable       = R_ICU_ExternalIrqDisable,
    .triggerSet    = R_ICU_ExternalIrqTriggerSet,
    .filterEnable  = R_ICU_ExternalIrqFilterEnable,
    .filterDisable = R_ICU_ExternalIrqFilterDisable,
    .close         = R_ICU_ExternalIrqClose,
    .versionGet    = R_ICU_ExternalIrqVersionGet
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @addtogroup ICU
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Configure an external input pin for use with the button interface.  Implements external_irq_api_t::open.
 *
 * The Open function is responsible for preparing an external input pin for operation. After completion
 * of the Open function the external input pin shall be enabled and ready to service interrupts. This
 * function must be called once prior to calling any other external input pin API functions. Once
 * successfully completed, the status of the selected external input pin will be set to "open". After
 * that this function should not be called again for the same external input pin without first
 * performing a "close" by calling R_ICU_ExternalIrqClose().
 *
 * @retval SSP_SUCCESS            Command successfully completed.
 * @retval SSP_ERR_ASSERTION      One of the following is invalid:
 *                                  - p_ctrl or p_cfg is NULL
 *                                  - The channel requested in p_cfg is not available on the device selected in
 *                                    r_bsp_cfg.h.
 * @retval SSP_ERR_INVALID_ARGUMENT   p_cfg->p_callback is not NULL, but ISR is not enabled. ISR must be enabled to
 *                                    use callback function.  Enable channel's overflow ISR in bsp_irq_cfg.h.
 * @retval SSP_ERR_IN_USE         The channel specified has already been opened. No configurations were changed. Call
 *                                the associated Close function to reconfigure the channel.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqOpen (external_irq_ctrl_t       * const p_ctrl,
                                 external_irq_cfg_t const * const  p_cfg)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT((1 << p_cfg->channel) & ICU_BUTTON_AVAILABLE_CHANNELS);
#endif

    SSP_PARAMETER_NOT_USED(g_module_name);

    /** Verify channel is not already used */
    /* All ICU channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    bsp_hw_lock_t lock    = (bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_ICU_IRQ0 + p_cfg->channel);
    ssp_err_t     bsp_err = R_BSP_HardwareLock(lock);
    ICU_ERROR_RETURN((SSP_SUCCESS == bsp_err), SSP_ERR_IN_USE);
    
    if (p_cfg->p_callback)
    {
        /** Store control block for use by ISR */
        gp_ctrls[p_cfg->channel] = p_ctrl;
        p_ctrl->p_callback       = p_cfg->p_callback;
        p_ctrl->p_context        = p_cfg->p_context;

        /** Lookup IRQ number */
        IRQn_Type irq = HW_ICU_IRQGet((uint8_t)p_cfg->channel);
        if (BSP_MAX_NUM_IRQn == irq)
        {
            /* IRQ is not enabled */
            R_BSP_HardwareUnlock(lock);
            ICU_ERROR_RETURN(false, SSP_ERR_INVALID_ARGUMENT);
        }

        /** Enable interrupts */
        if (true == p_cfg->autostart)
        {
            R_BSP_IrqStatusClear(irq);
            NVIC_ClearPendingIRQ(irq);
            NVIC_EnableIRQ(irq);
        }
    }
    else
    {
        /* Store control block because it is used by other functions to know if control block is open. */
        gp_ctrls[p_cfg->channel] = p_ctrl;
    }

    /** Initialize control block. */
    p_ctrl->channel    = p_cfg->channel;
    p_ctrl->p_callback = p_cfg->p_callback;
    p_ctrl->p_context  = p_cfg->p_context;

    HW_ICU_FilterDisable((uint8_t)p_ctrl->channel);
    HW_ICU_DivisorSet((uint8_t)p_ctrl->channel, p_cfg->pclk_div);
    HW_ICU_TriggerSet((uint8_t)p_ctrl->channel, p_cfg->trigger);

    if (p_cfg->filter_enable)
    {
        HW_ICU_FilterEnable((uint8_t)p_ctrl->channel);
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable external interrupt for specified channel at NVIC. Implements external_irq_api_t::enable.
 *
 * @retval SSP_SUCCESS         Interrupt disabled successfully.
 * @retval SSP_ERR_ASSERTION   The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN    The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqEnable (external_irq_ctrl_t * const p_ctrl)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    /* Check that control block pointer is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    ICU_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    IRQn_Type irq = HW_ICU_IRQGet((uint8_t)p_ctrl->channel);
    ICU_ERROR_RETURN(BSP_MAX_NUM_IRQn != irq, SSP_ERR_INTERNAL);

    NVIC_EnableIRQ(irq);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable external interrupt for specified channel at NVIC. Implements external_irq_api_t::disable.
 *
 * @retval SSP_SUCCESS         Interrupt disabled successfully.
 * @retval SSP_ERR_ASSERTION   The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN    The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqDisable (external_irq_ctrl_t * const p_ctrl)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    /* Check that control block pointer is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    ICU_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    IRQn_Type irq = HW_ICU_IRQGet((uint8_t)p_ctrl->channel);
    ICU_ERROR_RETURN(BSP_MAX_NUM_IRQn != irq, SSP_ERR_INTERNAL);

    NVIC_DisableIRQ(irq);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set trigger value provided. Implements external_irq_api_t::triggerSet.
 *
 * @retval SSP_SUCCESS         Period value written successfully.
 * @retval SSP_ERR_ASSERTION   The p_ctrl or p_period parameter was null.
 * @retval SSP_ERR_NOT_OPEN    The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqTriggerSet (external_irq_ctrl_t * const p_ctrl,
                                       external_irq_trigger_t      hw_trigger)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    /* Check that control block pointer is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    ICU_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    HW_ICU_TriggerSet((uint8_t)p_ctrl->channel, hw_trigger);
    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable external interrupt digital filter for specified channel. Implements external_irq_api_t::filterEnable.
 *
 * @retval SSP_SUCCESS         Interrupt disabled successfully.
 * @retval SSP_ERR_ASSERTION   The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN    The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqFilterEnable (external_irq_ctrl_t * const p_ctrl)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    /* Check that control block pointer is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    ICU_ERROR_RETURN(gp_ctrls[(uint8_t)p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    HW_ICU_FilterEnable((uint8_t)p_ctrl->channel);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable external interrupt digital filter for specified channel. Implements external_irq_api_t::filterDisable.
 *
 * @retval SSP_SUCCESS         Interrupt disabled successfully.
 * @retval SSP_ERR_ASSERTION   The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN    The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqFilterDisable (external_irq_ctrl_t * const p_ctrl)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    /* Check that control block pointer is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    ICU_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    HW_ICU_FilterDisable((uint8_t)p_ctrl->channel);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Set driver version based on compile time macros.  Implements external_irq_api_t::versionGet.
 *
 * @retval     SSP_SUCCESS        Successful close.
 * @retval     SSP_ERR_ASSERTION  The parameter p_version is NULL.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqVersionGet (ssp_version_t * const p_version)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id  = g_icu_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Disable external interrupt. Implements external_irq_api_t::close.
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_ASSERTION  The parameter p_ctrl is NULL.
 * @retval     SSP_ERR_NOT_OPEN  The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_ICU_ExternalIrqClose (external_irq_ctrl_t * const p_ctrl)
{
#if ICU_BUTTON_CFG_PARAM_CHECKING_ENABLE
    /* Check that control block pointer is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    ICU_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Cleanup. Disable interrupt */
    IRQn_Type irq = HW_ICU_IRQGet((uint8_t)p_ctrl->channel);
    if (BSP_MAX_NUM_IRQn != irq)
    {
        NVIC_DisableIRQ(irq);
    }

    /** Clear stored internal driver data */
    gp_ctrls[p_ctrl->channel]->p_callback = NULL;
    gp_ctrls[p_ctrl->channel]->p_context  = 0;
    gp_ctrls[p_ctrl->channel]             = NULL;

    /** Release BSP hardware lock */
    /* All ICU channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    bsp_hw_lock_t lock    = (bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_ICU_IRQ0 + p_ctrl->channel);
    R_BSP_HardwareUnlock(lock);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ICU)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Channel 0 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ0) && (BSP_IRQ_CFG_ICU_IRQ0 != BSP_IRQ_DISABLED))
void icu_irq0_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ0_IRQn);

    if (NULL != gp_ctrls[0]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 0;
        args.p_context = gp_ctrls[0]->p_context;
        gp_ctrls[0]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ0 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 1 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ1) && (BSP_IRQ_CFG_ICU_IRQ1 != BSP_IRQ_DISABLED))
void icu_irq1_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ1_IRQn);

    if (NULL != gp_ctrls[1]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 1;
        args.p_context = gp_ctrls[1]->p_context;
        gp_ctrls[1]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ1 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 2 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ2) && (BSP_IRQ_CFG_ICU_IRQ2 != BSP_IRQ_DISABLED))
void icu_irq2_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ2_IRQn);

    if (NULL != gp_ctrls[2]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 2;
        args.p_context = gp_ctrls[2]->p_context;
        gp_ctrls[2]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ2 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 3 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ3) && (BSP_IRQ_CFG_ICU_IRQ3 != BSP_IRQ_DISABLED))
void icu_irq3_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ3_IRQn);

    if (NULL != gp_ctrls[3]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 3;
        args.p_context = gp_ctrls[3]->p_context;
        gp_ctrls[3]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ3 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 4 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ4) && (BSP_IRQ_CFG_ICU_IRQ4 != BSP_IRQ_DISABLED))
void icu_irq4_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ4_IRQn);

    if (NULL != gp_ctrls[4]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 4;
        args.p_context = gp_ctrls[4]->p_context;
        gp_ctrls[4]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ4 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 5 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ5) && (BSP_IRQ_CFG_ICU_IRQ5 != BSP_IRQ_DISABLED))
void icu_irq5_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ5_IRQn);

    if (NULL != gp_ctrls[5]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 5;
        args.p_context = gp_ctrls[5]->p_context;
        gp_ctrls[5]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ5 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 6 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ6) && (BSP_IRQ_CFG_ICU_IRQ6 != BSP_IRQ_DISABLED))
void icu_irq6_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ6_IRQn);

    if (NULL != gp_ctrls[6]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 6;
        args.p_context = gp_ctrls[6]->p_context;
        gp_ctrls[6]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ6 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 7 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ7) && (BSP_IRQ_CFG_ICU_IRQ7 != BSP_IRQ_DISABLED))
void icu_irq7_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ7_IRQn);

    if (NULL != gp_ctrls[7]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 7;
        args.p_context = gp_ctrls[7]->p_context;
        gp_ctrls[7]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ7 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 8 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ8) && (BSP_IRQ_CFG_ICU_IRQ8 != BSP_IRQ_DISABLED))
void icu_irq8_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ8_IRQn);

    if (NULL != gp_ctrls[8]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 8;
        args.p_context = gp_ctrls[8]->p_context;
        gp_ctrls[8]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ8 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 9 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ9) && (BSP_IRQ_CFG_ICU_IRQ9 != BSP_IRQ_DISABLED))
void icu_irq9_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ9_IRQn);

    if (NULL != gp_ctrls[9]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 9;
        args.p_context = gp_ctrls[9]->p_context;
        gp_ctrls[9]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ9 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 10 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ10) && (BSP_IRQ_CFG_ICU_IRQ10 != BSP_IRQ_DISABLED))
void icu_irq10_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ10_IRQn);

    if (NULL != gp_ctrls[10]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 10;
        args.p_context = gp_ctrls[10]->p_context;
        gp_ctrls[10]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ10 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 11 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ11) && (BSP_IRQ_CFG_ICU_IRQ11 != BSP_IRQ_DISABLED))
void icu_irq11_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ11_IRQn);

    if (NULL != gp_ctrls[11]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 11;
        args.p_context = gp_ctrls[11]->p_context;
        gp_ctrls[11]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ11 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 12 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ12) && (BSP_IRQ_CFG_ICU_IRQ12 != BSP_IRQ_DISABLED))
void icu_irq12_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ12_IRQn);

    if (NULL != gp_ctrls[12]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 12;
        args.p_context = gp_ctrls[12]->p_context;
        gp_ctrls[12]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ12 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 13 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ13) && (BSP_IRQ_CFG_ICU_IRQ13 != BSP_IRQ_DISABLED))
void icu_irq13_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ13_IRQn);

    if (NULL != gp_ctrls[13]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 13;
        args.p_context = gp_ctrls[13]->p_context;
        gp_ctrls[13]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ13 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 14 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ14) && (BSP_IRQ_CFG_ICU_IRQ14 != BSP_IRQ_DISABLED))
void icu_irq14_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ14_IRQn);

    if (NULL != gp_ctrls[14]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 14;
        args.p_context = gp_ctrls[14]->p_context;
        gp_ctrls[14]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ14 != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 15 ISR.
 *
 * Saves context if RTOS is used, clears interrupts, calls callback if one was provided in the open function,
 * and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (defined(BSP_IRQ_CFG_ICU_IRQ15) && (BSP_IRQ_CFG_ICU_IRQ15 != BSP_IRQ_DISABLED))
void icu_irq15_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    R_BSP_IrqStatusClear (ICU_IRQ15_IRQn);

    if (NULL != gp_ctrls[15]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;
        args.channel   = 15;
        args.p_context = gp_ctrls[15]->p_context;
        gp_ctrls[15]->p_callback(&args);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
}
#endif /* if (BSP_IRQ_CFG_ICU_IRQ15 != BSP_IRQ_DISABLED) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/


