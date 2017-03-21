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
* File Name    : bsp_init.h
* Description  : This module calls any initialization code specific to this BSP.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_BOARD_ARIS
 * @defgroup BSP_BOARD_ARIS_INIT Board Specific Code
 * @brief Board specific code for the ARIS Board
 *
 * This is code specific to the ARIS board. It includes code to setup the SDRAM.
 *
 * @{
***********************************************************************************************************************/

#ifndef BSP_INIT_H_
#define BSP_INIT_H_

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
void bsp_init(void * p_args);

#endif /* BSP_INIT_H_ */

/** @} (end defgroup BSP_BOARD_ARIS_INIT) */
