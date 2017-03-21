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
 * File Name    : humidity_sensor_api.h
 * Description  : Generic Humidity Sensor Framework Layer Interface API.
 **********************************************************************************************************************/

#ifndef HUMIDITY_SENSOR_API_H
#define HUMIDITY_SENSOR_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup HUMIDITY_SENSOR_API Generic Humidity Sensor API
 * @brief Interface for a generic humidity sensor device.
 *
 * @section HUMIDITY_SENSOR_API_SUMMARY Summary
 * The Humidity Sensor Interface provides a common API for a generic humidity sensor's driver. The interface
 * supports:
 *        - Read the humidity.
 *        - Read the device information.
 *
 * Implemented as:
 * - @ref SF_SI7013
 *
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define HUMIDITY_SENSOR_API_VERSION_MAJOR       (1)
#define HUMIDITY_SENSOR_API_VERSION_MINOR       (0)

#define HUMIDITY_SENSOR_INFO_SIZE               (12)           ///< Length of the allocated buffer for the sensor info

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Device Info struct */
typedef struct st_humidity_sensor_info
{
    uint8_t buffer[HUMIDITY_SENSOR_INFO_SIZE];   ///< Device specific info (to be casted)
} humidity_sensor_info_t;

/** Humidity Sensor Driver configuration block. */
typedef struct st_humidity_sensor_cfg
{
    /** Implementation-specific configuration */
    void const *        p_extend_cfg;               ///< Pointer to the implementation specific configuration data
    void const * const  p_extend_ctrl;              ///< Pointer to the implementation specific control block
} humidity_sensor_cfg_t;

/** Humidity Sensor Driver control block. DO NOT INITIALIZE */
typedef struct st_humidity_sensor_ctrl
{
    /** Implementation-specific configuration */
    void const *        p_extend_ctrl;              ///< Pointer to the implementation specific control block
} humidity_sensor_ctrl_t;


/** Humidity Sensor Driver API structure. */
typedef struct st_humidity_sensor_api
{
    /** Open Lower Level Driver and initialize internal constant values.
     * @par Implemented as
     *  - SF_SI7013_H_Open()
     *
     * @param[in,out] p_ctrl   Pointer to a structure allocated by user. This control structure is initialized in
     *                         this function.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of the structure must be set by user.
     */
    ssp_err_t (* open)(humidity_sensor_ctrl_t      * const p_ctrl,
                       humidity_sensor_cfg_t const * const p_cfg);
    
    /** Begin single humidity sampling. Measured value in the relative humidity.
     * @par Implemented as
     *  - SF_SI7013_H_ReadHumidity()
     *
     * @param[in]   p_ctrl      Pointer to control block set in humidity_sensor_api_t::open.
     * @param[in]   humidity    Pointer in which the humidity value will be stored [%].
     */
    ssp_err_t (* readHumidity)( humidity_sensor_ctrl_t * const p_ctrl,
                                float * humidity );
    
    /** Reset device.
     * @par Implemented as
     *  - SF_SI7013_H_Reset()
     *
     * @param[in]   p_ctrl      Pointer to control block set in humidity_sensor_api_t::open.
     */
    ssp_err_t (* reset)(humidity_sensor_ctrl_t * const p_ctrl);
    
    /** Close I2C channel at Framework layer.
     * @par Implemented as
     *  - SF_SI7013_H_Close()
     *
     * @param[in]   p_ctrl      Pointer to control block set in humidity_sensor_api_t::open.
     */
    ssp_err_t (* close)(humidity_sensor_ctrl_t * const p_ctrl);
    
    /** Gets version and stores it in provided pointer p_version.
     * @par Implemented as
     *  - SF_SI7013_H_VersionGet()
     *
     * @param[out]  p_version   Code and API version used stored here.
     */
    ssp_err_t (* versionGet)(ssp_version_t     * const p_version);
    
    /** Read Firmware version, Serial Numbers or others device specific information.
     * @par Implemented as
     *  - SF_SI7013_H_ReadInfo()
     *
     * @param[in]   p_ctrl      Pointer to control block set in humidity_sensor_api_t::open.
     * @param[in]   info        Pointer to the struct in which the device information will be stored.
     */
    ssp_err_t (* readInfo)( humidity_sensor_ctrl_t * const p_ctrl,
                            humidity_sensor_info_t * info);
} humidity_sensor_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_humidity_sensor_instance
{
    humidity_sensor_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    humidity_sensor_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    humidity_sensor_api_t const * p_api;         ///< Pointer to the API structure for this instance
} humidity_sensor_instance_t;


/*******************************************************************************************************************//**
 * @} (end defgroup Humidity Sensor Driver API Interface)
 **********************************************************************************************************************/
#endif /* HUMIDITY_SENSOR_API_H */
