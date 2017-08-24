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

#ifndef SF_EL_UX_COMMS_PRIVATE_API_H
#define SF_EL_UX_COMMS_PRIVATE_API_H

/***********************************************************************************************************************
 * Private Instance API Functions. DO NOT USE! Use functions through Interface API structure instead.
 **********************************************************************************************************************/
ssp_err_t SF_EL_UX_COMMS_Open (sf_comms_ctrl_t * const p_ctrl, sf_comms_cfg_t const * const p_cfg);
ssp_err_t SF_EL_UX_COMMS_Close (sf_comms_ctrl_t * const p_ctrl);
ssp_err_t SF_EL_UX_COMMS_Read (sf_comms_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const bytes, UINT const timeout);
ssp_err_t SF_EL_UX_COMMS_Write (sf_comms_ctrl_t * const p_ctrl, uint8_t const * const p_src, uint32_t const bytes, UINT const timeout);
ssp_err_t SF_EL_UX_COMMS_Lock (sf_comms_ctrl_t * const p_ctrl, sf_comms_lock_t lock_type, UINT timeout);
ssp_err_t SF_EL_UX_COMMS_Unlock (sf_comms_ctrl_t * const p_ctrl, sf_comms_lock_t lock_type);
ssp_err_t SF_EL_UX_COMMS_VersionGet (ssp_version_t * const p_version);

#endif /* SF_EL_UX_COMMS_PRIVATE_API_H */
