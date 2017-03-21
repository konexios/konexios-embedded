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
 * File Name    : sf_winc1500_cfg.h
 * Description  : Build time configurations for Wi-Fi module WINC1500.
 **********************************************************************************************************************/

#ifndef SF_WINC1500_CFG_H
#define SF_WINC1500_CFG_H

/*******************************************************************************************************************//**
 * @ingroup SF_WINC1500
 * @defgroup SF_WINC1500_CFG Build Time Configurations
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
#define SF_WINC1500_CFG_PARAM_CHECKING_ENABLE   ( BSP_CFG_PARAM_CHECKING_ENABLE )

/** Specify the maximum number of concurrent sockets */
#define SF_WINC1500_CFG_PARAM_MAX_SOCKETS       ( 8 )

/** Specify RTOS tick rate, used for delays computation. */
#define THREADX_TICK_RATE                       ( TX_TIMER_TICKS_PER_SECOND )

/*******************************************************************************************************************//**
 * @} (end defgroup SF_WINC1500_CFG)
 **********************************************************************************************************************/
#endif /* SF_WINC1500_CFG_H */
