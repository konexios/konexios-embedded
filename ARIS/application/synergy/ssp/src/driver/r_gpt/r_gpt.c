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
 * File Name    : r_gpt.c
 * Description  : GPT functions used to implement various timer interfaces.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_gpt.h"
#include "r_gpt_cfg.h"
#include "hw/hw_gpt_private.h"
#include "r_gpt_private_api.h"
#include "r_cgc.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Maximum number of clock counts in 32 bit timer */
#define GPT_MAX_CLOCK_COUNTS_32 (0xFFFFFFFFULL)
#define GPT_MAX_CLOCK_COUNTS_16 (0xFFFFUL)

/** Macro for error logger. */
#ifndef GPT_ERROR_RETURN
#define GPT_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), "gpt", &g_gpt_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void gpt_hardware_initialize (uint8_t                   channel,
                                     timer_cfg_t const * const p_cfg,
                                     gpt_pclk_div_t            pclk_divisor,
                                     timer_size_t              pclk_counts,
                                     timer_size_t              duty_cycle);

static ssp_err_t period_to_pclk (uint8_t const channel, timer_size_t const period, timer_unit_t const unit, timer_size_t * const p_period_pclk,
                                 gpt_pclk_div_t * const p_divisor);
static ssp_err_t duty_cycle_to_pclk (uint8_t const channel, timer_size_t const duty_cycle, timer_pwm_unit_t const unit,
        timer_size_t const period, timer_size_t * const p_duty_cycle_pclk);

static ssp_err_t   gpt_set_period (uint8_t const channel, timer_size_t const period, timer_unit_t const unit);

static void gpt_set_duty_cycle (uint8_t const ch, timer_size_t const duty_cycle, uint8_t const pin);

static ssp_err_t gpt_common_open (timer_ctrl_t * const      p_ctrl, timer_cfg_t const * const p_cfg,
        gpt_pclk_div_t * const p_pclk_divisor, timer_size_t * const p_pclk_counts);

static void gpt_set_output_pin (uint8_t const ch, gpt_gtioc_t const gtio, gpt_pin_level_t const level,
                                timer_mode_t mode, timer_size_t duty_cycle);

static uint32_t gpt_clock_frequency_get(uint8_t const channel);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
void gpt0_counter_overflow_isr (void);
void gpt1_counter_overflow_isr (void);
void gpt2_counter_overflow_isr (void);
void gpt3_counter_overflow_isr (void);
void gpt4_counter_overflow_isr (void);
void gpt5_counter_overflow_isr (void);
void gpt6_counter_overflow_isr (void);
void gpt7_counter_overflow_isr (void);
void gpt8_counter_overflow_isr (void);
void gpt9_counter_overflow_isr (void);
void gpt10_counter_overflow_isr (void);
void gpt11_counter_overflow_isr (void);
void gpt12_counter_overflow_isr (void);
void gpt13_counter_overflow_isr (void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/** Bit vector of channels in one-shot mode */
static uint32_t     g_channel_one_shot = 0;

/** Array of callback functions and contexts */
static timer_ctrl_t * gp_ctrls[GPT_MAX_NUM_CHANNELS] =
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
static const ssp_version_t g_gpt_version =
{
    .api_version_minor  = TIMER_API_VERSION_MINOR,
    .api_version_major  = TIMER_API_VERSION_MAJOR,
    .code_version_major = GPT_CODE_VERSION_MAJOR,
    .code_version_minor = GPT_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** GPT Implementation of General Timer Driver  */
const timer_api_t g_timer_on_gpt =
{
    .open            = R_GPT_TimerOpen,
    .stop            = R_GPT_Stop,
    .start           = R_GPT_Start,
    .reset           = R_GPT_Reset,
    .periodSet       = R_GPT_PeriodSet,
    .counterGet      = R_GPT_CounterGet,
    .dutyCycleSet    = R_GPT_DutyCycleSet,
    .infoGet         = R_GPT_InfoGet,
    .close           = R_GPT_Close,
    .versionGet      = R_GPT_VersionGet
};

/*******************************************************************************************************************//**
 * @addtogroup GPT
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Powers on GPT, handles required initialization described in hardware manual. Implements timer_api_t::open.
 *
 * The Open function configures a single GPT channel, starts the channel, and provides a handle for use with the GPT API
 * Control and Close functions. This function must be called once prior to calling any other GPT API functions. After a
 * channel is opened, the Open function should not be called again for the same channel without first calling the
 * associated Close function.
 *
 * GPT hardware does not support one-shot functionality natively.  When using one-shot mode, the timer will be stopped
 * in an ISR after the requested period has elapsed.
 *
 * The GPT implementation of the general timer can accept a ::timer_on_gpt_cfg_t extension parameter.
 *
 * @retval SSP_SUCCESS           Initialization was successful and timer has started.
 * @retval SSP_ERR_ASSERTION     One of the following parameters is incorrect.  Either
 *                                 - p_cfg is NULL, OR
 *                                 - p_ctrl is NULL, OR
 *                                 - The channel requested in the p_cfg parameter is not available on the device
 *                                   selected in r_bsp_cfg.h.
 * @retval SSP_ERR_INVALID_ARGUMENT  One of the following parameters is invalid:
 *                                 - p_cfg->period: must be in the following range:
 *                                     - Lower bound: (1 / (PCLK frequency)
 *                                     - Upper bound: (0xFFFFFFFF * 1024 / (PCLK frequency))
 *                                 - p_cfg->p_callback not NULL, but ISR is not enabled. ISR must be enabled to
 *                                   use callback function.  Enable channel's overflow ISR in bsp_irq_cfg.h.
 *                                 - p_cfg->mode is ::TIMER_MODE_ONE_SHOT, but ISR is not enabled.  ISR must be
 *                                   enabled to use one-shot mode.  Enable channel's overflow ISR in bsp_irq_cfg.h.
 * @retval SSP_ERR_IN_USE        The channel specified has already been opened. No configurations were changed. Call
 *                               the associated Close function or use associated Control commands to reconfigure the
 *                               channel.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 **********************************************************************************************************************/
ssp_err_t R_GPT_TimerOpen  (timer_ctrl_t * const      p_ctrl,
                            timer_cfg_t const * const p_cfg)
{
    /** Calculate period and store internal variables */
    gpt_pclk_div_t pclk_divisor = GPT_PCLK_DIV_BY_1;
    timer_size_t pclk_counts = 0;
    ssp_err_t err = gpt_common_open(p_ctrl, p_cfg, &pclk_divisor, &pclk_counts);
    GPT_ERROR_RETURN((SSP_SUCCESS == err), err);

    /** Power on GPT before setting any hardware registers. Make sure the counter is stopped before setting mode
       register, PCLK divisor register, and counter register. */
    HW_GPT_PowerOn(p_ctrl->channel);
    HW_GPT_CounterStartStop(p_ctrl->channel, GPT_STOP);
    HW_GPT_RegisterInit(p_ctrl->channel);

    /** Calculate duty cycle */
    timer_size_t duty_cycle = 0;
    if (TIMER_MODE_PWM == p_cfg->mode)
    {
        err = duty_cycle_to_pclk(p_ctrl->channel, p_cfg->duty_cycle, p_cfg->duty_cycle_unit, pclk_counts, &duty_cycle);
        GPT_ERROR_RETURN((SSP_SUCCESS == err), err);
    }

    gpt_hardware_initialize(p_ctrl->channel, p_cfg, pclk_divisor, pclk_counts, duty_cycle);

    return SSP_SUCCESS;
} /* End of function R_GPT_TimerOpen */

/*******************************************************************************************************************//**
 * @brief  Stops timer. Implements timer_api_t::stop.
 *
 * @retval SSP_SUCCESS           Timer successfully stopped.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_Stop (timer_ctrl_t * const p_ctrl)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    GPT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Stop timer */
    HW_GPT_CounterStartStop(p_ctrl->channel, GPT_STOP);
    return SSP_SUCCESS;
} /* End of function R_GPT_Stop */

/*******************************************************************************************************************//**
 * @brief  Starts timer. Implements timer_api_t::start.
 *
 * @retval SSP_SUCCESS           Timer successfully started.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_Start (timer_ctrl_t * const p_ctrl)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    GPT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Start timer */
    HW_GPT_CounterStartStop(p_ctrl->channel, GPT_START);
    return SSP_SUCCESS;
} /* End of function R_GPT_Start */

/*******************************************************************************************************************//**
 * @brief  Sets counter value in provided p_value pointer. Implements timer_api_t::counterGet.
 *
 * @retval SSP_SUCCESS           Counter value read, p_value is valid.
 * @retval SSP_ERR_ASSERTION     The p_ctrl or p_value parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_CounterGet (timer_ctrl_t * const p_ctrl,
                            timer_size_t * const p_value)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure parameters are valid */
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_value);
    GPT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Read counter value */
    *p_value = HW_GPT_CounterGet(p_ctrl->channel);
    return SSP_SUCCESS;
} /* End of function R_GPT_CounterGet */

/*******************************************************************************************************************//**
 * @brief  Resets the counter value to 0. Implements timer_api_t::reset.
 *
 * @retval SSP_SUCCESS           Counter value written successfully.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_Reset (timer_ctrl_t * const p_ctrl)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    GPT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Write the counter value */
    __disable_irq();
    gpt_start_status_t status = HW_GPT_CounterStartBitGet(p_ctrl->channel);
    HW_GPT_CounterStartStop(p_ctrl->channel, GPT_STOP);
    HW_GPT_CounterSet(p_ctrl->channel, 0);
    HW_GPT_CounterStartStop(p_ctrl->channel, status);
    __enable_irq();
    return SSP_SUCCESS;
} /* End of function R_GPT_Reset */

/*******************************************************************************************************************//**
 * @brief  Sets period value provided. Implements timer_api_t::periodSet.
 *
 * @retval SSP_SUCCESS           Period value written successfully.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter was null.
 * @retval SSP_ERR_INVALID_ARGUMENT   One of the following is invalid:
 *                                    - p_period->unit: must be one of the options from timer_unit_t
 *                                    - p_period->value: must result in a period in the following range:
 *                                        - Lower bound: (1 / (PCLK frequency))
 *                                        - Upper bound: (0xFFFFFFFF * 1024 / (PCLK frequency))
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_PeriodSet (timer_ctrl_t * const p_ctrl,
                           timer_size_t   const period,
                           timer_unit_t const   unit)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    GPT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Make sure period is valid, then set period */
    ssp_err_t err = gpt_set_period(p_ctrl->channel, period, unit);
    GPT_ERROR_RETURN(SSP_SUCCESS == err, err);
    return SSP_SUCCESS;
} /* End of function R_GPT_PeriodSet */

/*******************************************************************************************************************//**
 * @brief  Sets status in provided p_status pointer. Implements pwm_api_t::dutyCycleSet.
 *
 * @retval SSP_SUCCESS           Counter value written successfully.
 * @retval SSP_ERR_ASSERTION     The p_ctrl parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_DutyCycleSet (timer_ctrl_t   * const p_ctrl,
                              timer_size_t     const duty_cycle,
                              timer_pwm_unit_t const unit,
                              uint8_t          const pin)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure handle is valid. */
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(p_ctrl->channel < 14);
#endif

    /** Converted duty cycle to PCLK counts before it can be set in registers */
    timer_size_t duty_cycle_counts = 0;
    ssp_err_t err = duty_cycle_to_pclk(p_ctrl->channel, duty_cycle, unit, HW_GPT_TimerCycleGet(p_ctrl->channel) + 1, &duty_cycle_counts);
    GPT_ERROR_RETURN((SSP_SUCCESS == err), err);

    /** Set duty cycle. */
    gpt_set_duty_cycle(p_ctrl->channel, duty_cycle_counts, pin);
    return SSP_SUCCESS;
} /* End of function R_GPT_DutyCycleSet */

/*******************************************************************************************************************//**
 * @brief  Get timer information and store it in provided pointer p_info.
 * Implements timer_api_t::infoGet.
 *
 * @retval SSP_SUCCESS           Period, count direction, frequency, and status value written to caller's
 *                               structure successfully.
 * @retval SSP_ERR_ASSERTION     The p_ctrl or p_info parameter was null.
 * @retval SSP_ERR_NOT_OPEN      The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_InfoGet (timer_ctrl_t * const p_ctrl, timer_info_t * const p_info)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure parameters are valid. */
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_info);
    GPT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Get and store period */
    p_info->period_counts = HW_GPT_TimerCycleGet(p_ctrl->channel) + 1;

    /** Get and store clock frequency */
    p_info->clock_frequency = gpt_clock_frequency_get(p_ctrl->channel);

    /** Get and store clock counting direction */
    p_info->count_direction = HW_GPT_DirectionGet(p_ctrl->channel);

    bool status = HW_GPT_CounterStartBitGet(p_ctrl->channel);
    if (status)
    {
        p_info->status = TIMER_STATUS_COUNTING;
    }
    else
    {
        p_info->status = TIMER_STATUS_STOPPED;
    }

    return SSP_SUCCESS;
} /* End of function R_GPT_InfoGet */

/*******************************************************************************************************************//**
 * @brief      Stops counter, disables interrupts, disables output pins, and clears internal driver data.
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_ASSERTION    The parameter p_ctrl is NULL.
 * @retval     SSP_ERR_NOT_OPEN     The channel is not opened.
 **********************************************************************************************************************/
ssp_err_t R_GPT_Close (timer_ctrl_t * const p_ctrl)
{
    ssp_err_t err = SSP_SUCCESS;

#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Make sure channel is open.  If not open, return. */
    SSP_ASSERT(NULL != p_ctrl);
    GPT_ERROR_RETURN(gp_ctrls[p_ctrl->channel] == p_ctrl, SSP_ERR_NOT_OPEN);
#endif

    /** Cleanup. Disable interrupts, stop counter, and disable output. */
    HW_GPT_InterruptDisable(p_ctrl->channel);
    HW_GPT_CounterStartStop(p_ctrl->channel, GPT_STOP);
    IRQn_Type irq = HW_GPT_IRQGet(p_ctrl->channel);
    if (BSP_MAX_NUM_IRQn > irq)
    {
        NVIC_DisableIRQ(irq);
    }

    gtior_t * p_reg = HW_GPT_GTIOCRegLookup(p_ctrl->channel, GPT_GTIOCA);
    HW_GPT_GTIOCPinOutputEnable(p_reg, false);
    p_reg = HW_GPT_GTIOCRegLookup(p_ctrl->channel, GPT_GTIOCB);
    HW_GPT_GTIOCPinOutputEnable(p_reg, false);

    /** Unlock channel */
    /* All GPT channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    bsp_hw_lock_t lock    = (bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_GPT0_A + p_ctrl->channel);
    R_BSP_HardwareUnlock(lock);

    /** Clear stored internal driver data */
    uint32_t channel_mask = (1UL << p_ctrl->channel);
    g_channel_one_shot &= (~channel_mask);
    if (gp_ctrls[p_ctrl->channel])
    {
        gp_ctrls[p_ctrl->channel]->p_callback = NULL;
        gp_ctrls[p_ctrl->channel]->p_context  = 0;
        gp_ctrls[p_ctrl->channel]             = NULL;
    }

    return err;
} /* End of function R_GPT_Close */

/*******************************************************************************************************************//**
 * @brief      Sets driver version based on compile time macros.
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_ASSERTION    The parameter p_version is NULL.
 **********************************************************************************************************************/
ssp_err_t R_GPT_VersionGet (ssp_version_t * const p_version)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    /** Verify parameters are valid */
    SSP_ASSERT(NULL != p_version);
#endif

    p_version->version_id = g_gpt_version.version_id;

    return SSP_SUCCESS;
} /* End of function R_GPT_VersionGet */

/** @} (end addtogroup GPT) */

/*********************************************************************************************************************//**
 * Private Functions
 **********************************************************************************************************************/

static ssp_err_t gpt_common_open (timer_ctrl_t * const      p_ctrl, timer_cfg_t const * const p_cfg,
                           gpt_pclk_div_t * const p_pclk_divisor, timer_size_t * const p_pclk_counts)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(p_cfg->channel < GPT_MAX_CH);
#endif /* if GPT_CFG_PARAM_CHECKING_ENABLE */

    /** Verify channel is not already used */
    /* All GPT channels are listed in order in the bsp_hw_lock_t enum, so adding the channel number offset from
     * the base channel 0 lock yields the channel's lock type. */
    bsp_hw_lock_t lock    = (bsp_hw_lock_t) ((uint8_t) BSP_HW_LOCK_GPT0_A + p_cfg->channel);
    ssp_err_t     bsp_err = R_BSP_HardwareLock(lock);
    GPT_ERROR_RETURN((SSP_SUCCESS == bsp_err), SSP_ERR_IN_USE);
    
    /** Convert the period into PCLK counts and clock divisor */
    ssp_err_t     err          = SSP_SUCCESS;
    err = period_to_pclk(p_cfg->channel, p_cfg->period, p_cfg->unit, p_pclk_counts, p_pclk_divisor);
    if (SSP_SUCCESS != err)
    {
        R_BSP_HardwareUnlock(lock);
        GPT_ERROR_RETURN(false, err);
    }

    /** If callback is not null or timer mode is one shot, make sure IRQ is enabled and store callback in handle.
     *  @note The GPT hardware does not support one-shot mode natively.  To support one-shot mode, the timer will be
     *  stopped and cleared using software in the ISR. */
    IRQn_Type irq;
    if ((p_cfg->p_callback) || (TIMER_MODE_ONE_SHOT == p_cfg->mode))
    {
        irq = HW_GPT_IRQGet(p_cfg->channel);
        if (BSP_MAX_NUM_IRQn <= irq)
        {
            R_BSP_HardwareUnlock(lock);
            GPT_ERROR_RETURN(false, SSP_ERR_IRQ_BSP_DISABLED);
        }

        p_ctrl->p_callback = p_cfg->p_callback;
        p_ctrl->p_context  = p_cfg->p_context;
    }

    /** Mark channel as used and initialize control block. */
    p_ctrl->channel          = p_cfg->channel;
    gp_ctrls[p_cfg->channel] = p_ctrl;
    if (TIMER_MODE_ONE_SHOT == p_cfg->mode)
    {
        uint32_t channel_mask = (1UL << p_ctrl->channel);
        g_channel_one_shot |= channel_mask;
    }

    return SSP_SUCCESS;
} /* End of function gpt_common_open */

/*******************************************************************************************************************//**
 * @brief  Performs hardware initialization of the GPT.
 * @pre    All parameter checking and calculations should be done prior to calling this function.
 *
 * @param[in]  ch            Channel to initialize.
 * @param[in]  p_cfg         Pointer to timer configuration.
 * @param[in]  pclk_divisor  Divisor applied to GPT clock.
 * @param[in]  pclk_counts   Counts before timer expires.
 * @param[in]  mode          Internal mode (periodic, one shot, or PWM).
 * @param[in]  duty_cycle    Duty cycle for this implementation.
 **********************************************************************************************************************/
static void gpt_hardware_initialize (uint8_t ch, timer_cfg_t const * const p_cfg, gpt_pclk_div_t pclk_divisor,
                                     timer_size_t pclk_counts, timer_size_t duty_cycle)
{
    /** Initialization following flowchart in hardware manual (Figure 24.4 in NoSecurity_r01uh0488ej0040_sc32.pdf) */
    /** In one-shot mode, timer will be stopped and reset in the ISR. */
    HW_GPT_ModeSet(ch, GPT_MODE_PERIODIC);      ///< In one-shot mode, ISR will stop and reset timer.
    HW_GPT_DirectionSet(ch, GPT_DIR_COUNT_UP);
    HW_GPT_DivisorSet(ch, pclk_divisor);
    HW_GPT_TimerCycleSet(ch, pclk_counts - 1);
    HW_GPT_CounterSet(ch, 0);

    /** Set output if requested */
    if (p_cfg->p_extend)
    {
        timer_on_gpt_cfg_t * p_ext = (timer_on_gpt_cfg_t *) p_cfg->p_extend;
        if (p_ext->gtioca.output_enabled)
        {
            gpt_set_output_pin(ch, GPT_GTIOCA, p_ext->gtioca.stop_level, p_cfg->mode, duty_cycle);
        }

        if (p_ext->gtiocb.output_enabled)
        {
            gpt_set_output_pin(ch, GPT_GTIOCB, p_ext->gtiocb.stop_level, p_cfg->mode, duty_cycle);
        }
    }

    /** Enable GPT interrupts. */
    HW_GPT_InterruptEnable(ch, GPT_INT_TYPE_OVERFLOW);

    /** Enable CPU interrupts if callback is not null.  Also enable interrupts for one shot mode.
     *  @note The GPT hardware does not support one-shot mode natively.  To support one-shot mode, the timer will be
     *  stopped and cleared using software in the ISR. */
    if ((p_cfg->p_callback) || (TIMER_MODE_ONE_SHOT == p_cfg->mode))
    {
        IRQn_Type irq = HW_GPT_IRQGet(p_cfg->channel);
        HW_ICU_InterruptClear(irq);
        NVIC_ClearPendingIRQ(irq);
        NVIC_EnableIRQ(irq);
    }

    /** Start the timer if requested by user */
    if (p_cfg->autostart)
    {
        HW_GPT_CounterStartStop(ch, GPT_START);
    }
} /* End of function gpt_hardware_initialize */

/*******************************************************************************************************************//**
 * @brief      Converts period from input value to PCLK counts
 *
 * @retval     SSP_SUCCESS          Successful conversion
 * @retval     SSP_ERR_INVALID_ARGUMENT  One of the following is invalid:
 *                                    - p_period->unit: must be one of the options from timer_unit_t
 *                                    - p_period->value: must result in a period in the following range:
 *                                        - Lower bound: (1 / (PCLK frequency))
 *                                        - Upper bound: (0xFFFFFFFF * 1024 / (PCLK frequency))
 **********************************************************************************************************************/
static ssp_err_t period_to_pclk (uint8_t const channel, timer_size_t const period, timer_unit_t const unit, timer_size_t * const p_period_pclk,
                                 gpt_pclk_div_t * const p_divisor)
{
    uint64_t temp_period = period;

    /** Read the current PCLK frequency from the clock module. */
    uint32_t pclk_freq_hz = 0;
    g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKD, &pclk_freq_hz);

    /** Convert period to PCLK counts so it can be set in hardware. */
    switch (unit)
    {
        case TIMER_UNIT_PERIOD_RAW_COUNTS:
            temp_period = period;
            break;
        case TIMER_UNIT_FREQUENCY_KHZ:
            temp_period = (pclk_freq_hz * 1ULL) / (1000 * period);
            break;
        case TIMER_UNIT_FREQUENCY_HZ:
            temp_period = (pclk_freq_hz * 1ULL) / period;
            break;
        case TIMER_UNIT_PERIOD_NSEC:
            temp_period = (period * (pclk_freq_hz * 1ULL)) / 1000000000;
            break;
        case TIMER_UNIT_PERIOD_USEC:
            temp_period = (period * (pclk_freq_hz * 1ULL)) / 1000000;
            break;
        case TIMER_UNIT_PERIOD_MSEC:
            temp_period = (period * (pclk_freq_hz * 1ULL)) / 1000;
            break;
        case TIMER_UNIT_PERIOD_SEC:
            temp_period = period * (pclk_freq_hz * 1ULL);
            break;
        default:
            GPT_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);
            break;
    }

    /** temp_period now represents PCLK counts, but it could potentially overflow 32 bits.  If so, convert it here and
     *  set divisor appropriately.
     */
    gpt_pclk_div_t temp_div = GPT_PCLK_DIV_BY_1;
    uint64_t max_counts     = GPT_MAX_CLOCK_COUNTS_32;
#ifdef BSP_MCU_GROUP_S124
    if(channel >= 1)
    {
        max_counts = GPT_MAX_CLOCK_COUNTS_16;
    }
#endif
    while ((temp_period > max_counts) && (temp_div < GPT_PCLK_DIV_BY_1024))
    {
        temp_period >>= 2;
        temp_div++;
    }
#ifndef BSP_MCU_GROUP_S124
    SSP_PARAMETER_NOT_USED(channel);

    /** If period is valid, set it.  Otherwise return error */
    GPT_ERROR_RETURN((temp_period <= GPT_MAX_CLOCK_COUNTS_32), SSP_ERR_INVALID_ARGUMENT);
#else
    if(channel >= 1)
    {
		/** If period is valid, set it.  Otherwise return error */
		GPT_ERROR_RETURN((temp_period <= GPT_MAX_CLOCK_COUNTS_16), SSP_ERR_INVALID_ARGUMENT);
    }
    else
    {
    	/** If period is valid, set it.  Otherwise return error */
    	GPT_ERROR_RETURN((temp_period <= GPT_MAX_CLOCK_COUNTS_32), SSP_ERR_INVALID_ARGUMENT);
    }
#endif
    *p_divisor     = temp_div;
    *p_period_pclk = (timer_size_t) temp_period;

    return SSP_SUCCESS;
} /* End of function period_to_pclk */

/*******************************************************************************************************************//**
 * @brief      Converts user input period to PCLK counts and divisor, then sets compare match value and divisor.
 *
 * @param[in]  ch      Channel to set
 * @param[in]  period  User defined period
 * @param[in]  unit    Unit of user defined period
 *
 * @retval     SSP_SUCCESS          Delay and divisor were set successfully.
 * @retval     SSP_ERR_INVALID_ARGUMENT  One of the following is invalid:
 *                                    - p_period->unit: must be one of the options from timer_unit_t
 *                                    - p_period->value: must result in a period in the following range:
 *                                        - Lower bound: (1 / (PCLK frequency))
 *                                        - Upper bound: (0xFFFFFFFF * 1024 / (PCLK frequency))
 **********************************************************************************************************************/
static ssp_err_t gpt_set_period (uint8_t const ch, timer_size_t const period, timer_unit_t const unit)
{
#if GPT_CFG_PARAM_CHECKING_ENABLE
    GPT_ERROR_RETURN((0 != period), SSP_ERR_INVALID_ARGUMENT);
#endif

    /** Delay must be converted to PCLK counts before it can be set in registers */
    ssp_err_t     err          = SSP_SUCCESS;
    timer_size_t      pclk_counts  = 0;
    gpt_pclk_div_t pclk_divisor = GPT_PCLK_DIV_BY_1;
    err = period_to_pclk(ch, period, unit, &pclk_counts, &pclk_divisor);
    GPT_ERROR_RETURN((SSP_SUCCESS == err), err);

    /** Store current status, then stop timer before setting divisor register */
    gpt_start_status_t status = HW_GPT_CounterStartBitGet(ch);
    HW_GPT_CounterStartStop(ch, GPT_STOP);
    HW_GPT_DivisorSet(ch, pclk_divisor);
    HW_GPT_TimerCycleSet(ch, pclk_counts - 1);

    /** Reset counter in case new cycle is less than current count value, then restore state (counting or stopped). */
    HW_GPT_CounterSet(ch, 0);
    HW_GPT_CounterStartStop(ch, status);

    return SSP_SUCCESS;
} /* End of function gpt_set_period */

/*******************************************************************************************************************//**
 * @brief      Converts user input period to PCLK counts and divisor, then sets compare match value and divisor.
 *
 * @param[in]  ch                 Channel to set
 * @param[in]  duty_cycle_counts  Duty cycle to set.
 * @param[in]  pin                Which pin to update (0 = A or 1 = B).
 *
 * @retval     SSP_SUCCESS          Delay and divisor were set successfully.
 * @retval     SSP_ERR_INVALID_ARGUMENT  One of the following is invalid:
 *                                    - p_period->unit: must be one of the options from timer_pwm_unit_t
 *                                    - p_period->value: must result in a period in the following range:
 *                                        - Lower bound: (1 / (PCLK frequency))
 *                                        - Upper bound: (0xFFFFFFFF * 1024 / (PCLK frequency))
 **********************************************************************************************************************/
static void gpt_set_duty_cycle (uint8_t const ch, timer_size_t const duty_cycle_counts, uint8_t const pin)
{
    /* Pin is represented by gpt_gtioc_t internally for readability. */
    if (0 == duty_cycle_counts)
    {
        HW_GPT_DutyCycleModeSet(ch, (gpt_gtioc_t) pin, GPT_DUTY_CYCLE_MODE_0_PERCENT);
    }
    else if (GPT_MAX_CLOCK_COUNTS_32 == duty_cycle_counts)
    {
        HW_GPT_DutyCycleModeSet(ch, (gpt_gtioc_t) pin, GPT_DUTY_CYCLE_MODE_100_PERCENT);
    }
    else
    {
        HW_GPT_DutyCycleModeSet(ch, (gpt_gtioc_t) pin, GPT_DUTY_CYCLE_MODE_REGISTER);
        HW_GPT_CompareMatchSet(ch, (gpt_gtioc_t) pin, duty_cycle_counts);
    }
} /* End of function gpt_set_duty_cycle */

/*******************************************************************************************************************//**
 * @brief      Lookup function for clock frequency of GPT counter.  Divides GPT clock by GPT clock divisor.
 *
 * @param[in]  ch                 Channel
 *
 * @return     Clock frequency of the GPT counter.
 **********************************************************************************************************************/
static uint32_t gpt_clock_frequency_get(uint8_t const channel)
{
    uint32_t pclk_freq_hz  = 0;
    uint32_t divisor;
    gpt_pclk_div_t pclk_divisor = HW_GPT_DivisorGet(channel);
    switch (pclk_divisor)
    {
    case GPT_PCLK_DIV_BY_1:
        divisor = 1;
        break;
    case GPT_PCLK_DIV_BY_4:
        divisor = 4;
        break;
    case GPT_PCLK_DIV_BY_16:
        divisor = 16;
        break;
    case GPT_PCLK_DIV_BY_64:
        divisor = 64;
        break;
    case GPT_PCLK_DIV_BY_256:
        divisor = 256;
        break;
    case GPT_PCLK_DIV_BY_1024:
        divisor = 1024;
        break;
    default:
        divisor = 1;
        break;
    }
    g_cgc_on_cgc.systemClockFreqGet(CGC_SYSTEM_CLOCKS_PCLKD, &pclk_freq_hz);
    return (pclk_freq_hz / divisor);
}

/*******************************************************************************************************************//**
 * @brief      Converts duty cycle from input value to PCLK counts
 *
 * @retval     SSP_SUCCESS          Successful conversion
 * @retval     SSP_ERR_INVALID_ARGUMENT  One of the following is invalid:
 *                                    - unit: must be one of the options from timer_pwm_unit_t
 *                                    - duty_cycle: must result in a period in the following range:
 *                                        - Lower bound: (1 / (PCLK frequency))
 *                                        - Upper bound: period
 **********************************************************************************************************************/
static ssp_err_t duty_cycle_to_pclk (uint8_t const channel, timer_size_t const duty_cycle, timer_pwm_unit_t const unit,
        timer_size_t const period, timer_size_t * const p_duty_cycle_pclk)
{
    /** Initially set to an invalid value */
    uint64_t temp_duty_cycle = 0xFFFFFFFEULL;

    /** Convert duty cycle to PCLK counts so it can be set in hardware. */
    switch (unit)
    {
        case TIMER_PWM_UNIT_RAW_COUNTS:
            if (duty_cycle == period)
            {
#ifndef BSP_MCU_GROUP_S124
            	SSP_PARAMETER_NOT_USED(channel);

                /** 100% duty cycle */
                temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_32;
#else
                if(channel >=1)
                {
					/** 100% duty cycle: 16-bit timer*/
					temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_16;
                }
                else
                {
                    /** 100% duty cycle: 32-bit timer */
                    temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_32;
                }
#endif
            }
            else
            {
                temp_duty_cycle = duty_cycle;
            }
            break;
        case TIMER_PWM_UNIT_PERCENT:
            if (duty_cycle == 100UL)
            {
#ifndef BSP_MCU_GROUP_S124
                /** 100% duty cycle */
                temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_32;
#else
                if(channel >=1)
                {
                    /** 100% duty cycle: 16-bit timer */
                    temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_16;
                }
                else
                {
                    /** 100% duty cycle: 32-bit timer */
                    temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_32;
                }
#endif

            }
            else
            {
                temp_duty_cycle = ((uint64_t) period * duty_cycle) / 100ULL;
            }
            break;
        case TIMER_PWM_UNIT_PERCENT_X_1000:
            if (duty_cycle == 100000UL)
            {
#ifndef BSP_MCU_GROUP_S124
                /** 100% duty cycle */
                temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_32;
#else
                if(channel >=1)
                {
                    /** 100% duty cycle: 16-bit timer */
                    temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_16;
                }
                else
                {
                    /** 100% duty cycle: 32-bit timer */
                    temp_duty_cycle = GPT_MAX_CLOCK_COUNTS_32;
                }
#endif
            }
            else
            {
                temp_duty_cycle = ((uint64_t) period * duty_cycle) / 100000ULL;
            }
            break;
        default:
            GPT_ERROR_RETURN(0, SSP_ERR_INVALID_ARGUMENT);
            break;
    }
#ifndef BSP_MCU_GROUP_S124
    /** If duty_cycle is valid, set it.  Otherwise return error */
    GPT_ERROR_RETURN(((GPT_MAX_CLOCK_COUNTS_32 == temp_duty_cycle) || (temp_duty_cycle <= period)), SSP_ERR_INVALID_ARGUMENT);
#else
    if(channel >=1)
    {
        /** If duty_cycle is valid, set it.  Otherwise return error */
        GPT_ERROR_RETURN(((GPT_MAX_CLOCK_COUNTS_16 == temp_duty_cycle) || (temp_duty_cycle <= period)), SSP_ERR_INVALID_ARGUMENT);
    }
    else
    {
        /** If duty_cycle is valid, set it.  Otherwise return error */
        GPT_ERROR_RETURN(((GPT_MAX_CLOCK_COUNTS_32 == temp_duty_cycle) || (temp_duty_cycle <= period)), SSP_ERR_INVALID_ARGUMENT);
    }
#endif
    *p_duty_cycle_pclk = (timer_size_t) temp_duty_cycle;

    return SSP_SUCCESS;
} /* End of function duty_cycle_to_pclk */

/*******************************************************************************************************************//**
 * @brief      Sets output pin to toggle at cycle end.
 *
 * @param[in]  ch          Channel to set
 * @param[in]  gtio        Which pin to toggle
 * @param[in]  level       Output level after timer stops
 * @param[in]  mode        PWM or timer interface
 * @param[in]  duty_cycle  Initial duty cycle
 **********************************************************************************************************************/
static void gpt_set_output_pin (uint8_t const ch, gpt_gtioc_t const gtio, gpt_pin_level_t const level,
                                timer_mode_t mode, timer_size_t duty_cycle)
{
    /** When the counter starts, set the output pin to the default state */
    gtior_t * p_reg = HW_GPT_GTIOCRegLookup(ch, gtio);
    HW_GPT_GTIOCInitialOutputSet(p_reg, level);

    /** Configure pin to toggle at each overflow */
    HW_GPT_GTIOCPinLevelStoppedSet(p_reg, level);
    HW_GPT_GTIOCCycleEndOutputSet(p_reg, GPT_OUTPUT_TOGGLED);
    if (TIMER_MODE_ONE_SHOT == mode)
    {
        HW_GPT_InitialCompareMatchSet(ch, gtio, 0);
        HW_GPT_GTIOCCompareMatchOutputSet(p_reg, GPT_OUTPUT_TOGGLED);
    }
    else if (TIMER_MODE_PERIODIC == mode)
    {
        HW_GPT_GTIOCCompareMatchOutputSet(p_reg, GPT_OUTPUT_RETAINED);
    }
    else // (TIMER_MODE_PWM == mode)
    {
        HW_GPT_GTIOCCycleEndOutputSet(p_reg, GPT_OUTPUT_HIGH);
        HW_GPT_GTIOCCompareMatchOutputSet(p_reg, GPT_OUTPUT_LOW);
        HW_GPT_SingleBufferEnable(ch, gtio);
        gpt_set_duty_cycle(ch, duty_cycle, gtio);
        if (0 == duty_cycle)
        {
            HW_GPT_DutyCycleModeSet(ch, gtio, GPT_DUTY_CYCLE_MODE_0_PERCENT);
        }
        else if (0xFFFFFFFF == duty_cycle)
        {
            HW_GPT_DutyCycleModeSet(ch, gtio, GPT_DUTY_CYCLE_MODE_100_PERCENT);
        }
        else
        {
            HW_GPT_DutyCycleModeSet(ch, gtio, GPT_DUTY_CYCLE_MODE_REGISTER);
            HW_GPT_CompareMatchSet(ch, gtio, duty_cycle);
        }
    }

    HW_GPT_GTIOCPinOutputEnable(p_reg, true);
} /* End of function gpt_set_output_pin */

/*******************************************************************************************************************//**
 * @brief      Sets output pin to toggle at cycle end.
 *
 * Stops the timer if one-shot mode, clears interrupts, and calls callback if one was provided in the open function.
 *
 * @param[in]  channel      Channel of ISR
 * @param[in]  irq     IRQ to clear
 **********************************************************************************************************************/
#if( (defined(BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW != BSP_IRQ_DISABLED)) || \
     (defined(BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW) && (BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW != BSP_IRQ_DISABLED))    \
    )
static inline void gpt_counter_overflow_common (uint8_t channel, IRQn_Type irq)
{
    /** If one-shot mode is selected, stop the timer since period has expired. */
    if (g_channel_one_shot & (1UL << channel))
    {
        HW_GPT_CounterStartStop(channel, GPT_STOP);
        HW_GPT_CounterSet(channel, 0);
        HW_GPT_InterruptDisable(channel);
        HW_GPT_InterruptClear(channel);
    }

    /** Clear pending IRQ to make sure it doesn't fire again after exiting */
    HW_ICU_InterruptClear(irq);

    if (NULL != gp_ctrls[channel]->p_callback)
    {
        /** Set data to identify callback to user, then call user callback. */
        timer_callback_args_t cb_data;
        cb_data.p_context = gp_ctrls[channel]->p_context;
        gp_ctrls[channel]->p_callback(&cb_data);
    }
} /* End of function gpt_counter_overflow_common */
#endif /* (BSP_IRQ_CFG_GPTn_COUNTER_OVERFLOW != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      Channel 0 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_0
void gpt0_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (0, GPT0_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt0_counter_overflow_isr */
#endif /* if GPT_INSTANCE_CHANNEL_0 */
#endif /* if BSP_IRQ_CFG_GPT0_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 1 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_1
void gpt1_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (1, GPT1_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt1_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_1 */
#endif /* if BSP_IRQ_CFG_GPT1_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 2 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_2
void gpt2_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (2, GPT2_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt2_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_2 */
#endif /* if BSP_IRQ_CFG_GPT2_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 3 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_3
void gpt3_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (3, GPT3_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt3_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_3 */
#endif /* if BSP_IRQ_CFG_GPT3_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 4 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_4
void gpt4_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (4, GPT4_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt4_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_4 */
#endif /* if BSP_IRQ_CFG_GPT4_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 5 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_5
void gpt5_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (5, GPT5_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt5_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_5 */
#endif /* if BSP_IRQ_CFG_GPT5_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 6 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_6
void gpt6_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (6, GPT6_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt6_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_6 */
#endif /* if BSP_IRQ_CFG_GPT6_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 7 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_7
void gpt7_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (7, GPT7_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt7_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_7 */
#endif /* if BSP_IRQ_CFG_GPT7_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 8 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_8
void gpt8_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (8, GPT8_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt8_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_8 */
#endif /* if BSP_IRQ_CFG_GPT8_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 9 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_9
void gpt9_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (9, GPT9_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt9_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_9 */
#endif /* if BSP_IRQ_CFG_GPT9_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif

/*******************************************************************************************************************//**
 * @brief      Channel 10 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_10
void gpt10_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (10, GPT10_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt10_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_10 */
#endif /* if BSP_IRQ_CFG_GPT10_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif
/*******************************************************************************************************************//**
 * @brief      Channel 11 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_11
void gpt11_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (11, GPT11_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt11_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_11 */
#endif /* if BSP_IRQ_CFG_GPT11_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif
/*******************************************************************************************************************//**
 * @brief      Channel 12 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_12
void gpt12_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (12, GPT12_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt12_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_12 */
#endif /* if BSP_IRQ_CFG_GPT12_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif
/*******************************************************************************************************************//**
 * @brief      Channel 13 ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#ifdef BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW
#if BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW != BSP_IRQ_DISABLED
#if GPT_INSTANCE_CHANNEL_13
void gpt13_counter_overflow_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    gpt_counter_overflow_common (13, GPT13_COUNTER_OVERFLOW_IRQn);

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function gpt13_counter_overflow_isr */
#endif /*if GPT_INSTANCE_CHANNEL_13 */
#endif /* if BSP_IRQ_CFG_GPT13_COUNTER_OVERFLOW != BSP_IRQ_DISABLED */
#endif
