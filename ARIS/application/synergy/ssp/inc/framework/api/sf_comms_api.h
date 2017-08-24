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
 * File Name    : sf_comms_api.h
 * Description  : Communications API header file
 **********************************************************************************************************************/

#ifndef SF_COMMS_API_H
#define SF_COMMS_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_COMMS_API Communications Framework Interface
 * @brief RTOS-integrated communications Framework Interface.
 *
 * Implemented by:
 *
 * - @ref SF_UART_COMMS  - UART implementation
 * - @ref SF_EL_UX_COMMS - USBX CDC ACM device implementation
 * - @ref SF_EL_NX_COMMS - NetX telnet server implementation
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * See also Framework Communications Interface description: @ref FrameworkCommsModule
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
/* Includes driver interface. */
#include "tx_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
/** Version of the API defined in this file */
#define SF_COMMS_API_VERSION_MAJOR (1)
#define SF_COMMS_API_VERSION_MINOR (2)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Communications locks */
typedef enum e_sf_comms_lock
{
    SF_COMMS_LOCK_TX = 0,       ///< Lock Transmit
    SF_COMMS_LOCK_RX,           ///< Lock Receive
    SF_COMMS_LOCK_ALL,          ///< Lock Transmit and Receive
} sf_comms_lock_t;

/** Communications control structure. DO NOT INITIALIZE.  Initialization occurs when sf_comms_api_t::open is called */
typedef struct st_sf_comms_ctrl
{
	void const * p_extend;       ///< Pointer to lower level communications control structure
} sf_comms_ctrl_t;

/** Configuration for RTOS integrated communications driver */
typedef struct st_sf_comms_cfg
{
	void const * p_extend;       ///< Pointer to lower level communications control structure
} sf_comms_cfg_t;

/** Framework communications API structure.  Implementations will use the following API. */
typedef struct st_sf_comms_api
{
    /** Initialize communications driver.
     * @param[in,out] p_ctrl   Pointer to a control structure allocated by user. The control structure is
     *                         initialized in this function.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of the structure must be set by user.
     */
    ssp_err_t (* open)(sf_comms_ctrl_t       * const p_ctrl,
                       sf_comms_cfg_t  const * const p_cfg);   

    /** Clean up communications driver.
     * @param[in]   p_ctrl      Pointer to device control block initialized in Open call for communications driver.
     */
    ssp_err_t (* close)(sf_comms_ctrl_t * const p_ctrl);    

    /** Read data from communications driver.  This call will return after the number of bytes requested is read or if 
	 * a timeout occurs while waiting for access to the driver.
     * @param[in]   p_ctrl     Pointer to device control block initialized in Open call for communications driver.
     * @param[in]   p_dest     Destination address to read data out
     * @param[in]   bytes      Read data length
     * @param[in]   timeout    ThreadX timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), 
     *                         and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* read)(sf_comms_ctrl_t * const p_ctrl,
                       uint8_t        * const p_dest,
                       uint32_t         const bytes,
                       UINT             const timeout);

    /** Write data to communications driver.  This call will return after all bytes are written or if a timeout occurs
	 * while waiting for access to the driver.
     * @param[in]   p_ctrl     Pointer to device control block initialized in Open call for communications driver.
     * @param[in]   p_src      Source address to read data out from
     * @param[in]   bytes      Write data length
     * @param[in]   timeout    ThreadX timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), 
     *                         and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* write)(sf_comms_ctrl_t * const p_ctrl,
                        uint8_t  const * const p_src,
                        uint32_t         const bytes,
                        UINT             const timeout);

    /** Lock the communications driver. Reserve exclusive access to the communications driver.
     * @param[in]  p_ctrl      Pointer to device control block initialized in Open call for communications driver.
     * @param[in]  lock_type   Locking type, transmission channel or reception channel
     * @param[in]  timeout     ThreadX timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), 
     *                         and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* lock)(sf_comms_ctrl_t * const p_ctrl, 
                       sf_comms_lock_t         lock_type, 
                       UINT                   timeout);    

    /** Unlock the communications driver. Release exclusive access to the communications driver.
     * @param[in]  p_ctrl      Pointer to device control block initialized in Open call for communications driver.
     * @param[in]  lock_type   Locking type, transmission channel or reception channel
     */
    ssp_err_t (* unlock)(sf_comms_ctrl_t * const p_ctrl, 
                         sf_comms_lock_t         lock_type);    

    /** Store the driver version in the provided p_version.
     * @param[in]   p_ctrl     Pointer to device control block initialized in Open call for communications driver.
     * @param[in]   p_version  Pointer to memory version to be stored.
     */
    ssp_err_t (* versionGet)(ssp_version_t  * const p_version);
} sf_comms_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_comms_instance
{
    sf_comms_ctrl_t       * p_ctrl;    ///< Pointer to the control structure for this instance
    sf_comms_cfg_t  const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sf_comms_api_t  const * p_api;     ///< Pointer to the API structure for this instance
} sf_comms_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup SF_COMMS_API)
 **********************************************************************************************************************/

#endif /* SF_COMMS_API_H */
