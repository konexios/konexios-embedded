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
 * File Name        : trace.c
 * Description      : Console Trace and CLI service.
 *********************************************************************************************************************/

#define MODULE_NAME "TRACE"


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include <stdarg.h>
#include "reloc_instance_integrity.h"
#include "reloc_assert.h"
#include "reloc_macro.h"
#include "TRACE_OWN.h"
#include "TRACE_USE.h"
#if defined(TRACE_ON_RS232) || defined(TRACE_ON_USB)
#include "sf_comms_api.h"
#endif


/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef struct cli_command_group
{
    const cli_command_t * p_command;
    uint8_t count;
} cli_command_group_t;

#if 0
typedef struct trace_module_
{
    uint8_t id;
    char* name;
    bool status;
} trace_module_t;
#endif


/**********************************************************************************************************************
 * Private function prototypes
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/

static TRACE local_this;
static TRACE* this_ = NULL;
static char send_str[DBG_LINE_SIZE];
static TX_MUTEX send_mtx;

int trace_created() {
    return this_?1:0;
}

void dbg_line(const char *fmt, ...) {
    if ( ! trace_created() ) return;
    va_list args;
    va_start(args, fmt);
    tx_mutex_get(&send_mtx, TX_WAIT_FOREVER);
    send_str[0] = '\0';
    vsnprintf(send_str, DBG_LINE_SIZE-2, fmt, args);
    strcat(send_str, "\r\n");
    TRACE(send_str);
    tx_mutex_put(&send_mtx);
    va_end(args);
}

#ifdef TRACE_COMMAND_LINE_INTERPRETER
static cli_command_group_t g_cli_command_groups[TRACE_COMMAND_LINE_COMMAND_GROUPS_MAX];
static uint8_t g_cli_groups_registered = 0;
static char cli_input[TRACE_COMMAND_LINE_INPUT_LENGTH_MAX+1];
static char cli_output[TRACE_COMMAND_LINE_OUTPUT_LENGTH_MAX+1];
#endif


/**********************************************************************************************************************
 * Functions
 *********************************************************************************************************************/

#if defined(TRACE_ON_RS232) || defined(TRACE_ON_USB)
/**
 * TRACE_build
 * @param portToUse
 */
void TRACE_build( sf_comms_instance_t const * portToUse )
#else
/**
 * TRACE_build
 * @param interfaceToUse
 */
void TRACE_build( void const * interfaceToUse )
#endif
{
    this_ = &local_this;
    tx_mutex_create(&send_mtx, "TRACE mutex", TX_NO_INHERIT);
    
#if defined(TRACE_ON_RS232) || defined(TRACE_ON_USB)
    ASSERT( portToUse != NULL );
    this_->portToUse = portToUse;
#else
    UNUSED_PARAMETER( interfaceToUse );
#endif
    INSTANCE_INTEGRITY_SET( this_ )
}


/**
 * TRACE_destroy
 * @return
 */
bool TRACE_destroy( void )
{
    INSTANCE_INTEGRITY_CHECK( this_ )
    INSTANCE_INTEGRITY_DESTROY( this_ )
    return true;
}


/**
 * TRACE_write
 * @param str
 * @param strLen
 */
void TRACE_write( char* str, uint8_t strLen )
{
    INSTANCE_INTEGRITY_CHECK( this_ )
#if defined(TRACE_ON_RS232) || defined(TRACE_ON_USB)
        this_->portToUse->p_api->write( this_->portToUse->p_ctrl,
                                        (uint8_t*) str,
                                        strLen,
                                        TX_NO_WAIT );
                                        //TX_WAIT_FOREVER );
#else
    UNUSED_PARAMETER( str );
    UNUSED_PARAMETER( strLen );
#endif
}


/**
 * TRACE_read
 * @param str
 * @param strLen
 * @return
 */
bool TRACE_read( char* str, uint8_t strLen )
{
    INSTANCE_INTEGRITY_CHECK( this_ )

#if defined(TRACE_ON_RS232) || defined(TRACE_ON_USB)
    if ( TX_SUCCESS == this_->portToUse->p_api->read( this_->portToUse->p_ctrl,
                                    (uint8_t*) str,
                                    strLen,
                                    TX_NO_WAIT ) )
    {
        return 0;
    }
    else
    {
        return 1;
    }

#else
    UNUSED_PARAMETER( str );
    UNUSED_PARAMETER( strLen );
#endif
}


/**
 * TRACE_cli_command_register
 * @param p_cli_item
 * @return
 */
bool TRACE_cli_command_register( const cli_command_t * const p_cli_commands, uint8_t count )
{
#ifdef TRACE_COMMAND_LINE_INTERPRETER
    if (g_cli_groups_registered < TRACE_COMMAND_LINE_COMMAND_GROUPS_MAX)
    {
        g_cli_command_groups[g_cli_groups_registered].p_command = p_cli_commands;
        g_cli_command_groups[g_cli_groups_registered].count = count;
        g_cli_groups_registered++;
        return 0;
    }
    else
    {
        return 1;
    }

#else
    UNUSED_PARAMETER( p_cli_commands );
    UNUSED_PARAMETER( count );
    return 0;
#endif
}


/**
 * TRACE_cli_params_count
 * @param p_command
 * @return
 */
uint8_t TRACE_cli_params_count( const char *p_command )
{
    uint8_t params = 0;
    bool was_space = false;


    while( *p_command != 0x00 )
    {
        if (*p_command == ' ')
        {
            if (was_space != true)
            {
                params++;
                was_space = true;
            }
        }
        else
        {
            was_space = false;
        }

        p_command++;
    }

    if (was_space == true)
    {
        params--;
    }

    return params;
}


/**
 * TRACE_cli_param_get
 * @param p_command
 * @param param_index
 * @param param_length
 * @return
 */
const char* TRACE_cli_param_get( const char *p_command, uint8_t param_index, uint8_t *param_length )
{
    uint8_t found = 0;
    const char *p_param = NULL;


    *param_length = 0;

    while( found < param_index )
    {
        // initial command
        while( ( ( *p_command ) != 0x00 ) && ( ( *p_command ) != ' ' ) )
        {
            p_command++;
        }

        // search for start of the next string
        while( ( ( *p_command ) != 0x00 ) && ( ( *p_command ) == ' ' ) )
        {
            p_command++;
        }

        if( *p_command != 0x00 )
        {
            found++;

            if( found == param_index )
            {
                // parameter length
                p_param = p_command;
                while( ( ( *p_command ) != 0x00 ) && ( ( *p_command ) != ' ' ) )
                {
                    ( *param_length )++;
                    p_command++;
                }

                if( *param_length == 0 )
                {
                    p_param = NULL;
                }

                break;
            }
        }
        else
        {
            break;
        }
    }

    return p_param;
}


/**
 * TRACE_cli_run
 * @return
 */
bool TRACE_cli_run( void )
{
#ifdef TRACE_COMMAND_LINE_INTERPRETER
    uint8_t input_len;
    uint8_t i;
    uint8_t j;
    cli_command_group_t * p_cli_command_group;
    cli_command_t * p_cli_command;
    uint8_t cli_command_length;
    bool found = false;

    input_len = 0;

    while(1)
    {
        if (0 == TRACE_read( &cli_input[input_len], 1 ))
        {
            if ((input_len != 0) || ((cli_input[0] != '\r') && (cli_input[0] != '\n')))
            {
                // console echo
                TRACE_write( &cli_input[input_len], 1 );

                // CLI parse
                if ( (cli_input[input_len] == '\n') || (cli_input[input_len] == '\r') )
                {
                    cli_input[input_len] = 0x00;
                    p_cli_command_group = g_cli_command_groups;

                    // TODO Add a thread safe operations
                    // groups cycle
                    for (j=0; j<g_cli_groups_registered; j++)
                    {
                        p_cli_command = (cli_command_t *)p_cli_command_group->p_command;

                        // group cycle
                        for (i=0; i<p_cli_command_group->count; i++)
                        {
                            cli_command_length = (uint8_t)strlen(p_cli_command->p_command);

                            if( ( cli_input[ cli_command_length ] == ' ' ) || ( cli_input[ cli_command_length ] == 0x00 ) )
                            {
                                if ( strncmp( cli_input, p_cli_command->p_command, cli_command_length ) == 0 )
                                {
                                    if ( TRACE_cli_params_count(cli_input) == p_cli_command->parameters_count )
                                    {
                                        // command found
                                        if (p_cli_command->p_interpreter != (p_cli_interpreter_cb_t)NULL)
                                        {
                                            cli_output[0] = 0;
                                            p_cli_command->p_interpreter( cli_output,
                                                                          TRACE_COMMAND_LINE_OUTPUT_LENGTH_MAX,
                                                                          cli_input );
                                            TRACE( cli_output );
                                        }
                                        else
                                        {
                                            TRACE( "CLI: Command recognized, but no action is configured.\r\n" );
                                        }
                                        found = true;
                                        break;
                                    }
                                    else
                                    {
                                        found = true;
                                        TRACE( "CLI: Invalid parameter(s) count.\r\n" );
                                    }
                                }
                            }
                            p_cli_command++;
                        }
                        p_cli_command_group++;
                    }

                    if (found == false)
                    {
                        TRACE( "CLI: Command not recognized.\r\n" );
                    }

                    // reset input
                    input_len = 0;
                }
                else
                {
                    if (input_len < TRACE_COMMAND_LINE_INPUT_LENGTH_MAX)
                    {
                        input_len++;
                    }
                    else
                    {
                        // TODO ...
                    }
                }
            }
        }
        tx_thread_sleep (1);
    }
#endif
    return 0;
}

