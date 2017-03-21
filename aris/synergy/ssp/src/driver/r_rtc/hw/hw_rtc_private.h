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
 * File Name    : hw_rtc_private.h
 * Description  : Real-Time Clock (RTC) Module hardware private header file.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup RTC
 * @{
 **********************************************************************************************************************/

#ifndef HW_RTC_PRIVATE_H
#define HW_RTC_PRIVATE_H

#if defined(__GNUC__) && defined(__ARM_EABI__)
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_rtc.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/

/*Clock and count mode setting */
__STATIC_INLINE void               HW_RTC_ClockSourceSet (rtc_clock_source_t clock_source) INLINE_ATTRIBUTE;

__STATIC_INLINE rtc_clock_source_t HW_RTC_ClockSourceGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_CounterStart (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_CounterStop (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_CounterStartStopBitGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_FrequencyRegisterLSet (uint16_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_FrequencyRegisterHSet (uint16_t value) INLINE_ATTRIBUTE;

__STATIC_INLINE uint16_t            HW_RTC_FrequencyRegisterLGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE rtc_count_mode_t   HW_RTC_CounterModeGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_Counter24HourSet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_SWReset (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_SWResetBitGet (void) INLINE_ATTRIBUTE;

/* Time setting */
__STATIC_INLINE void               HW_RTC_SecondSet (uint8_t second) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_MinuteSet (uint8_t minute) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_HourSet (uint8_t hour) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_DayOfWeekSet (uint8_t dayofweek) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_DayOfMonthSet (uint8_t dayofmonth) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_MonthSet (uint8_t month) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_YearSet (uint8_t year) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_SecondGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_MinuteGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_HourGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_DayOfWeekGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_DayOfMonthGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t            HW_RTC_MonthGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint16_t           HW_RTC_YearGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void               HW_RTC_ClkErrorAdjustmentSet (rtc_error_adjustment_t error_adjustment)
INLINE_ATTRIBUTE;

/* Alarm setting */
__STATIC_INLINE void    HW_ALARM_SecondSet (uint8_t second, bool match_flag) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_ALARM_MinuteSet (uint8_t minute, bool match_flag) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_ALARM_HourSet (uint8_t hour, bool match_flag) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_ALARM_DayOfWeekSet (uint8_t dayofweek, bool match_flag) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_ALARM_DayOfMonthSet (uint8_t dayofmonth, bool match_flag) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_ALARM_MonthSet (uint8_t month, bool match_flag) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_ALARM_YearSet (uint8_t year, bool match_flag) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t HW_ALARM_SecondGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t HW_ALARM_MinuteGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t HW_ALARM_HourGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t HW_ALARM_DayOfWeekGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t HW_ALARM_DayOfMonthGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint8_t HW_ALARM_MonthGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE uint16_t HW_ALARM_YearGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_ALARM_SecondMatchGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_ALARM_MinuteMatchGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_ALARM_HourMatchGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_ALARM_DayOfWeekMatchGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_ALARM_DayOfMonthMatchGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_ALARM_MonthMatchGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_ALARM_YearMatchGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_RTC_RTCCarryIRQGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_RTC_RTCCarryIRQDisable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_RTC_RTCCarryIRQEnable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_RTC_RTCAlarmIRQGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_RTC_RTCAlarmIRQDisable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_RTC_RTCAlarmIRQEnable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE bool    HW_RTC_RTCPeriodicIRQGet (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_RTC_RTCPeriodicIRQDisable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_RTC_RTCPeriodicIRQEnable (void) INLINE_ATTRIBUTE;

__STATIC_INLINE void    HW_RTC_RTCPeriodicInterruptSelect (rtc_periodic_irq_select_t rate)
INLINE_ATTRIBUTE;

__STATIC_INLINE rtc_periodic_irq_select_t HW_RTC_RTCPeriodicInterruptRateGet (void);


/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Select implementation based on BSP here */
#include "common/hw_rtc_common.h"
#endif /* HW_RTC_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RTC)
 **********************************************************************************************************************/
