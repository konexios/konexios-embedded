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
/***********************************************************************************************************************
* File Name    : bsp.h
* Description  : Includes and API function available for this board.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_Boards
 * @defgroup BSP_BOARD_ARIS BSP for the ARIS Board
 * @brief BSP for the ARIS Board
 *
 * ARIS is a development board for the Renesas SynergyTM S7G2 microcontroller in a LQFP176 package. The board is equipped
 * with peripherals designed for the IoT device development, such as WiFi and BLE modules, NFC, USB Host/Device and Ethernet.
 * Others useful devices are present: 3-axis Accelemeter, 2-axis Gyro, Temperature and Humidity sensor, QSPI Flash,
 * SDRAM, SD card slot, LCD connector with onboard resistive touch controller, SEGGER J-Link on-board debug, switches and
 * LED indicators.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_H_
#define BSP_H_

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_BOARD_ARIS
 * @defgroup BSP_CONFIG_ARIS Build Time Configurations
 *
 * The BSP has multiple header files that contain build-time configuration options. Currently there are header files to
 * configure the following settings:
 *
 * - General BSP Options
 * - Clocks
 * - Interrupts
 * - Pin Configuration
 *
 * @{
 **********************************************************************************************************************/

/** @} (end defgroup BSP_CONFIG_DK2M) */
/* BSP MCU Specific Includes. */
#include "../../src/bsp/mcu/s7g2/bsp_mcu_info.h"

/* BSP Board Specific Includes. */
#include "../../src/bsp/board/aris/bsp_init.h"
#include "../../src/bsp/board/aris/bsp_sdram.h"
#include "../../src/bsp/board/aris/bsp_qspi.h"
#include "../../src/bsp/board/aris/bsp_leds.h"



/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

/** @} (end defgroup BSP_BOARD_ARIS) */

#endif /* BSP_H_ */
