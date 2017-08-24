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
 * File Name    : sf_adxrs290_cfg.h
 * Description  : Build time configurations for Dual-Axis MEMS Gyroscope ADXRS290.
 **********************************************************************************************************************/

#ifndef SF_ADXRS290_CFG_H
#define SF_ADXRS290_CFG_H

/*******************************************************************************************************************//**
 * @ingroup SF_ADXRS290
 * @defgroup SF_ADXRS290_CFG Build Time Configurations
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
#define SF_ADXRS290_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

/** Specify RTOS tick rate, used for delays computation. */
#define THREADX_TICK_RATE           (TX_TIMER_TICKS_PER_SECOND)

/** Power Control Register values */
#define ADXRS290_MEAS_ENABLE         0x02
#define ADXRS290_MEAS_DISABLE        0x00

#define ADXRS290_TEMP_ENABLE         0x01
#define ADXRS290_TEMP_DISABLE        0x00

#define ADXRS290_POWER_SETTINGS     (ADXRS290_MEAS_ENABLE | ADXRS290_TEMP_DISABLE)
/*    ----------------------    */

/** Band-Pass Filter Register values */
#define ADXRS290_LPF_480_HZ          0x00
#define ADXRS290_LPF_320_HZ          0x01
#define ADXRS290_LPF_160_HZ          0x02
#define ADXRS290_LPF_80_HZ           0x03
#define ADXRS290_LPF_56_6_HZ         0x04
#define ADXRS290_LPF_40_HZ           0x05
#define ADXRS290_LPF_28_3_HZ         0x06
#define ADXRS290_LPF_20_HZ           0x07

#define ADXRS290_HPF_0_HZ            0x00
#define ADXRS290_HPF_0_011_HZ        0x10
#define ADXRS290_HPF_0_022_HZ        0x20
#define ADXRS290_HPF_0_044_HZ        0x30
#define ADXRS290_HPF_0_087_HZ        0x40
#define ADXRS290_HPF_0_175_HZ        0x50
#define ADXRS290_HPF_0_350_HZ        0x60
#define ADXRS290_HPF_0_700_HZ        0x70
#define ADXRS290_HPF_1_400_HZ        0x80
#define ADXRS290_HPF_2_800_HZ        0x90
#define ADXRS290_HPF_11_30_HZ        0xA0

#define ADXRS290_FILTER_SETTINGS     (ADXRS290_LPF_480_HZ | ADXRS290_HPF_0_HZ)
/*    ----------------------    */

/** Data Ready Register values */
#define ADXRS290_GENERATE_INT        0x01
#define ADXRS290_NO_INT              0x00

#define ADXRS290_DATA_READY_SETTING  (ADXRS290_NO_INT)
/*    ----------------------    */

/*******************************************************************************************************************//**
 * @} (end defgroup SF_ADXRS290_CFG)
 **********************************************************************************************************************/
#endif /* SF_ADXRS290_CFG_H */
