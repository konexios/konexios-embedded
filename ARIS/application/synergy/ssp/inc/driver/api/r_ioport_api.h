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
 * File Name    : r_ioport_api.h
 * Description  : IOPORT driver interface.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup Interface_Library
 * @defgroup IOPORT_API I/O Port Interface
 * @brief Interface  for accessing I/O ports and configuring I/O functionality.
 *
 * The IOPort shared interface provides the ability to access the IOPorts of a device at both bit and port level.
 * Port and pin direction can be changed.
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 * IOPORT Interface description: @ref HALIOPORTInterface
 *
 * @{
 **********************************************************************************************************************/

#ifndef DRV_IOPORT_API_H
#define DRV_IOPORT_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Common error codes and definitions. */
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define IOPORT_API_VERSION_MAJOR (1)
#define IOPORT_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Levels that can be set and read for individual pins */
typedef enum e_ioport_level
{
    IOPORT_LEVEL_LOW = 0,               ///< Low
    IOPORT_LEVEL_HIGH                   ///< High
} ioport_level_t;

/** Direction of individual pins */
typedef enum e_ioport_dir
{
    IOPORT_DIRECTION_INPUT = 0,         ///< Input
    IOPORT_DIRECTION_OUTPUT             ///< Output
} ioport_direction_t;

/** Pin identifier and pin PFS pin configuration value */
typedef struct st_ioport_pin_cfg
{
    uint32_t           pin_cfg;         ///< Pin PFS configuration - Use ioport_cfg_options_t parameters to configure
    ioport_port_pin_t  pin;             ///< Pin identifier
} ioport_pin_cfg_t;

/** Multiple pin configuration data for loading into PFS registers by R_IOPORT_Init()  */
typedef struct st_ioport_cfg
{
    uint16_t                 number_of_pins; ///< Number of pins for which there is configuration data
    ioport_pin_cfg_t const * p_pin_cfg_data; ///< Pin configuration data
} ioport_cfg_t;

/** IOPort driver structure. IOPort functions implemented at the HAL layer will follow this API. */
typedef struct st_ioport_api
{
    /** Initialize configuration of multiple pins.
     * @par Implemented as
     * - R_IOPORT_Init()
     * @param[in]  p_cfg				Pointer to pin configuration data array.
     */
    ssp_err_t (* init)(const ioport_cfg_t * p_cfg);

    /** Configure settings for an individual pin.
     * @par Implemented as
     * - R_IOPORT_PinCfg()
     * @param[in]  pin                  Pin to be read.
     * @param[in]  cfg                  Configuration options for the pin.
     */
    ssp_err_t (* pinCfg)(ioport_port_pin_t pin,     uint32_t cfg);

    /** Set the pin direction of a pin.
     * @par Implemented as
     * - R_IOPORT_PinDirectionSet()
     * @param[in]  pin                  Pin being configured.
     * @param[in]  direction            Direction to set pin to which is a member of ioport_direction_t.
     */
    ssp_err_t (* pinDirectionSet)(ioport_port_pin_t pin,    ioport_direction_t direction);

    /** Read the event input data of the specified pin and return the level.
     * @par Implemented as
     * - R_IOPORT_PinEventInputRead()
     * @param[in]  pin                  Pin to be read.
     * @param[in]  p_pin_event         Pointer to return the event data.
     */
    ssp_err_t (* pinEventInputRead)(ioport_port_pin_t pin,    ioport_level_t * p_pin_event);

    /** Write pin event data.
     * @par Implemented as
     * - R_IOPORT_PinEventOutputWrite()
     * @param[in]  pin 					Pin event data is to be written to.
     * @param[in]  pin_value			Level to be written to pin output event.
     */
    ssp_err_t (* pinEventOutputWrite)(ioport_port_pin_t pin,    ioport_level_t pin_value);

    /** Configure the PHY mode of the Ethernet channels.
     * @par Implemented as
     * - R_IOPORT_EthernetModeCfg()
     * @param[in]  channel              Channel configuration will be set for.
     * @param[in]  mode                 PHY mode to set the channel to.
     */
    ssp_err_t (* pinEthernetModeCfg)(ioport_ethernet_channel_t channel,     ioport_ethernet_mode_t mode);

    /** Read level of a pin.
     * @par Implemented as
     * - R_IOPORT_PinRead()
     * @param[in]  pin                  Pin to be read.
     * @param[in]  p_pin_value          Pointer to return the pin level.
     */
    ssp_err_t (* pinRead)(ioport_port_pin_t pin,    ioport_level_t * p_pin_value);

    /** Write specified level to a pin.
     * @par Implemented as
     * - R_IOPORT_PinWrite()
     * @param[in]  pin                  Pin to be written to.
     * @param[in]  level                State to be written to the pin.
     */
    ssp_err_t (* pinWrite)(ioport_port_pin_t pin,    ioport_level_t level);

    /** Set the direction of one or more pins on a port.
     * @par Implemented as
     * - R_IOPORT_PortDirectionSet()
     * @param[in]  port                 Port being configured.
     * @param[in]  direction_values     Value controlling direction of pins on port (1 - output, 0 - input).
     * @param[in]  mask                 Mask controlling which pins on the port are to be configured.
     */
    ssp_err_t (* portDirectionSet)(ioport_port_t port,   ioport_size_t direction_values, ioport_size_t mask);

    /** Read captured event data for a port.
     * @par Implemented as
     * - R_IOPORT_PortEventInputRead()
     * @param[in]  port                 Port to be read.
     * @param[in]  p_event_data         Pointer to return the event data.
     */
    ssp_err_t (* portEventInputRead)(ioport_port_t port,   ioport_size_t * p_event_data);

    /** Write event output data for a port.
     * @par Implemented as
     * - R_IOPORT_PortEventOutputWrite()
     * @param[in]  port                 Port event data will be written to.
     * @param[in]  event_data           Data to be written as event data to specified port.
     * @param[in]  mask_value           Each bit set to 1 in the mask corresponds to that bit's value in event data.
     * being written to port.
     */
    ssp_err_t (* portEventOutputWrite)(ioport_port_t port,   ioport_size_t event_data, ioport_size_t mask_value);

    /** Read states of pins on the specified port.
     * @par Implemented as
     * - R_IOPORT_PortRead()
     * @param[in]  port                 Port to be read.
     * @param[in]  p_port_value         Pointer to return the port value.
     */
    ssp_err_t (* portRead)(ioport_port_t port,   ioport_size_t * p_port_value);

    /** Write to multiple pins on a port.
     * @par Implemented as
     * - R_IOPORT_PortWrite()
     * @param[in]  port                 Port to be written to.
     * @param[in]  value                Value to be written to the port.
     * @param[in]  mask                 Mask controlling which pins on the port are written to.
     */
    ssp_err_t (* portWrite)(ioport_port_t port,   ioport_size_t value, ioport_size_t mask);

    /** Return the version of the IOPort driver.
     * @par Implemented as
     * - R_IOPORT_VersionGet()
     * @param[out]  p_data              Memory address to return version information to.
     */
    ssp_err_t (* versionGet)(ssp_version_t * p_data);
} ioport_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_ioport_instance
{
    ioport_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    ioport_api_t const * p_api;     ///< Pointer to the API structure for this instance
} ioport_instance_t;

#endif /* DRV_IOPORT_API_H */

/*******************************************************************************************************************//**
 * @} (end defgroup IOPORT_API)
 **********************************************************************************************************************/
