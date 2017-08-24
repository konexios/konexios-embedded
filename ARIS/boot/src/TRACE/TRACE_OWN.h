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
 * File Name    : trace_own.h
 * Description  : Trace module own header file.
 **********************************************************************************************************************/

#ifndef TRACE_OWN_H
#define TRACE_OWN_H


/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/

#include "TRACE.h"
#if defined(TRACE_ON_RS232)
#include "sf_uart_comms.h"
#else
#include "sf_comms_api.h"
#endif


/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

#define TRACE_COMMAND_LINE_INTERPRETER

#ifdef TRACE_COMMAND_LINE_INTERPRETER
#define TRACE_COMMAND_LINE_COMMAND_GROUPS_MAX       5
#define TRACE_COMMAND_LINE_INPUT_LENGTH_MAX         100
#define TRACE_COMMAND_LINE_OUTPUT_LENGTH_MAX        1000
#endif  /* TRACE_COMMAND_LINE_INTERPRETER */


/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

struct TRACE_
{
#if defined(TRACE_ON_RS232) || defined(TRACE_ON_USB)
    sf_comms_instance_t const * portToUse;
#endif
#ifndef SKIP_INTEGRITY_CHECK
    TRACE* self;
#endif
};


/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public functions
 **********************************************************************************************************************/

#if defined(TRACE_ON_RS232) || defined(TRACE_ON_USB)
void TRACE_build( sf_comms_instance_t const * serialToUse );
#else
void TRACE_build( void const * interfaceToUse );
#endif
bool TRACE_destroy( void );


#endif  /* TRACE_OWN_H */

