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

#ifndef SF_CONSOLE_PRIVATE_API_H
#define SF_CONSOLE_PRIVATE_API_H

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Instance API Functions. DO NOT USE! Use functions through Interface API structure instead.
 **********************************************************************************************************************/
ssp_err_t SF_CONSOLE_Open(sf_console_ctrl_t      * const p_ctrl,
                          sf_console_cfg_t const * const p_cfg );
ssp_err_t SF_CONSOLE_Close(sf_console_ctrl_t     * const p_ctrl);
ssp_err_t SF_CONSOLE_Prompt(sf_console_ctrl_t       * const p_ctrl,
                            sf_console_menu_t const * const p_menu,
                            UINT                       const timeout);
ssp_err_t SF_CONSOLE_Parse(sf_console_ctrl_t        * const p_ctrl,
                           sf_console_menu_t  const * const p_cmd_list,
                           uint8_t            const * const p_input,
                           uint32_t                   const bytes);
ssp_err_t SF_CONSOLE_Read(sf_console_ctrl_t * const p_ctrl,
                          uint8_t           * const p_dest,
                          uint32_t            const bytes,
                          uint32_t            const timeout);
ssp_err_t SF_CONSOLE_Write(sf_console_ctrl_t * const p_ctrl,
                           uint8_t     const * const p_src,
                           uint32_t            const timeout);
ssp_err_t SF_CONSOLE_ArgumentFind(uint8_t const * const p_arg,
                                  uint8_t const * const p_str,
                                  int32_t       * const p_index,
                                  int32_t       * const p_data);
void SF_CONSOLE_CallbackNextMenu(sf_console_callback_args_t * p_args);
ssp_err_t SF_CONSOLE_VersionGet(ssp_version_t * const p_version);

#endif /* SF_CONSOLE_PRIVATE_API_H */
