/***********************************************************************************************************************
 * Copyright [2015] Renesas Electronics Corporation and/or its licensors. All Rights Reserved.
 * 
 * This file is part of Renesas SynergyTM Software Package (SSP)
 *
 * The contents of this file (the "contents") are proprietary and confidential to Renesas Electronics Corporation
 * and/or its licensors ("Renesas") and subject to statutory and contractual protections.
 *
 * This file is subject to a Renesas SSP license agreement. Unless otherwise agreed in an SSP license agreement with
 * Renesas: 1) you may not use, copy, modify, distribute, display, or perform the contents; 2) you may not use any name
 * or mark of Renesas for advertising or publicity purposes or in connection with your use of the contents; 3) RENESAS
 * MAKES NO WARRANTY OR REPRESENTATIONS ABOUT THE SUITABILITY OF THE CONTENTS FOR ANY PURPOSE; THE CONTENTS ARE PROVIDED
 * "AS IS" WITHOUT ANY EXPRESS OR IMPLIED WARRANTY, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, AND NON-INFRINGEMENT; AND 4) RENESAS SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, OR
 * CONSEQUENTIAL DAMAGES, INCLUDING DAMAGES RESULTING FROM LOSS OF USE, DATA, OR PROJECTS, WHETHER IN AN ACTION OF
 * CONTRACT OR TORT, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE CONTENTS. Third-party contents
 * included in this file may be subject to different terms.
 **********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : sf_console_api.h
* Description  : SSP console framework interface
***********************************************************************************************************************/

#ifndef SF_CONSOLE_API_H
#define SF_CONSOLE_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_CONSOLE_API Console Framework Interface
 * @brief RTOS-integrated Console Framework Interface.
 *
 * @section SF_CONSOLE_API_SUMMARY Summary
 * This module is a ThreadX-aware Console Framework.
 *
 *
 * Related SSP architecture topics:
 *  - @ref ssp-interfaces
 *  - @ref ssp-predefined-layers
 *  - @ref using-ssp-modules
 *
 *
 * See also Console Interface description @ref FrameworkConsoleModule
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdio.h>
#include "bsp_api.h"
#include "sf_comms_api.h"
#include "sf_console_cfg.h"

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Version of the API defined in this file */
#define SF_CONSOLE_API_VERSION_MAJOR   (1)
#define SF_CONSOLE_API_VERSION_MINOR   (2)

/** Command to print each command and help in menu */
#define SF_CONSOLE_HELP_COMMAND  ((uint8_t *) "?")
/** Previous command */
#define SF_CONSOLE_MENU_PREVIOUS_COMMAND ((uint8_t *) "^")
/** Root menu command */
#define SF_CONSOLE_ROOT_MENU_COMMAND ((uint8_t *) "~")
/** Input length */
#ifndef SF_CONSOLE_MAX_INPUT_LENGTH
#define SF_CONSOLE_MAX_INPUT_LENGTH (128)
#endif

/** Use this macro to access the next menu layer from this command. */
#define SF_CONSOLE_CALLBACK_NEXT_FUNCTION (void(*)(sf_console_callback_args_t * p_args)) 0x70000000

/**********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Forward facing structure to be used later in this file */
typedef struct st_sf_console_menu sf_console_menu_t;

/** Console handle. DO NOT INITIALIZE.  Initialization occurs when SF_CONSOLE_Open is called */
typedef struct st_sf_console_ctrl
{
    sf_console_menu_t   const * p_current_menu;   ///< Current menu is stored here.
    											   //     This menu is used by console API functions if no menu is passed in.
    sf_comms_instance_t const * p_comms;          ///< Pointer to communications driver instance
    uint8_t                     new_line;         ///< Whether to echo input commands to transmitter
    bool                        echo;             ///< Whether to echo input commands to transmitter
    uint8_t                     input[SF_CONSOLE_MAX_INPUT_LENGTH]; ///< Input buffer used to store user input
} sf_console_ctrl_t;

/** Console callback arguments */
typedef struct st_sf_console_callback_args
{
    sf_console_ctrl_t  * p_ctrl;             ///< Pointer to console that received the command that caused this callback
    uint8_t      const * p_remaining_string; ///< String remaining after parsing command.
    uint8_t      const * context;            ///< Pointer to user provided data.
    uint32_t             bytes;              ///< The number of bytes remaining in the input string
} sf_console_callback_args_t;

/** DEPRECATED definition, please use sf_console_callback_args_t instead. */
typedef sf_console_callback_args_t sf_console_cb_args_t;

/** Console command structure, used to create a console menu with associated callbacks. */
typedef struct st_sf_console_command
{
    uint8_t  * command;                      ///< Command string
    uint8_t  * help;                         ///< Description of command
    void    (* callback)(sf_console_callback_args_t * p_args);  ///< Callback to call when command is selected
    void const * context;                    ///< User provided context passed into callback
} sf_console_command_t;

/** Console menu structure. */
struct st_sf_console_menu
{
    struct st_sf_console_menu  const * menu_prev;       ///< Previous menu
    uint8_t                    const * menu_name;       ///< Menu name, used as a prompt
    uint32_t                           num_commands;    ///< Number of commands in this menu
    sf_console_command_t       const * command_list;    ///< Pointer to an array of commands of length num_commands
};

/** Configuration for RTOS integrated console framework. */
typedef struct st_sf_console_cfg
{
    sf_comms_instance_t const * p_comms;          ///< Pointer to communications driver instance
    sf_console_menu_t   const * p_initial_menu;   ///< First menu to print during Open.
    bool                        echo;             ///< Whether to echo input commands to transmitter
    bool                        autostart;        ///< If true, prompt will occur with p_initial_menu after initialization
} sf_console_cfg_t;

/** Console framework API structure.  Console implementations will use the following API. */
typedef struct st_sf_console_api
{
     /** @brief  This function configures the console.  This function must be called before any other console functions.
     * @par Implemented as
     *  - SF_CONSOLE_Open()
     *
     * @param[in,out] p_ctrl   Pointer to a device structure allocated by user. The device control structure is
     *                         initialized in this function.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of the structure must be set by user.
     */
    ssp_err_t (* open)(sf_console_ctrl_t       * const p_ctrl,
                       sf_console_cfg_t  const * const p_cfg);

     /** @brief  The close API handles cleans up internal driver data.
     * @par Implemented as
     *  - SF_CONSOLE_Close()
     *
     * @param[in]   p_ctrl     Pointer to device control block initialized in Open call for UART driver.
     */
    ssp_err_t (* close)(sf_console_ctrl_t     * const p_ctrl);

    /** @brief  Prints prompt string from menu, waits for input, parses input based on menu, and calls callback function
     *         if a command is identified.
     * @par Implemented as
     *  - SF_CONSOLE_Prompt()
     *
     * @param[in]   p_ctrl     Pointer to device control block initialized in Open call for UART driver.
     * @param[in]   p_menu     Pointer to a menu of valid input commands for this prompt
     * @param[in]   timeout    ThreadX timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), 
     *                         and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* prompt)(sf_console_ctrl_t       * const p_ctrl,
                         sf_console_menu_t const * const p_menu,
                         UINT                      const timeout);    

     /** @brief  Looks for input string in menu, and calls callback function if found.
     * @par Implemented as
     *  - SF_CONSOLE_Parse()
     *
     * @param[in]   p_ctrl     Pointer to device control block initialized in Open call for UART driver.
     * @param[in]   p_cmd_list Pointer to a menu of valid input commands for this prompt
     * @param[in]   p_input    Pointer to a null terminated string to search for in the command list
     * @param[in]   bytes      Length of the input string.
     */
    ssp_err_t (* parse)(sf_console_ctrl_t       * const p_ctrl,
                        sf_console_menu_t const * const p_cmd_list,
                        uint8_t           const * const p_input,
                        uint32_t                  const bytes);

     /** @brief  Reads data into the destination byte by byte and echos input to the console.  Backspace, delete, and
     * left/right arrow keys supported.  Read completes when a line ending CR, CR+LF, or CR+NULL is received,
     * or when the input exceeds the number of bytes input. If the buffer overflows SF_CONSOLE_MAX_INPUT_LENGTH,
     * read will return an error code.
     * @par Implemented as
     *  - SF_CONSOLE_Read()
     *
     * @param[in]   p_ctrl      Pointer to device control block initialized in Open call for UART driver.
     * @param[in]   p_dest      Destination address to read data out
     * @param[in]   bytes       Read data length
     * @param[in]   timeout     ThreadX timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF), 
     *                          and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
  	 */
    ssp_err_t (* read)(sf_console_ctrl_t * const p_ctrl,
                       uint8_t           * const p_dest,
                       uint32_t            const bytes,
                       uint32_t            const timeout);


     /** @brief  The write API gets mutex object and handles UART data transmission at UART HAL layer. gets event flag to
     *          synchronize to completion of data transfer.
     * @par Implemented as
     *  - SF_CONSOLE_Write()
     * @param[in]   p_ctrl      Pointer to device control block initialized in Open call for UART driver.
     * @param[in]   p_src       Pointer to a NULL terminated string.  Length must be less than SF_CONSOLE_MAX_WRITE_LENGTH.
     * @param[in]   timeout     ThreadX timeout. Options include TX_NO_WAIT (0x00000000), TX_WAIT_FOREVER (0xFFFFFFFF),
     *                          and timeout value (0x00000001 through 0xFFFFFFFE) in ThreadX tick counts.
     */
    ssp_err_t (* write)(sf_console_ctrl_t        * const p_ctrl,
                        uint8_t            const * const p_src,
                        uint32_t                   const timeout);

     /** @brief  Finds a command line argument in an input string and returns the index of the character immediately
     *         following the argument and any string numbers converted to integers.
     * @par Implemented as
     *  - SF_CONSOLE_ArgumentFind()
     *
     * @param[in]   p_arg      Pointer to argument to find.
     * @param[in]   p_src      Pointer to source string to find the argument in.
     * @param[out]  p_index    Pointer to location to store index.  Set to -1 if argument is not found in input string.
     *                         Pass NULL if index is not requested.
     * @param[out]  p_data     Pointer to location to store data following the argument.  Set to -1 if argument is not
     *                         found in input string.  Pass NULL if data is not requested.
     */
    ssp_err_t (* argumentFind)(uint8_t const * const p_arg,
                               uint8_t const * const p_str,
                               int32_t       * const p_index,
                               int32_t       * const p_data);

     /** @brief  Stores version information in provided pointer.
     * @par Implemented as
     *  - SF_CONSOLE_VersionGet()
     *
     * @param[out]  p_version  Code and API version used stored here.
     */
    ssp_err_t (* versionGet)(ssp_version_t * const p_version);    
} sf_console_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_console_instance
{
    sf_console_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    sf_console_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sf_console_api_t const * p_api;     ///< Pointer to the API structure for this instance
} sf_console_instance_t;

/*******************************************************************************************************************//**
 * @} (end defgroup SF_CONSOLE_API)
 **********************************************************************************************************************/

#endif /* SF_CONSOLE_API_H */
