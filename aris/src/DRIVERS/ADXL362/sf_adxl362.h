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
 * File Name    : sf_adxl362.h
 * Description  : ADXL362 3-Axis MEMS Accelerometer Framework Layer API Driver.
 **********************************************************************************************************************/

#ifndef SF_ADXL362_H
#define SF_ADXL362_H

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @defgroup SF_ADXL362 Framework Layer Driver
 * @brief Framework layer driver implementation for ADXL362 3-Axis MEMS Accelerometer.
 *
 * @section SF_ADXL362_API_SUMMARY Summary
 * This is the framework driver implemented for ADXL362 external 3-Axis MEMS Accelerometer.
 *
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_adxl362_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_ADXL362_CODE_VERSION_MAJOR (0)
#define SF_ADXL362_CODE_VERSION_MINOR (1)

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
/** Filled in Interface API structure for this Instance. */
extern const sf_adxl362_api_t      g_sf_adxl362_api;

/*******************************************************************************************************************//**
 * @} (end defgroup SF_ADXL362)
 **********************************************************************************************************************/
#endif /* SF_ADXL362_H */
