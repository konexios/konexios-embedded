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
 * File Name        : semaphore.c
 * Description      : Application semaphore.
 *********************************************************************************************************************/

#define MODULE_NAME "CLOUD"

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// synergy
#include "hal_data.h"
#include "tx_api.h"

// TODO manage include paths
#include "../COMMON/reloc_instance_integrity.h"
#include "../COMMON/reloc_macro.h"
#include "TRACE_USE.h"

#include "semaphore.h"


/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private function prototypes
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
static TX_MUTEX a_mutex;


/**********************************************************************************************************************
 * Functions
 *********************************************************************************************************************/

/**
 * Build an application semaphore (test purpose)
 */
void semaphore_build( void )
{
    // initialize timeout timer
    ASSERT( tx_mutex_create(    &a_mutex,
                                (CHAR*)"Semaphore Mutex",
                                TX_NO_INHERIT) == TX_SUCCESS );
}

/**
 * Get an application semaphore
 *
 * @return semaphore get status
 */
bool semaphore_get( uint32_t timeout_ms )
{
    if ( tx_mutex_get ( &a_mutex, CONV_MS_TO_TICK(timeout_ms) ) == TX_SUCCESS )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * Put an application semaphore
 *
 * @return semaphore put status
 */
bool semaphore_put( void )
{
    if ( tx_mutex_put ( &a_mutex ) == TX_SUCCESS )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

