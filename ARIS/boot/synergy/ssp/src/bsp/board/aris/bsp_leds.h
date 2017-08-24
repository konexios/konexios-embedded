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
* File Name    : bsp_leds.h
* Description  : This module has information about the LEDs on this board.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_BOARD_ARIS
 * @defgroup BSP_ARIS_LEDS Board LEDs
 * @brief LED information for this board.
 *
 * This is code specific to the ARIS board. It includes info on the number of LEDs and which pins are they are on.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_BOARD_LEDS_H
#define BSP_BOARD_LEDS_H

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Available user-controllable LEDs on this board. These enums can be can be used to index into the array of LED pins
 * found in the bsp_leds_t structure. */
typedef enum e_bsp_led
{
    BSP_LED_LED1_BLUE= 0,    	///< LED1 Blue
    BSP_LED_LED2_ORANGE,    	///< LED2 Orange
} bsp_led_t; 

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Public Functions
 **********************************************************************************************************************/

#endif /* BSP_BOARD_LEDS_H */

/** @} (end defgroup BSP_ARIS_LEDS) */
