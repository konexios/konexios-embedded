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
 * File Name    : sf_adxl362_cfg.h
 * Description  : Build time configurations for 3-Axis MEMS Accelerometer ADXL362.
 **********************************************************************************************************************/

#ifndef SF_ADXL362_CFG_H
#define SF_ADXL362_CFG_H

/*******************************************************************************************************************//**
 * @ingroup SF_ADXL362
 * @defgroup SF_ADXL362_CFG Build Time Configurations
 * @{
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Specify whether to include code for API parameter checking. Valid settings include:
 *    - BSP_CFG_PARAM_CHECKING_ENABLE : Utilizes the system default setting from bsp_cfg.h
 *    - 1 : Includes parameter checking
 *    - 0 : Compiles out parameter checking
 */
#define SF_ADXL362_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/** Specify RTOS tick rate, used for delays computation. */
#define THREADX_TICK_RATE           (TX_TIMER_TICKS_PER_SECOND)

/** Filter Control Register values */
#define ADXL362_2G_RANGE            0x00
#define ADXL362_4G_RANGE            0x40
#define ADXL362_8G_RANGE            0x80
#define ADXL362_RANGE_MASK          0xC0

#define ADXL362_HALF_BANDWIDTH      0x00
#define ADXL362_QUARTER_BANDWIDTH   0x10

#define ADXL362_DATA_RATE_12_5_HZ   0x00
#define ADXL362_DATA_RATE_25_HZ     0x01
#define ADXL362_DATA_RATE_50_HZ     0x02
#define ADXL362_DATA_RATE_100_HZ    0x03
#define ADXL362_DATA_RATE_200_HZ    0x04
#define ADXL362_DATA_RATE_400_HZ    0x05

#define ADXL362_FILTER_SETTINGS     (ADXL362_4G_RANGE | ADXL362_QUARTER_BANDWIDTH | ADXL362_DATA_RATE_25_HZ)
/*    ----------------------    */

/** Power Control Register values */
#define ADXL362_WAKE_UP_MODE        0x08

#define ADXL362_AUTOSLEEP_MODE      0x04

#define ADXL362_MEAS_MODE_STANDBY   0x00
#define ADXL362_MEAS_MODE_ACTIVE    0x02

#define ADXL362_POWER_SETTING       (ADXL362_MEAS_MODE_ACTIVE)
/*    ----------------------    */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_ADXL362_CFG)
 **********************************************************************************************************************/
#endif /* SF_ADXL362_CFG_H */
