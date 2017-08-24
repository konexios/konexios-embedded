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
 * File Name    : sf_adxl362_api.h
 * Description  : ADXL362 Framework Layer Driver API.
 **********************************************************************************************************************/

#ifndef SF_ADXL362_API_H
#define SF_ADXL362_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_ADXL362_API 3-Axis MEMS Accelerometer ADXL362 Driver Interface
 * @brief 3-Axis MEMS Accelerometer ADXL362, Framework Layer, Driver Interface.
 *
 * @section SF_ADXL362_API_SUMMARY Summary
 * This module is Framework layer driver for the 3-Axis MEMS Accelerometer ADXL362.
 * This Interface is implemented by @ref SF_ADXL362.
 *
 * Interfaces used: @see SPI_API
 *
 * Related SSP architecture topics:
 *  - What is an SSP Interface? @ref ssp-interfaces
 *  - What is an SSP Layer? @ref ssp-predefined-layers
 *  - How to use SSP Interfaces and Modules? @ref using-ssp-modules
 *
 * ADXL362 Driver Interface user manual:
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
#define SF_ADXL362_API_VERSION_MAJOR (0)
#define SF_ADXL362_API_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when SF_ADXL362_Open is called */
typedef struct st_sf_adxl362_ctrl
{
    uint32_t                    open;               ///< Used by driver to check if control block is valid
    float                       scale_factor;       ///< Used in acceleration computation
    const sf_spi_instance_t *   device;             ///< SPI Framework Device
} sf_adxl362_ctrl_t;

/** Configuration for RTOS integrated ADXL362 Driver. */
typedef struct st_sf_adxl362_cfg
{
    const sf_spi_instance_t *   device;             ///< SPI Framework Device
} sf_adxl362_cfg_t;


/** ADXL362 Driver API structure. */
typedef struct st_sf_adxl362_api
{
    /** Open SPI Framework Driver and initialize internal constant values.
     * @par Implemented as
     *  - SF_ADXL362_Open()
     *
     * @param[in,out] p_ctrl   Pointer to a structure allocated by user. This control structure is initialized in
     *                         this function.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of the structure must be set by user.
     */
    ssp_err_t (* open)(sf_adxl362_ctrl_t      * const p_ctrl,
                       sf_adxl362_cfg_t const * const p_cfg);

    /** Sample XYZ-axes accelerations. Measured value will be expressed in 'g' unit.
     * @par Implemented as
     *  - SF_ADXL362_ReadAccelerations()
     *
     * @param[in]   p_ctrl      Handle set in adxl362_api_t::open.
     * @param[in]   x_value     Pointer in which the X-axis acceleration value will be stored [g].
     * @param[in]   y_value     Pointer in which the Y-axis acceleration value will be stored [g].
     * @param[in]   z_value     Pointer in which the Z-axis acceleration value will be stored [g].
     */
    ssp_err_t (* readAccelerations)(sf_adxl362_ctrl_t * const p_ctrl,
                                    float * x_value,
                                    float * y_value,
                                    float * z_value );
                                
    /** Reset ADXL362 device.
     * @par Implemented as
     *  - SF_ADXL362_Reset()
     *
     * @param[in]   p_ctrl      Handle set in adxl362_api_t::open.
     */
    ssp_err_t (* reset)(sf_adxl362_ctrl_t * const p_ctrl);
    
    /** Close SPI channel at Framework layer.
     * @par Implemented as
     *  - SF_ADXL362_Close()
     *
     * @param[in]   p_ctrl       Handle set in adxl362_api_t::open.
     */
    ssp_err_t (* close)(sf_adxl362_ctrl_t      * const p_ctrl);

    /** Gets version and stores it in provided pointer p_version.
     * @par Implemented as
     *  - SF_ADXL362_VersionGet()
     *
     * @param[out]  p_version  Code and API version used stored here.
     */
    ssp_err_t (* versionGet)(ssp_version_t     * const p_version);
} sf_adxl362_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_adxl362_instance
{
    sf_adxl362_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    sf_adxl362_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sf_adxl362_api_t const * p_api;     ///< Pointer to the API structure for this instance
} sf_adxl362_instance_t;


/*******************************************************************************************************************//**
 * @} (end defgroup SF_ADXL362_API)
 **********************************************************************************************************************/
#endif /* SF_ADXL362_API_H */
