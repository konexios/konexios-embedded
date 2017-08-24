/***********************************************************************************************************************
 * Copyright [2016] RELOC s.r.l. - All rights strictly reserved
 * This Software is provided for evaluation purposes; any other use — including reproduction, modification, use for
 * a commercial product, distribution, or republication — without the prior written permission of the Copyright holder
 * is strictly prohibited. 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : sf_adxrs290_api.h
 * Description  : ADXRS290 Framework Layer Driver API.
 **********************************************************************************************************************/

#ifndef SF_ADXRS290_API_H
#define SF_ADXRS290_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_ADXRS290_API Dual-Axis MEMS Gyroscope ADXRS290 Driver Interface
 * @brief Dual-Axis MEMS Gyroscope ADXRS290, Framework Layer, Driver Interface.
 *
 * @section SF_ADXRS290_API_SUMMARY Summary
 * This module is Framework layer driver for the Dual-Axis MEMS Gyroscope ADXRS290.
 * This Interface is implemented by @ref SF_ADXRS290.
 *
 * Interfaces used: @see SPI_API
 *
 * Related SSP architecture topics:
 *  - What is an SSP Interface? @ref ssp-interfaces
 *  - What is an SSP Layer? @ref ssp-predefined-layers
 *  - How to use SSP Interfaces and Modules? @ref using-ssp-modules
 *
 * ADXRS290 Driver Interface user manual:
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_spi_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_ADXRS290_API_VERSION_MAJOR (0)
#define SF_ADXRS290_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when SF_ADXRS290_Open is called */
typedef struct st_sf_adxrs290_ctrl
{
    uint32_t                    open;               ///< Used by driver to check if control block is valid
    const sf_spi_instance_t *   device;             ///< SPI Framework Device
} sf_adxrs290_ctrl_t;

/** Configuration for RTOS integrated ADXRS290 Driver. */
typedef struct st_sf_adxrs290_cfg
{
    const sf_spi_instance_t *   device;             ///< SPI Framework Device
} sf_adxrs290_cfg_t;


/** ADXRS290 Driver API structure. */
typedef struct st_sf_adxrs290_api
{
    /** Open SPI Framework Driver and initialize internal constant values.
     * @par Implemented as
     *  - SF_ADXRS290_Open()
     *
     * @param[in,out] p_ctrl   Pointer to a structure allocated by user. This control structure is initialized in
     *                         this function.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of the structure must be set by user.
     */
    ssp_err_t (* open)(sf_adxrs290_ctrl_t      * const p_ctrl,
                       sf_adxrs290_cfg_t const * const p_cfg);
    
    /** Sample XY-axes angular rates. Measured value will be expressed in '°/s' unit.
     * @par Implemented as
     *  - SF_ADXRS290_ReadAngularRates()
     *
     * @param[in]   p_ctrl      Handle set in adxrs290_api_t::open.
     * @param[in]   x_value     Pointer in which the X-axis angular rate value will be stored [°/s].
     * @param[in]   y_value     Pointer in which the Y-axis angular rate value will be stored [°/s].
     */
    ssp_err_t (* readAngularRates)( sf_adxrs290_ctrl_t * const p_ctrl,
                                    float * x_value,
                                    float * y_value);
    
    /** Reset ADXRS290 device.
     * @par Implemented as
     *  - SF_ADXRS290_Reset()
     *
     * @param[in]   p_ctrl      Handle set in adxrs290_api_t::open.
     */
    ssp_err_t (* reset)(sf_adxrs290_ctrl_t * const p_ctrl);
    
    /** Close SPI channel at Framework layer.
     * @par Implemented as
     *  - SF_ADXRS290_Close()
     *
     * @param[in]   p_ctrl       Handle set in adxrs290_api_t::open.
     */
    ssp_err_t (* close)(sf_adxrs290_ctrl_t      * const p_ctrl);
    
    /** Gets version and stores it in provided pointer p_version.
     * @par Implemented as
     *  - SF_ADXRS290_VersionGet()
     *
     * @param[out]  p_version  Code and API version used stored here.
     */
    ssp_err_t (* versionGet)(ssp_version_t     * const p_version);
} sf_adxrs290_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_adxrs290_instance
{
    sf_adxrs290_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    sf_adxrs290_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sf_adxrs290_api_t const * p_api;     ///< Pointer to the API structure for this instance
} sf_adxrs290_instance_t;


/*******************************************************************************************************************//**
 * @} (end defgroup SF_ADXRS290_API)
 **********************************************************************************************************************/
#endif /* SF_ADXRS290_API_H */
