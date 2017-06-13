/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "time/time.h"
#include <debug.h>

#include <cmsis_os.h>

extern RTC_HandleTypeDef hrtc;

int mon2dec(int mon) {
  switch(mon) {
    case RTC_MONTH_JANUARY: return 0;
    case RTC_MONTH_FEBRUARY: return 1;
    case RTC_MONTH_MARCH: return 2;
    case RTC_MONTH_APRIL: return 3;
    case RTC_MONTH_MAY: return 4;
    case RTC_MONTH_JUNE: return 5;
    case RTC_MONTH_JULY: return 6;
    case RTC_MONTH_AUGUST: return 7;
    case RTC_MONTH_SEPTEMBER: return 8;
    case RTC_MONTH_OCTOBER: return 9;
    case RTC_MONTH_NOVEMBER: return 10;
    case RTC_MONTH_DECEMBER: return 11;
  }
  return -1;
}

int dec2mon(int mon) {
  switch(mon) {
    case 0: return RTC_MONTH_JANUARY;
    case 1: return RTC_MONTH_FEBRUARY;
    case 2: return RTC_MONTH_MARCH;
    case 3: return RTC_MONTH_APRIL;
    case 4: return RTC_MONTH_MAY;
    case 5: return RTC_MONTH_JUNE;
    case 6: return RTC_MONTH_JULY;
    case 7: return RTC_MONTH_AUGUST;
    case 8: return RTC_MONTH_SEPTEMBER;
    case 9: return RTC_MONTH_OCTOBER;
    case 10: return RTC_MONTH_NOVEMBER;
    case 11: return RTC_MONTH_DECEMBER;
  }
  return -1;
}

void get_time(char *ts) {
  RTC_TimeTypeDef stime;
  RTC_DateTypeDef sdate;
  struct tm *tmp;
  int ms;
  time_t s = time(NULL);
  HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);
  tmp = gmtime(&s);
  ms = 1000 - (stime.SubSeconds * 1000 / ( stime.SecondFraction - 1 ) );
  sprintf(ts, "%04d-%02d-%02dT%02d:%02d:%02d.%03dZ", 1900+tmp->tm_year, tmp->tm_mon+1, tmp->tm_mday,
          tmp->tm_hour, tmp->tm_min, tmp->tm_sec, ms);
  printf("ts: %s\r\n", ts);
}

int gettimeofday(struct timeval *__restrict __p,  void *__restrict __tz) {
  SSP_PARAMETER_NOT_USED(__tz);
  RTC_TimeTypeDef stime;
  RTC_DateTypeDef sdate;
  HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);
  __p->tv_sec = time(NULL);
  __p->tv_usec = (suseconds_t) 1000000 - (stime.SubSeconds * 1000000 / ( stime.SecondFraction - 1 ) );
  return 0;
}

time_t time(time_t *timer) {
  RTC_TimeTypeDef stime;
  RTC_DateTypeDef sdate;
  struct tm _tm;
  time_t t;

  HAL_RTC_GetTime(&hrtc, &stime, RTC_FORMAT_BIN);
  _tm.tm_hour = stime.Hours;
  _tm.tm_min = stime.Minutes;
  _tm.tm_sec = stime.Seconds;
  _tm.tm_isdst = stime.DayLightSaving;

  HAL_RTC_GetDate(&hrtc, &sdate, RTC_FORMAT_BIN);
  _tm.tm_year = sdate.Year;
  _tm.tm_mon = mon2dec(sdate.Month);
  _tm.tm_mday = sdate.Date;
  _tm.tm_wday = sdate.WeekDay;

  t = mktime(&_tm);
  if ( timer ) *timer = t;
  return t;
}

int stime(const time_t *timer) {
  RTC_TimeTypeDef stime;
  RTC_DateTypeDef sdate;
  struct tm *_tm = localtime(timer);

  stime.Hours = _tm->tm_hour;
  stime.Minutes = _tm->tm_min;
  stime.Seconds = _tm->tm_sec;
  stime.DayLightSaving = _tm->tm_isdst;
  stime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &stime, RTC_FORMAT_BIN) != HAL_OK) {
    return -1;
  }

  sdate.WeekDay = _tm->tm_wday + 1;
  if ( _tm->tm_mon )
  sdate.Month = dec2mon(_tm->tm_mon);
  sdate.Date = _tm->tm_mday;
  sdate.Year = _tm->tm_year;

  if (HAL_RTC_SetDate(&hrtc, &sdate, RTC_FORMAT_BIN) != HAL_OK) {
    return -1;
  }
  return 0;
}

int msleep(int m_sec) {
  osDelay(m_sec);
  return 0;
}
