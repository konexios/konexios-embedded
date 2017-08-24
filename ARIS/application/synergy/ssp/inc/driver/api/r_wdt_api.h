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
 * File Name    : r_wdt_api.h
 * Description  : Watchdog Timer (WDT) driver interface.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup WDT_API WDT Interface
 * @brief Interface for watch dog timer functions.
 *
 * @section WDT_API_Summary Summary
 * The WDT interface for the Watchdog Timer (WDT) peripheral provides watchdog functionality including resetting the
 * device or generating an interrupt.
 *
 * See Also
 * @ref WDT_API and @ref SF_THREAD_MONITOR_API
 *
 * The watchdog timer interface can be implemented by:
 * - @ref WDT
 * - @ref IWDT
 *
 * Related SSP architecture topics:
 * - @ref ssp-interfaces
 * - @ref ssp-predefined-layers
 * - @ref using-ssp-modules
 *
 * WDT Interface description: @ref HALWDTInterface
 * @{
 **********************************************************************************************************************/

#ifndef DRV_WDT_API_H
#define DRV_WDT_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define WDT_API_VERSION_MAJOR (1)
#define WDT_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** WDT time-out periods. */
typedef enum e_wdt_timeout
{
    WDT_TIMEOUT_128 = 0,                        ///< 128 clock cycles
    WDT_TIMEOUT_512,                            ///< 512 clock cycles
    WDT_TIMEOUT_1024,                           ///< 1024 clock cycles
    WDT_TIMEOUT_2048,                           ///< 2048 clock cycles
    WDT_TIMEOUT_4096,                           ///< 4096 clock cycles
    WDT_TIMEOUT_8192,                           ///< 8192 clock cycles
    WDT_TIMEOUT_16384,                          ///< 16384 clock cycles
} wdt_timeout_t;

/** WDT clock division ratio. */
typedef enum e_wdt_clock_division
{
    WDT_CLOCK_DIVISION_1    = 0x0000u,          ///< CLK/1
    WDT_CLOCK_DIVISION_4    = 0x0010u,          ///< CLK/4
    WDT_CLOCK_DIVISION_16   = 0x0020u,          ///< CLK/16
    WDT_CLOCK_DIVISION_32   = 0x0030u,          ///< CLK/32
    WDT_CLOCK_DIVISION_64   = 0x0040u,          ///< CLK/64
    WDT_CLOCK_DIVISION_128  = 0x00F0u,          ///< CLK/128
    WDT_CLOCK_DIVISION_256  = 0x0050u,          ///< CLK/256
    WDT_CLOCK_DIVISION_512  = 0x0060u,          ///< CLK/512
    WDT_CLOCK_DIVISION_2048 = 0x0070u,          ///< CLK/2048
    WDT_CLOCK_DIVISION_8192 = 0x0080u,          ///< CLK/8192
} wdt_clock_division_t;

/** WDT refresh permitted period window start position. */
typedef enum e_wdt_window_start
{
    WDT_WINDOW_START_25  = 0x0000u,             ///< Start position = 25%
    WDT_WINDOW_START_50  = 0x1000u,             ///< Start position = 50%
    WDT_WINDOW_START_75  = 0x2000u,             ///< Start position = 75%
    WDT_WINDOW_START_100 = 0x3000u,             ///< Start position = 100%
} wdt_window_start_t;

/** WDT refresh permitted period window end position. */
typedef enum e_wdt_window_end
{
    WDT_WINDOW_END_75 = 0x0000u,                ///< End position = 75%
    WDT_WINDOW_END_50 = 0x0100u,                ///< End position = 50%
    WDT_WINDOW_END_25 = 0x0200u,                ///< End position = 25%
    WDT_WINDOW_END_0  = 0x0300u,                ///< End position = 0%
} wdt_window_end_t;

/** WDT Counter underflow and refresh error control. */
typedef enum e_wdt_reset_control
{
    WDT_RESET_CONTROL_NMI   = 0x00u,            ///< NMI request when counter underflows.
    WDT_RESET_CONTROL_RESET = 0x80u,            ///< Reset request when counter underflows.
} wdt_reset_control_t;

/** WDT Counter operation in sleep mode. */
typedef enum e_wdt_stop_control
{
    WDT_STOP_CONTROL_DISABLE = 0x00u,           ///< Count will not stop when device enters sleep mode.
    WDT_STOP_CONTROL_ENABLE  = 0x80u,           ///< Count will automatically stop when device enters sleep mode.
} wdt_stop_control_t;

/** WDT status */
typedef enum e_wdt_status
{
    WDT_STATUS_NO_ERROR                    = 0x00u,     ///< No status flags set.
    WDT_STATUS_UNDERFLOW_ERROR             = 0x01u,     ///< Underflow flag set.
    WDT_STATUS_REFRESH_ERROR               = 0x02u,     ///< Refresh error flag set. Refresh outside of permitted
                                                        // window.
    WDT_STATUS_UNDERFLOW_AND_REFRESH_ERROR = 0x03u,     ///< Underflow and refresh error flags set.
} wdt_status_t;

/** WDT start mode. Used to check the WDT is configured correctly.  */
typedef enum e_wdt_start_mode
{
    WDT_START_MODE_REGISTER = 0,                ///< WDT is to be configured using the WDT registers.
    WDT_START_MODE_AUTO,                        ///< WDT is to be configured using OFS0 hardware register.
    WDT_START_MODE_DISABLED                     ///< WDT is disabled.
} wdt_start_mode_t;

/** Callback function parameter data */
typedef struct st_wdt_callback_args
{
    void const  * p_context;   ///< Placeholder for user data.  Set in wdt_api_t::open function in ::wdt_cfg_t.
} wdt_callback_args_t;

/** WDT timeout data. Used to return frequency of WDT clock and timeout period */
typedef struct st_wdt_timeout_values
{
    uint32_t  clock_frequency_hz;               ///< Frequency of watchdog clock after divider.
    uint32_t  timeout_clocks;                   ///< Timeout period in units of watchdog clock ticks.
} wdt_timeout_values_t;

/** WDT control block. DO NOT INITIALIZE.  Initialization occurs when wdt_api_t::open is called. */
typedef struct st_wdt_ctrl
{
    bool        wdt_open;                              ///< Indicates whether the open() API has been successfully
                                                       ///< called.
    void const  * p_context;                           ///< Placeholder for user data.  Passed to the user callback in
                                                       ///< wdt_callback_args_t.
    void (* p_callback)(wdt_callback_args_t * p_args); ///< Callback provided when a WDT NMI ISR occurs.
} wdt_ctrl_t;

/** WDT configuration parameters. */
typedef struct st_wdt_cfg
{
    wdt_start_mode_t  start_mode;                       ///< The expected start mode for the WDT.

    /** When true the WDT is started as part of its configuration (register start mode). If false the WDT needs to be
     * started manually by calling the refresh API. */
    bool                  autostart;
    wdt_timeout_t         timeout;                      ///< Timeout period.
    wdt_clock_division_t  clock_division;               ///< Clock divider.
    wdt_window_start_t    window_start;                 ///< Refresh permitted window start position.
    wdt_window_end_t      window_end;                   ///< Refresh permitted window end position.
    wdt_reset_control_t   reset_control;                ///< Select NMI or reset generated on underflow.
    wdt_stop_control_t    stop_control;                 ///< Select whether counter operates in sleep mode.
    void (* p_callback)(wdt_callback_args_t * p_args);  ///< Callback provided when a WDT NMI ISR occurs.
    /** Placeholder for user data.  Passed to the user callback in wdt_callback_args_t. */
    void const  * p_context;
    void const  * p_extend;								///< Placeholder for user extension.
} wdt_cfg_t;

/** WDT functions implemented at the HAL layer will follow this API. */
typedef struct st_wdt_api
{
    /** Initialize the WDT in register start mode. In auto-start mode with NMI output it registers the NMI callback.
     * @par Implemented as
     * - R_WDT_CfgGet()
     * - R_IWDT_CfgGet()
     *
     * @param[in]  p_ctrl       Pointer to control structure.
     * @param[out] p_cfg        Pointer to pin configuration structure for reading WDT configuration.
     */
    ssp_err_t (* cfgGet)(wdt_ctrl_t * const p_ctrl, wdt_cfg_t  * const p_cfg);

    /** Initialize the WDT in register start mode. In auto-start mode with NMI output it registers the NMI callback.
     * @par Implemented as
     * - R_WDT_Open()
     * - R_IWDT_Open()
     *
     * @param[in]  p_ctrl       Pointer to control structure.
     * @param[in]  p_cfg        Pointer to pin configuration structure.
     */
    ssp_err_t (* open)(wdt_ctrl_t * const p_ctrl, wdt_cfg_t  const * const p_cfg);

    /** Refresh the watchdog timer.
     * @par Implemented as
     * - R_WDT_Refresh()
     * - R_IWDT_Refresh()
     *
     * @pre                     If the WDT is in auto-start mode ensure the OFS0 register is configured before using
     *                          this function.
     * @warning                 Calling this function in register-start mode before calling ::R_WDT_Open will start the
     *                          WDT in it's default state and further changes to the configuration will not be possible.
     * @param[in]  p_ctrl       Pointer to control structure.
     */
    ssp_err_t (* refresh)(wdt_ctrl_t * const p_ctrl);

    /** Read the status of the WDT.
     * @par Implemented as
     * - R_WDT_StatusGet()
     * - R_IWDT_StatusGet()
     *
     * @param[in]  p_ctrl       Pointer to control structure.
     * @param[out] p_status     Pointer to variable to return status information through.
     */
    ssp_err_t (* statusGet)(wdt_ctrl_t * const p_ctrl, wdt_status_t * const p_status);

    /** Clear the status flags of the WDT.
     * @par Implemented as
     * - R_WDT_StatusClear()
     * - R_IWDT_StatusClear()
     *
     * @param[in] p_ctrl        Pointer to control structure.
     * @param[in] status        Status condition(s) to clear.
     */
    ssp_err_t (* statusClear)(wdt_ctrl_t * const p_ctrl, const wdt_status_t status);

    /** Read the current WDT counter value.
     * @par Implemented as
     * - R_WDT_CounterGet()
     * - R_IWDT_CounterGet()
     *
     * @param[in]  p_ctrl       Pointer to control structure.
     * @param[out] p_count      Pointer to variable to return current WDT counter value.
     */
    ssp_err_t (* counterGet)(wdt_ctrl_t * const p_ctrl, uint32_t * const p_count);

    /** Read the watchdog timeout values.
     * @par Implemented as
     * - R_WDT_TimeoutGet()
     * - R_IWDT_TimeoutGet()
     *
     * @param[in]  p_ctrl            Pointer to control structure.
     * @param[out] p_timeout        Pointer to structure to return timeout values.
     */
    ssp_err_t (* timeoutGet)(wdt_ctrl_t * const p_ctrl, wdt_timeout_values_t * const p_timeout);

    /** Return the version of the IOPort driver.
     * @par Implemented as
     * - R_WDT_VersionGet()
     * - R_IWDT_VersionGet()
     * @param[in]  p_ctrl       Pointer to control structure.
     * @param[out] p_data       Memory address to return version information to.
     */
    ssp_err_t (* versionGet)(ssp_version_t * const p_data);
} wdt_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_wdt_instance
{
    wdt_ctrl_t       * p_ctrl;  ///< Pointer to the control structure for this instance
    wdt_cfg_t const  * p_cfg;   ///< Pointer to the configuration structure for this instance
    wdt_api_t const  * p_api;   ///< Pointer to the API structure for this instance
} wdt_instance_t;
#endif /* DRV_WDT_API_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup WDT_API)
 **********************************************************************************************************************/
