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
 * File Name    : hw_rtc_common.h
 * Description  : Real-Time Clock (RTC) Module hardware common header file.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup RTC
 * @{
 **********************************************************************************************************************/

#ifndef HW_RTC_COMMON_H
#define HW_RTC_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RTC_RHRCNT_HOUR_MASK (0x3f)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Set the RTC clock source
 * @param  clock_source    The clock source
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_ClockSourceSet (rtc_clock_source_t clock_source)
{
    R_RTC->RCR4_b.RCKSEL = clock_source;
}

/*******************************************************************************************************************//**
 * Get the RTC clock source
 * @return  The clock source
 **********************************************************************************************************************/
__STATIC_INLINE rtc_clock_source_t HW_RTC_ClockSourceGet (void)
{
    return (rtc_clock_source_t) R_RTC->RCR4_b.RCKSEL;
}

/*******************************************************************************************************************//**
 * Start the counter
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_CounterStart (void)
{
    R_RTC->RCR2_b.START = 1;
}

/*******************************************************************************************************************//**
 * Stopt the counter
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_CounterStop (void)
{
    R_RTC->RCR2_b.START = 0;
}

/*******************************************************************************************************************//**
 * Get the state of the start bit
 * @return  The start bit
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_CounterStartStopBitGet (void)
{
    return R_RTC->RCR2_b.START;
}

/*******************************************************************************************************************//**
 * Set the lower half of the frequency
 * @param  value    The lower half of the frequency
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_FrequencyRegisterLSet (uint16_t value)
{
    R_RTC->RFRL = value & 0x1ffU;
}

/*******************************************************************************************************************//**
 * Set the upper half of the frequency
 * @param  value    The upper half of the frequency
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_FrequencyRegisterHSet (uint16_t value)
{
    R_RTC->RFRH = value & 0x01U;
}

/*******************************************************************************************************************//**
 * Get the upper half of the frequency
 * @return  The upper half of the frequency
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_FrequencyRegisterHGet (void)
{
    return R_RTC->RFRH_b.RFC16;
}

/*******************************************************************************************************************//**
 * Get the lower half of the frequency
 * @return  The lower half of the frequency
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_RTC_FrequencyRegisterLGet (void)
{
    return R_RTC->RFRL;
}

/*******************************************************************************************************************//**
 * Set the counter mode
 * @param  mode    The counter mode
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_CounterModeSet (rtc_count_mode_t mode)
{
    R_RTC->RCR2_b.CNTMD = mode;
}

/*******************************************************************************************************************//**
 * Get the counter mode
 * @return    The counter mode
 **********************************************************************************************************************/
__STATIC_INLINE rtc_count_mode_t HW_RTC_CounterModeGet (void)
{
    return (rtc_count_mode_t) R_RTC->RCR2_b.CNTMD;
}

/*******************************************************************************************************************//**
 * Set the 24 hour mode
**********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_Counter24HourSet (void)
{
    R_RTC->RCR2_b.HR24 = 1;
}

/*******************************************************************************************************************//**
 * Perform a software reset
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_SWReset (void)
{
    R_RTC->RCR2_b.RESET = 1;
}

/*******************************************************************************************************************//**
 * Get the state of the reset bit
 * @return    the state of the reset bit
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_SWResetBitGet (void)
{
    return R_RTC->RCR2_b.RESET;
}

/*******************************************************************************************************************//**
 * Set the seconds field
 * @param  second    the seconds field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_SecondSet (uint8_t second)
{
    R_RTC->RSECCNT = second;
}

/*******************************************************************************************************************//**
 * Set the minute field
 * @param  minute    the minute field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_MinuteSet (uint8_t minute)
{
    R_RTC->RMINCNT = minute;
}

/*******************************************************************************************************************//**
 * Set the hour field
 * @param  hour    the hour field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_HourSet (uint8_t hour)
{
    R_RTC->RHRCNT = hour & RTC_RHRCNT_HOUR_MASK;
}

/*******************************************************************************************************************//**
 * Set the PM bit
 * @return      the PM bit
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_PMSet (bool PM_bit)
{
    R_RTC->RHRCNT_b.PM = PM_bit;
}

/*******************************************************************************************************************//**
 * Set the dayofweek field
 * @param  dayofweek    the dayofweek field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_DayOfWeekSet (uint8_t dayofweek)
{
    R_RTC->RWKCNT = dayofweek;
}

/*******************************************************************************************************************//**
 * Set the dayofmonth field
 * @param  dayofmonth    the dayofmonth field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_DayOfMonthSet (uint8_t dayofmonth)
{
    R_RTC->RDAYCNT = dayofmonth;
}

/*******************************************************************************************************************//**
 * Set the month field
 * @param  month    the month field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_MonthSet (uint8_t month)
{
    R_RTC->RMONCNT =    month;
}

/*******************************************************************************************************************//**
 * Set the year field
 * @param  year    the year field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_YearSet (uint8_t year)
{
    R_RTC->RYRCNT = year;
}

/*******************************************************************************************************************//**
 * Get the second field
 * @return      the second field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_SecondGet (void)
{
    return R_RTC->RSECCNT;
}

/*******************************************************************************************************************//**
 * Get the minute field
 * @return      the minute field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_MinuteGet (void)
{
    return R_RTC->RMINCNT;
}

/*******************************************************************************************************************//**
 * Get the hour field
 * @return      the hour field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_HourGet (void)
{
    return R_RTC->RHRCNT & RTC_RHRCNT_HOUR_MASK;
}

/*******************************************************************************************************************//**
 * Get the PM bit
 * @return      the PM bit
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_RTC_PMGet (void)
{
    return R_RTC->RHRCNT_b.PM;
}

/*******************************************************************************************************************//**
 * Get the dayofweek field
 * @return      the dayofweek field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_DayOfWeekGet (void)
{
    return R_RTC->RWKCNT;
}

/*******************************************************************************************************************//**
 * Get the dayofmonth field
 * @return      the dayofmonth field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_DayOfMonthGet (void)
{
    return R_RTC->RDAYCNT;
}

/*******************************************************************************************************************//**
 * Get the month field
 * @return      the month field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RTC_MonthGet (void)
{
    return R_RTC->RMONCNT;
}

/*******************************************************************************************************************//**
 * Get the year field
 * @return      the year field
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_RTC_YearGet (void)
{
    return R_RTC->RYRCNT;
}

/*******************************************************************************************************************//**
 * Set the alarm second field
 * @param  second     the alarm second field
 * @param  match_flag the alarm flag for this field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ALARM_SecondSet (uint8_t second, bool match_flag)
{
    volatile uint8_t field;

    field         = second | (uint8_t) (match_flag << 7);
    R_RTC->RSECAR = field;
}

/*******************************************************************************************************************//**
 * Set the alarm minute field
 * @param  minute     the alarm minute field
 * @param  match_flag the alarm flag for this field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ALARM_MinuteSet (uint8_t minute, bool match_flag)
{
    volatile uint8_t field;

    field         = minute | (uint8_t) (match_flag << 7);
    R_RTC->RMINAR = field;
}

/*******************************************************************************************************************//**
 * Set the alarm hour field
 * @param  hour       the alarm hour field
 * @param  match_flag the alarm flag for this field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ALARM_HourSet (uint8_t hour, bool match_flag)
{
    volatile uint8_t field;

    field        = hour | (uint8_t) (match_flag << 7);
    R_RTC->RHRAR = field;
}

/*******************************************************************************************************************//**
 * Set the alarm dayofweek field
 * @param  dayofweek  the alarm dayofweek field
 * @param  match_flag the alarm flag for this field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ALARM_DayOfWeekSet (uint8_t dayofweek, bool match_flag)
{
    volatile uint8_t field;

    field        = dayofweek | (uint8_t) (match_flag << 7);
    R_RTC->RWKAR = field;
}

/*******************************************************************************************************************//**
 * Set the alarm dayofmonth field
 * @param  dayofmonth the alarm dayofmonth field
 * @param  match_flag the alarm flag for this field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ALARM_DayOfMonthSet (uint8_t dayofmonth, bool match_flag)
{
    volatile uint8_t field;

    field         = dayofmonth | (uint8_t) (match_flag << 7);
    R_RTC->RDAYAR = field;
}

/*******************************************************************************************************************//**
 * Set the alarm month field
 * @param  month      the alarm month field
 * @param  match_flag the alarm flag for this field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ALARM_MonthSet (uint8_t month, bool match_flag)
{
    volatile uint8_t field;

    field         = month | (uint8_t) (match_flag << 7);
    R_RTC->RMONAR = field;
}

/*******************************************************************************************************************//**
 * Set the alarm year field
 * @param  year       the alarm year field
 * @param  match_flag the alarm flag for this field
 **********************************************************************************************************************/
__STATIC_INLINE void HW_ALARM_YearSet (uint8_t year, bool match_flag)
{
    volatile uint8_t field;

    field        = year | (uint8_t) (match_flag << 7);
    R_RTC->RYRAR =  field;
}

/*******************************************************************************************************************//**
 * Get the alarm second field
 * @return      the alarm second field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_ALARM_SecondGet (void)
{
    return R_RTC->RSECAR & 0x7f;
}

/*******************************************************************************************************************//**
 * Get the alarm minute field
 * @return      the alarm minute field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_ALARM_MinuteGet (void)
{
    return R_RTC->RMINAR & 0x7f;
}

/*******************************************************************************************************************//**
 * Get the alarm hour field
 * @return      the alarm hour field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_ALARM_HourGet (void)
{
    return R_RTC->RHRAR & 0x7f;
}

/*******************************************************************************************************************//**
 * Get the alarm dayofweek field
 * @return      the alarm dayofweek field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_ALARM_DayOfWeekGet (void)
{
    return R_RTC->RWKAR & 0x7f;
}

/*******************************************************************************************************************//**
 * Get the alarm dayofmonth field
 * @return      the alarm dayofmonth field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_ALARM_DayOfMonthGet (void)
{
    return R_RTC->RDAYAR & 0x7f;
}

/*******************************************************************************************************************//**
 * Get the alarm month field
 * @return      the alarm month field
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_ALARM_MonthGet (void)
{
    return R_RTC->RMONAR & 0x7f;
}

/*******************************************************************************************************************//**
 * Get the alarm year field
 * @return      the alarm year field
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_ALARM_YearGet (void)
{
    return R_RTC->RYRAR;
}

/*******************************************************************************************************************//**
 * Get the alarm second ENB field
 * @return      the alarm second ENB field
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_ALARM_SecondMatchGet (void)
{
    return R_RTC->RSECAR_b.ENB;
}

/*******************************************************************************************************************//**
 * Get the alarm minute ENB field
 * @return      the alarm minute ENB field
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_ALARM_MinuteMatchGet (void)
{
    return R_RTC->RMINAR_b.ENB;
}

/*******************************************************************************************************************//**
 * Get the alarm hour ENB field
 * @return      the alarm hour ENB field
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_ALARM_HourMatchGet (void)
{
    return R_RTC->RHRAR_b.ENB;
}

/*******************************************************************************************************************//**
 * Get the alarm dayofweek ENB field
 * @return      the alarm dayofweek ENB field
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_ALARM_DayOfWeekMatchGet (void)
{
    return R_RTC->RWKAR_b.ENB;
}

/*******************************************************************************************************************//**
 * Get the alarm dayofmonth ENB field
 * @return      the alarm dayofmonth ENB field
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_ALARM_DayOfMonthMatchGet (void)
{
    return R_RTC->RDAYAR_b.ENB;
}

/*******************************************************************************************************************//**
 * Get the alarm month ENB field
 * @return      the alarm month ENB field
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_ALARM_MonthMatchGet (void)
{
    return R_RTC->RMONAR_b.ENB;
}

/*******************************************************************************************************************//**
 * Get the alarm year ENB field
 * @return      the alarm year ENB field
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_ALARM_YearMatchGet (void)
{
    return R_RTC->RYRAREN_b.ENB;
}

/*******************************************************************************************************************//**
 * Set the clock error adjustment
 * @param  error_adjustment       the clock error adjustment
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_ClkErrorAdjustmentSet (rtc_error_adjustment_t error_adjustment)
{
    R_RTC->RADJ_b.PMADJ = error_adjustment;
}

/*******************************************************************************************************************//**
 * Get the state of the carry interrupt enable bit
 * @return      the carry interrupt enable bit
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_RTC_RTCCarryIRQGet (void)
{
    return R_RTC->RCR1_b.CIE;
}

/*******************************************************************************************************************//**
 * Disable the carry interrupt
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_RTCCarryIRQDisable (void)
{
    R_RTC->RCR1_b.CIE = 0;
}

/*******************************************************************************************************************//**
 * Enable the carry interrupt
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_RTCCarryIRQEnable (void)
{
    R_RTC->RCR1_b.CIE = 1;
}

/*******************************************************************************************************************//**
 * Get the state of the alarm interrupt enable bit
 * @return      the alarm interrupt enable bit
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_RTC_RTCAlarmIRQGet (void)
{
    return R_RTC->RCR1_b.AIE;
}

/*******************************************************************************************************************//**
 * Disable the alarm interrupt
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_RTCAlarmIRQDisable (void)
{
    R_RTC->RCR1_b.AIE = 0;
}

/*******************************************************************************************************************//**
 * Enable the alarm interrupt
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_RTCAlarmIRQEnable (void)
{
    R_RTC->RCR1_b.AIE = 1;
}

/*******************************************************************************************************************//**
 * Get the state of the periodic interrupt enable bit
 * @return      the periodic interrupt enable bit
 **********************************************************************************************************************/
__STATIC_INLINE bool HW_RTC_RTCPeriodicIRQGet (void)
{
    return R_RTC->RCR1_b.PIE;
}

/*******************************************************************************************************************//**
 * Disable the periodic interrupt
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_RTCPeriodicIRQDisable (void)
{
    R_RTC->RCR1_b.PIE = 0;
}

/*******************************************************************************************************************//**
 * Enable the periodic interrupt
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_RTCPeriodicIRQEnable (void)
{
    R_RTC->RCR1_b.PIE = 1;
}

/*******************************************************************************************************************//**
 * Set the rate of the periodic interrupt
 * @param  rate       the rate of the periodic interrupt
 **********************************************************************************************************************/
__STATIC_INLINE void HW_RTC_RTCPeriodicInterruptSelect (rtc_periodic_irq_select_t rate)
{
    R_RTC->RCR1_b.PES = rate;
}

/*******************************************************************************************************************//**
 * Get the rate of the periodic interrupt
 * @param  rate       the rate of the periodic interrupt
 **********************************************************************************************************************/
__STATIC_INLINE rtc_periodic_irq_select_t HW_RTC_RTCPeriodicInterruptRateGet (void)
{
    return (rtc_periodic_irq_select_t) R_RTC->RCR1_b.PES;
}

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
#endif /* HW_RTC_COMMON_H */
