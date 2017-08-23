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

/***********************************************************************************************************************
 * File Name    : r_flash_api.h
 * Description  : API Interface for the FLASH peripheral on SC32 MCUs.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup FLASH_API Flash Interface
 * @brief Interface for the flash controller.
 *
 * @section FLASH_API_SUMMARY Summary
 * The Flash interface provides the functionality necessary to read, write, erase and blank check the
 * Flash memory. Additionally functions are provided to configure the access window and swap areas of the
 * flash memory.
 *
 * Implemented by:
 * - @ref FLASH_HP
 * - @ref FLASH
 *
 * Related SSP architecture topics:
 * - @ref ssp-interfaces
 * - @ref ssp-predefined-layers
 * - @ref using-ssp-modules
 *
 * Flash Interface description: @ref ModuleFlash
 *
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_FLASH_API_H
#define DRV_FLASH_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
#define FLASH_API_VERSION_MAJOR (1)        /**< FLASH HAL API version number (Major) */
#define FLASH_API_VERSION_MINOR (1)        /**< FLASH HAL API version number (Minor) */

/*********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/** Result type for certain operations */
typedef enum e_flash_result
{
    FLASH_RESULT_BLANK,                      ///< Return status for Blank Check Function
    FLASH_RESULT_NOT_BLANK,                  ///< Return status for Blank Check Function
    FLASH_RESULT_BGO_ACTIVE                  ///< Flash is configured for BGO mode. Result is returned in callback.
} flash_result_t;

/** Parameter for specifying the startup area swap being requested by startupAreaSelect() */
typedef enum e_flash_startup_area_swap
{
    FLASH_STARTUP_AREA_BLOCK1 = 0,        ///< Startup area will be set to Block 1
    FLASH_STARTUP_AREA_BLOCK0,            ///< Startup area will be set to Block 0
    FLASH_STARTUP_AREA_BTFLG              ///< Startup area will be set based on the value of the BTFLG
} flash_startup_area_swap_t;

/** Event types returned by the ISR callback when used in Data Flash BGO mode */
typedef enum e_flash_event
{
    FLASH_EVENT_ERASE_COMPLETE,     ///< Erase operation successfully completed
    FLASH_EVENT_WRITE_COMPLETE,     ///< Write operation successfully completed
    FLASH_EVENT_BLANK,              ///< Blank check operation successfully completed. Specified area is blank
    FLASH_EVENT_NOT_BLANK,          ///< Blank check operation successfully completed. Specified area is NOT blank
    FLASH_EVENT_ERR_DF_ACCESS,      ///< Data Flash operation failed. Can occur when writing an unerased section.
    FLASH_EVENT_ERR_CF_ACCESS,      ///< Code Flash operation failed. Can occur when writing an unerased section.
    FLASH_EVENT_ERR_CMD_LOCKED,     ///< Operation failed, FCU is in Locked state (often result of an illegal command)
    FLASH_EVENT_ERR_FAILURE         ///< Erase or Program Operation failed
} flash_event_t;

/** FLASH control block */
typedef struct st_flash_ctrl
{
    bool opened;
} flash_ctrl_t;

/** Callback function parameter data */
typedef struct st_flash_user_cb_data
{
    flash_event_t  event;        ///< Event can be used to identify what caused the callback (flash ready or error).
    void const   * p_context;    ///< Placeholder for user data.  Set in flash_api_t::open function in::flash_cfg_t.
} flash_callback_args_t;

/** FLASH Configuration */
typedef struct st_flash_cfg
{
    bool     data_flash_bgo;        ///< True if BGO (Background Operation) is enabled for Data Flash.
    /* Configuration for FLASH Event processing */
    void  (* p_callback)(flash_callback_args_t * p_args); ///< Callback provided when a Flash interrupt ISR occurs.
    /* Pointer to FLASH peripheral specific configuration */
    void const * p_extend;          ///< FLASH hardware dependent configuration
    void const * p_context;         ///< Placeholder for user data.  Passed to user callback in ::flash_callback_args_t.
} flash_cfg_t;

/** Shared Interface definition for FLASH */
typedef struct st_flash_api
{
    /** Open FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_Open()
     * - R_FLASH_HP_Open()
     *
     * @param[out]  p_ctrl       Pointer to FLASH device control. Must be declared by user. Value set here.
     * @param[in]   flash_cfg_t  Pointer to FLASH configuration structure. All elements of this structure
     *                           must be set by the user.
     */
    ssp_err_t (* open)(flash_ctrl_t         * const p_ctrl,
                       flash_cfg_t    const * const p_cfg);

    /** Write FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_Write()
     * - R_FLASH_HP_Write()
     *
     * @param[in]   p_ctrl         Control for the FLASH device context.
     * @param[in]   src_address    Address of the buffer containing the data to write to Flash.
     * @param[in]   flash_address  Code Flash or Data Flash address to write. The address must be on a
     *                             programming line boundary.
     * @param[in]   num_bytes      The number of bytes to write. This number must be a multiple
     *                             of the programming size. For Code Flash this is FLASH_MIN_PGM_SIZE_CF.
     *                             For Data Flash this is FLASH_MIN_PGM_SIZE_DF.
     *                             @warning Specifying a number that is not a multiple of the programming size
     *                             will result in SF_FLASH_ERR_BYTES being returned and no data written.
     */
    ssp_err_t (* write)(flash_ctrl_t  * const p_ctrl,
                        uint32_t        const src_address,
                        uint32_t        const flash_address,
                        uint32_t        const num_bytes);

    /** Read FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_Read()
     * - R_FLASH_HP_Read()
     *
     * @param[in]   p_ctrl             Control for the FLASH device context.
     * @param[in]   p_dest_address     Pointer to caller's destination buffer used to hold the data read from Flash.
     * @param[in]   flash_address      Code Flash or Data Flash starting address to read from.
     * @param[in]   num_bytes          The number of bytes to read.
     */
    ssp_err_t (* read)(flash_ctrl_t  * const p_ctrl,
                       uint8_t       * const p_dest_address,
                       uint32_t        const flash_address,
                       uint32_t        const num_bytes);

    /** Erase FLASH device.
     * @par Implemented as
     * R_FLASH_LP_Erase()
     * R_FLASH_HP_Erase()
     *
     * @param[in]  p_ctrl               Control for the FLASH device.
     * @param[in]  address              The block containing this address is the first block erased.
     * @param[in]  num_blocks           Specifies the number of blocks to be erased, the starting block determined
     *                                  by the block_erase_address.
     */
    ssp_err_t (* erase)(flash_ctrl_t  * const p_ctrl,
                        uint32_t        const address,
                        uint32_t        const num_blocks);

    /** Blank check FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_BlankCheck()
     * - R_FLASH_HP_BlankCheck()
     *
     * @param[in]   p_ctrl    Control for the FLASH device context.
     * @param[in]   address   The starting address of the Flash area to blank check.
     * @param[in]   num_bytes Specifies the number of bytes that need to be checked.
     *                        See the specific handler for details.
     * @param[out]  p_blank_check_result  Pointer that will be populated by the API with the results of the blank check
     *                                    operation in non-BGO (blocking) mode. In this case the blank check operation
     *                                    completes here and the result is returned. In Data Flash BGO mode the blank
     *                                    check operation is only started here and the result obtained later when the
     *                                    supplied callback routine is called. In this case FLASH_RESULT_BGO_ACTIVE will
     *                                    be returned in p_blank_check_result.
     */
    ssp_err_t (* blankCheck)(flash_ctrl_t   * const p_ctrl,
                             uint32_t         const address,
                             uint32_t         const num_bytes,
                             flash_result_t * const p_blank_check_result);

    /** Close FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_Close()
     * - R_FLASH_HP_Close()
     *
     * @param[in]  p_ctrl        Pointer to FLASH device control.
     */
    ssp_err_t (* close)(flash_ctrl_t  * const p_ctrl);

    /** Get Status for FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_StatusGet()
     * - R_FLASH_HP_StatusGet()
     *
     * @param[in]  p_ctrl        Pointer to FLASH device control.
     */
    ssp_err_t (* statusGet)(flash_ctrl_t  * const p_ctrl);

    /** Set Access Window for FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_AccessWindowSet()
     * - R_FLASH_HP_AccessWindowSet()
     *
     * @param[in]  p_ctrl        Pointer to FLASH device control.
     * @param[in]  start_addr    Determines the Starting block for the Code Flash access window.
     * @param[in]  end_addr      Determines the Ending block for the Code Flash access window.
     */
    ssp_err_t (* accessWindowSet)(flash_ctrl_t  * const p_ctrl, uint32_t const start_addr, uint32_t const end_addr);

    /** Clear any existing Code Flash access window for FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_AccessWindowClear()
     * - R_FLASH_HP_AccessWindowClear()
     *
     * @param[in]  p_ctrl        Pointer to FLASH device control.
     * @param[in]  start_addr    Determines the Starting block for the Code Flash access window.
     * @param[in]  end_addr      Determines the Ending block for the Code Flash access window.
     */
    ssp_err_t (* accessWindowClear)(flash_ctrl_t  * const p_ctrl);

    /** Reset function for FLASH device.
     * @par Implemented as
     * - R_FLASH_LP_Reset()
     * - R_FLASH_HP_Reset()
     *
     * @param[in]  p_ctrl        Pointer to FLASH device control.
     */
    ssp_err_t (* reset)(flash_ctrl_t  * const p_ctrl);

    /** Update Flash clock frequency (FCLK) and recalculate timeout values
     * @par Implemented as
     * - R_FLASH_LP_UpdateFlashClockFreq()
     * - R_FLASH_HP_UpdateFlashClockFreq()
     * @param[in]  p_ctrl        Pointer to FLASH device control.
     */
     ssp_err_t (* updateFlashClockFreq)(flash_ctrl_t  * const p_ctrl);


     /** Select which block - Default (Block 0) or Alternate (Block 1) is used as the start-up area block.
     * @par Implemented as
     * - R_FLASH_LP_StartUpAreaSelect()
     * - R_FLASH_HP_StartUpAreaSelect()
     *
     * @param[in]  p_ctrl        Pointer to FLASH device control.
     * @param[in]  swap_type     FLASH_STARTUP_AREA_BLOCK0, FLASH_STARTUP_AREA_BLOCK1 or FLASH_STARTUP_AREA_BTFLG.
     * @param[in]  is_temporary  True or false. See table below.
     *
     * swap_type             |    is_temporary |   Operation
     * FLASH_STARTUP_AREA_BLOCK0:   false        On next reset Startup area will be Block 0.\n
     * FLASH_STARTUP_AREA_BLOCK0 |  false  |     On next reset Startup area will be Block 0.
     *                                           Block 0.\n
     * FLASH_STARTUP_AREA_BLOCK1:   false        On next reset Startup area will be Block 1.\n
     * FLASH_STARTUP_AREA_BLOCK1 |  true   |     Startup area is immediately, but temporarily switched to Block 1.
     *                                           Block 1.\n
     * FLASH_STARTUP_AREA_BTFLG  |  true   |     Startup area is immediately, but temporarily switched to...
     *                                           taken.\n

     *                                           the Block determined by the Configuration BTFLG.\n
     */
    ssp_err_t (* startupAreaSelect)(flash_ctrl_t  * const       p_ctrl,
                                    flash_startup_area_swap_t   swap_type,
                                    bool                        is_temporary);

    /** Get Flash driver version.
     * @par Implemented as
     * - R_FLASH_LP_VersionGet()
     * - R_FLASH_HP_VersionGet()
     *
     * @param[out]  p_version Returns version.
     */
    ssp_err_t (* versionGet)(ssp_version_t * p_version);
} flash_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_flash_instance
{
    flash_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    flash_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    flash_api_t const * p_api;     ///< Pointer to the API structure for this instance
} flash_instance_t;

/******************************************************************************************************************//**
 * @} (end addtogroup FLASH_API)
 *********************************************************************************************************************/
#endif /* DRV_FLASH_API_H */
