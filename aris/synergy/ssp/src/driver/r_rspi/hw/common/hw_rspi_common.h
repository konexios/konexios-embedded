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
 * File Name    : hw_rspi_common.h
 * Description  : Serial Peripheral Interface (RSPI) hardware common header file.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup RSPI
 * @{
 **********************************************************************************************************************/

#ifndef HW_RSPI_COMMON_H
#define HW_RSPI_COMMON_H

/**********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/* Public interface header file for this module. */
#include "r_rspi.h"
#include "../../r_rspi_private.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/** RSPI register access or macro  */
#define RSPI_REG(channel) ((R_RSPI0_Type *) ((uint32_t) R_RSPI0 + ((uint32_t) R_RSPI1 - (uint32_t) R_RSPI0) * (channel)))

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static IRQn_Type r_rspi_select_irqn (uint32_t const channel, rspi_int_t rspi_int);
/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/*****************************************************************************************************************//**
 * @brief     Sets BRR for baud rate generator register
 * @param[in] channel    RSPI channel number
 * @param[in] brr        SPBR register setting  value
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_BitRateSet (uint32_t const channel, const uint8_t brr)
{
    /* Setting the BRR bits for the bit rate register */
    RSPI_REG(channel)->SPBR = brr;
}  /* End of function HW_RSPI_BitRateSet() */

/***************************************************************************************************************
 * @brief   This function switches power on or off to a RSPI channel.
 *
 * @param[in]  channel    channel number
 *             on_or_off  Power output condition
 * @retval  void
 * @note      the parameter check must be held by HLD
 ***************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_Power_On_Off (uint8_t channel, uint8_t on_or_off)
{
    switch (channel)
    {
        case 0:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
            R_MSTP->MSTPCRB_b.MSTPB19 = (uint8_t) (on_or_off & 0x01);
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
            break;

        case 1:
            R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_OM_LPC_BATT);
            R_MSTP->MSTPCRB_b.MSTPB18 = (uint8_t) (on_or_off & 0x01);
            R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_OM_LPC_BATT);
            break;

        default:
            break;
    }
} /* End of function HW_RSPI_Power_On_Off() */

/*****************************************************************************************************************//**
 * @brief     Clears the SPI command register 0
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_CommandClear (uint32_t const channel)
{
    /* Clears the SPI command register SPCMdn */
    RSPI_REG(channel)->SPCMDn[0] = 0;
}  /* End of function HW_RSPI_CommandClear() */

/*****************************************************************************************************************//**
 * @brief     Clears the RSPI Data Control register
 * @param[in] channel    RSPI channel number
 * @param[in] frames     number of frames
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_DataControlClear (uint32_t const channel)
{
    /* Clears data control register */
    RSPI_REG(channel)->SPDCR = 0;
}  /* End of function HW_RSPI_DataControlClear() */

/*****************************************************************************************************************//**
 * @brief     Sets the number of frames to used for SPI communication
 * @param[in] channel    RSPI channel number
 * @param[in] frames     number of frames
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_DefaultDataFrame (uint32_t const channel)
{
     /* Sets the frames number in data control register SPDCR */
     RSPI_REG(channel)->SPDCR_b.SPFC = 0;
}  /* End of function HW_RSPI_DataFrameSet() */

/*****************************************************************************************************************//**
 * @brief     Sets the default data access length to use longword
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_DefaultDataBitLength (uint32_t const channel)
{
     /* Sets the default data access to use long word */
	/* 0 = word; 1 = long word */
     RSPI_REG(channel)->SPDCR_b.SPLW = 1;
}  /* End of function HW_RSPI_DefaultDataBitLength() */

/*****************************************************************************************************************//**
 * @brief     Sets the data access length to use halfword
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_DataBitLength_HalfWord (uint32_t const channel)
{
     /* Sets the data access to use half word */
	/* 0 = word; 1 = long word */
     RSPI_REG(channel)->SPDCR_b.SPLW = 0;
}  /* End of function HW_RSPI_DefaultDataBitLength() */

/*****************************************************************************************************************//**
 * @brief     Sets the default Bit Rate Divisor in the SPI command register 0
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_DefaultBRDV (uint32_t const channel)
{
     /* Sets the default Bit Rate Divisor for the BRR register */
	 RSPI_REG(channel)->SPCMDn_b[0].BRDV = 0;
}  /* End of function HW_RSPI_DefaultBRDVSet() */

/*****************************************************************************************************************//**
 * @brief     Sets the default sequence length for transmission the Sequence Control register SPSCR
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_DefaultSequence (uint32_t const channel)
{
     /* Sets the default sequence for the data frames */
	 RSPI_REG(channel)->SPSCR_b.SPSLN = 0;
}  /* End of function HW_RSPI_DefaultSequenceSet() */

/*****************************************************************************************************************//**
 * @brief     Sets the operating mode in master or slave in SPCR register
 * @param[in] channel    RSPI channel number
 * @param[in] master     true-- master; false--slave
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_OperatingModeMaster (uint32_t const channel, bool const master)
{
     /* Sets the operating mode to master or slave */
	 RSPI_REG(channel)->SPCR_b.MSTR = master;
}  /* End of function HW_RSPI_OperatingModeMaster() */

/*****************************************************************************************************************//**
 * @brief     Sets the clock phase in the SPI command register (SPCMD0)
 * @param[in] channel    RSPI channel number
 * @param[in] even       true--phase shift(even); false--no phase shift(odd)
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ClockPhaseEven (uint32_t const channel, bool const even)
{
    /* Sets the clock phase */
	 RSPI_REG(channel)->SPCMDn_b[0].CPHA = even;
}  /* End of function HW_RSPI_ClockPhaseEven() */

/*****************************************************************************************************************//**
 * @brief     Sets the clock polarity in the SPI command register (SPCMD0)
 * @param[in] channel    RSPI channel number
 * @param[in] high       true--clock high at idle; false--clock low at idle
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ClockPolarityHigh (uint32_t const channel, bool const high)
{
     /* Sets the clock polarity */
	 RSPI_REG(channel)->SPCMDn_b[0].CPOL = high;
}  /* End of function HW_RSPI_ClockPolarityHigh() */

/*****************************************************************************************************************//**
 * @brief     Enables/disables the mode fault detection in the SPCR register
 * @param[in] channel    RSPI channel number
 * @param[in] on         true--detection 0n; false--detection off
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ModeFaultDetectionOn (uint32_t const channel, bool const on)
{
     /* Enable/disable the mode fault detection */
	 RSPI_REG(channel)->SPCR_b.MODFEN = on;
}  /* End of function HW_RSPI_ModeFaultDetectionOn() */

/*****************************************************************************************************************//**
 * @brief     Sets the bit order to MSB or LSB in the SPI command register
 * @param[in] channel    RSPI channel number
 * @param[in] lsb        true--LSB first; false--MSB first
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_BitOrderLSB (uint32_t const channel, bool const lsb)
{
     /* Sets the bit order to LSB or MSB */
	 RSPI_REG(channel)->SPCMDn_b[0].LSBF = lsb;
}  /* End of function HW_RSPI_BitOrderLSB() */

/*****************************************************************************************************************//**
 * @brief     Sets the communication to SPI mode or clock synchronous mode in the SPCR register.
 * @param[in] channel    RSPI channel number
 * @param[in] clk_syn    true--clock synchronous mode; false-- SPI mode
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE bool HW_RSPI_OperationClkSyn (uint32_t const channel, bool const clk_syn)
{
     /* Sets the communication to SPI or Clock Synchronous mode */
	return (RSPI_REG(channel)->SPCR_b.SPMS = clk_syn);
} /* End of function HW_RSPI_OperationClkSyn() */

/*****************************************************************************************************************//**
 * @brief     Sets the communication mode to full duplex or transmit only in the SPCR register.
 * @param[in] channel    RSPI channel number
 * @param[in] transmit_only  true--transmit only; false--full duplex communication mode
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_CommunicationTransmitOnly (uint32_t const channel, bool const transmit_only)
{
     /* Sets the communication to transmit only or full duplex mode */
	 RSPI_REG(channel)->SPCR_b.TXMD = transmit_only;
}  /* End of function HW_RSPI_CommunicationTransmitOnly() */

/*****************************************************************************************************************//**
 * @brief     Sets the slave select polarity in the SSLP register.
 * @param[in] channel    RSPI channel number
 * @param[in] ssl        SSL0P, SSL1P, SSL2P, SSL3P
 * @param[in] high       true--high; false--low
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_SlaveSelectPolarity (uint32_t const channel, rspi_ssl_select_t const ssl, bool const high)
{
     /* Sets the slave select polarity to high or low. True--high; false--low */
	switch(ssl)
	{
        case RSPI_SSL_SELECT_SSL0:
       	    RSPI_REG(channel)->SSLP_b.SSL0P = high;
            break;
        case RSPI_SSL_SELECT_SSL1:
       	    RSPI_REG(channel)->SSLP_b.SSL1P = high;
            break;
        case RSPI_SSL_SELECT_SSL2:
       	    RSPI_REG(channel)->SSLP_b.SSL2P = high;
            break;
        case RSPI_SSL_SELECT_SSL3:
       	    RSPI_REG(channel)->SSLP_b.SSL3P = high;
            break;
        default:
            break;
	}
}  /* End of function HW_RSPI_SlaveSelectPolarity() */

/*****************************************************************************************************************//**
 * @brief     Sets loopback1 mode in pin control (SPPCR) register.
 * @param[in] channel    RSPI channel number
 * @param[in] loopback1   true--loopback1 mode with data inverted; false--normal mode
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_loopback1 (uint32_t const channel, bool const loopback1)
{
     /* Sets the data pin to loopback1 or normal mode. True--loopback1 mode; false--normal mode */
	 RSPI_REG(channel)->SPPCR_b.SPLP = loopback1;
}  /* End of function HW_RSPI_loopback1() */

/*****************************************************************************************************************//**
 * @brief     Sets loopback2 mode in pin control (SPPCR) register.
 * @param[in] channel    RSPI channel number
 * @param[in] loopback2   true--loopback2 mode with data not inverted; false--normal mode
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_loopback2 (uint32_t const channel, bool const loopback2)
{
     /* Sets the data pin to loopback2 or normal mode. True--loopback2 mode; false--normal mode */
	 RSPI_REG(channel)->SPPCR_b.SPLP2 = loopback2;
}  /* End of function HW_RSPI_loopback2() */

/*****************************************************************************************************************//**
 * @brief     Sets the MOSI idle fixed value (MOIFV) in the pin control (SPPCR) register.
 * @param[in] channel    RSPI channel number
 * @param[in] high   true--high; false--low
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_MOSIIdleLevelHigh (uint32_t const channel, bool const high)
{
     /* Sets the MOSI idle fixed value. True--high; false--low */
	 RSPI_REG(channel)->SPPCR_b.MOIFV = high;
}  /* End of function HW_RSPI_MOSIIdleLevelHigh() */

/*****************************************************************************************************************//**
 * @brief     Enables/disables the MOSI idle fixed value (MOIFE) in the pin control (SPPCR) register.
 * @param[in] channel    RSPI channel number
 * @param[in] enable     true--enable; false--disable
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_MOSIIdleEnable (uint32_t const channel, bool const enable)
{
     /* Enables/disables the MOSI idle fixed value. True--enable; false--disable */
	 RSPI_REG(channel)->SPPCR_b.MOIFE = enable;
}  /* End of function HW_RSPI_MOSIIdleEnable() */

/*****************************************************************************************************************//**
 * @brief     Enables/disables the parity in the control register 2 (SPCR2).
 * @param[in] channel    RSPI channel number
 * @param[in] enable     true--enable parity; false--disable parity
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ParityEnable (uint32_t const channel, bool const enable)
{
     /* Enables/disables the parity. True--enables parity; false--disable parity */
	 RSPI_REG(channel)->SPCR2_b.SPPE = enable;
}  /* End of function HW_RSPI_ParityEnable() */

/*****************************************************************************************************************//**
 * @brief     Sets the parity type in the control register 2 (SPCR2).
 * @param[in] channel    RSPI channel number
 * @param[in] odd        true--odd parity; false--even parity
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ParityOdd (uint32_t const channel, bool const odd)
{
     /* Sets the parity type to odd or even. True--odd parity; false--even parity */
	 RSPI_REG(channel)->SPCR2_b.SPOE = odd;
}  /* End of function HW_RSPI_ParityOdd() */

/*****************************************************************************************************************//**
 * @brief     Sets slave selection in the SPI command register(SPCMD0).
 * @param[in] channel    RSPI channel number
 * @param[in] ssl        SSL0, SSL1, SSL2, SSL3
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_SlaveSelect (uint32_t const channel, rspi_ssl_select_t const ssl)
{
     /* Selects the slave. */
	 RSPI_REG(channel)->SPCMDn_b[0].SSLA = ssl;
}  /* End of function HW_RSPI_SlaveSelect() */

/*****************************************************************************************************************//**
 * @brief     Sets the slave select signal level in the SPI command register (SPCMD0).
 * @param[in] channel    RSPI channel number
 * @param[in] keep       true--keep the signal level; false--negate the signal level
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_SlaveSelectLevelKeep (uint32_t const channel, bool const keep)
{
     /* Select to keep or negate the slave select signal at the end of transfer. True--keep; false--negate */
	 RSPI_REG(channel)->SPCMDn_b[0].SSLKP = keep;
}  /* End of function HW_RSPI_SlaveSelectLevelKeep() */

/*****************************************************************************************************************//**
 * @brief     Sets the data bit length for the SPI communication in the SPI command register (SPCMD0).
 * @param[in] channel   RSPI channel number
 * @param[in] length    8, 16, 32 bits
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_DataBitLength (uint32_t const channel, uint16_t const length)
{
     /* Sets the data bit length */
	 RSPI_REG(channel)->SPCMDn_b[0].SPB = (uint8_t) (length & 0x0F);
}  /* End of function HW_RSPI_DataBitLength() */

/*****************************************************************************************************************//**
 * @brief     Enables/disables the clock delay and sets the delay value in SPCMD0 and the clock delay register (SPCKD).
 * @param[in] channel   RSPI channel number
 * @param[in] enable    true--enable; false--disable
 * @param[in] count     number of clock cycles
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ClockDelay (uint32_t const channel, bool const enable, rspi_clock_delay_count_t const count)
{
     /* Enables/disables clock delay and sets the clock delay value */
	 RSPI_REG(channel)->SPCMDn_b[0].SCKDEN = enable;
	 RSPI_REG(channel)->SPCKD = count;
}  /* End of function HW_RSPI_ClockDelay() */

/*****************************************************************************************************************//**
 * @brief     Enables/disables the slave select negation delay and sets the value in SPCMD0 and the SSLND register.
 * @param[in] channel   RSPI channel number
 * @param[in] enable    true--enable; false--disable
 * @param[in] count     number of clock cycles
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_SlaveSelectNegationDelay (uint32_t const channel, bool const enable,
		                                               rspi_ssl_negation_delay_count_t const count)
{
     /* Enables/disables the slave select negation delay and sets the delay value */
	 RSPI_REG(channel)->SPCMDn_b[0].SLNDEN = enable;
	 RSPI_REG(channel)->SSLND = count;
}  /* End of function HW_RSPI_SlaveSelectNegationDelay() */

/*****************************************************************************************************************//**
 * @brief     Enables/disables the next access delay and sets the value in SPCMD0 and the SPND register.
 * @param[in] channel   RSPI channel number
 * @param[in] enable    true--enable; false--disable
 * @param[in] count     number of clock cycles
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_NextAccessDelay (uint32_t const channel, bool const enable,
		                                      rspi_next_access_delay_count_t const count)
{
     /* Enables/disables next access delay and sets the delay value */
	 RSPI_REG(channel)->SPCMDn_b[0].SPNDEN = enable;
	 RSPI_REG(channel)->SPND = count;
}  /* End of function HW_RSPI_NextAccessDelay() */

/*******************************************************************************************************************//**
* @brief     Checks if Master mode is set or not
* @param[in] channel RSPI channel number
* @retval    true  : master mode
* @retval    false : other mode
* @note      the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE bool HW_RSPI_MasterModeCheck (uint32_t const channel)
{
    return (1 == RSPI_REG(channel)->SPCR_b.MSTR);
}  /* End of function HW_RSPI_MasterModeCheck() */

/*****************************************************************************************************************//**
 * @brief     Disables the idle interrupt in the control register 2 (SPCR2).
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_IdleInterruptDisable (uint32_t const channel)
{
     /* Disable the Idle interrupt. */
	 RSPI_REG(channel)->SPCR2_b.SPIIE = 0;
}  /* End of function HW_RSPI_IdleInterruptDisable() */

/*****************************************************************************************************************//**
 * @brief     Check the idle flag status in status register (SPSR).
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE bool HW_RSPI_IdleCheck (uint32_t const channel)
{
     /* returns the idle flag status */
	return (RSPI_REG(channel)->SPSR_b.IDLNF);
} /* End of function HW_RSPI_IdleCheck() */

/*****************************************************************************************************************//**
 * @brief     Gets the status of the status register (SPSR).
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE bool HW_RSPI_Status (uint32_t const channel)
{
     /* returns the status of the status register */
	return (RSPI_REG(channel)->SPSR);
} /* End of function HW_RSPI_Status() */

/*****************************************************************************************************************//**
 * @brief     Clears all the error source in the status (SPSR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE bool HW_RSPI_ErrorsClear (uint32_t const channel)
{
     /* Clears all the error condition */
	 return (RSPI_REG(channel)->SPSR & (RSPI_SPSR_OVRF | RSPI_SPSR_MODF | RSPI_SPSR_PERF));
}  /* End of function HW_RSPI_ErrorsClear */

/*****************************************************************************************************************//**
 * @brief    Sets receive buffer full and under-run flag in status (SPSR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_RxBufferFull_Underrun_Set (uint32_t const channel)
{
     /* Sets the Rx buffer full and under-run flags */
	 RSPI_REG(channel)->SPSR = RSPI_SPSR_MASK;
}  /* End of function HW_RSPI_RxBufferFull_Underrun_Set */

/*****************************************************************************************************************//**
 * @brief     Enable RX buffer full/TX buffer empty/Error Interrupt and SPI function in the control (SPCR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_InterruptEnable (uint32_t const channel)
{
     /* Enable RX buffer full/TX buffer empty/Error Interrupt and SPI function bits */
	 RSPI_REG(channel)->SPCR |= (RSPI_SPCR_SPTIE | RSPI_SPCR_SPRIE | RSPI_SPCR_SPEIE | RSPI_SPCR_SPE);
}  /* End of function HW__RSPI_InterruptEnable */

/*****************************************************************************************************************//**
 * @brief     Disable RX buffer full/TX buffer empty/Error Interrupt and SPI function in the control (SPCR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_InterruptDisable (uint32_t const channel)
{
     /* Disable RX buffer full/TX buffer empty/Error Interrupt and SPI function bits */
	 RSPI_REG(channel)->SPCR &= (uint8_t) (~((RSPI_SPCR_SPTIE | RSPI_SPCR_SPRIE | RSPI_SPCR_SPEIE | RSPI_SPCR_SPE)));
}  /* End of function HW__RSPI_InterruptDisable */

/*****************************************************************************************************************//**
 * @brief     Disable TX buffer empty interrupt and the SPI function in the control (SPCR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_TxIntr_SPI_Disable (uint32_t const channel)
{
     /* Disable the TX buffer empty interrupt and the SPI function */
	 RSPI_REG(channel)->SPCR &= (uint8_t) (~(RSPI_SPCR_SPTIE | RSPI_SPCR_SPE));
}  /* End of function HW_RSPI_TxIntr_SPI_Disable */

/*****************************************************************************************************************//**
 * @brief     Disable RX buffer full interrupt in the control (SPCR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_RxIntrDisable (uint32_t const channel)
{
     /* Disable the RX buffer full interrupt */
	 RSPI_REG(channel)->SPCR_b.SPRIE = 0;
}  /* End of function HW_RSPI_RxIntrDisable() */

/*****************************************************************************************************************//**
 * @brief     Disable TX buffer empty interrupt in the control (SPCR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_TxIntrDisable (uint32_t const channel)
{
     /* Disable the TX buffer empty interrupt */
	 RSPI_REG(channel)->SPCR_b.SPTIE = 0;
}  /* End of function HW_RSPI_TxIntrDisable() */

/*****************************************************************************************************************//**
 * @brief     Disable RSPI function in the control (SPCR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_RSPIDisable (uint32_t const channel)
{
     /* Disable the RSPI function */
	 RSPI_REG(channel)->SPCR_b.SPE = 0;
}  /* End of function HW_RSPI_RSPIDisable() */

/*****************************************************************************************************************//**
 * @brief     Clears the RX buffer full error in the status (SPSR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_RxBufferFullClear (uint32_t const channel)
{
     /* Clears the RX buffer full error */
	 RSPI_REG(channel)->SPSR_b.SPRF = 0;
}  /* End of function HW_RSPI_RxBufferFullClear() */

/*****************************************************************************************************************//**
 * @brief     Clears overrun error in the status (SPSR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_OverRunErrorClear (uint32_t const channel)
{
     /* Clears the overrun error */
	 RSPI_REG(channel)->SPSR_b.OVRF = 0;
}  /* End of function HW_RSPI_OverRunErrorClear() */

/*****************************************************************************************************************//**
 * @brief     Clears mode fault error in the status (SPSR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ModeFaultErrorClear (uint32_t const channel)
{
     /* Clears the mode fault error */
	 RSPI_REG(channel)->SPSR_b.MODF = 0;
}  /* End of function HW_RSPI_ModeFaultErrorrClear() */

/*****************************************************************************************************************//**
 * @brief     Clears underflow error in the status (SPSR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_UnderflowErrorClear (uint32_t const channel)
{
     /* Clears the parity error */
	 RSPI_REG(channel)->SPSR_b.UDRF = 0;
}  /* End of function HW_RSPI_UnderflowErrorClear() */

/*****************************************************************************************************************//**
 * @brief     Clears parity error in the status (SPSR) register.
 * @param[in] channel    RSPI channel number
 * @retval    void
 * @note      the parameter check must be held by HLD
 *********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_ParityErrorrClear (uint32_t const channel)
{
     /* Clears the parity error */
	 RSPI_REG(channel)->SPSR_b.PERF = 0;
}  /* End of function HW_RSPI_ParityErrorrClear() */

#if (BSP_CFG_MCU_PART_SERIES == 1)
/*******************************************************************************************************************//**
* @brief  Writes long word data (32 bits) to transmit data register.
* @param[in] channel  RSPI channel number
* @param[in] data     Data to be sent
* @retval    void
* @note      All the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_Write (uint32_t const channel, uint16_t const data)
{
    /* Write data to data register */
    RSPI_REG(channel)->SPDR_HA = data;
}  /* End of function HW_RSPI_Write() */

/*******************************************************************************************************************//**
* @brief  Reads data from receive data register
* @param[in] channel    RSPI channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint16_t HW_RSPI_Read (uint32_t const channel)
{
    return RSPI_REG(channel)->SPDR_HA;
}  /* End of function HW_RSPI_Read() */
#else
/*******************************************************************************************************************//**
* @brief  Writes long word data (32 bits) to transmit data register.
* @param[in] channel  RSPI channel number
* @param[in] data     Data to be sent
* @retval    void
* @note      All the parameter check must be held by HLD
***********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_Write (uint32_t const channel, uint32_t const data)
{
    /* Write data to data register */
    RSPI_REG(channel)->SPDR = data;
}  /* End of function HW_RSPI_Write() */

/*******************************************************************************************************************//**
* @brief  Reads data from receive data register
* @param[in] channel    RSPI channel number
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
__STATIC_INLINE uint32_t HW_RSPI_Read (uint32_t const channel)
{
    return RSPI_REG(channel)->SPDR;
}  /* End of function HW_RSPI_Read() */
#endif

/*******************************************************************************************************************//**
* @brief     This function returns the address of the RSPI write and read register
* @param[in] channel     RSPI channel
* @note      Channel number is not checked in this function, caller function must check it.
***********************************************************************************************************************/
#if (BSP_CFG_MCU_PART_SERIES != 1)
__STATIC_INLINE uint32_t volatile const * HW_RSPI_WriteReadAddrGet (uint32_t const channel)
{
        return (uint32_t volatile const *) (&(RSPI_REG(channel)->SPDR));
}  /* End of function HW_SCI_SPIWriteAddrGet() */
#else
__STATIC_INLINE uint16_t volatile const * HW_RSPI_WriteReadAddrGet (uint32_t const channel)
{
        return (uint16_t volatile const *) (&(RSPI_REG(channel)->SPDR_HA));
}  /* End of function HW_SCI_SPIWriteAddrGet() */
#endif
/*******************************************************************************************************************//**
* @brief  Clears the IRQ Status for RSPI channel
* @param[in] channel  RSPI channel
* @param[in] rspi_int RSPI interrupt type (RSPI_TX_INT|RSPI_RE_INT|RSPI_ERR_INT|RSPI_ALL_INT)
* @retval    void
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_IrqStatusClear(uint32_t const channel, rspi_int_t rspi_int)
{
    void (*func_irq)(IRQn_Type IRQn);
    volatile uint32_t dummy;

    /* Assign function */
    func_irq = R_BSP_IrqStatusClear;

    /* Set registers */
    switch (rspi_int)
    {
    case RSPI_ALL_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_TX_INTR));
        func_irq (r_rspi_select_irqn (channel, RSPI_RX_INTR));
        func_irq (r_rspi_select_irqn (channel, RSPI_ERR_INTR));
    break;
    case RSPI_TX_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_TX_INTR));
    break;
    case RSPI_RX_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_RX_INTR));
    break;
    case RSPI_ERR_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_ERR_INTR));
    break;
    default:
    break;
    }
    dummy = R_ICU->IELSRn[(uint32_t)rspi_int];  /* Barrier register read required by the hardware to settle the bit. */
    SSP_PARAMETER_NOT_USED(dummy);
}  /* End of function HW_RSPI_IrqStatusClear() */

/*******************************************************************************************************************//**
* @brief  Enable/Disable IRQ interrupt for RSPI reception in interrupt controller
* @param[in] channel  RSPI channel
* @param[in] rspi_int RSPI interrupt type (RSPI_TX_INT|RSPI_RX_INT|RSPI_ERR_INT|RSPI_ALL_INT)
* @param[in] enable   True(Enable)|False(Disable)
* @retval    void
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE void HW_RSPI_IRQ_InterruptEnable (uint32_t const channel, rspi_int_t rspi_int, bool const enable)
{
    void (*func_irq)(IRQn_Type IRQn);

    /* Assign function */
    if(enable)
    {
        func_irq = NVIC_EnableIRQ;
    }
    else
    {
        func_irq = NVIC_DisableIRQ;
    }

    /* Set registers */
    switch (rspi_int)
    {
    case RSPI_ALL_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_TX_INTR));
        func_irq (r_rspi_select_irqn (channel, RSPI_RX_INTR));
        func_irq (r_rspi_select_irqn (channel, RSPI_ERR_INTR));
    break;
    case RSPI_TX_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_TX_INTR));
    break;
    case RSPI_RX_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_RX_INTR));
    break;
    case RSPI_ERR_INTR:
        func_irq (r_rspi_select_irqn (channel, RSPI_ERR_INTR));
    break;
    default:
    break;
    }
} /* End of function HW_RSPI_IRQ_InterruptEnable() */

/*******************************************************************************************************************//**
* Omitted channel checker
* @param[in] channel  RSPI channel
* @param[in] rspi_int RSPI interrupt type (RSPI_TX_INT|RSPI_RE_INT|RSPI_ERR_INT)
* @retval    IRQ number in interrupt controller
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE IRQn_Type r_rspi_select_irqn (uint32_t const channel, rspi_int_t rspi_int)
{
	switch (channel)
    {
        case 0:
            if (RSPI_TX_INTR == rspi_int)
            {
                #if (BSP_IRQ_CFG_SPI0_TXI != BSP_IRQ_DISABLED)
                    return SPI0_TXI_IRQn;
                #else
                    return BSP_MAX_NUM_IRQn;
                #endif
            }
            else if(RSPI_RX_INTR == rspi_int)
            {
                #if (BSP_IRQ_CFG_SPI0_RXI != BSP_IRQ_DISABLED)
                    return SPI0_RXI_IRQn;
                #else
                    return BSP_MAX_NUM_IRQn;
                #endif
            }
            else if(RSPI_ERR_INTR == rspi_int)
            {
                #if (BSP_IRQ_CFG_SPI0_ERI != BSP_IRQ_DISABLED)
                    return SPI0_ERI_IRQn;
                #else
                    return BSP_MAX_NUM_IRQn;
                #endif
            }
            else
            {
                return BSP_MAX_NUM_IRQn;
            }
         break;

	    case 1:
            if (RSPI_TX_INTR == rspi_int)
            {
                #if (BSP_IRQ_CFG_SPI1_TXI != BSP_IRQ_DISABLED)
                    return SPI1_TXI_IRQn;
                #else
                    return BSP_MAX_NUM_IRQn;
                #endif
            }
            else if(RSPI_RX_INTR == rspi_int)
            {
                #if (BSP_IRQ_CFG_SPI1_RXI != BSP_IRQ_DISABLED)
                    return (SPI1_RXI_IRQn);
                #else
                    return BSP_MAX_NUM_IRQn;
                #endif
            }
            else if(RSPI_ERR_INTR == rspi_int)
            {
                #if (BSP_IRQ_CFG_SPI1_ERI != BSP_IRQ_DISABLED)
                    return (SPI1_ERI_IRQn);
                #else
                    return BSP_MAX_NUM_IRQn;
                #endif
            }
            else
            {
                return BSP_MAX_NUM_IRQn;
            }
        break;
        default:
        break;
    }
    return BSP_MAX_NUM_IRQn;

} /* End of function r_rspi_select_irqn() */

/*******************************************************************************************************************//**
* Event lookup for RXI.
* @param[in] channel  RSPI channel
* @retval    ELC event
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE elc_event_t r_rspi_rxi_event_lookup (uint32_t const channel)
{
    return rxi_event_list[channel];
}

/*******************************************************************************************************************//**
* Event lookup for TXI.
* @param[in] channel  RSPI channel
* @retval    ELC event
* @note      Parameter check is not held in this function
***********************************************************************************************************************/
__STATIC_INLINE elc_event_t r_rspi_txi_event_lookup (uint32_t const channel)
{
    return txi_event_list[channel];
}

#endif /* HW_RSPI_COMMON_H */
