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
 * File Name    : sf_si7013.h
 * Description  : Si7013 Temperature and Humidity Sensor Framework Layer API Driver.
 **********************************************************************************************************************/

#ifndef SF_SI7013_H
#define SF_SI7013_H

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_SI7013 Framework Layer Driver
 * @brief Framework layer driver implementation for Si7013 Temperature and Humidity sensor.
 *
 * @section SF_SI7013_API_SUMMARY Summary
 * This is the framework driver implemented for Si7013 external Temperature and Humidity sensor.
 *
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "temperature_sensor_api.h"
#include "humidity_sensor_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_SI7013_CODE_VERSION_MAJOR (1)
#define SF_SI7013_CODE_VERSION_MINOR (0)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** Filled in Interface API structures for this Instance. */
extern const temperature_sensor_api_t       g_sf_si7013_temperature_api;
extern const humidity_sensor_api_t          g_sf_si7013_humidity_api;

/*******************************************************************************************************************//**
 * @} (end defgroup SF_SI7013)
 **********************************************************************************************************************/
#endif /* SF_SI7013_H */
