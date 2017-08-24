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
* File Name    : r_i2c_api.h
* Description  : API for I2C operation in master mode for the SC32 MCUs
***********************************************************************************************************************/

#ifndef R_I2C_API_H
#define R_I2C_API_H

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup I2C_API I2C Interface
 * @brief Interface for I2C communication.
 *
 * @section I2C_API_SUMMARY Summary
 * The I2C master interface provides a common API for I2C HAL drivers. The I2C master interface supports:
 *        - Interrupt driven transmit/receive processing
 *        - Callback function support which can return an event code
 *
 * Implemented by:
 * - @ref SIIC
 * - @ref RIIC
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * I2C Interface description: @ref HALI2CInterface
 *
 * @{
***********************************************************************************************************************/

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
/* Register definitions, common services and error codes. */
#include "bsp_api.h"

/**********************************************************************************************************************
* Macro definitions
***********************************************************************************************************************/
#define I2C_MASTER_API_VERSION_MAJOR   (1)
#define I2C_MASTER_API_VERSION_MINOR   (2)

/**********************************************************************************************************************
* Typedef definitions
***********************************************************************************************************************/
/** Communication speed options */
typedef enum e_i2c_rate
{
    I2C_RATE_STANDARD   = 100000,     ///< 100 kHz
    I2C_RATE_FAST       = 400000,     ///< 400 kHz
    I2C_RATE_FASTPLUS   = 1000000     ///<   1 MHz
} i2c_rate_t;

/** Addressing mode options */
typedef enum e_i2c_addr_mode
{
    I2C_ADDR_MODE_7BIT = 1, ///< Use 7-bit addressing mode
    I2C_ADDR_MODE_10BIT     ///< Use 10-bit addressing mode
} i2c_addr_mode_t;

/** Callback events */
typedef enum e_i2c_event
{
    I2C_EVENT_ABORTED       = 1,    ///< A transfer was aborted
    I2C_EVENT_RX_COMPLETE   = 2,    ///< A receive operation was completed successfully
    I2C_EVENT_TX_COMPLETE   = 3     ///< A transmit operation was completed successfully
} i2c_event_t;

/** I2C callback parameter definition */
typedef struct st_i2c_callback_args
{
    void        const   * const p_context;      ///< Pointer to user-provided context
    uint32_t    const           bytes;          ///< Number of received/transmitted bytes in buff
    i2c_event_t const           event;          ///< Event code
} i2c_callback_args_t;

/** I2C configuration block */
typedef struct st_i2c_cfg
{
    /** Generic configuration */
    uint32_t        channel;                                    ///< Identifier recognizable by implementation
    i2c_rate_t      rate;                                       ///< Device's maximum clock rate from enum i2c_rate_t
    uint16_t        slave;                                      ///< The address of the slave device
    i2c_addr_mode_t addr_mode;                                  ///< Indicates how slave fields should be interpreted
    uint16_t        sda_delay;                                  ///< The SDA output delay

    /** Parameters to control software behavior */
    void         (* p_callback)(i2c_callback_args_t * p_args);  ///< Pointer to callback function
    void const    * p_context;                                  ///< Pointer to the user-provided context

    /** Implementation-specific configuration */
    void const    * p_extend;                                   ///< Any configuration data needed by the hardware
} i2c_cfg_t;

/** I2C control structure. DO NOT INITIALIZE */
typedef struct st_i2c_ctrl
{
    i2c_cfg_t   info;   ///< Information describing I2C device
    uint32_t    open;   ///< Flag to determine if the device is open
} i2c_ctrl_t;

/** Interface definition for I2C access as master */
typedef struct st_i2c_api_master
{
    /** Opens the I2C driver and initializes the hardware.
     * @par Implemented as
     * - R_RIIC_MasterOpen()
     * - R_SCI_SIIC_MasterOpen()
     *
     * @param[in] p_ctrl    Pointer to control block. Must be declared by user. Elements are set here.
     * @param[in] p_cfg     Pointer to configuration structure.
     */
    ssp_err_t (* open)(i2c_ctrl_t             * const p_ctrl,
                        i2c_cfg_t      const   * const p_cfg);

    /** Closes the driver and releases the I2C device.
     * @par Implemented as
     * - R_RIIC_MasterClose()
     * - R_SCI_SIIC_MasterClose()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     */
    ssp_err_t (* close)(i2c_ctrl_t             * const p_ctrl);

    /** Performs a read operation on an I2C device.
     * @par Implemented as
     * - R_RIIC_MasterRead()
     * - R_SCI_SIIC_MasterRead()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     * @param[in] p_dest    Pointer to the location to store read data.
     * @param[in] bytes     Number of bytes to read.
     * @param[in] restart   Specify if the restart condition should be issued after reading.
     */
    ssp_err_t (* read)(i2c_ctrl_t             * const p_ctrl,
                       uint8_t                * const p_dest,
                       uint32_t                 const bytes,
                       bool                     const restart);

    /** Performs a write operation on an I2C device.
     * @par Implemented as
     * - R_RIIC_MasterWrite()
     * - R_SCI_SIIC_MasterWrite()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     * @param[in] p_src     Pointer to the location to get write data from.
     * @param[in] bytes     Number of bytes to write.
     * @param[in] restart   Specify if the restart condition should be issued after writing.
     */
    ssp_err_t (* write)(i2c_ctrl_t             * const p_ctrl,
                        uint8_t                * const p_src,
                        uint32_t                 const bytes,
                        bool                     const restart);

    /** Performs a reset of the peripheral.
     * @par Implemented as
     * - R_RIIC_MasterReset()
     * - R_SCI_SIIC_MasterReset()
     *
     * @param[in] p_ctrl    Pointer to control block set in i2c_api_master_t::open call.
     */
    ssp_err_t (* reset)(i2c_ctrl_t             * const p_ctrl);

    /** Gets version information and stores it in the provided version struct.
     * @par Implemented as
     * - R_RIIC_MasterVersionGet()
     * - R_SCI_SIIC_MasterVersionGet()
     *
     * @param[out] p_version  Code and API version used.
     */
    ssp_err_t (* versionGet)(ssp_version_t          * const p_version);
} i2c_api_master_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_i2c_instance
{
    i2c_ctrl_t             * p_ctrl;    ///< Pointer to the control structure for this instance
    i2c_cfg_t        const * p_cfg;     ///< Pointer to the configuration structure for this instance
    i2c_api_master_t const * p_api;     ///< Pointer to the API structure for this instance
} i2c_master_instance_t;

/******************************************************************************************************************//**
 * @} (end addtogroup I2C_API)
 *********************************************************************************************************************/
#endif /* R_I2C_API_H */
