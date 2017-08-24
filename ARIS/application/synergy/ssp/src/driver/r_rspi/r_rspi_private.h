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

/***********************************************************************************************************************
 * File Name    : r_rspi_private.h
 * Description  : Private Definitions for RSPI module.
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @addtogroup RSPI
 * @{
 **********************************************************************************************************************/

#ifndef RSPI_PRIVATE_H
#define RSPI_PRIVATE_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
/* Fixed width integer support. */
/* Access to peripherals and board defines. */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RSPI_BYTE_DATA 0x01
#define RSPI_WORD_DATA 0x02
#define RSPI_LONG_DATA 0x04
#define RSPI_POWER_ON  (0)
#define RSPI_POWER_OFF (1)
/* Fixed data that is transmitted during receive-only operations. Change as needed. */
#if (BSP_CFG_MCU_PART_SERIES == 1)
#define RSPI_DUMMY_TXDATA (0xFFFF)
#else
#define RSPI_DUMMY_TXDATA (0xFFFFFFFF)
#endif

/* Bitmask definitions of RSPI register settings options. These are for registers that contain bit fields
 *  to control individual options. Do not change. */
/* RSPI Control Register (SPCR) */
#define RSPI_SPCR_SPMS      (0x01) /* 0: SPI (four-wire). Uses internal slave select. */
                                   /* 1: Clock synchronous operation (three-wire). Uses external slave select. */
#define RSPI_SPCR_TXMD      (0x02) /* 0: Full-duplex synchronous.    1: Only transmit operations  */
#define RSPI_SPCR_MODFEN    (0x04) /* 0: Mode fault error detection disabled. 1: Detection enabled. */
#define RSPI_SPCR_MSTR      (0x08) /* 0: Slave mode.                 1: Master mode. */
#define RSPI_SPCR_SPEIE     (0x10) /* 0: Error Interrupt disabled.   1: Error Interrupt enabled.*/
#define RSPI_SPCR_SPTIE     (0x20) /* 0: Transmit Interrupt disabled 1: Transmit Interrupt Enabled */
#define RSPI_SPCR_SPE       (0x40) /* 0: RSPI operation disabled.    1: RSPI operation enabled. */
#define RSPI_SPCR_SPRIE     (0x80) /* 0: Receive Interrupt disabled. 1: Receive Interrupt Enabled. */
#define RSPI_SPCR_MODE_MASK (0x0F) /* allow to set b0 to b4 for mode setting only */

/*RSPI Slave Select Polarity Register (SSLP) */
#define RSPI_SSLP_SSL0P (0x01)  /* 0 = active low. 1 = active high. */
#define RSPI_SSLP_SSL1P (0x02)
#define RSPI_SSLP_SSL2P (0x04)
#define RSPI_SSLP_SSL3P (0x08)
#define RSPI_SSLP_MASK  (0x0F)

/* RSPI Pin Control Register (SPPCR) */
#define RSPI_SPPCR_SPLP  (0x01) /* 0: Normal mode. 1: Loopback mode (reversed transmit data = receive). */
#define RSPI_SPPCR_SPLP2 (0x02) /* 0: Normal mode. 1: Loopback mode (transmit data = receive data). */
#define RSPI_SPPCR_MASK  (0x33) /* Protect reserved bits */
#define RSPI_SPPCR_MOIFV (0x10) /* 0: MOSI pin idles low. 1: MOSI pin idles high. */
#define RSPI_SPPCR_MOIFE (0x20) /* 0: MOSI pin idles at final previous data. 1: MOSI pin idles at MOIFV. */

/* RSPI Sequence Control Register (SPSCR) */
#define RSPI_SPSCR (0x07)  /* Sequence SPCMD0-7 by 0-n */

/* RSPI Status Register  (SPSR) */
#define RSPI_SPSR_OVRF  (0x01) /* 0: No overrun error.          1: An overrun error occurred. */
#define RSPI_SPSR_IDLNF (0x02) /* 0: RSPI is in the idle state. 1: RSPI is in the transfer state. */
#define RSPI_SPSR_MODF  (0x04) /* 0: No mode fault error        1: A mode fault error occurred */
#define RSPI_SPSR_PERF  (0x08) /* 0: No parity error.           1: A parity error occurred. */
#define RSPI_SPSR_UDRF  (0x10) /* 0: mode fault error (if MODF =1) 1: Underrun error (if MODF = 1)*/
#define RSPI_SPSR_MASK  (0xA0) /* Protect reserved bits. */

/* RSPI Data Control Register (SPDCR) masks*/
#define RSPI_SPDCR_SPFC   (0x03) /* 0: Transfer n+1 frames at a time */
#define RSPI_SPDCR_SPRDTD (0x10) /* 0: read SPDR values from RX buffer. 1: values read from transmit buffer*/
#define RSPI_SPDCR_SPLW   (0x20) /* 0: SPDR is accessed in words. 1: SPDR is accessed in longwords. */

/* RSPI Control Register 2 (SPCR2) */
#define RSPI_SPCR2_SPPE  (0x01) /* 0: No parity.                      1: Adds parity bit. */
#define RSPI_SPCR2_SPOE  (0x02) /* 0: Even parity.                    1: Odd parity. */
#define RSPI_SPCR2_SPIIE (0x04) /* 0: Disable Idle interrupt          1: Enable Idle interrupt */
#define RSPI_SPCR2_PTE   (0x08) /* 0: Disable parity self diagnostic. 1: Enable parity self diagnostic. */
#define RSPI_SPCR2_MASK  (0x0F) /* Protect reserved bits. */

/* Bit masks for RSPI Command Registers 0 to 7 (SPCMD0 to SPCMD7). */
#define RSPI_SPCMD_CPHA      (0x0001) /* 0: Data sampling on odd edge, data variation on even edge. */
                                      /* 1: Data variation on odd edge, data sampling on even edge. */
#define RSPI_SPCMD_CPOL      (0x0002) /* 0: RSPCK is low when idle. 1: RSPCK is high when idle. */
#define RSPI_SPCMD_BRDV      (0x000C) /* Base bit-rate divisor. Div 2^N.  2-bits  */
#define RSPI_SPCMD_SSLA      (0x0070) /* n = Slave select 0-3 when SPI mode. */
#define RSPI_SPCMD_SSLKP     (0x0080) /* 0: SSL signal negated at end of transfer. 1: Keep SSL level at end until next.
                                       **/
#define RSPI_SPCMD_SPB       (0x0F00) /* b11 to b8 SPB[3:0] bitmask */
#define RSPI_SPCMD_LSBF      (0x1000) /* 0: MSB first. 1: LSB first. */
#define RSPI_SPCMD_SPB_8BIT  (0x4)    /* value for 8 bits data length */
#define RSPI_SPCMD_SPB_16BIT (0xF)    /* value for 8 bits data length */
#define RSPI_SPCMD_SPB_32BIT (0x3)    /* value for 8 bits data length */

/** RSPI interrupt definition */
typedef enum e_rspi_int
{
    RSPI_TX_INTR,        /**< TXI interrupt */
    RSPI_RX_INTR,        /**< RXI interrupt */
    RSPI_ERR_INTR,       /**< Error interrupt */
    RSPI_ALL_INTR        /**< All interrupt */
} rspi_int_t;

/** RSPI interrupt events list */
static elc_event_t rxi_event_list[] =
{
	ELC_EVENT_SPI0_RXI,
	ELC_EVENT_SPI1_RXI,
};

static elc_event_t txi_event_list[] =
{
	ELC_EVENT_SPI0_TXI,
	ELC_EVENT_SPI1_TXI,
};

#endif /* RSPI_PRIVATE_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup RSPI)
 **********************************************************************************************************************/
