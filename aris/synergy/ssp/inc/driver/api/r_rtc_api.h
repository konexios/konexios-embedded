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
 * File Name    : r_rtc_api.h
 * Description  : RTC Interface
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup RTC_API RTC Interface
 * @brief Interface for accessing the Realtime Clock.
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * RTC description: @ref HALRTCInterface
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_RTC_API_H
#define DRV_RTC_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"
/** Use of time structure, tm */
#include <time.h>

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RTC_API_VERSION_MAJOR (1)
#define RTC_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Events that can trigger a callback function */
typedef enum e_rtc_event
{
    RTC_EVENT_ALARM_IRQ,       ///< Real Time Clock ALARM IRQ
    RTC_EVENT_PERIODIC_IRQ,    ///< Real Time Clock PERIODIC IRQ
    RTC_EVENT_CARRY_IRQ        ///< Real Time Clock CARRY IRQ
} rtc_event_t;

/** Callback function parameter data */
typedef struct st_rtc_callback_args
{
    rtc_event_t   event;       ///< The event can be used to identify what caused the callback (compare match or error).
    void const  * p_context;   ///< Placeholder for user data.  Set in r_timer_t::open function in ::timer_cfg_t.
} rtc_callback_args_t;

/** Clock source for the RTC block */
typedef enum e_rtc_count_source
{
    RTC_CLOCK_SOURCE_SUBCLK = 0,   ///< Sub-clock oscillator
    RTC_CLOCK_SOURCE_LOCO   = 1    ///< Low power On Chip Oscillator
} rtc_clock_source_t;

/** Time error adjustment settings */
typedef enum e_rtc_error_adjustment
{
    RTC_ERROR_ADJUSTMENT_NONE               = 0, ///< Adjustment is not performed
    RTC_ERROR_ADJUSTMENT_ADD_PRESCALER      = 1, ///< Adjustment is performed by the addition to the prescaler
    RTC_ERROR_ADJUSTMENT_SUBTRACT_PRESCALER = 2, ///< Adjustment is performed by the subtraction from the prescaler
} rtc_error_adjustment_t;

/** Periodic Interrupt select */
typedef enum e_rtc_periodic_irq_select
{
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_256_SECOND = 6, ///< A periodic irq is generated every 1/256 second
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_128_SECOND,     ///< A periodic irq is generated every 1/128 second
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_64_SECOND,      ///< A periodic irq is generated every 1/64 second
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_32_SECOND,      ///< A periodic irq is generated every 1/32 second
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_16_SECOND,      ///< A periodic irq is generated every 1/16 second
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_8_SECOND,       ///< A periodic irq is generated every 1/8 second
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_4_SECOND,       ///< A periodic irq is generated every 1/4 second
    RTC_PERIODIC_IRQ_SELECT_1_DIV_BY_2_SECOND,       ///< A periodic irq is generated every 1/2 second
    RTC_PERIODIC_IRQ_SELECT_1_SECOND,                ///< A periodic irq is generated every 1 second
    RTC_PERIODIC_IRQ_SELECT_2_SECONDS,               ///< A periodic irq is generated every 2 seconds
} rtc_periodic_irq_select_t;

/** Date and time structure defined in C standard library <time.h> */
typedef struct tm rtc_time_t;

/** Alarm time setting structure */
typedef struct st_rtc_alarm_time
{
    rtc_time_t  time;            ///< Time structure
    bool        sec_match;       ///< Enable the alarm based on a match of the seconds field
    bool        min_match;       ///< Enable the alarm based on a match of the minutes field
    bool        hour_match;      ///< Enable the alarm based on a match of the hours field
    bool        mday_match;      ///< Enable the alarm based on a match of the days field
    bool        mon_match;       ///< Enable the alarm based on a match of the months field
    bool        year_match;      ///< Enable the alarm based on a match of the years field
    bool        dayofweek_match; ///< Enable the alarm based on a match of the dayofweek field
} rtc_alarm_time_t;

/** RTC Information Structure for information returned by  infoGet() */
typedef struct st_rtc_info
{
    rtc_clock_source_t      clock_source;        ///< Clock source for the RTC block
} rtc_info_t;

/** User configuration structure, used in open function */
typedef struct st_rtc_cfg
{
    rtc_clock_source_t      clock_source;              ///< Clock source for the RTC block
    uint32_t                error_adjustment_value;    ///< Value of the prescaler for error adjustment
    rtc_error_adjustment_t  error_adjustment_type;     ///< How the prescaler value is applied
    void                 (* p_callback)(rtc_callback_args_t * p_args); ///< Called from the ISR.
    void const            * p_context;                 ///< Passed to the callback.
    void const            * p_extend;                  ///< RTC hardware dependant configuration.
} rtc_cfg_t;

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when r_rtc_t::open is called */
typedef struct st_rtc_ctrl
{
    rtc_clock_source_t      clock_source;              ///< Clock source for the RTC block
} rtc_ctrl_t;

/** RTC driver structure. General RTC functions implemented at the HAL layer follow this API. */
typedef struct st_rtc_api
{
    /** Open the RTC driver.
     * @par Implemented as
     * - R_RTC_Open()
     *
     * @param[in] p_ctrl     Pointer to RTC device handle
     * @param[in] p_cfg      Pointer to the configuration structure
     */
    ssp_err_t (* open)(rtc_ctrl_t * const p_ctrl, rtc_cfg_t const * const p_cfg);

    /** Close the RTC driver.
     * @par Implemented as
     * - R_RTC_Close()
     *
     * @param[in] p_ctrl     Pointer to RTC device handle.
     */
    ssp_err_t (* close)(rtc_ctrl_t * const p_ctrl);

    /** Set the calendar time.
     * @par Implemented as
     * - R_RTC_CalendarTimeSet()
     *
     * @param[in] p_ctrl      Pointer to RTC device handle
     * @param[in] p_time      Pointer to a time structure that contains the time to set
     * @param[in] clock_start Flag that starts the clock right after it is set
     */
    ssp_err_t (* calendarTimeSet)(rtc_ctrl_t * const p_ctrl, rtc_time_t * p_time, bool clock_start);

    /** Get the calendar time.
     * @par Implemented as
     * - R_RTC_CalendarTimeGet()
     *
     * @param[in] p_ctrl      Pointer to RTC device handle
     * @param[out] p_time     Pointer to a time structure that contains the time to get
     */
    ssp_err_t (* calendarTimeGet)(rtc_ctrl_t * const p_ctrl, rtc_time_t * p_time);

    /** Set the calendar alarm time.
     * @par Implemented as
     * - R_RTC_CalendarAlarmSet()
     *
     * @param[in] p_ctrl                 Pointer to RTC device handle
     * @param[in] p_alarm                Pointer to an alarm structure that contains the alarm time to set
     * @param[in] irq_enable_flag        Enable the ALARM irq if set
     */
    ssp_err_t (* calendarAlarmSet)(rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * p_alarm, bool irq_enable_flag);

    /** Get the calendar alarm time.
     * @par Implemented as
     * - R_RTC_CalendarAlarmGet()
     *
     * @param[in] p_ctrl       Pointer to RTC device handle
     * @param[out] p_alarm     Pointer to an alarm structure to fill up with the alarm time
     */
    ssp_err_t (* calendarAlarmGet)(rtc_ctrl_t * const p_ctrl, rtc_alarm_time_t * p_alarm);

    /** Start the calendar counter.
     * @par Implemented as
     * - R_RTC_CalendarCounterStart()
     *
     * @param[in] p_ctrl       Pointer to RTC device handle
     */
    ssp_err_t (* calendarCounterStart)(rtc_ctrl_t * const p_ctrl);

    /** Stop the calendar counter.
     * @par Implemented as
     * - R_RTC_CalendarCounterStop()
     *
     * @param[in] p_ctrl       Pointer to RTC device handle
     */
    ssp_err_t (* calendarCounterStop)(rtc_ctrl_t * const p_ctrl);

    /** Enable the alarm irq.
     * @par Implemented as
     * - R_RTC_IrqEnable()
     *
     * @param[in] p_ctrl       Pointer to RTC device handle
     */
    ssp_err_t (* irqEnable)(rtc_ctrl_t * const p_ctrl, rtc_event_t irq);

    /** Disable the alarm irq.
     * @par Implemented as
     * - R_RTC_IrqDisable()
     *
     * @param[in] p_ctrl       Pointer to RTC device handle
     */
    ssp_err_t (* irqDisable)(rtc_ctrl_t * const p_ctrl, rtc_event_t irq);

    /** Set the periodic irq rate
     * @par Implemented as
     * - R_RTC_PeriodicIrqRateSet()
     *
     * @param[in] p_ctrl       Pointer to RTC device handle
     * @param[in] rate         Rate of periodic interrupts
     */
    ssp_err_t (* periodicIrqRateSet)(rtc_ctrl_t * const p_ctrl, rtc_periodic_irq_select_t rate);

    /** Return the currently configure clock source for the RTC
         *
         * @par Implemented as
         *  - R_RTC_InfoGet()
         *
         * @param[in]   p_ctrl       Pointer to control handle structure
         * @param[out]  p_rtc_info   Pointer to ADC information structure
         */
        ssp_err_t (* infoGet) (rtc_ctrl_t * p_ctrl, rtc_info_t * p_rtc_info);

    /** Gets version and stores it in provided pointer p_version.
     * @par Implemented as
     * - R_RTC_VersionGet()
     *
     * @param[out]  p_version  Code and API version used
     */
    ssp_err_t (* versionGet)(ssp_version_t * version);
} rtc_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_rtc_instance
{
    rtc_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    rtc_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    rtc_api_t const * p_api;     ///< Pointer to the API structure for this instance
} rtc_instance_t;


#endif /* DRV_RTC_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RTC_API)
 **********************************************************************************************************************/
