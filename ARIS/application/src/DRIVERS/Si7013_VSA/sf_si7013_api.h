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
 * File Name    : sf_si7013_api.h
 * Description  : Definition of Si7013 Framework Layer Device Driver control and configuration blocks.
 **********************************************************************************************************************/

#ifndef SF_SI7013_API_H
#define SF_SI7013_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Library
 * @addtogroup SF_SI7013
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_i2c_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when SF_SI7013_Open is called */
typedef struct st_sf_si7013_ctrl
{
    uint32_t                    temp_open;          ///< Used by driver to check if temperature interface is open
    uint32_t                    hum_open;           ///< Used by driver to check if humidity interface is open
    const sf_i2c_instance_t *   device;             ///< I2C Framework Device
} sf_si7013_ctrl_t;

/** Configuration for RTOS integrated Si7013 Driver. */
typedef struct st_sf_si7013_cfg
{
    const sf_i2c_instance_t *   device;             ///< I2C Framework Device
} sf_si7013_cfg_t;

/** Device Info struct */
typedef struct st_sf_si7013_info
{
    uint8_t fw_ver;                                 ///< Firmware Version
    uint32_t sn_a;                                  ///< Serial Number part A
    uint32_t sn_b;                                  ///< Serial Number part B
} sf_si7013_info_t;

/*******************************************************************************************************************//**
 * @} (end defgroup SF_SI7013_API)
 **********************************************************************************************************************/
#endif /* SF_SI7013_API_H */
