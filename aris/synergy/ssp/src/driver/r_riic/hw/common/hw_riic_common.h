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
* File Name    : hw_riic_common.h
* Description  : LLD implementation of the RIIC hardware
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup RIIC
 * @{
***********************************************************************************************************************/

#ifndef HW_RIIC_COMMON_H
#define HW_RIIC_COMMON_H

/**********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "bsp_api.h"
#include "r_riic_cfg.h"
#include "r_riic.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** Used to get pointer to specified I2C channel's registers */
#define RIIC_REG_SIZE           (0x0100)

/** Used to get pointer to specified I2C channel's registers */
#define RIIC_REGS(channel)      ((R_IIC0_Type*)(R_IIC0_BASE + (RIIC_REG_SIZE * (channel))))

/** Used to find the module stop bit corresponding to the specified I2C channel */
#define RIIC_MSTP_BIT(channel)  (0x00000200U >> (channel))

/** The timeout interrupt enable bit */
#define RIIC_TMO_EN_BIT     (0x01)
/** The arbitration loss detection interrupt enable bit */
#define RIIC_ALD_EN_BIT     (0x02)
/** The start condition detection interrupt enable bit */
#define RIIC_STR_EN_BIT     (0x04)
/** The stop condition detection interrupt enable bit */
#define RIIC_STP_EN_BIT     (0x08)
/** The NAK reception interrupt enable bit */
#define RIIC_NAK_EN_BIT     (0x10)
/** The receive data full interrupt enable bit */
#define RIIC_RXI_EN_BIT     (0x20)
/** The transmit end interrupt enable bit */
#define RIIC_TEI_EN_BIT     (0x40)
/** The transmit data empty interrupt enable bit */
#define RIIC_TXI_EN_BIT     (0x80)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/** Hardware errors and events that can occur on the RIIC */
typedef enum e_riic_err_event
{
    RIIC_ERR_EVENT_NONE = 0,
    RIIC_ERR_EVENT_TIMEOUT = 1,
    RIIC_ERR_EVENT_ARBITRATION_LOSS = 2,
    RIIC_ERR_EVENT_START = 4,
    RIIC_ERR_EVENT_STOP = 8,
    RIIC_ERR_EVENT_NACK = 16
} riic_err_t;

IRQn_Type riic_rxi[] =
{
#if (defined(BSP_IRQ_CFG_IIC0_RXI) && (BSP_IRQ_CFG_IIC0_RXI != BSP_IRQ_DISABLED))
    IIC0_RXI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (defined(BSP_IRQ_CFG_IIC1_RXI) && (BSP_IRQ_CFG_IIC1_RXI != BSP_IRQ_DISABLED))
    IIC1_RXI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (BSP_CFG_MCU_PART_SERIES != 1)
#if (defined(BSP_IRQ_CFG_IIC2_RXI) && (BSP_IRQ_CFG_IIC2_RXI != BSP_IRQ_DISABLED))
    IIC2_RXI_IRQn
#else
    BSP_MAX_NUM_IRQn,
#endif
#endif
};

IRQn_Type riic_txi[] =
{
#if (defined(BSP_IRQ_CFG_IIC0_TXI) && (BSP_IRQ_CFG_IIC0_TXI != BSP_IRQ_DISABLED))
    IIC0_TXI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (defined(BSP_IRQ_CFG_IIC1_TXI) && (BSP_IRQ_CFG_IIC1_TXI != BSP_IRQ_DISABLED))
    IIC1_TXI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (defined(BSP_IRQ_CFG_IIC2_TXI) && (BSP_IRQ_CFG_IIC2_TXI != BSP_IRQ_DISABLED))
    IIC2_TXI_IRQn
#else
    BSP_MAX_NUM_IRQn
#endif
};

IRQn_Type riic_tei[] =
{
#if (defined(BSP_IRQ_CFG_IIC0_TEI) && (BSP_IRQ_CFG_IIC0_TEI != BSP_IRQ_DISABLED))
    IIC0_TEI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (defined(BSP_IRQ_CFG_IIC1_TEI) && (BSP_IRQ_CFG_IIC1_TEI != BSP_IRQ_DISABLED))
    IIC1_TEI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (defined(BSP_IRQ_CFG_IIC2_TEI) && (BSP_IRQ_CFG_IIC2_TEI != BSP_IRQ_DISABLED))
    IIC2_TEI_IRQn
#else
    BSP_MAX_NUM_IRQn
#endif
};

IRQn_Type riic_eei[] =
{
#if (defined(BSP_IRQ_CFG_IIC0_ERI) && (BSP_IRQ_CFG_IIC0_ERI != BSP_IRQ_DISABLED))
    IIC0_ERI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (defined(BSP_IRQ_CFG_IIC1_ERI) && (BSP_IRQ_CFG_IIC1_ERI != BSP_IRQ_DISABLED))
    IIC1_ERI_IRQn,
#else
    BSP_MAX_NUM_IRQn,
#endif
#if (defined(BSP_IRQ_CFG_IIC2_ERI) && (BSP_IRQ_CFG_IIC2_ERI != BSP_IRQ_DISABLED))
    IIC2_ERI_IRQn
#else
    BSP_MAX_NUM_IRQn
#endif
};

/***********************************************************************************************************************
Private function prototypes
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Private Functions
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Supplies power to the specified RIIC block.
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_Power           (uint32_t   const channel, bool     const   enable)
{
    if (enable)
    {
        R_MSTP->MSTPCRB &= ~(RIIC_MSTP_BIT(channel));
    }
    else
    {
        R_MSTP->MSTPCRB |=   RIIC_MSTP_BIT(channel);
    }
}

/*******************************************************************************************************************//**
 * Enable or disable output of SDA/SCL.
 * @param   channel The hardware channel to access
 * @param   enable  True when enabling SDA/SCL pins, otherwise disable SDA/SCL pin output
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_Output          (uint32_t   const channel, bool     const   enable)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICCR1_b.ICE = enable;

    /* Give time for value to be updated */
    volatile uint8_t dummy;
    dummy  = p_riic_n->ICCR1_b.ICE;
    dummy  = p_riic_n->ICCR1_b.ICE;
    dummy  = p_riic_n->ICCR1_b.ICE;
    SSP_PARAMETER_NOT_USED(dummy);
}

/*******************************************************************************************************************//**
 * Set whether the peripheral is in the reset state.
 * @param   channel The hardware channel to access
 * @param   enable  True when enabling SDA/SCL pins, otherwise disable SDA/SCL pin output
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_Reset           (uint32_t   const channel, bool     const   enable)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICCR1_b.IICRST = enable;

    /* Give time for value to be updated */
    volatile uint8_t dummy;
    dummy  = p_riic_n->ICCR1_b.IICRST;
    dummy  = p_riic_n->ICCR1_b.IICRST;
    dummy  = p_riic_n->ICCR1_b.IICRST;
    SSP_PARAMETER_NOT_USED(dummy);
}

/*******************************************************************************************************************//**
 * Sets the communications rate of the specified RIIC block.
 * @param   channel     The hardware channel to access
 * @param   highCount   The number of counts to spend with the clock line at high level
 * @param   lowCount    The number of counts to spend with the clock line at low level
 * @param   refClock    The setting that chooses the divisor for the reference clock
 * @param   fmplus      Whether to enable or disable Fm+ mode
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ClockCfg        (uint32_t   const channel,
                                                 uint8_t    const highCount,
                                                 uint8_t    const lowCount,
                                                 uint8_t    const refClock,
                                                 bool       const fmplus)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Set the number of counts that the clock remains high */
    p_riic_n->ICBRH_b.BRH = (uint8_t)(0x1F & highCount);

    /* Set the number of counts that the clock remains low */
    p_riic_n->ICBRL_b.BRL = (uint8_t)(0x1F & lowCount);

    /* Set the internal reference clock source for generating IIC clock */
    p_riic_n->ICMR1_b.CKS = (uint8_t)(0x7 & refClock);

    /* Set whether the Fm+ slope control circuit is used */
    p_riic_n->ICFER_b.FMPE = fmplus;
}

/*******************************************************************************************************************//**
 * Enables the timeout function of the specified RIIC block.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_TimeoutCfg      (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Enable timeout function */
    p_riic_n->ICFER_b.TMOE = 1;

    /* Disable writing to TMOCNT, and disable counting when clock is high */
    p_riic_n->ICMR2 &= 0xF3;

    /* Select short mode, count when SCL line is at low level */
    p_riic_n->ICMR2 |= 0x03;
}


/*******************************************************************************************************************//**
 * Enables/Disables the timeout function of the specified RIIC block.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_TimeoutEnable      (uint32_t   const channel, bool enable)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);
    if (true == enable)
    {
        /* Enable timeout function */
        p_riic_n->ICFER_b.TMOE = 1;
    }
    else
    {
        /* Disable timeout function */
        p_riic_n->ICFER_b.TMOE = 0;
    }

}

/*******************************************************************************************************************//**
 * Function for disabling features of the peripheral that are not supported.
 * @param   channel The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_NoSupport       (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Disable SDA delay features */
    p_riic_n->ICMR2 &= 0x0F;

    /* Disable noise filter, wait, and SMBus features */
    p_riic_n->ICMR3 = 0x00;
    p_riic_n->ICFER_b.NFE = 0;
}

/*******************************************************************************************************************//**
 * Sets up features on the specified RIIC block for master mode.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_SetupMaster     (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Enable master arbitration loss detection */
    p_riic_n->ICFER_b.MALE  = 1;

    /* Enable suspending transfer when NACK received */
    p_riic_n->ICFER_b.NACKE = 1;
}

/*******************************************************************************************************************//**
 * Signals the specified RIIC block to send the start condition.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_SendStart       (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Request RIIC to issue the start condition */
    p_riic_n->ICCR2_b.ST = 1;
}

/*******************************************************************************************************************//**
 * Read one byte from the specified RIIC block.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RIIC_ReadByte        (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);
    return p_riic_n->ICDRR;
}

/*******************************************************************************************************************//**
 * Write one byte to the specified RIIC block.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_WriteByte       (uint32_t   const channel, uint8_t  const   byte)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICDRT = byte;
}

/*******************************************************************************************************************//**
 * Signals the specified RIIC block to send the stop condition.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_SendStop        (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Request RIIC to issue the stop condition */
    p_riic_n->ICCR2_b.SP = 1;

    /* Give time for value to be updated */
    volatile uint8_t dummy;
    dummy  = p_riic_n->ICCR2;
    dummy  = p_riic_n->ICCR2;
    dummy  = p_riic_n->ICCR2;
    SSP_PARAMETER_NOT_USED(dummy);
}

/*******************************************************************************************************************//**
 * Signals the specified RIIC block to send the restart condition.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_SendRestart     (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Request RIIC to issue the restart condition */
    p_riic_n->ICCR2_b.RS = 1;
}

/*******************************************************************************************************************//**
 * Sets whether errors in the specified RIIC block should cause interrupts.
 * @param   channel     The hardware channel to access
 * @param   enable      True when enabling errors, otherwise disable error reporting
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ErrIntEn        (uint32_t   const channel, bool     const   enable)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    if (enable)
    {
        p_riic_n->ICIER |=  (RIIC_TMO_EN_BIT | RIIC_ALD_EN_BIT | RIIC_NAK_EN_BIT | RIIC_STR_EN_BIT | RIIC_STP_EN_BIT);

        NVIC_EnableIRQ(riic_eei[channel]);
    }
    else
    {
        p_riic_n->ICIER &= (uint8_t)~(RIIC_TMO_EN_BIT | RIIC_ALD_EN_BIT | RIIC_NAK_EN_BIT | RIIC_STR_EN_BIT | RIIC_STP_EN_BIT);

        NVIC_DisableIRQ(riic_eei[channel]);
    }
}

/*******************************************************************************************************************//**
 * Sets whether receive data register becoming full should cause an interrupt from the specified RIIC block.
 * @param   channel     The hardware channel to access
 * @param   enable      True when enabling, otherwise disable
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_RxFullIntEn     (uint32_t   const channel, bool     const   enable)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    if (enable)
    {
        p_riic_n->ICIER |=  RIIC_RXI_EN_BIT;

        NVIC_EnableIRQ(riic_rxi[channel]);
    }
    else
    {
        p_riic_n->ICIER &= (uint8_t)~RIIC_RXI_EN_BIT;

        NVIC_DisableIRQ(riic_rxi[channel]);
    }
}

/*******************************************************************************************************************//**
 * Sets whether transmit finishing should cause an interrupt from the specified RIIC block.
 * @param   channel     The hardware channel to access
 * @param   enable      True when enabling, otherwise disable
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_TxEndIntEn      (uint32_t   const channel, bool     const   enable)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    if (enable)
    {
        p_riic_n->ICIER |=  RIIC_TEI_EN_BIT;

        NVIC_EnableIRQ(riic_tei[channel]);
    }
    else
    {
        p_riic_n->ICIER &= (uint8_t)~RIIC_TEI_EN_BIT;

        NVIC_DisableIRQ(riic_tei[channel]);
    }
}

/*******************************************************************************************************************//**
 * Sets whether transmit data register becoming empty should cause an interrupt from the specified RIIC block.
 * @param   channel     The hardware channel to access
 * @param   enable      True when enabling, otherwise disable
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_TxEmptyIntEn    (uint32_t   const channel, bool     const   enable)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    if (enable)
    {
        p_riic_n->ICIER |=  RIIC_TXI_EN_BIT;

        NVIC_EnableIRQ(riic_txi[channel]);
    }
    else
    {
        p_riic_n->ICIER &= (uint8_t)~RIIC_TXI_EN_BIT;

        NVIC_DisableIRQ(riic_txi[channel]);
    }
}

/*******************************************************************************************************************//**
 * Disables all interrupt generation from the specified RIIC block.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_DisableInts     (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICIER = 0x00;

    NVIC_DisableIRQ(riic_eei[channel]);
    NVIC_DisableIRQ(riic_rxi[channel]);
    NVIC_DisableIRQ(riic_tei[channel]);
    NVIC_DisableIRQ(riic_txi[channel]);
}

/*******************************************************************************************************************//**
 * Clears the transmit end interrupt.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ClearTxEnd      (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Clear peripheral flag */
    if(1 == p_riic_n->ICSR2_b.TEND)
    {
        p_riic_n->ICSR2_b.TEND = 0;
    }
    p_riic_n->ICSR2_b.TEND = 0;

    R_BSP_IrqStatusClear(riic_tei[channel]);
}

/*******************************************************************************************************************//**
 * Clears all interrupts.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ClearIrqs       (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICSR2 = 0x00;

    R_BSP_IrqStatusClear(riic_eei[channel]);
    NVIC_ClearPendingIRQ(riic_eei[channel]);

    R_BSP_IrqStatusClear(riic_rxi[channel]);
    NVIC_ClearPendingIRQ(riic_rxi[channel]);

    R_BSP_IrqStatusClear(riic_tei[channel]);
    NVIC_ClearPendingIRQ(riic_tei[channel]);

    R_BSP_IrqStatusClear(riic_txi[channel]);
    NVIC_ClearPendingIRQ(riic_txi[channel]);
}

/*******************************************************************************************************************//**
 * Returns true when this channel is the master using the bus.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE bool    HW_RIIC_ActiveMaster    (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    return (1 == p_riic_n->ICCR2_b.MST);
}

/*******************************************************************************************************************//**
 * Returns true when the SDA line is being held low.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE bool    HW_RIIC_SDALow          (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    return (0 == p_riic_n->ICCR1_b.SDAI);
}

/*******************************************************************************************************************//**
 * Forces an extra clock cycle to be output according to the configured rate.
 *
 * @see HW_RIIC_ExtraClockDone
 *
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ExtraClock      (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICCR1_b.CLO = 1;

    /* Give time for value to be updated */
    volatile uint8_t dummy;
    dummy  = p_riic_n->ICCR1_b.CLO;
    dummy  = p_riic_n->ICCR1_b.CLO;
    dummy  = p_riic_n->ICCR1_b.CLO;
    SSP_PARAMETER_NOT_USED(dummy);
}

/*******************************************************************************************************************//**
 * Returns true when the extra clock cycle output has finished.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE bool    HW_RIIC_ExtraClockDone  (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    return (0 == p_riic_n->ICCR1_b.CLO);
}

/*******************************************************************************************************************//**
 * Returns a bitfield of the errors that were recorded on the channel and then clears the hardware status.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RIIC_GetAndClearErrsEvents (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    uint8_t errs_events = 0;

    if (p_riic_n->ICSR2_b.TMOF)
    {
        errs_events |= RIIC_ERR_EVENT_TIMEOUT;
    }
    if (p_riic_n->ICSR2_b.AL)
    {
        errs_events |= RIIC_ERR_EVENT_ARBITRATION_LOSS;
    }
    if (p_riic_n->ICSR2_b.NACKF)
    {
        errs_events |= RIIC_ERR_EVENT_NACK;
    }
    if (p_riic_n->ICSR2_b.START)
    {
        errs_events |= RIIC_ERR_EVENT_START;
    }
    if (p_riic_n->ICSR2_b.STOP)
    {
        errs_events |= RIIC_ERR_EVENT_STOP;
    }

    /** Clear all the event flags except the receive data full, transmit end and transmit data empty flags*/
    p_riic_n->ICSR2 &= 0xE0;

    return errs_events;
}

/*******************************************************************************************************************//**
 * Returns true when a requested stop condition has not yet been issued.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE bool    HW_RIIC_StopPending     (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    return (1 == p_riic_n->ICCR2_b.SP);
}

/*******************************************************************************************************************//**
 * Resets the internal timeout counter.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ResetTimeout    (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Disable timeout function */
    p_riic_n->ICFER_b.TMOE = 0;

    /* Clear the timeout status flag */
    if(1 == p_riic_n->ICSR2_b.TMOF)
    {
        p_riic_n->ICSR2_b.TMOF = 0;
    }
    p_riic_n->ICSR2_b.TMOF = 0;

    /* Enable writing to timeout counter */
    p_riic_n->ICMR2_b.DLCS = 1;

    /* Reset timeout counter */
    p_riic_n->SARLnRC0[0].SARLn = 0;
    p_riic_n->SARLnRC0[0].SARUn = 0;

    /* Disable writing to timeout counter */
    p_riic_n->ICMR2_b.DLCS = 0;

    /* Enable timeout function */
    p_riic_n->ICFER_b.TMOE = 1;
}

/*******************************************************************************************************************//**
 * Returns true when the internal timer has timed out.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE bool    HW_RIIC_TimedOut        (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    return (1 == p_riic_n->ICSR2_b.TMOF);
}

/*******************************************************************************************************************//**
 * Returns the number of data bits left in the current byte transfer.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t HW_RIIC_BitsPending     (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    uint8_t bits = p_riic_n->ICMR1_b.BC;

    if (bits == 0)
    {
        bits = 8;
    }

    return ++bits;
}

/*******************************************************************************************************************//**
 * Ensures the SDA pin is not being driven low by the peripheral.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_SDARelease      (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    /* Clear the SOWP bit and set SDAO to stop driving SDA */
    p_riic_n->ICCR1_b.SOWP = 0;
    p_riic_n->ICCR1_b.SDAO = 1;
    // ??? p_riic_n->ICCR1 = (uint8_t)(p_riic_n->ICCR1 & (uint8_t)0xEF) | (uint8_t)0x04;
}

/*******************************************************************************************************************//**
 * Sets the wait bit so the hardware knows when to perform wait processing.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_SetWait         (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICMR3_b.WAIT = 1;
}

/*******************************************************************************************************************//**
 * Clear the wait bit so the hardware knows when to perform wait processing.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ClrWait         (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICMR3_b.WAIT = 0;
}


/*******************************************************************************************************************//**
 * Clear the stop bit before issuing the stop signal.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ClrStop         (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICSR2_b.STOP = 0;
}

/*******************************************************************************************************************//**
 * Clear the STOP and NACKF bits for processing next transfer operation.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_ClrStopNackf         (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICSR2_b.NACKF = 0;
    p_riic_n->ICSR2_b.STOP  = 0;
}

/*******************************************************************************************************************//**
 * Sets the acknowledge bit when getting ready to signal the slave to stop transmitting.
 * @param   channel     The hardware channel to access
 **********************************************************************************************************************/
__STATIC_INLINE void    HW_RIIC_SetAck          (uint32_t   const channel)
{
    R_IIC0_Type * const p_riic_n = RIIC_REGS(channel);

    p_riic_n->ICMR3_b.ACKWP = 1;

    /* Give time for value to be updated */
    volatile uint8_t dummy;
    dummy  = p_riic_n->ICMR3_b.ACKWP;
    dummy  = p_riic_n->ICMR3_b.ACKWP;
    dummy  = p_riic_n->ICMR3_b.ACKWP;
    SSP_PARAMETER_NOT_USED(dummy);

    p_riic_n->ICMR3_b.ACKBT = 1;
}

#endif /* HW_RIIC_COMMON_H */
