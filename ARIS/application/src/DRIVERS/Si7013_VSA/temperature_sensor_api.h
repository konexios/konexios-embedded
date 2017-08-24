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
 * File Name    : temperature_sensor_api.h
 * Description  : Generic Temperature Sensor Framework Layer Interface API.
 **********************************************************************************************************************/

#ifndef TEMPERATURE_SENSOR_API_H
#define TEMPERATURE_SENSOR_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup TEMPERATURE_SENSOR_API Generic Temperature Sensor API
 * @brief Interface for a generic temperature sensor device.
 *
 * @section TEMPERATURE_SENSOR_API_SUMMARY Summary
 * The Temperature Sensor Interface provides a common API for a generic temperature sensor's driver. The interface
 * supports:
 *        - Read the temperature.
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
#define TEMPERATURE_SENSOR_API_VERSION_MAJOR    (1)
#define TEMPERATURE_SENSOR_API_VERSION_MINOR    (0)

#define TEMPERATURE_SENSOR_INFO_SIZE            (12)           ///< Length of the allocated buffer for the sensor info

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Device Info struct */
typedef struct st_temperature_sensor_info
{
    uint8_t buffer[TEMPERATURE_SENSOR_INFO_SIZE];   ///< Device specific info (to be casted)
} temperature_sensor_info_t;

/** Temperature Sensor Driver configuration block. */
typedef struct st_temperature_sensor_cfg
{
    /** Implementation-specific configuration */
    void const *        p_extend_cfg;               ///< Pointer to the implementation specific configuration data
    void const * const  p_extend_ctrl;              ///< Pointer to the implementation specific control block
} temperature_sensor_cfg_t;

/** Temperature Sensor Driver control block. DO NOT INITIALIZE */
typedef struct st_temperature_sensor_ctrl
{
    /** Implementation-specific configuration */
    void const *        p_extend_ctrl;              ///< Pointer to the implementation specific control block
} temperature_sensor_ctrl_t;


/** Temperature Sensor Driver API structure. */
typedef struct st_temperature_sensor_api
{
    /** Open Lower Level Driver and initialize internal constant values.
     * @par Implemented as
     *  - SF_SI7013_T_Open()
     *
     * @param[in,out] p_ctrl   Pointer to a structure allocated by user. This control structure is initialized in
     *                         this function.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of the structure must be set by user.
     */
    ssp_err_t (* open)( temperature_sensor_ctrl_t      * const p_ctrl,
                        temperature_sensor_cfg_t const * const p_cfg);
    
    /** Begin single temperature sampling. Measured value will be expressed in Celsius degree.
     * @par Implemented as
     *  - SF_SI7013_T_ReadTemperature()
     *
     * @param[in]   p_ctrl       Pointer to control block set in temperature_sensor_api_t::open.
     * @param[in]   temperature  Pointer in which the temperature value will be stored [°C].
     */
    ssp_err_t (* readTemperature)(  temperature_sensor_ctrl_t * const p_ctrl,
                                    float * temperature );
    
    /** Reset device.
     * @par Implemented as
     *  - SF_SI7013_T_Reset()
     *
     * @param[in]   p_ctrl      Pointer to control block set in temperature_sensor_api_t::open.
     */
    ssp_err_t (* reset)(temperature_sensor_ctrl_t * const p_ctrl);
    
    /** Close I2C channel at Framework layer.
     * @par Implemented as
     *  - SF_SI7013_T_Close()
     *
     * @param[in]   p_ctrl      Pointer to control block set in temperature_sensor_api_t::open.
     */
    ssp_err_t (* close)(temperature_sensor_ctrl_t * const p_ctrl);
    
    /** Gets version and stores it in provided pointer p_version.
     * @par Implemented as
     *  - SF_SI7013_T_VersionGet()
     *
     * @param[out]  p_version   Code and API version used stored here.
     */
    ssp_err_t (* versionGet)(ssp_version_t     * const p_version);
    
    /** Read Firmware version, Serial Numbers or others device specific information.
     * @par Implemented as
     *  - SF_SI7013_T_ReadInfo()
     *
     * @param[in]   p_ctrl      Pointer to control block set in temperature_sensor_api_t::open.
     * @param[in]   info        Pointer to the struct in which the device information will be stored.
     */
    ssp_err_t (* readInfo)( temperature_sensor_ctrl_t * const p_ctrl,
                            temperature_sensor_info_t * info);
} temperature_sensor_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_temperature_sensor_instance
{
    temperature_sensor_ctrl_t      * p_ctrl;        ///< Pointer to the control structure for this instance
    temperature_sensor_cfg_t const * p_cfg;         ///< Pointer to the configuration structure for this instance
    temperature_sensor_api_t const * p_api;         ///< Pointer to the API structure for this instance
} temperature_sensor_instance_t;


/*******************************************************************************************************************//**
 * @} (end defgroup Temperature Sensor Driver API Interface)
 **********************************************************************************************************************/
#endif /* TEMPERATURE_SENSOR_API_H */
