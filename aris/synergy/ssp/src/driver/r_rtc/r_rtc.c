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

/**********************************************************************************************************************
 * File Name    : r_rtc.c
 * Description  : RTC module HAL APIs
 **********************************************************************************************************************/




/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <string.h>
#include "r_rtc.h"
#include "r_rtc_private.h"
#include "r_rtc_private_api.h"
#include "r_cgc_api.h"
#include "r_cgc.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define TIMEOUT (100)

/** Macro for error logger. */
#ifndef RTC_ERROR_RETURN
#define RTC_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), s_module_name, &s_rtc_version)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Structure to hold the callback pointer and context pointer */
typedef struct st_rtc_cb
{
    void (* callback)(rtc_callback_args_t * cb_data);   ///< Called from the ISR.
    void const * p_context;                             ///< Passed to the callback.
} rtc_cb_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static uint8_t rtc_dec_to_bcd (uint8_t to_convert);
static uint8_t rtc_bcd_to_dec (uint8_t to_convert);
void rtc_alarm_isr (void);
void rtc_period_isr (void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
#if defined(__GNUC__)
/* This structure is affected by warnings from the GCC compiler bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60784
 * This pragma suppresses the warnings in this structure only, and will be removed when the SSP compiler is updated to
 * v5.3.*/
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif
/** Version data structure used by error logger macro. */
static const ssp_version_t s_rtc_version =
{
    .api_version_minor  = RTC_API_VERSION_MINOR,
    .api_version_major  = RTC_API_VERSION_MAJOR,
    .code_version_major = RTC_CODE_VERSION_MAJOR,
    .code_version_minor = RTC_CODE_VERSION_MINOR
};
#if defined(__GNUC__)
/* Restore warning settings for 'missing-field-initializers' to as specified on command line. */
/*LDRA_INSPECTED 69 S */
#pragma GCC diagnostic pop
#endif

/** ISR callback and context pointers */
static rtc_cb_t            rtc_cb;

#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED) || (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED) || \
    (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
/** ISR context structure */
static rtc_callback_args_t rtc_context_data;
#endif

/** Name of module used by error logger macro */
#if BSP_CFG_ERROR_LOG != 0
static const char          s_module_name[] = "rtc";
#endif

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
/** RTC Implementation of Real Time Clock  */
const rtc_api_t g_rtc_on_rtc =
{
    .open                 = R_RTC_Open,
    .close                = R_RTC_Close,
    .calendarTimeGet      = R_RTC_CalendarTimeGet,
    .calendarTimeSet      = R_RTC_CalendarTimeSet,
    .calendarAlarmGet     = R_RTC_CalendarAlarmGet,
    .calendarAlarmSet     = R_RTC_CalendarAlarmSet,
    .calendarCounterStart = R_RTC_CalendarCounterStart,
    .calendarCounterStop  = R_RTC_CalendarCounterStop,
    .irqEnable            = R_RTC_IrqEnable,
    .irqDisable           = R_RTC_IrqDisable,
    .periodicIrqRateSet   = R_RTC_PeriodicIrqRateSet,
    .infoGet              = R_RTC_InfoGet,
    .versionGet           = R_RTC_VersionGet
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
static ssp_err_t rtc_set_clock_source (rtc_cfg_t const * const p_cfg);

static ssp_err_t rtc_start_bit_clear (void);

static ssp_err_t rtc_start_bit_set (void);

static ssp_err_t rtc_software_reset (void);

/*******************************************************************************************************************//**
 * @addtogroup RTC
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Open the RTC driver.
 *
 *  Implements rtc_api_t::open.
 *
 * Opens and configures the RTC driver module.
 * Configuration includes clock source, error correction, 12/24 hour mode, and interrupt callback function.
 * If the sub-clock oscillator is the clock source it is started in this function.
 *
 * @retval SSP_SUCCESS          Initialization was successful and RTC has started.
 * @retval SSP_ERR_ASSERTION    Invalid p_ctrl or p_cfg pointer.
 **********************************************************************************************************************/
ssp_err_t R_RTC_Open (rtc_ctrl_t * const p_ctrl, rtc_cfg_t const * const p_cfg)
{
    ssp_err_t err = SSP_SUCCESS;
    SSP_PARAMETER_NOT_USED(rtc_cb);          /// Prevent compiler 'unused' warning.

    /* Parameter checking */
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_cfg);
#endif

    /* Force RTC into 24 hour mode. */
    HW_RTC_Counter24HourSet();

    /* Remember the clock source */
    p_ctrl->clock_source = p_cfg->clock_source;

    /* Start the sub-clock */
    if (p_ctrl->clock_source == RTC_CLOCK_SOURCE_SUBCLK)
    {
        cgc_clock_cfg_t pll_cfg;
		g_cgc_on_cgc.clockStart(CGC_CLOCK_SUBCLOCK, &pll_cfg);
    }

    /* Set the clock source of the RTC block according to the UM */
    err = rtc_set_clock_source(p_cfg);
    if (SSP_SUCCESS == err)
    {
    	/* Always set RFC16 to 0 as per v0.80 UM */
    	HW_RTC_FrequencyRegisterHSet(0);

        /* Set clock error adjustment values */
        if (RTC_CLOCK_SOURCE_LOCO == p_cfg->clock_source)
        {
            HW_RTC_FrequencyRegisterLSet(0x00ff);
        }
        else
        {
            HW_RTC_FrequencyRegisterLSet((uint16_t) p_cfg->error_adjustment_value);
        }

        /* Save the passed in callback and context pointers to local storage. These will be used in the ISR. */
        if (p_cfg->p_callback)
        {
			rtc_cb.callback  = p_cfg->p_callback;
			rtc_cb.p_context = p_cfg->p_context;
        }
    }

    RTC_ERROR_RETURN(SSP_SUCCESS == err, err);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Close the RTC driver.
 *
 *  Implements rtc_api_t::close
 *
 * @retval SSP_SUCCESS          Initialization was successful and RTC has started.
 * @retval SSP_ERR_ASSERTION    Invalid p_cfg pointer.
 **********************************************************************************************************************/
ssp_err_t R_RTC_Close (rtc_ctrl_t * const p_ctrl)
{
    /* Parameter checking */
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the calendar time.
 *
 *  Implements rtc_api_t::calendarTimeSet.
 *
 * @retval SSP_SUCCESS          Initialization was successful and RTC has started.
 * @retval SSP_ERR_ASSERTION    Invalid p_ctrl or p_time pointer.
 **********************************************************************************************************************/
ssp_err_t R_RTC_CalendarTimeSet (rtc_ctrl_t * const p_ctrl, rtc_time_t * p_time, bool clock_start)
{
    ssp_err_t err = SSP_SUCCESS;

#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_time);
#endif

    /* Set the START bit to 0 */
    err = rtc_start_bit_clear();
    if (SSP_SUCCESS == err)
    {
        /* Execute an RTC software reset */
        err = rtc_software_reset();
        if (SSP_SUCCESS == err)
        {
            /* Set the year, month, day of the week, ... */
            HW_RTC_SecondSet(rtc_dec_to_bcd((uint8_t) p_time->tm_sec));
            HW_RTC_MinuteSet(rtc_dec_to_bcd((uint8_t) p_time->tm_min));
            HW_RTC_HourSet(rtc_dec_to_bcd((uint8_t) p_time->tm_hour));
            HW_RTC_DayOfWeekSet(rtc_dec_to_bcd((uint8_t) p_time->tm_wday));
            HW_RTC_DayOfMonthSet(rtc_dec_to_bcd((uint8_t) p_time->tm_mday));
            HW_RTC_MonthSet(rtc_dec_to_bcd((uint8_t) p_time->tm_mon));
            HW_RTC_YearSet(rtc_dec_to_bcd((uint8_t) p_time->tm_year));

            /* Set the START bit to 1 */
            if (clock_start)
            {
                err = rtc_start_bit_set();
            }
        }
    }

    RTC_ERROR_RETURN(SSP_SUCCESS == err, err);

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Get the calendar time.
 *
 *  Implements rtc_api_t::calendarTimeGet
 *
 * @retval SSP_SUCCESS          Initialization was successful and RTC has started.
 * @retval SSP_ERR_ASSERTION    Invalid p_time pointer.
 **********************************************************************************************************************/
ssp_err_t R_RTC_CalendarTimeGet (rtc_ctrl_t * const p_ctrl, rtc_time_t * p_time)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_time);
#endif

    p_time->tm_sec  = rtc_bcd_to_dec(HW_RTC_SecondGet());
    p_time->tm_min  = rtc_bcd_to_dec(HW_RTC_MinuteGet());
    p_time->tm_hour = rtc_bcd_to_dec(HW_RTC_HourGet());
    p_time->tm_wday = rtc_bcd_to_dec(HW_RTC_DayOfWeekGet());
    p_time->tm_mday = rtc_bcd_to_dec(HW_RTC_DayOfMonthGet());
    p_time->tm_mon  = rtc_bcd_to_dec(HW_RTC_MonthGet());
    p_time->tm_year = rtc_bcd_to_dec((uint8_t) HW_RTC_YearGet());

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the calendar alarm time.
 *
 *  Implements rtc_api_t::calendarAlarmSet.
 *
 * @pre The calendar counter must be running before the alarm can be set.
 *
 * @retval SSP_SUCCESS           Initialization was successful and RTC has started.
 * @retval SSP_ERR_ASSERTION     Invalid p_alarm pointer.
 * @retval SSP_ERR_INVALID_MODE  Calendar counter not running.
 **********************************************************************************************************************/
ssp_err_t R_RTC_CalendarAlarmSet (rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * p_alarm, bool interrupt_enable_flag)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_alarm);
#endif

#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
    /* Disable the ICU alarm interrupt request */
    NVIC_DisableIRQ(RTC_ALARM_IRQn);
#endif

    /* Set alarm time */
    HW_ALARM_SecondSet(rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_sec), p_alarm->sec_match);
    HW_ALARM_MinuteSet(rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_min), p_alarm->min_match);
    HW_ALARM_HourSet(rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_hour), p_alarm->hour_match);
    HW_ALARM_DayOfWeekSet((uint8_t) p_alarm->time.tm_wday, p_alarm->dayofweek_match);
    HW_ALARM_DayOfMonthSet(rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_mday), p_alarm->mday_match);
    HW_ALARM_MonthSet(rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_mon), p_alarm->mon_match);
    HW_ALARM_YearSet(rtc_dec_to_bcd((uint8_t) p_alarm->time.tm_year), p_alarm->year_match);

    /* Enable the RTC alarm interrupt request */
    if (interrupt_enable_flag)
    {
        static uint32_t timeout = 0xfff;
        HW_RTC_RTCAlarmIRQEnable();

        /* wait for the AIE bit to show as set */
        while ((!HW_RTC_RTCAlarmIRQGet()) && timeout)
        {
            timeout--;
        }

        if (!timeout)
        {
            RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
        }
    }

    /* Wait for the completion of the alarm time setting by writing to this register twice */
    HW_RTC_RTCPeriodicInterruptSelect(RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_64_SECOND);
    HW_RTC_RTCPeriodicInterruptSelect(RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_64_SECOND);

#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
    /* Set the alarm flag to 0 */
    R_BSP_IrqStatusClear(RTC_ALARM_IRQn);

    /* Enable the ICU alarm interrupt request */
    NVIC_EnableIRQ(RTC_ALARM_IRQn);
#endif

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Get the calendar alarm time.
 *
 *  Implements rtc_api_t::calendarAlarmGet
 *
 * @retval SSP_SUCCESS    Initialization was successful and RTC has started.
 * @retval SSP_ERR_ASSERTION     Invalid p_alarm pointer.
 **********************************************************************************************************************/
ssp_err_t R_RTC_CalendarAlarmGet (rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * p_alarm)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
    SSP_ASSERT(p_alarm);
#endif

    p_alarm->time.tm_sec     = rtc_bcd_to_dec(HW_ALARM_SecondGet());
    p_alarm->time.tm_min     = rtc_bcd_to_dec(HW_ALARM_MinuteGet());
    p_alarm->time.tm_hour    = rtc_bcd_to_dec(HW_ALARM_HourGet());
    p_alarm->time.tm_wday    = rtc_bcd_to_dec(HW_ALARM_DayOfWeekGet());
    p_alarm->time.tm_mday    = rtc_bcd_to_dec(HW_ALARM_DayOfMonthGet());
    p_alarm->time.tm_mon     = rtc_bcd_to_dec(HW_ALARM_MonthGet());
    p_alarm->time.tm_year    = rtc_bcd_to_dec((uint8_t) HW_ALARM_YearGet());

    p_alarm->sec_match       = HW_ALARM_SecondMatchGet();
    p_alarm->min_match       = HW_ALARM_MinuteMatchGet();
    p_alarm->hour_match      = HW_ALARM_HourMatchGet();
    p_alarm->dayofweek_match = HW_ALARM_DayOfWeekMatchGet();
    p_alarm->mon_match       = HW_ALARM_MonthMatchGet();
    p_alarm->year_match      = HW_ALARM_YearMatchGet();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Start the calendar counter.
 *
 *  Implements rtc_api_t::calendarCounterStart.
 *
 * @retval SSP_SUCCESS    Calendar counter started.
 **********************************************************************************************************************/
ssp_err_t R_RTC_CalendarCounterStart (rtc_ctrl_t * const p_ctrl)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    HW_RTC_CounterStart();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Stop the calendar counter.
 *
 *  Implements rtc_api_t::calendarCounterStop.
 *
 * @retval SSP_SUCCESS    Calendar counter stopped.
 **********************************************************************************************************************/
ssp_err_t R_RTC_CalendarCounterStop (rtc_ctrl_t * const p_ctrl)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    HW_RTC_CounterStop();

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Enable the alarm interrupt.
 *
 *  Implements rtc_api_t::interruptEnable.
 *
 * @retval SSP_SUCCESS    Alarm interrupt enabled.
 **********************************************************************************************************************/
ssp_err_t R_RTC_IrqEnable (rtc_ctrl_t * const p_ctrl, rtc_event_t interrupt)
{
    uint32_t timeout = 0xfffff;
    SSP_PARAMETER_NOT_USED(timeout);  /* timeout is only used if interrupts are enabled. */

#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    switch (interrupt)
    {
#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
        case RTC_EVENT_ALARM_IRQ:
            HW_RTC_RTCAlarmIRQEnable();                   /* Enable this interrupt in the RTC block */
            while ((!HW_RTC_RTCAlarmIRQGet()) && timeout) /* wait for the bit to set */
            {
                timeout--;
            }

            if (!timeout)
            {
                RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
            }

            NVIC_EnableIRQ(RTC_ALARM_IRQn);  /* Enable this interrupt in the NVIC */
            break;
#endif /* if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED) */
#if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED)
        case RTC_EVENT_PERIODIC_IRQ:
            HW_RTC_RTCPeriodicIRQEnable();                   /* Enable this interrupt in the RTC block */
            while ((!HW_RTC_RTCPeriodicIRQGet()) && timeout) /* wait for the bit to set */
            {
                timeout--;
            }

            if (!timeout)
            {
                RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
            }

            NVIC_EnableIRQ(RTC_PERIOD_IRQn);   /* Enable this interrupt in the NVIC */
            break;
#endif /* if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED) */
#if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
        case RTC_EVENT_CARRY_IRQ:
            HW_RTC_RTCCarryIRQEnable();                   /* Enable this interrupt in the RTC block */
            while ((!HW_RTC_RTCCarryIRQGet()) && timeout) /* wait for the bit to set */
            {
                timeout--;
            }

            if (!timeout)
            {
                RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
            }

            NVIC_EnableIRQ(RTC_CARRY_IRQn);  /* Enable this interrupt in the NVIC */
            break;
#endif /* if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED) */
        default:
            break;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Disable the alarm interrupt.
 *
 *  Implements rtc_api_t::interruptDisable.
 *
 * @retval SSP_SUCCESS    Alarm interrupt disabled.
 **********************************************************************************************************************/
ssp_err_t R_RTC_IrqDisable (rtc_ctrl_t * const p_ctrl, rtc_event_t interrupt)
{
    uint32_t timeout = 0xfffff;
    SSP_PARAMETER_NOT_USED(timeout);  /* timeout is only used if interrupts are enabled. */

#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    switch (interrupt)
    {
#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
        case RTC_EVENT_ALARM_IRQ:
            NVIC_DisableIRQ(RTC_ALARM_IRQn);               /* Disable this interrupt in the NVIC */
            HW_RTC_RTCAlarmIRQDisable();                 /* Disable this interrupt in the RTC block */
            while ((HW_RTC_RTCAlarmIRQGet()) && timeout) /* wait for the bit to clear */
            {
                timeout--;
            }

            if (!timeout)
            {
                RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
            }
            break;
#endif /* if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED) */
#if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED)
        case RTC_EVENT_PERIODIC_IRQ:
            NVIC_DisableIRQ(RTC_PERIOD_IRQn);                  /* Disable this interrupt in the NVIC */
            HW_RTC_RTCPeriodicIRQDisable();                 /* Disable this interrupt in the RTC block */
            while ((HW_RTC_RTCPeriodicIRQGet()) && timeout) /* wait for the bit to clear */
            {
                timeout--;
            }

            if (!timeout)
            {
                RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
            }
            break;
#endif /* if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED) */
#if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
        case RTC_EVENT_CARRY_IRQ:
            NVIC_DisableIRQ(RTC_CARRY_IRQn);               /* Disable this interrupt in the NVIC */
            HW_RTC_RTCCarryIRQDisable();                 /* Disable this interrupt in the RTC block */
            while ((HW_RTC_RTCCarryIRQGet()) && timeout) /* wait for the bit to clear */
            {
                timeout--;
            }

            if (!timeout)
            {
                RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
            }
            break;
#endif /* if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED) */
        default:
            break;
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the periodic interrupt rate.
 *
 *  Implements rtc_api_t::periodicInterruptRateSet.
 *
 * @retval SSP_SUCCESS    The periodic interrupt rate was successfully set.
 **********************************************************************************************************************/
ssp_err_t R_RTC_PeriodicIrqRateSet (rtc_ctrl_t * const p_ctrl, rtc_periodic_irq_select_t rate)
{
    uint32_t timeout = 0xffff;

#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_ctrl);
#endif

    HW_RTC_RTCPeriodicInterruptSelect(rate);

    /* wait for the rate to set */
    while ((HW_RTC_RTCPeriodicInterruptRateGet() != rate) && timeout)
    {
        timeout--;
    }

    if (!timeout)
    {
        RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      This function returns information about the driver including:
 * The source clock
 *
 *  Implements rtc_api_t::infoGet
 *
 * @retval     SSP_SUCCESS          Success.
 * @retval     SSP_ERR_ASSERTION    The parameter p_ctrl is NULL.
 **********************************************************************************************************************/
ssp_err_t R_RTC_InfoGet(rtc_ctrl_t * p_ctrl, rtc_info_t * p_rtc_info)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_rtc_info);
#endif

    p_rtc_info->clock_source = p_ctrl->clock_source;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief      Get driver version based on compile time macros.
 *
 *  Implements rtc_api_t::versionGet
 *
 * @retval     SSP_SUCCESS          Successful close.
 * @retval     SSP_ERR_INVALID_PTR  The parameter p_version is NULL.
 **********************************************************************************************************************/
ssp_err_t R_RTC_VersionGet (ssp_version_t * p_version)
{
#if RTC_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(p_version);
#endif

    p_version->version_id = s_rtc_version.version_id;

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtpgroup RTC)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  TODO_RTC <Brief description>.
 *
 * TODO_RTC <Detailed description>
 *
 * @retval SSP_SUCCESS           TODO_RTC <What does success mean here?>
 * @retval TODO_RTC <Add all possible return codes with @retval.>
 *
 * @note TODO_RTC <Is this function reentrant?>
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief  Clear the start bit
 *
 * @retval SSP_SUCCESS           start bit cleared
 * @retval SSP_ERR_TIMEOUT       start bit not cleared
 **********************************************************************************************************************/
static ssp_err_t rtc_start_bit_clear (void)
{
    volatile uint32_t timeout = 0xfffff; /* TODO replace with a better mechanism */

    HW_RTC_CounterStop();

    /* Wait for the START bit to become 0 */
    while (timeout && HW_RTC_CounterStartStopBitGet())
    {
        timeout--;
    }

    if (!timeout)
    {
        RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the start bit
 *
 * @retval SSP_SUCCESS           start bit set
 * @retval SSP_ERR_TIMEOUT       start bit not set
 **********************************************************************************************************************/
static ssp_err_t rtc_start_bit_set (void)
{
    volatile uint32_t timeout = 0xfffff; /* TODO replace with a better mechanism */

    HW_RTC_CounterStart();

    /* Wait for the START bit to become 0 */
    while (timeout && (!HW_RTC_CounterStartStopBitGet()))
    {
        timeout--;
    }

    if (!timeout)
    {
        RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Perform a software reset
 *
 * @retval SSP_SUCCESS           software reset complete
 * @retval SSP_ERR_TIMEOUT       software reset not complete
 **********************************************************************************************************************/
static ssp_err_t rtc_software_reset (void)
{
    volatile uint32_t timeout = 0xfffff; /* TODO replace with a better mechanism */

    HW_RTC_SWReset();

    /* Wait for the reset bit to become 0 */
    timeout = 0xfff;
    while (timeout && HW_RTC_SWResetBitGet())
    {
        timeout--;
    }

    if (!timeout)
    {
        RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
    }

    return SSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief  Set the RTC clock source
 *
 * @retval SSP_SUCCESS           RTC clock source set
 * @retval SSP_ERR_TIMEOUT       RTC clock source not set
 **********************************************************************************************************************/
static ssp_err_t rtc_set_clock_source (rtc_cfg_t const * const p_cfg)
{
    ssp_err_t         error;
    volatile uint32_t timeout = 0xfffff;

    /* Select the count source */
    HW_RTC_ClockSourceSet(p_cfg->clock_source);

    /* Supply 6 clocks of the count source (LOCO, 183us, 32kHZ). */
    if (p_cfg->clock_source == RTC_CLOCK_SOURCE_SUBCLK)
    {
        /* According to HM a fixed time for stabilization is required for oscillation to become stable after selecting the sub clock operation with the
        SOSTP bit. */
        R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
    }
    else
    {
        R_BSP_SoftwareDelay(190, BSP_DELAY_UNITS_MICROSECONDS);
    }

    /* Set the START bit to 0 */
    error = rtc_start_bit_clear();

    if (SSP_SUCCESS == error)
    {
        /* Select count mode */
        HW_RTC_CounterModeSet(RTC_CALENDAR_MODE);

        /* Wait for the CNTMD bit to become 0 */
        while (timeout && HW_RTC_CounterModeGet())
        {
            timeout--;
        }

        if (!timeout)
        {
            RTC_ERROR_RETURN(NULL, SSP_ERR_TIMEOUT);
        }

        /* Execute RTC software reset */
        error = rtc_software_reset();
    }

    RTC_ERROR_RETURN(SSP_SUCCESS == error, error);

    return error;  /* Required to return SSP_SUCCESS */
}

/*******************************************************************************************************************//**
 * @brief      RTC Alarm ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
/* @cond SKIP */
#if (BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED)
void rtc_alarm_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear (RTC_ALARM_IRQn);

    /* Call the callback routine if one is available */
    if (rtc_cb.callback)
    {
        rtc_context_data.event     = RTC_EVENT_ALARM_IRQ;
        rtc_context_data.p_context = rtc_cb.p_context;
        rtc_cb.callback(&rtc_context_data);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function rtc_alarm_isr */
#endif /* BSP_IRQ_CFG_RTC_ALARM != BSP_IRQ_DISABLED */

/*******************************************************************************************************************//**
 * @brief      RTC Periodic ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED)
void rtc_period_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear (RTC_PERIOD_IRQn);

    /* Call the callback routine if one is available */
    if (rtc_cb.callback)
    {
        rtc_context_data.event     = RTC_EVENT_PERIODIC_IRQ;
        rtc_context_data.p_context = rtc_cb.p_context;
        rtc_cb.callback(&rtc_context_data);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function rtc_period_isr */
#endif /* (BSP_IRQ_CFG_RTC_PERIOD != BSP_IRQ_DISABLED) */

/*******************************************************************************************************************//**
 * @brief      RTC Carry ISR.
 *
 * Saves context if RTOS is used, stops the timer if one-shot mode, clears interrupts, calls callback if one was
 * provided in the open function, and restores context if RTOS is used.
 **********************************************************************************************************************/
#if (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED)
void rtc_carry_isr (void)
{
    /* Save context if RTOS is used */
    SF_CONTEXT_SAVE

    /* Clear the IR flag in the ICU */
    R_BSP_IrqStatusClear (RTC_CARRY_IRQn);

    /* Call the callback routine if one is available */
    if (rtc_cb.callback)
    {
        rtc_context_data.event     = RTC_EVENT_CARRY_IRQ;
        rtc_context_data.p_context = rtc_cb.p_context;
        rtc_cb.callback(&rtc_context_data);
    }

    /* Restore context if RTOS is used */
    SF_CONTEXT_RESTORE
} /* End of function rtc_carry_isr */
#endif /*  (BSP_IRQ_CFG_RTC_CARRY != BSP_IRQ_DISABLED) */

#if (RTC_CFG_PARAM_CHECKING_ENABLE) || (RTC_CFG_CALCULATE_YDAY == 1)
/* @endcond */

/*******************************************************************************************************************//**
 * @brief      Convert decimal to BCD
 *
 **********************************************************************************************************************/
static uint8_t rtc_dec_to_bcd (uint8_t to_convert)
{
    return (uint8_t) ((((to_convert / 10) << 4) & 0xF0) | (to_convert % 10));
}

/*******************************************************************************************************************//**
 * @brief      Convert  BCD to decimal
 *
 **********************************************************************************************************************/
static uint8_t rtc_bcd_to_dec (uint8_t to_convert)
{
    return (uint8_t) ((((to_convert & 0xF0) >> 4) * 10) + (to_convert & 0x0F));
}
#endif  /* (RTC_CFG_PARAM_CHECKING_ENABLE) || (RTC_CFG_CALCULATE_YDAY == 1) */


