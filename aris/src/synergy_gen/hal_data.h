/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_rtc.h"
#include "r_rtc_api.h"
#include "r_wdt.h"
#include "r_wdt_api.h"
#include "r_hash_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
/** RTC on RTC Instance. */
extern const rtc_instance_t g_for_wdt;
#ifdef timer_wdt_cb
#define RTC_CALLBACK_USED_g_for_wdt (0)
#else
#define RTC_CALLBACK_USED_g_for_wdt (1)
#endif
#if RTC_CALLBACK_USED_g_for_wdt
void timer_wdt_cb(rtc_callback_args_t * p_args);
#endif
/** WDT on WDT Instance. */
extern const wdt_instance_t g_watchdog;
#ifdef watchdog_cb
#define WATCHDOG_ON_WDT_CALLBACK_USED (0)
#else
#define WATCHDOG_ON_WDT_CALLBACK_USED (1)
#endif
#if WATCHDOG_ON_WDT_CALLBACK_USED
void watchdog_cb(wdt_callback_args_t * p_args);
#endif
/** RTC on RTC Instance. */
extern const rtc_instance_t g_rtc;
#ifdef NULL
#define RTC_CALLBACK_USED_g_rtc (0)
#else
#define RTC_CALLBACK_USED_g_rtc (1)
#endif
#if RTC_CALLBACK_USED_g_rtc
void NULL(rtc_callback_args_t * p_args);
#endif
extern const hash_instance_t g_sce_hash;
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer;
#ifdef NULL
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (0)
#else
#define TIMER_ON_GPT_CALLBACK_USED_g_timer (1)
#endif
#if TIMER_ON_GPT_CALLBACK_USED_g_timer
void NULL(timer_callback_args_t * p_args);
#endif
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** ELC Instance */
extern const elc_instance_t g_elc;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
void hal_entry(void);
void g_hal_init(void);
#endif /* HAL_DATA_H_ */
