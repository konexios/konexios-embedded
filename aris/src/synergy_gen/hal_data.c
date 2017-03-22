/* generated HAL source file - do not edit */
#include "hal_data.h"
#if RTC_CALLBACK_USED_g_for_wdt
#if defined(__ICCARM__)
#define for_wdt_cb_WEAK_ATTRIBUTE
#pragma weak for_wdt_cb                            = for_wdt_cb_internal
#elif defined(__GNUC__)
#define for_wdt_cb_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("for_wdt_cb_internal")))
#endif
void for_wdt_cb(rtc_callback_args_t * p_args) for_wdt_cb_WEAK_ATTRIBUTE;
#endif
rtc_ctrl_t g_for_wdt_ctrl;
const rtc_cfg_t g_for_wdt_cfg =
{ .clock_source = RTC_CLOCK_SOURCE_LOCO, .error_adjustment_value = 0,
  .error_adjustment_type = RTC_ERROR_ADJUSTMENT_NONE, .p_callback = for_wdt_cb, .p_context = &g_for_wdt, };
/* Instance structure to use this module. */
const rtc_instance_t g_for_wdt =
{ .p_ctrl = &g_for_wdt_ctrl, .p_cfg = &g_for_wdt_cfg, .p_api = &g_rtc_on_rtc };

#if RTC_CALLBACK_USED_g_for_wdt
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void for_wdt_cb(rtc_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void for_wdt_cb_internal(rtc_callback_args_t * p_args);
void for_wdt_cb_internal(rtc_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if WATCHDOG_ON_WDT_CALLBACK_USED
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(wdt_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif

static wdt_ctrl_t g_watchdog_ctrl;

static const wdt_cfg_t g_watchdog_cfg =
{ .start_mode = WDT_START_MODE_REGISTER, .autostart = true, .timeout = WDT_TIMEOUT_16384, .clock_division =
          WDT_CLOCK_DIVISION_8192,
  .window_start = WDT_WINDOW_START_100, .window_end = WDT_WINDOW_END_0, .reset_control = WDT_RESET_CONTROL_RESET,
  .stop_control = WDT_STOP_CONTROL_ENABLE, .p_callback = NULL, };

/* Instance structure to use this module. */
const wdt_instance_t g_watchdog =
{ .p_ctrl = &g_watchdog_ctrl, .p_cfg = &g_watchdog_cfg, .p_api = &g_wdt_on_wdt };

#if WATCHDOG_ON_WDT_CALLBACK_USED
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function
 *             with the prototype below.
 *               - void NULL(wdt_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(wdt_callback_args_t * p_args);
void NULL_internal(wdt_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if RTC_CALLBACK_USED_g_rtc
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(rtc_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
rtc_ctrl_t g_rtc_ctrl;
const rtc_cfg_t g_rtc_cfg =
{ .clock_source = RTC_CLOCK_SOURCE_LOCO, .error_adjustment_value = 0,
  .error_adjustment_type = RTC_ERROR_ADJUSTMENT_NONE, .p_callback = NULL, .p_context = &g_rtc, };
/* Instance structure to use this module. */
const rtc_instance_t g_rtc =
{ .p_ctrl = &g_rtc_ctrl, .p_cfg = &g_rtc_cfg, .p_api = &g_rtc_on_rtc };

#if RTC_CALLBACK_USED_g_rtc
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(rtc_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(rtc_callback_args_t * p_args);
void NULL_internal(rtc_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
#if !defined(BSP_MCU_GROUP_S7G2)
#error  "HASH Driver on SCE Feature available for S7G2 MCU Only"
#endif
hash_ctrl_t g_sce_hash_ctrl;
hash_cfg_t g_sce_hash_cfg =
{ .p_crypto_api = &g_sce_crypto_api };
const hash_instance_t g_sce_hash =
{ .p_ctrl = &g_sce_hash_ctrl, .p_cfg = &g_sce_hash_cfg, .p_api = &g_sha256_hash_on_sce };
#if TIMER_ON_GPT_CALLBACK_USED_g_timer
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(timer_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
static timer_ctrl_t g_timer_ctrl;
static const timer_on_gpt_cfg_t g_timer_extend =
{ .gtioca =
{ .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW },
  .gtiocb =
  { .output_enabled = false, .stop_level = GPT_PIN_LEVEL_LOW } };
static const timer_cfg_t g_timer_cfg =
{ .mode = TIMER_MODE_PERIODIC, .period = 1000000, .unit = TIMER_UNIT_PERIOD_USEC, .duty_cycle = 50, .duty_cycle_unit =
          TIMER_PWM_UNIT_RAW_COUNTS,
  .channel = 0, .autostart = true, .p_callback = NULL, .p_context = &g_timer, .p_extend = &g_timer_extend };
/* Instance structure to use this module. */
const timer_instance_t g_timer =
{ .p_ctrl = &g_timer_ctrl, .p_cfg = &g_timer_cfg, .p_api = &g_timer_on_gpt };

#if TIMER_ON_GPT_CALLBACK_USED_g_timer
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(timer_callback_args_t * p_args)
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(timer_callback_args_t * p_args);
void NULL_internal(timer_callback_args_t * p_args)
{
    /** Do nothing. */
    SSP_PARAMETER_NOT_USED(p_args);
}
#endif
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
const elc_instance_t g_elc =
{ .p_api = &g_elc_on_elc, .p_cfg = NULL };
const cgc_instance_t g_cgc =
{ .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
void g_hal_init(void)
{
    g_common_init ();
}
