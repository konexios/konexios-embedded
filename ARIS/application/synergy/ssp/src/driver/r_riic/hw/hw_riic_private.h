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
* File Name    : hw_riic_private.h
* Description  : I2C Bus (RIIC) Module hardware private header file.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup RIIC
 * @{
***********************************************************************************************************************/

#ifndef HW_RIIC_PRIVATE_H
#define HW_RIIC_PRIVATE_H

/**********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"
#include "r_riic.h"

/**********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#if defined(__GNUC__) && defined(__ARM_EABI__)
#define INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define INLINE_ATTRIBUTE
#endif

#define RIIC_MAX_CH             (3)
#define RIIC_MAX_DIVISOR        (128)
#define RIIC_MAX_BITS_BR_REGS   (5)

/**********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/**********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_Power                 (uint32_t const channel, bool const enable)      INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_Output                (uint32_t const channel, bool const enable)      INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_Reset                 (uint32_t const channel, bool const enable)      INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_ClockCfg              (uint32_t const channel, uint8_t const highCount,
                                                       uint8_t  const lowCount,uint8_t const refClock,
                                                       bool     const fmplus)                          INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_NoSupport             (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_SetupMaster           (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_SendStart             (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE uint8_t HW_RIIC_ReadByte              (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_WriteByte             (uint32_t const channel, uint8_t const byte)     INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_SendStop              (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_SendRestart           (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_ErrIntEn              (uint32_t const channel, bool const enable)      INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_RxFullIntEn           (uint32_t const channel, bool const enable)      INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_TxEndIntEn            (uint32_t const channel, bool const enable)      INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_TxEmptyIntEn          (uint32_t const channel, bool const enable)      INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_DisableInts           (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_ClearTxEnd            (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_ClearIrqs             (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE bool    HW_RIIC_ActiveMaster          (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE bool    HW_RIIC_SDALow                (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_ExtraClock            (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE bool    HW_RIIC_ExtraClockDone        (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE uint8_t HW_RIIC_GetAndClearErrsEvents (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE bool    HW_RIIC_StopPending           (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_ResetTimeout          (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE bool    HW_RIIC_TimedOut              (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE uint8_t HW_RIIC_BitsPending           (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_SDARelease            (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_SetWait               (uint32_t const channel)                         INLINE_ATTRIBUTE;
__STATIC_INLINE void    HW_RIIC_SetAck                (uint32_t const channel)                         INLINE_ATTRIBUTE;

/**********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* Select implementation based on BSP here */
#include "common/hw_riic_common.h"

#endif /* HW_RIIC_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RIIC)
***********************************************************************************************************************/
