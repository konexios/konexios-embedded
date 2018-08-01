/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#include "defs.h"

#include "time/time.h"

#include "system.h"


void get_time(char *out) {
	timestamp_t ts;
	if(!out) return;
	timestamp(&ts);
	sprintf(out,"%02d/%02d/%02d %02d:%02d:%02d\n",ts.mon,ts.day,ts.year,ts.hour,ts.min,ts.sec);
	return;
}

/*
void get_time_of_day(struct timeval *tval)
{
    if(!tval) return;
	tval->tv_sec = g_timestamp;
	tval->tv_usec = 0;
	return';
}
*/

int gettimeofday(struct timeval * tval,  void * tz)
{
    if(tval){
		tval->tv_sec = g_timestamp;
		tval->tv_usec = 0;
	}
    return 0;
}

time_t time(time_t *timer) {
    return (time_t)g_timestamp;
}

int stime(const time_t *timer)
{
	uint32_t tm;
	tm = *((uint32_t*)timer);
	//printf("tm is %d\n",tm);
	g_timestamp = tm;
    return 0;
}

int msleep(int m_sec) {
  // FIXME implementation
  nop();
  return 0;
}

void timestamp(timestamp_t *tsptr)
{
	struct tm lt;
	localtime_r((time_t *)&g_timestamp, &lt);
	tsptr->mon = (lt.tm_mon+1);
	tsptr->day = lt.tm_mday;
	tsptr->year = (lt.tm_year+1900);
	tsptr->hour = lt.tm_hour;
	tsptr->min = lt.tm_min;
	tsptr->sec = lt.tm_sec;
	tsptr->msec = 0;
}

/*
// https://stackoverflow.com/questions/7136385/calculate-day-number-from-an-unix-timestamp-in-a-math-way
#define SEC_PER_HOUR	(60*60)
#define SEC_PER_DAY		(24*SEC_PER_HOUR)
uint8_t days_per_month[] = {31,28,31,30,31,30,31,31,30,31,30,31};
int days;
int years;
int dayofyear;
int x;
int sum;
int leapdays=0;
	
printf("Calc from %d\n",ts);    
// These don't change with leap years
tsptr->sec = (ts % 60);
tsptr->min = ((ts / 60) % 60 );
tsptr->hour = ((ts / SEC_PER_HOUR) % 24);
	
// Days and years since Jan 1 1970
days = (ts  / SEC_PER_DAY);
years = (int)( ((float)days) / 365.25);
tsptr->year = 1970 + years;
printf(" days: %d\n",days);
printf(" years: %d\n",years);
printf(" tsptr->year: %d\n",tsptr->year);
	
// Leapdays are the number of leap years since 1972
if(years>=2) leapdays = ((years-2)/4)+1;
printf(" leapdays: %d\n",leapdays);
	
// day of the year is (days since the start of the year + the number of leap days)
dayofyear = (days-(years*365)+leapdays);
	
// If this years is a leap year
if((tsptr->year % 4)==0)
	days_per_month[1] = 29;

// Find the day of the month	
sum =0;
for(x=0;x<sizeof(days_per_month);x++)
{
	if(dayofyear<= sum+days_per_month[x]){
		tsptr->mon = x+1;
		tsptr->day = dayofyear-sum;
	}
	sum+=days_per_month[x];
}
*/

// EOF