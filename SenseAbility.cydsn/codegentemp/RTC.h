/*******************************************************************************
* File Name: RTC.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the RTC_P4 Component.
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_RTC_P4_RTC_H)
#define CY_RTC_P4_RTC_H

#include <cytypes.h>
#include <CyLib.h>


/***************************************
*    Data types definition.
***************************************/
typedef enum
{
    RTC_DST_DATE_RELATIVE = 0u,
    RTC_DST_DATE_FIXED = 1u
}RTC_DST_DATETYPE_ENUM;

typedef struct
{
    uint32 time;
    uint32 date;
    uint32 dayOfWeek;
    uint32 status;
}RTC_DATE_TIME;

typedef struct
{
    uint32 hour;
    uint32 dayOfWeek;
    uint32 dayOfMonth;
    uint32 weekOfMonth;
    uint32 month;
    uint8  timeFormat;
}RTC_DST_TIME;


/**************************************
*    Enumerated Types and Parameters
**************************************/
/* Time Format setting constants */
#define RTC__HOUR_12 1
#define RTC__HOUR_24 0

/*  Date Format setting constants */
#define RTC__MM_DD_YYYY 0
#define RTC__DD_MM_YYYY 1
#define RTC__YYYY_MM_DD 2

/* Date Type setting constants */
#define RTC__RELATIVE 0
#define RTC__FIXED 1

/* DST Hour setting constants */
#define RTC__H0000 0
#define RTC__H0100 1
#define RTC__H0200 2
#define RTC__H0300 3
#define RTC__H0400 4
#define RTC__H0500 5
#define RTC__H0600 6
#define RTC__H0700 7
#define RTC__H0800 8
#define RTC__H0900 9
#define RTC__H1000 10
#define RTC__H1100 11
#define RTC__H1200 12
#define RTC__H1300 13
#define RTC__H1400 14
#define RTC__H1500 15
#define RTC__H1600 16
#define RTC__H1700 17
#define RTC__H1800 18
#define RTC__H1900 19
#define RTC__H2000 20
#define RTC__H2100 21
#define RTC__H2200 22
#define RTC__H2300 23

/* DST DayOfWeek setting constants */
#define RTC__SUNDAY 1
#define RTC__MONDAY 2
#define RTC__TUESDAY 3
#define RTC__WEDNESDAY 4
#define RTC__THURSDAY 5
#define RTC__FRIDAY 6
#define RTC__SATURDAY 7

/* DST Month setting constants */
#define RTC__JAN 1
#define RTC__FEB 2
#define RTC__MAR 3
#define RTC__APR 4
#define RTC__MAY 5
#define RTC__JUN 6
#define RTC__JUL 7
#define RTC__AUG 8
#define RTC__SEP 9
#define RTC__OCT 10
#define RTC__NOV 11
#define RTC__DEC 12

/* DST WeekOfMonth setting constants */
#define RTC__FIRST 1
#define RTC__SECOND 2
#define RTC__THIRD 3
#define RTC__FOURTH 4
#define RTC__FIFTH 5
#define RTC__LAST 6



#define RTC_INITIAL_DATA_FORMAT     (0u)
#define RTC_INITIAL_TIME_FORMAT     (0u)
#define RTC_INITIAL_UPDATE_MODE     (1u)
#define RTC_INITIAL_SECOND          (0u)
#define RTC_INITIAL_MINUTE          (0u)
#define RTC_INITIAL_HOUR            (0u)
#define RTC_INITIAL_DAY             (1u)
#define RTC_INITIAL_MONTH           (1u)
#define RTC_INITIAL_YEAR            (1970u)

#define RTC_INITIAL_ALARM_STATUS    (0u)

#define RTC_INITIAL_DST_STATUS      (0u)
#define RTC_INITIAL_DST_DATE_TYPE   (0u)
#define RTC_INITIAL_DST_START_MONTH (3uL)
#define RTC_INITIAL_DST_START_WOM   (6uL)
#define RTC_INITIAL_DST_START_DOM   (22uL)
#define RTC_INITIAL_DST_START_DOW   (1uL)
#define RTC_INITIAL_DST_START_HRS   (0uL)
#define RTC_INITIAL_DST_STOP_MONTH  (10uL)
#define RTC_INITIAL_DST_STOP_DOM    (22uL)
#define RTC_INITIAL_DST_STOP_DOW    (1uL)
#define RTC_INITIAL_DST_STOP_WOM    (6uL)
#define RTC_INITIAL_DST_STOP_HRS    (0uL)


/* Day of week definitions */
#define RTC_SUNDAY     (RTC__SUNDAY)
#define RTC_MONDAY     (RTC__MONDAY)
#define RTC_TUESDAY    (RTC__TUESDAY)
#define RTC_WEDNESDAY  (RTC__WEDNESDAY)
#define RTC_THURSDAY   (RTC__THURSDAY)
#define RTC_FRIDAY     (RTC__FRIDAY)
#define RTC_SATURDAY   (RTC__SATURDAY)

/* DST WeekOfMonth setting constants definitions */
#define RTC_FIRST      (RTC__FIRST)
#define RTC_SECOND     (RTC__SECOND)
#define RTC_THIRD      (RTC__THIRD)
#define RTC_FOURTH     (RTC__FOURTH)
#define RTC_FIFTH      (RTC__FIFTH)
#define RTC_LAST       (RTC__LAST)

/* Month definitions */
#define RTC_JANUARY    (RTC__JAN)
#define RTC_FEBRUARY   (RTC__FEB)
#define RTC_MARCH      (RTC__MAR)
#define RTC_APRIL      (RTC__APR)
#define RTC_MAY        (RTC__MAY)
#define RTC_JUNE       (RTC__JUN)
#define RTC_JULY       (RTC__JUL)
#define RTC_AUGUST     (RTC__AUG)
#define RTC_SEPTEMBER  (RTC__SEP)
#define RTC_OCTOBER    (RTC__OCT)
#define RTC_NOVEMBER   (RTC__NOV)
#define RTC_DECEMBER   (RTC__DEC)

#define RTC_DAYS_PER_WEEK       (7u)

/* Number of days in month definitions */
#define RTC_DAYS_IN_JANUARY     (31u)
#define RTC_DAYS_IN_FEBRUARY    (28u)
#define RTC_DAYS_IN_MARCH       (31u)
#define RTC_DAYS_IN_APRIL       (30u)
#define RTC_DAYS_IN_MAY         (31u)
#define RTC_DAYS_IN_JUNE        (30u)
#define RTC_DAYS_IN_JULY        (31u)
#define RTC_DAYS_IN_AUGUST      (31u)
#define RTC_DAYS_IN_SEPTEMBER   (30u)
#define RTC_DAYS_IN_OCTOBER     (31u)
#define RTC_DAYS_IN_NOVEMBER    (30u)
#define RTC_DAYS_IN_DECEMBER    (31u)

#define RTC_MONTHS_PER_YEAR     (12uL)

#define RTC_HOURS_PER_DAY       (24uL)
#define RTC_HOURS_PER_HALF_DAY  (12uL)

#define RTC_SECONDS_PER_MINUTE  (60uL)
#define RTC_SECONDS_PER_HOUR    (3600uL)
#define RTC_SECONDS_PER_DAY     (24uL * 3600uL)

#define RTC_SECONDS_PER_LEAP_YEAR    (366uL * 24uL * 3600uL)
#define RTC_SECONDS_PER_NONLEAP_YEAR (365uL * 24uL * 3600uL)

#define RTC_UNIX_TIME_PM   ((12uL * 3600uL) + 1uL)

/* AM/PM status definitions */
#define RTC_AM                 (0u)
#define RTC_PM                 (1u)

/* Time format definitions */
#define RTC_12_HOURS_FORMAT    (RTC__HOUR_12)
#define RTC_24_HOURS_FORMAT    (RTC__HOUR_24)

/* UNIX time begins in 1970 year.  */
#define RTC_YEAR_0             (1970u)

/* Definition of date register fields */
#if(RTC_INITIAL_DATA_FORMAT == RTC__MM_DD_YYYY)
    #define RTC_10_MONTH_OFFSET   (28u)
    #define RTC_MONTH_OFFSET      (24u)
    #define RTC_10_DAY_OFFSET     (20u)
    #define RTC_DAY_OFFSET        (16u)
    #define RTC_1000_YEAR_OFFSET  (12u)
    #define RTC_100_YEAR_OFFSET   (8u)
    #define RTC_10_YEAR_OFFSET    (4u)
    #define RTC_YEAR_OFFSET       (0u)
#elif(RTC_INITIAL_DATA_FORMAT == RTC__DD_MM_YYYY)
    #define RTC_10_MONTH_OFFSET   (20u)
    #define RTC_MONTH_OFFSET      (16u)
    #define RTC_10_DAY_OFFSET     (28u)
    #define RTC_DAY_OFFSET        (24u)
    #define RTC_1000_YEAR_OFFSET  (12u)
    #define RTC_100_YEAR_OFFSET   (8u)
    #define RTC_10_YEAR_OFFSET    (4u)
    #define RTC_YEAR_OFFSET       (0u)
#else
    #define RTC_10_MONTH_OFFSET   (12u)
    #define RTC_MONTH_OFFSET      (8u)
    #define RTC_10_DAY_OFFSET     (4u)
    #define RTC_DAY_OFFSET        (0u)
    #define RTC_1000_YEAR_OFFSET  (28u)
    #define RTC_100_YEAR_OFFSET   (24u)
    #define RTC_10_YEAR_OFFSET    (20u)
    #define RTC_YEAR_OFFSET       (16u)
#endif /* (RTC_INITIAL_DATA_FORMAT == RTC__MM_DD_YYYY) */

#define RTC_10_MONTH_MASK          (0x00000001uL << RTC_10_MONTH_OFFSET)
#define RTC_MONTH_MASK             (0x0000000FuL << RTC_MONTH_OFFSET)
#define RTC_10_DAY_MASK            (0x00000003uL << RTC_10_DAY_OFFSET)
#define RTC_DAY_MASK               (0x0000000FuL << RTC_DAY_OFFSET)
#define RTC_1000_YEAR_MASK         (0x00000003uL << RTC_1000_YEAR_OFFSET)
#define RTC_100_YEAR_MASK          (0x0000000FuL << RTC_100_YEAR_OFFSET)
#define RTC_10_YEAR_MASK           (0x0000000FuL << RTC_10_YEAR_OFFSET)
#define RTC_YEAR_MASK              (0x0000000FuL << RTC_YEAR_OFFSET)

#define  RTC_MONTH_FULL_MASK       (RTC_10_MONTH_MASK  | RTC_MONTH_MASK)
#define  RTC_DAY_FULL_MASK         (RTC_10_DAY_MASK    | RTC_DAY_MASK)
#define  RTC_YEAR_FULL_MASK        (RTC_1000_YEAR_MASK | RTC_100_YEAR_MASK |\
                                                 RTC_10_YEAR_MASK   | RTC_YEAR_MASK)


/* Definition of time register fields */
#define RTC_TIME_FORMAT_OFFSET     (23u)
#define RTC_PERIOD_OF_DAY_OFFSET   (22u)
#define RTC_10_HOURS_OFFSET        (20u)
#define RTC_HOURS_OFFSET           (16u)
#define RTC_10_MINUTES_OFFSET      (12u)
#define RTC_MINUTES_OFFSET         (8u)
#define RTC_10_SECONDS_OFFSET      (4u)
#define RTC_SECONDS_OFFSET         (0u)

#define RTC_TIME_FORMAT_MASK       (0x00000001uL << RTC_TIME_FORMAT_OFFSET)
#define RTC_PERIOD_OF_DAY_MASK     (0x00000001uL << RTC_PERIOD_OF_DAY_OFFSET)
#define RTC_10_HOURS_MASK          (0x00000003uL << RTC_10_HOURS_OFFSET)
#define RTC_HOURS_MASK             (0x0000000FuL << RTC_HOURS_OFFSET)
#define RTC_10_MINUTES_MASK        (0x00000007uL << RTC_10_MINUTES_OFFSET)
#define RTC_MINUTES_MASK           (0x0000000FuL << RTC_MINUTES_OFFSET)
#define RTC_10_SECONDS_MASK        (0x00000007uL << RTC_10_SECONDS_OFFSET)
#define RTC_SECONDS_MASK           (0x0000000FuL << RTC_SECONDS_OFFSET)

#define RTC_HOURS_FULL_MASK        (RTC_10_HOURS_MASK   | RTC_HOURS_MASK)
#define RTC_MINUTES_FULL_MASK      (RTC_10_MINUTES_MASK | RTC_MINUTES_MASK)
#define RTC_SECONDS_FULL_MASK      (RTC_10_SECONDS_MASK | RTC_SECONDS_MASK)

#define RTC_ALARM_SEC_MASK         (0x00000001uL)
#define RTC_ALARM_MIN_MASK         (0x00000002uL)
#define RTC_ALARM_HOUR_MASK        (0x00000004uL)
#define RTC_ALARM_DAYOFWEEK_MASK   (0x00000008uL)
#define RTC_ALARM_DAYOFMONTH_MASK  (0x00000010uL)
#define RTC_ALARM_MONTH_MASK       (0x00000020uL)
#define RTC_ALARM_YEAR_MASK        (0x00000040uL)

#define RTC_STATUS_DST_OFFSET      (1u)
#define RTC_STATUS_ALARM_OFFSET    (2u)
#define RTC_STATUS_LY_OFFSET       (3u)
#define RTC_STATUS_AM_PM_OFFSET    (4u)

/* Definition of the RTC status values. */
#define RTC_STATUS_DST             (1uL << RTC_STATUS_DST_OFFSET)
#define RTC_STATUS_LY              (1uL << RTC_STATUS_LY_OFFSET)
#define RTC_STATUS_AM_PM           (1uL << RTC_STATUS_AM_PM_OFFSET)

/* Number of bits per one BCD digit. */
#define RTC_BCD_NUMBER_SIZE        (4u)
#define RTC_BCD_ONE_DIGIT_MASK     (0x0000000FuL)

/***************************************
*    Function Prototypes
***************************************/
void   RTC_Start(void);
void   RTC_Stop(void);
void   RTC_Init(void);
void   RTC_SetUnixTime(uint64 unixTime);
uint64 RTC_GetUnixTime(void);
void   RTC_SetPeriod(uint32 ticks, uint32 refOneSecTicks);
uint32 RTC_GetPeriod(void);
uint32 RTC_GetRefOneSec(void);
void   RTC_SetDateAndTime(uint32 inputTime, uint32 inputDate);
void   RTC_GetDateAndTime(RTC_DATE_TIME* dateTime);
uint32 RTC_GetTime(void);
uint32 RTC_GetDate(void);
void   RTC_SetAlarmDateAndTime(const RTC_DATE_TIME* alarmTime);
void   RTC_GetAlarmDateAndTime(RTC_DATE_TIME* alarmTimeDate);
void   RTC_SetAlarmMask(uint32 mask);
uint32 RTC_GetAlarmMask(void);
uint32 RTC_ReadStatus(void);
uint32 RTC_GetAlarmStatus(void);
void   RTC_ClearAlarmStatus(void);
void   RTC_SetDSTStartTime(const RTC_DST_TIME* dstStartTime,
                                              RTC_DST_DATETYPE_ENUM type);
void   RTC_SetDSTStopTime(const RTC_DST_TIME* dstStopTime,
                                             RTC_DST_DATETYPE_ENUM type);
uint32 RTC_ConvertBCDToDec(uint32 bcdNum);
uint32 RTC_ConvertDecToBCD(uint32 decNum);
void   RTC_Update(void);
void*  RTC_SetAlarmHandler(void (*CallbackFunction)(void));

static uint32 RTC_ConstructDate(uint32 month, uint32 day, uint32 year);
static uint32 RTC_ConstructTime(uint32 timeFormat, uint32 stateAmPm, uint32 hour, uint32 min, uint32 sec);

static uint32 RTC_GetTimeFormat(uint32 inputTime);
static uint32 RTC_SetTimeFormat(uint32 inputTime, uint32 timeFormat);

static uint32 RTC_LeapYear(uint32 year);
static uint32 RTC_IsBitSet(uint32 var, uint32 mask);
static uint32 RTC_GetSecond(uint32 inputTime);
static uint32 RTC_GetMinutes(uint32 inputTime);
static uint32 RTC_GetHours(uint32 inputTime);
static uint32 RTC_GetAmPm(uint32 inputTime);
static uint32 RTC_GetDay(uint32 date);
static uint32 RTC_GetMonth(uint32 date);
static uint32 RTC_GetYear(uint32 date);

static uint32 RTC_SetSecond(uint32 inputTime, uint32 inputSecond);
static uint32 RTC_SetMinutes(uint32 inputTime, uint32 inputMinute);
static uint32 RTC_SetHours(uint32 inputTime, uint32 inputHours);
static uint32 RTC_SetAmPm(uint32 inputTime, uint32 periodOfDay);
static uint32 RTC_SetDay(uint32 inputDate, uint32 inputDay);
static uint32 RTC_SetMonth(uint32 inputDate, uint32 inputMonth);
static uint32 RTC_SetYear(uint32 inputDate, uint32 inputYear);

uint64 RTC_ConstructUnixAlarmTime(const RTC_DATE_TIME* alarmTime, uint8 alarmCfgMask);
uint64 RTC_GetDstUnixTime(const RTC_DST_TIME* dstTime);
uint64 RTC_GetNexAlarmTime(uint64 curUnixTime, uint8 alarmCfgMask);

static uint32 RTC_GetNextMinSec(uint32 curVal);
static uint32 RTC_GetNextHour(uint32 curVal);
static uint32 RTC_GetNextDay(uint32 curYear, uint32 curMonth, uint32 curDay, uint32 alarmCfgMask);
static uint32 RTC_GetNextMonth(uint32 curMonth);
static uint32 RTC_GetNextYear(uint32 curYear);
static uint32 RTC_GetDayOfWeek(uint32 day, uint32 month, uint32 year);

uint32 RTC_DaysInMonth(uint32 month, uint32 year);
uint32 RTC_DaysBeforeMonth(uint32 month, uint32 year);

void   RTC_UnixToDateTime(RTC_DATE_TIME* dateTime, uint64 unixTime, uint32 timeFormat);
uint64 RTC_DateTimeToUnix(uint32 inputDate, uint32 inputTime);
uint32 RTC_RelativeToFixed(uint32 dayOfWeek, uint32 weekOfMonth, uint32 month, uint32 year);

#if defined(CYDEV_RTC_SOURCE_WDT)
    #if((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_INITIAL_UPDATE_MODE))
        static void RTC_CySysRtcSetCallback(uint32 wdtNumber);
        static void RTC_CySysRtcResetCallback(uint32 wdtNumber);
    #endif /* ((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_INITIAL_UPDATE_MODE)) */
#endif /* (CYDEV_RTC_SOURCE_WDT) */

/*******************************************************************************
* Function Name: RTC_IsBitSet
********************************************************************************
*
* Summary:
*  Checks the state of a bit passed through parameter.
*
* Parameters:
*  var:    The variable to be checked.
*  mask:   The mask for a bit to be checked.
*
* Return:
*  0u - Bit is not set.
*  1u - Bit is set.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_IsBitSet(uint32 var, uint32 mask)
{
    return ((mask == (var & mask)) ? 1Lu : 0Lu);
}


/*******************************************************************************
* Function Name: RTC_LeapYear
********************************************************************************
*
* Summary:
* Checks whether the year passed through the parameter is
* leap or no.
*
* Parameters:
*  year: The year to be checked.
*
*
* Return:
*  0u - The year is not leap.
*  1u - The year is leap.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_LeapYear(uint32 year)
{
    uint32 retVal;

    if(((0u == (year % 4Lu)) && (0u != (year % 100Lu))) || (0u == (year % 400Lu)))
    {
        retVal = 1uL;
    }
    else
    {
        retVal = 0uL;
    }

    return(retVal);
}


/*******************************************************************************
* Function Name: RTC_GetSecond
********************************************************************************
*
* Summary:
*  Returns the seconds value from the time value that is passed as a/the parameter.
*
* Parameters:
*  time: The time value.
*
* Return:
*  The seconds value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetSecond(uint32 inputTime)
{
    uint32 retVal;

    retVal  = ((inputTime & RTC_10_SECONDS_MASK) >> RTC_10_SECONDS_OFFSET) * 10u;
    retVal += (inputTime & RTC_SECONDS_MASK) >> RTC_SECONDS_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_GetMinutes
********************************************************************************
*
* Summary:
*  Returns the minutes value from the time value that is passed as a/the parameter.
*
* Parameters:
*  time: The time value.
*
* Return:
*  The minutes value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetMinutes(uint32 inputTime)
{
    uint32 retVal;

    retVal  = ((inputTime & RTC_10_MINUTES_MASK) >> RTC_10_MINUTES_OFFSET) * 10u;
    retVal += (inputTime & RTC_MINUTES_MASK) >> RTC_MINUTES_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_GetHours
********************************************************************************
*
* Summary:
*  Returns the hours value from the time value that is passed as a/the parameter.
*
* Parameters:
*  time: The time value.
*
* Return:
*  Th ehours value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetHours(uint32 inputTime)
{
    uint32 retVal;

    retVal  = ((inputTime & RTC_10_HOURS_MASK) >> RTC_10_HOURS_OFFSET) * 10u;
    retVal += (inputTime & RTC_HOURS_MASK) >> RTC_HOURS_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_GetAmPm
********************************************************************************
*
* Summary:
*  Returns the AM/PM status from the time value that is passed as a/the parameter. ?
*
* Parameters:
*  time: The time value.
*
* Return:
*  RTC_AM - AM.
*  RTC_PM - PM.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetAmPm(uint32 inputTime)
{
    return (RTC_IsBitSet(inputTime, RTC_PERIOD_OF_DAY_MASK));
}


/*******************************************************************************
* Function Name: RTC_GetDay
********************************************************************************
*
* Summary:
*  Returns the day value from the date value that is passed as a/the parameter.  ?
*
* Parameters:
*  date: The date value.
*
* Return:
*  The day value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetDay(uint32 date)
{
    uint32 retVal;

    retVal  = ((date & RTC_10_DAY_MASK) >> RTC_10_DAY_OFFSET) * 10u;
    retVal += (date & RTC_DAY_MASK) >> RTC_DAY_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_GetMonth
********************************************************************************
*
* Summary:
*  Returns the month value from the date value that is passed as a/the parameter.
*
* Parameters:
*  date: The date value.
*
* Return:
*  The month value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetMonth(uint32 date)
{
    uint32 retVal;

    retVal  = ((date & RTC_10_MONTH_MASK) >> RTC_10_MONTH_OFFSET) * 10u;
    retVal += (date & RTC_MONTH_MASK) >> RTC_MONTH_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_GetYear
********************************************************************************
*
* Summary:
*  Returns the year value from the date value that is passed as a/the parameter.?
*
* Parameters:
*  date: The date value.
*
* Return:
*  The year value.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetYear(uint32 date)
{
    uint32 retVal;

    retVal  = ((date & RTC_1000_YEAR_MASK) >> RTC_1000_YEAR_OFFSET) * 1000u;
    retVal += ((date & RTC_100_YEAR_MASK) >> RTC_100_YEAR_OFFSET) * 100u;
    retVal += ((date & RTC_10_YEAR_MASK) >> RTC_10_YEAR_OFFSET) * 10u;
    retVal += (date & RTC_YEAR_MASK) >> RTC_YEAR_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_SetSecond
********************************************************************************
*
* Summary:
*  Updates the time with the new second value.
*
* Parameters:
*  time: The current date.
*  second: The seconds value to be set to the time variable.
*
* Return:
*  The updated time variable.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetSecond(uint32 inputTime, uint32 inputSecond)
{
    inputTime &= ~(RTC_SECONDS_FULL_MASK);

    inputTime |= (inputSecond / 10u) << RTC_10_SECONDS_OFFSET;
    inputTime |= (inputSecond % 10u) << RTC_SECONDS_OFFSET;

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_SetMinutes
********************************************************************************
*
* Summary:
*  Updates the time with the new minute value.
*
* Parameters:
*  time: The current date.
*  minutes: The minutes value to be set to the time variable.
*
* Return:
*  The updated time variable.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetMinutes(uint32 inputTime, uint32 inputMinute)
{
    inputTime &= ~(RTC_MINUTES_FULL_MASK);

    inputTime |= (inputMinute / 10u) << RTC_10_MINUTES_OFFSET;
    inputTime |= (inputMinute % 10u) << RTC_MINUTES_OFFSET;

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_SetHours
********************************************************************************
*
* Summary:
*  Updates the time with the new hour value.
*
* Parameters:
*  time: The current date.
*  hours: The hours value to be set to the time variable.
*
* Return:
*  The updated time variable.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetHours(uint32 inputTime, uint32 inputHours)
{
    inputTime &= ~(RTC_HOURS_FULL_MASK);

    inputTime |= (inputHours / 10u) << RTC_10_HOURS_OFFSET;
    inputTime |= (inputHours % 10u) << RTC_HOURS_OFFSET;

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_SetAmPm
********************************************************************************
*
* Summary:
*  Updates the time variable with the AmPm status.
*
* Parameters:
*  time: The current date.
*  periodOfDay: The AmPm status to be set to the time variable.
*
* Return:
*  The updated time variable.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetAmPm(uint32 inputTime, uint32 periodOfDay)
{
    if(0u != periodOfDay)
    {
        inputTime &= ~(RTC_PERIOD_OF_DAY_MASK);
    }
    else
    {
        inputTime |= RTC_PERIOD_OF_DAY_MASK;
    }

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_SetDay
********************************************************************************
*
* Summary:
*  Updates the date variable with the new day value.
*
* Parameters:
*  date: The current date.
*  day: The month to be set to the date variable.
*
* Return:
*  The updated date variable.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetDay(uint32 inputDate, uint32 inputDay)
{
    inputDate &= ~(RTC_DAY_FULL_MASK);

    inputDate |= (inputDay / 10u) << RTC_10_DAY_OFFSET;
    inputDate |= (inputDay % 10u) << RTC_DAY_OFFSET;

    return(inputDate);
}


/*******************************************************************************
* Function Name: RTC_SetMonth
********************************************************************************
*
* Summary:
*  Updates the date variable with the new month value.
*
* Parameters:
*  date: The current date.
*  month: The month that to be set to the date variable.
*
* Return:
*  The updated date variable.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetMonth(uint32 inputDate, uint32 inputMonth)
{
    inputDate &= ~(RTC_MONTH_FULL_MASK);

    inputDate |= (inputMonth / 10u) << RTC_10_MONTH_OFFSET;
    inputDate |= (inputMonth % 10u) << RTC_MONTH_OFFSET;

    return(inputDate);
}


/*******************************************************************************
* Function Name: RTC_SetYear
********************************************************************************
*
* Summary:
*  Updates the date variable with the new year value.
*
* Parameters:
*  date: The current date.
*  year: The year to be set to the date variable.
*
* Return:
*  The updated date variable.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetYear(uint32 inputDate, uint32 inputYear)
{
    inputDate  &= ~(RTC_YEAR_FULL_MASK);

    inputDate |= (inputYear / 1000u) << RTC_1000_YEAR_OFFSET;
    inputYear %= 1000u;

    inputDate |= (inputYear / 100u) << RTC_100_YEAR_OFFSET;
    inputYear %= 100u;

    inputDate |= (inputYear / 10u) << RTC_10_YEAR_OFFSET;
    inputDate |= (inputYear % 10u) << RTC_YEAR_OFFSET;

    return(inputDate);
}


/*******************************************************************************
* Function Name: RTC_GetNextMinSec
********************************************************************************
*
* Summary:
*  This is an internal function that calculates the value of the next Second/Minute
*  that follows after the current Minute/Second.
*
* Parameters:
*  curVal: The current Second/Minute value.
*
* Return:
*  Returns the Second/Minute which follows after the current Second/Minute.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetNextMinSec(uint32 curVal)
{
    return((curVal < 59u) ? (curVal + 1u) : 0u);
}


/*******************************************************************************
* Function Name: RTC_GetNextHour
********************************************************************************
*
* Summary:
*  This is an internal function that calculates the value of the next Hour
*  that follows after the current Hour.
*
* Parameters:
*  curVal: The current Hour value.
*
* Return:
*  Returns the Hour which follows after the current Hour.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetNextHour(uint32 curVal)
{
    return((curVal < 23u) ? (curVal + 1u) : 0u);
}


/*******************************************************************************
* Function Name: RTC_GetNextDay
********************************************************************************
*
* Summary:
*  This is an internal function that calculates the value of the next Day
*  that follows after the current Day.
*
* Parameters:
*  curYear         : The current year.
*  curMonth        : The current month.
*  curDay       : The current day.
*  alarmCfgMask : Alarm Config Mask.
*
* Return:
*  Returns the day which follows after the current Day.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetNextDay(uint32 curYear, uint32 curMonth, uint32 curDay, uint32 alarmCfgMask)
{
    uint32 daysInMonth;
    uint32 tmpVal;

    daysInMonth = RTC_DaysInMonth(curMonth, curYear);

    if(0u != (alarmCfgMask & RTC_ALARM_DAYOFWEEK_MASK))
    {
        tmpVal = curDay + RTC_DAYS_PER_WEEK;
        tmpVal = (tmpVal > daysInMonth) ? (tmpVal - daysInMonth) : tmpVal;
    }
    else
    {
        tmpVal = (curDay < daysInMonth) ? (curDay + 1u) : 1u;
    }

    return(tmpVal);
}


/*******************************************************************************
* Function Name: RTC_GetNextMonth
********************************************************************************
*
* Summary:
*  This is an internal function that calculates the value of the next month
*  that follows after the current month.
*
* Parameters:
*  curMonth : The current month.
*
* Return:
*  Returns the month which follows after the current month.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetNextMonth(uint32 curMonth)
{
    return((curMonth < 12u) ? (curMonth + 1u) : 1u);
}


/*******************************************************************************
* Function Name: RTC_GetNextYear
********************************************************************************
*
* Summary:
*  This is an internal function that calculates the value of the next year
*  that follows after the current year.
*
* Parameters:
*  curYear : The current year.
*
* Return:
*  Returns the year which follows after the current year.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetNextYear(uint32 curYear)
{
    return(curYear + 1u);
}


/*******************************************************************************
* Function Name: RTC_SetTimeFormat
********************************************************************************
*
* Summary:
*  Updates the "Time Format" value in the variable that contains time in the
*  "HH:MM:SS" format.
*
* Parameters:
*  inputTime: The current value of the time in the "HH:MM:SS" format.
*
* Return:
*  Returns the updated value of the time in the "HH:MM:SS" format.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_SetTimeFormat(uint32 inputTime, uint32 timeFormat)
{
    inputTime &= ~(RTC_TIME_FORMAT_MASK);

    if((uint32)RTC_12_HOURS_FORMAT != timeFormat)
    {
        inputTime |= RTC_TIME_FORMAT_MASK;
    }

    return(inputTime);
}


/*******************************************************************************
* Function Name: RTC_GetTimeFormat
********************************************************************************
*
* Summary:
*  Reads the time format from the variable that contains time in the
*  "HH:MM:SS" format.
*
* Parameters:
*  inputTime:  The current value of the time in the "HH:MM:SS" format.
*
* Return:
*  Returns the time format that is stored in the variable that contains time
*  in the "HH:MM:SS" format.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetTimeFormat(uint32 inputTime)
{
    return ((0uL != (inputTime & (1uL << RTC_TIME_FORMAT_OFFSET))) ? (uint32)RTC_12_HOURS_FORMAT :
                                                                                  (uint32)RTC_24_HOURS_FORMAT);
}


/*******************************************************************************
* Function Name: RTC_ConstructTime
********************************************************************************
*
* Summary:
*  Returns the time in the format used in APIs from individual elements
*  passed (hour, min, sec etc)
*
* Parameters:
*  timeFormat: The 12/24 hours time format:
*              RTC_24_HOURS_FORMAT - The 24 hours format.
*               RTC_12_HOURS_FORMAT - The 12 hours format.
*
*  stateAmPm:  The AM/PM status:
*              RTC_AM - AM.
*              RTC_PM - PM.
*
*  hour:       The hour.
*  min:        The minute.
*  sec:        The second.
*
* Return:
*  Time in the format used in API.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_ConstructTime(uint32 timeFormat, uint32 stateAmPm, uint32 hour, uint32 min, uint32 sec)
{
    uint32 retVal;

    retVal  = timeFormat << RTC_TIME_FORMAT_OFFSET;
    retVal |= stateAmPm << RTC_PERIOD_OF_DAY_OFFSET;

    retVal |= (hour / 10u) << RTC_10_HOURS_OFFSET;
    retVal |= (hour % 10u) << RTC_HOURS_OFFSET;

    retVal |= (min / 10u) << RTC_10_MINUTES_OFFSET;
    retVal |= (min % 10u) << RTC_MINUTES_OFFSET;

    retVal |= (sec / 10u) << RTC_10_SECONDS_OFFSET;
    retVal |= (sec % 10u) << RTC_SECONDS_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_ConstructDate
********************************************************************************
*
* Summary:
*  Returns the date in the format used in APIs from individual elements
*  passed (day. Month and year)
*
* Parameters:
*  month:       The month.
*  day:         The day.
*  year:        The year.
*
* Return:
*  The date in the format used in API.
*
*******************************************************************************/
static CY_INLINE uint32 RTC_ConstructDate(uint32 month, uint32 day, uint32 year)
{
    uint32 retVal;
    uint32 tmpVal = year;

    retVal  = (month / 10u) << RTC_10_MONTH_OFFSET;
    retVal |= (month % 10u) << RTC_MONTH_OFFSET;

    retVal |= (day / 10u) << RTC_10_DAY_OFFSET;
    retVal |= (day % 10u) << RTC_DAY_OFFSET;

    retVal |= (year / 1000u) << RTC_1000_YEAR_OFFSET;
    tmpVal %= 1000u;

    retVal |= (tmpVal / 100u) << RTC_100_YEAR_OFFSET;
    tmpVal %= 100u;

    retVal |= (tmpVal / 10u) << RTC_10_YEAR_OFFSET;
    retVal |= (tmpVal % 10u) << RTC_YEAR_OFFSET;

    return (retVal);
}


/*******************************************************************************
* Function Name: RTC_GetDayOfWeek
********************************************************************************
*
* Summary:
*  Returns a day of the week for a year, month, and day of month that are passed
*  through parameters. Zeller's congruence is used to calculate the day of
*  the week.
*
*  For the Georgian calendar, Zeller's congruence is:
*  h = (q + [13 * (m + 1)] + K + [K/4] + [J/4] - 2J) mod 7
*
*  h - The day of the week (0 = Saturday, 1 = Sunday, 2 = Monday, ..., 6 = Friday).
*  q - The day of the month.
*  m - The month (3 = March, 4 = April, 5 = May, ..., 14 = February)
*  K - The year of the century (year \mod 100).
*  J - The zero-based century (actually [year/100]) For example, the zero-based
*      centuries for 1995 and 2000 are 19 and 20 respectively (not to be
*      confused with the common ordinal century enumeration which indicates
*      20th for both cases).
*
* Note: In this algorithm January and February are counted as months 13 and 14
* of the previous year.
*
* Parameters:
*  day  : The day of the month(1..31)
*
*  month: The month of the year:
*                   RTC_JANUARY
*                  RTC_FEBRUARY
*                  RTC_MARCH
*                  RTC_APRIL
*                  RTC_MAY
*                  RTC_JUNE
*                  RTC_JULY
*                  RTC_AUGUST
*                  RTC_SEPTEMBER
*                  RTC_OCTOBER
*                  RTC_NOVEMBER
*                  RTC_DECEMBER
*
*  year:  The year value.
*
* Return:
*  Returns a day of the week: ?
*                RTC_SUNDAY
*                RTC_MONDAY
*                RTC_TUESDAY
*                RTC_WEDNESDAY
*                RTC_THURSDAY
*                RTC_FRIDAY
*                RTC_SATURDAY
*
*******************************************************************************/
static CY_INLINE uint32 RTC_GetDayOfWeek(uint32 day, uint32 month, uint32 year)
{
    uint32 retVal;

    /* Converts month number from regular convention
     * (1=January,..., 12=December) to convention required for this
     * algorithm(January and February are counted as months 13 and 14 of
     * previous year).
    */
    if(month < (uint32)RTC_MARCH)
    {
        month = 12u + month;
        year--;
    }

    /* Calculates Day of Week using Zeller's congruence algorithms.
    */
    retVal = (day + (((month + 1u) * 26u) / 10u) + year + (year / 4u) + (6u * (year / 100u)) + (year / 400u)) % 7u;

    /* Makes correction for Saturday. Saturday number should be 7 instead of 0. */
    if(0u == retVal)
    {
        retVal = (uint32)RTC_SATURDAY;
    }

    return(retVal);
}


#if defined(CYDEV_RTC_SOURCE_WDT)
    #if((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_INITIAL_UPDATE_MODE))

        /*******************************************************************************
        * Function Name: RTC_CySysRtcSetCallback
        ********************************************************************************
        *
        * Summary:
        *  This is an internal function that registers a callback for the
        *  RTC_Update() function by address "0".
        *
        * Parameters:
        *  wdtNumber: The number of the WDT timer to be used to pull the
        *             RTC_Update() function.
        *
        * Return:
        *  None.
        *
        * Side Effects:
        *  The callback registered before by address "0" is replaced
        *  by the RTC_Update() function.
        *
        *******************************************************************************/
        static CY_INLINE void RTC_CySysRtcSetCallback(uint32 wdtNumber)
        {
            (void)CySysWdtSetIsrCallback(wdtNumber, RTC_Update);
        }


        /*******************************************************************************
        * Function Name: RTC_CySysRtcResetCallback
        ********************************************************************************
        *
        * Summary:
        *  This is an internal function that clears a callback by address "0".
        *
        * Parameters:
        *  wdtNumber: The number of the WDT timer to be cleared callback for.
        *
        * Return:
        *  None.
        *
        * Side Effects:
        *  The callback registered before by address "0" is replaced
        *  by the NULL pointer.
        *
        *******************************************************************************/
        static CY_INLINE void RTC_CySysRtcResetCallback(uint32 wdtNumber)
        {
            (void)CySysWdtSetIsrCallback(wdtNumber, (void *)0);
        }

    #endif /* ((0u != CYDEV_WDT_GENERATE_ISR) && (0u == RTC_INITIAL_UPDATE_MODE)) */
#endif /* (CYDEV_RTC_SOURCE_WDT) */

/***************************************
*    External Variables
***************************************/
extern uint8  RTC_initVar;
extern uint8  RTC_dstStatus;

extern volatile uint64 RTC_unixTime;

extern RTC_DST_TIME RTC_dstStartTime;
extern RTC_DST_TIME RTC_dstStopTime;
extern RTC_DATE_TIME RTC_currentTimeDate;
extern RTC_DATE_TIME RTC_alarmCfgTimeDate;

extern uint32  RTC_alarmCfgMask;
extern uint32 RTC_alarmCurStatus;

extern void (*RTC_alarmCallbackPtr)(void);

extern const uint16 RTC_daysBeforeMonthTbl[RTC_MONTHS_PER_YEAR];
extern const uint8  RTC_daysInMonthTbl[RTC_MONTHS_PER_YEAR];


#endif /* CY_RTC_P4_RTC_H */

/* [] END OF FILE */
