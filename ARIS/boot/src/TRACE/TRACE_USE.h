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
 * File Name    : trace_use.h
 * Description  : Trace module use header file.
 **********************************************************************************************************************/

#ifndef TRACE_USE_H
#define TRACE_USE_H


/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/

#include "string.h"
#include <stdio.h>
#include <stdbool.h>


/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
/**
 * Callback to be invoked on a successful command parse
 */
typedef void (* p_cli_interpreter_cb_t)( char * p_output, uint16_t length, const char *p_command);

/**
 * cli_command_t structure defines command line commands. A command line command
 * should be defined by declaring a constant structure of this type.
 */
typedef struct cli_command
{
    const char * const p_command;
    const char * const p_help;
    const p_cli_interpreter_cb_t p_interpreter;
    uint8_t parameters_count;
} cli_command_t;


/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public functions
 **********************************************************************************************************************/

void TRACE_write( char* str, uint8_t strLen );

#define TRACE( str )    TRACE_write( str, (uint8_t)strlen(str) );

#define DBG_LINE_SIZE 150

int trace_created();
void dbg_line(const char *fmt, ...);

#define DBG(...) dbg_line(__VA_ARGS__)

#define DBGby8(ptr, lines) { DBG("dump {%p}:", (ptr)); int i= 0; for(;i<lines; i++) DBG("%02x %02x %02x %02x %02x %02x %02x %02x", (ptr)[0+i*8], (ptr)[1+i*8], (ptr)[2+i*8], (ptr)[3+i*8], (ptr)[4+i*8], (ptr)[5+i*8], (ptr)[6+i*8], (ptr)[7+i*8]); }

bool TRACE_read( char* str, uint8_t strLen );

bool TRACE_cli_command_register( const cli_command_t * const p_cli_commands, uint8_t count );

uint8_t TRACE_cli_params_count( const char *p_command );

const char* TRACE_cli_param_get( const char *p_command, uint8_t param_index, uint8_t *param_length );

bool TRACE_cli_run( void );


#endif  /* TRACE_USE_H */

