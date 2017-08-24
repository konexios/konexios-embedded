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

#ifndef SF_SPI_PRIVATE_API_H
#define SF_SPI_PRIVATE_API_H

/***********************************************************************************************************************
 * Private Instance API Functions. DO NOT USE! Use functions through Interface API structure instead.
 **********************************************************************************************************************/
ssp_err_t SF_SPI_Open(sf_spi_ctrl_t * const p_ctrl, sf_spi_cfg_t const * const p_cfg);
ssp_err_t SF_SPI_Close(sf_spi_ctrl_t * const p_ctrl);
ssp_err_t SF_SPI_Read(sf_spi_ctrl_t  * const  p_ctrl,
                      void * const p_dest,
                      uint32_t const length,
                      spi_bit_width_t const bit_width,
                      uint32_t const timeout);
ssp_err_t SF_SPI_Write(sf_spi_ctrl_t * const  p_ctrl,
                       void * const p_src,
                       uint32_t const length,
                       spi_bit_width_t const  bit_width,
                       uint32_t const timeout);
ssp_err_t SF_SPI_WriteRead(sf_spi_ctrl_t * const  p_ctrl,
                           void * const p_src,
                           void * const p_dest,
                           uint32_t const length,
                           spi_bit_width_t const bit_width,
                           uint32_t const timeout);
ssp_err_t SF_SPI_Lock(sf_spi_ctrl_t * const p_ctrl);
ssp_err_t SF_SPI_Unlock(sf_spi_ctrl_t * const p_ctrl);
ssp_err_t SF_SPI_VersionGet (ssp_version_t   * const p_version);

#endif /* SF_SPI_PRIVATE_API_H */
