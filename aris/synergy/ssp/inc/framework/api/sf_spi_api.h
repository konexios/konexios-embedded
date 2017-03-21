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
 * File Name    : sf_spi_api.h
 * Description  : This file provides data definitions, data structures and function prototype for SPI Framework
 * interface.
 **********************************************************************************************************************/

#ifndef SF_SPI_API_H
#define SF_SPI_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
/* Includes driver interface. */
#include "r_spi_api.h"
#include "sf_spi_cfg.h"
#include "tx_api.h"
#include "r_ioport.h"

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_SPI_API SPI Framework Interface
 * @brief RTOS-integrated SPI Framework Interface.
 *
 * @section SF_SPI_API_SUMMARY Summary
 * This SSP Interface provides access to the ThreadX-aware SPI Framework. The Interface is implemented by the  @ref SF_SPI.
 *
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * SPI Framework Interface description: @ref FrameworkSPIInterface
 *
 *
 *
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_SPI_API_VERSION_MAJOR (1)
#define SF_SPI_API_VERSION_MINOR (2)

/*********************************************************************************************************************
 * Typedef definitions
 ********************************************************************************************************************/
/** SF SPI device state */
typedef enum e_sf_spi_dev_state
{
    SF_SPI_DEV_STATE_CLOSED = 0,       ///< SPI device is closed
    SF_SPI_DEV_STATE_OPENED,           ///< SPI device is opened
} sf_spi_dev_state_t;


/* SPI Framework control structure */
typedef struct st_sf_spi_ctrl sf_spi_ctrl_t;

/** Data structure defining an SPI bus. */
typedef struct st_sf_spi_bus
{
    uint8_t               channel;              ///< Channel
    uint32_t              freq_hz_max;          ///< Bus max frequency supported
    uint32_t              freq_hz_min;          ///< Bus min frequency supported
    TX_MUTEX              * p_lock_mutex;       ///< Lock mutex handle for this channel
    TX_EVENT_FLAGS_GROUP  * p_sync_eventflag;   ///< Pointer to the event flag object for SPI data transfer
    sf_spi_ctrl_t         ** pp_curr_ctrl;      ///< Current device using the bus
    uint8_t               * p_bus_name;         ///< peripheral name SCI_SPI/RSPI
    spi_api_t             * p_lower_lvl_api;    ///< Pointer to SPI HAL interface to be used in the framework
    uint8_t               device_count;         ///< Number of devices on the bus, initialize to 0
} sf_spi_bus_t;

/** Configuration for Framework SPI driver. */
typedef struct st_sf_spi_cfg
{
    sf_spi_bus_t       * p_bus;                        ///< Bus used by the device
    ioport_port_pin_t  chip_select;                    ///< Chip select for this device
    ioport_level_t     chip_select_level_active;       ///< Polarity of CS, active High or Low
    spi_cfg_t const    * p_lower_lvl_cfg;              ///< Pointer to SPI HAL configuration
} sf_spi_cfg_t;

/** SPI device context. DO NOT INITIALIZE.  Initialization occurs when SF_SPI_Open is called. */
struct st_sf_spi_ctrl
{
    sf_spi_bus_t        * p_bus;                      ///< Bus using this device (copy from cfg)
    ioport_port_pin_t   chip_select;                  ///< Chip select for this device (copy from cfg)
    ioport_level_t      chip_select_level_active;     ///< Polarity of CS, active High or Low (copy from cfg)
    spi_cfg_t           lower_lvl_cfg;                ///< SPI peripheral configuration, use for bus reconfiguration
    spi_ctrl_t          lower_lvl_ctrl;               ///< SPI peripheral control block
    sf_spi_dev_state_t  dev_state;                    ///< Device status
    bool                locked;                       ///< Lock and unlock bus for a device
};

/** Definition of the SPI framework interface. */
typedef struct st_sf_spi_api
{

     /** @brief          Open a designated SPI device on a bus.
     * @param[in, out] p_ctrl Pointer to user-provided storage for the control block.
     * @param[in]      p_cfg   Pointer to SPI Framework configuration structure.
     */
    ssp_err_t (* open)(sf_spi_ctrl_t              * p_ctrl,
                       sf_spi_cfg_t const * const p_cfg);

     /** @brief  Receive data from SPI device.
     * @pre Call sf_spi_api_t::open to configure the SPI device before using this function.
     * @param[in]  p_ctrl    Pointer to the control block for the device.
     * @param[out] p_dest    Pointer to destination buffer into which data will be copied that is received from a SPI
     *                       device. It is the responsibility of the caller to ensure that adequate space is available
     *                       to hold the requested data count.
     * @param[in]  length    Indicates the number of units of data to be transferred (unit size specified by the
     *                       bit_width).
     * @param[in]  bit_width Indicates data bit width to be transferred.
     * @param[in]  timeout   Timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), and
     *                         timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* read)(sf_spi_ctrl_t              * const p_ctrl,
                       void                       * const p_dest,
                       uint32_t const                     length,
                       spi_bit_width_t const              bit_width,
                       uint32_t const                     timeout);

     /** @brief  Transmit data to SPI device.
     * @pre Call sf_spi_api_t::open to configure the SPI device before using this function.
     * @param[in]  p_ctrl    Pointer to the control block for the device.
     * @param[in]  p_src     Pointer to a source data buffer from which data will be transmitted to a SPI device.<BR>
     *                       The argument must not be NULL.
     * @param[in]  length    Indicates the number of units of data to be transferred (unit size specified by the
     *                       bit_width).
     * @param[in]  bit_width Indicates data bit width to be transferred.
     * @param[in]  timeout   Timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), and
     *                         timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* write)(sf_spi_ctrl_t         * const p_ctrl,
                        void                  * const p_src,
                        uint32_t const                length,
                        spi_bit_width_t const         bit_width,
                        uint32_t const                timeout);


     /** @brief  Simultaneously transmit data to an SPI device while receiving data
     *  from an SPI device (full duplex).
     *
     *         The writeread API gets mutex object, handles SPI data transmission at SPI HAL layer and receive data from
     *         the SPI HAL layer.
     *         The API uses the event flag wait to synchronize to completion of data transfer .
     * @pre Call sf_spi_api_t::open to configure the SPI before using this function.
     * @param[in]  p_ctrl    Pointer to the control block for the channel.
     * @param[in]  p_src     Pointer to a source data buffer from which data will be transmitted to a SPI device.<BR>
     *                       The argument must not be NULL.
     * @param[out] p_dest    Pointer to destination buffer into which data will be copied that is received from a SPI
     *                       device. It is the responsibility of the caller to ensure that adequate space is available
     *                       to hold the requested data count.
     * @param[in]  length    Indicates the number of units of data to be transferred (unit size specified by the
     *                       bit_width).<BR>
     * @param[in]  bit_width Indicates data bit width to be transferred.
     * @param[in]  timeout   Timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), and
     *                         timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* writeRead)(sf_spi_ctrl_t         * const p_ctrl,
                            void                  * const p_src,
                            void                  * const p_dest,
                            uint32_t const                length,
                            spi_bit_width_t const         bit_width,
                            uint32_t const                timeout);


    /**  @brief Disable the SPI device designated by the control handle and close the RTOS services used by the bus
     *if no devices are connected to the bus. This function removes power to the SPI channel designated by the handle and
     *disables the associated interrupts.
     * @param[in]  p_ctrl  Pointer to the control block for the device.
     */
    ssp_err_t (* close)(sf_spi_ctrl_t * const p_ctrl);

     /** @brief  Lock the bus for a device. The locking allows devices to reserve a bus to themselves for a given
     *		   period of time (i.e. between lock and unlock).
     *         This allows devices to complete several reads and writes on the bus without interrupt.
     * @param[in]  p_ctrl  Pointer to the control block for the device.
     */
    ssp_err_t (* lock)(sf_spi_ctrl_t * const p_ctrl);

     /** @brief  Unlock the bus for a particular device and make the bus usable for other devices.
     * @param[in]  p_ctrl  Pointer to the control block for the device.
     */
    ssp_err_t (* unlock)(sf_spi_ctrl_t * const p_ctrl);


    /** @brief  Get the version information of the underlying driver.
     * @param[out]  p_version pointer to memory location to return version number
     */
    ssp_err_t (* version) (ssp_version_t   * const p_version);
} sf_spi_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_spi_instance
{
    sf_spi_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    sf_spi_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sf_spi_api_t const * p_api;     ///< Pointer to the API structure for this instance
} sf_spi_instance_t;


/*******************************************************************************************************************//**
 * @} (end defgroup SF_SPI_API)
 **********************************************************************************************************************/
#endif /* SF_SPI_API_H */
