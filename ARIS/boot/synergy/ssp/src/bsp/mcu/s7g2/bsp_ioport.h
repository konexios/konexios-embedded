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
* File Name    : bsp_ioport.h
* Description  : IOPORT S7G2 specific macros.
***********************************************************************************************************************/

#ifndef HW_IOPORT_S7G2_H_
#define HW_IOPORT_S7G2_H_

#include "bsp_api.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define IOPORT_PRV_PFS_PSEL_OFFSET 24

#define IOPORT                     ((IOPORT_Type             *) R_IOPORT0_BASE)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** IO port type used with ports */
typedef uint16_t ioport_size_t;                     ///< IO port size on this device

/* IOPORT configuration for S7G2 224 pin package                                                                      */
/* ================================================================================================================== */
/* ================                                 IOPORT                                          ================= */
/* ================================================================================================================== */
typedef struct st_IOPORT_Type
{
    R_IOPORT0_Type  IOPORT00;
    uint32_t        padding0[5];
    R_IOPORT1_Type  IOPORT01;
    uint32_t        padding1[4];
    R_IOPORT1_Type  IOPORT02;
    uint32_t        padding2[4];
    R_IOPORT1_Type  IOPORT03;
    uint32_t        padding3[4];
    R_IOPORT1_Type  IOPORT04;
    uint32_t        padding4[4];
    R_IOPORT0_Type  IOPORT05;
    uint32_t        padding5[5];
    R_IOPORT0_Type  IOPORT06;
    uint32_t        padding6[5];
    R_IOPORT0_Type  IOPORT07;
    uint32_t        padding7[5];
    R_IOPORT0_Type  IOPORT08;
    uint32_t        padding8[5];
    R_IOPORT0_Type  IOPORT09;
    uint32_t        padding9[5];
    R_IOPORT0_Type  IOPORT10;
    uint32_t        padding10[5];
    R_IOPORT0_Type  IOPORT11;
} IOPORT_Type;

/** Available IO ports */
typedef enum e_ioport_port
{
    IOPORT_PORT_00 = 0x0000,                            ///< IO port 0
    IOPORT_PORT_01 = 0x0100,                            ///< IO port 1
    IOPORT_PORT_02 = 0x0200,                            ///< IO port 2
    IOPORT_PORT_03 = 0x0300,                            ///< IO port 3
    IOPORT_PORT_04 = 0x0400,                            ///< IO port 4
    IOPORT_PORT_05 = 0x0500,                            ///< IO port 5
    IOPORT_PORT_06 = 0x0600,                            ///< IO port 6
    IOPORT_PORT_07 = 0x0700,                            ///< IO port 7
    IOPORT_PORT_08 = 0x0800,                            ///< IO port 8
    IOPORT_PORT_09 = 0x0900,                            ///< IO port 9
    IOPORT_PORT_10 = 0x0A00,                            ///< IO port 10
    IOPORT_PORT_11 = 0x0B00,                            ///< IO port 11
    IOPORT_PORT_END
} ioport_port_t;

/** Available IO pins */
typedef enum e_ioport_port_pin_t
{
    IOPORT_PORT_00_PIN_00 = 0x0000,                        ///< IO port 0 pin 0
    IOPORT_PORT_00_PIN_01 = 0x0001,                        ///< IO port 0 pin 1
    IOPORT_PORT_00_PIN_02 = 0x0002,                        ///< IO port 0 pin 2
    IOPORT_PORT_00_PIN_03 = 0x0003,                        ///< IO port 0 pin 3
    IOPORT_PORT_00_PIN_04 = 0x0004,                        ///< IO port 0 pin 4
    IOPORT_PORT_00_PIN_05 = 0x0005,                        ///< IO port 0 pin 5
    IOPORT_PORT_00_PIN_06 = 0x0006,                        ///< IO port 0 pin 6
    IOPORT_PORT_00_PIN_07 = 0x0007,                        ///< IO port 0 pin 7
    IOPORT_PORT_00_PIN_08 = 0x0008,                        ///< IO port 0 pin 8
    IOPORT_PORT_00_PIN_09 = 0x0009,                        ///< IO port 0 pin 9
    IOPORT_PORT_00_PIN_10 = 0x000A,                        ///< IO port 0 pin 10
    IOPORT_PORT_00_PIN_11 = 0x000B,                        ///< IO port 0 pin 11
    IOPORT_PORT_00_PIN_14 = 0x000E,                        ///< IO port 0 pin 14
    IOPORT_PORT_00_PIN_15 = 0x000F,                        ///< IO port 0 pin 15

    IOPORT_PORT_01_PIN_00 = 0x0100,                        ///< IO port 1 pin 0
    IOPORT_PORT_01_PIN_01 = 0x0101,                        ///< IO port 1 pin 1
    IOPORT_PORT_01_PIN_02 = 0x0102,                        ///< IO port 1 pin 2
    IOPORT_PORT_01_PIN_03 = 0x0103,                        ///< IO port 1 pin 3
    IOPORT_PORT_01_PIN_04 = 0x0104,                        ///< IO port 1 pin 4
    IOPORT_PORT_01_PIN_05 = 0x0105,                        ///< IO port 1 pin 5
    IOPORT_PORT_01_PIN_06 = 0x0106,                        ///< IO port 1 pin 6
    IOPORT_PORT_01_PIN_07 = 0x0107,                        ///< IO port 1 pin 7
    IOPORT_PORT_01_PIN_08 = 0x0108,                        ///< IO port 1 pin 8
    IOPORT_PORT_01_PIN_09 = 0x0109,                        ///< IO port 1 pin 9
    IOPORT_PORT_01_PIN_10 = 0x010A,                        ///< IO port 1 pin 10
    IOPORT_PORT_01_PIN_11 = 0x010B,                        ///< IO port 1 pin 11
    IOPORT_PORT_01_PIN_12 = 0x010C,                        ///< IO port 1 pin 12
    IOPORT_PORT_01_PIN_13 = 0x010D,                        ///< IO port 1 pin 13
    IOPORT_PORT_01_PIN_14 = 0x010E,                        ///< IO port 1 pin 14
    IOPORT_PORT_01_PIN_15 = 0x010F,                        ///< IO port 1 pin 15

    IOPORT_PORT_02_PIN_00 = 0x0200,                        ///< IO port 2 pin 0
    IOPORT_PORT_02_PIN_01 = 0x0201,                        ///< IO port 2 pin 1
    IOPORT_PORT_02_PIN_02 = 0x0202,                        ///< IO port 2 pin 2
    IOPORT_PORT_02_PIN_03 = 0x0203,                        ///< IO port 2 pin 3
    IOPORT_PORT_02_PIN_04 = 0x0204,                        ///< IO port 2 pin 4
    IOPORT_PORT_02_PIN_05 = 0x0205,                        ///< IO port 2 pin 5
    IOPORT_PORT_02_PIN_06 = 0x0206,                        ///< IO port 2 pin 6
    IOPORT_PORT_02_PIN_07 = 0x0207,                        ///< IO port 2 pin 7
    IOPORT_PORT_02_PIN_12 = 0x020C,                        ///< IO port 2 pin 12
    IOPORT_PORT_02_PIN_13 = 0x020D,                        ///< IO port 2 pin 13

    IOPORT_PORT_03_PIN_00 = 0x0300,                        ///< IO port 3 pin 0
    IOPORT_PORT_03_PIN_01 = 0x0301,                        ///< IO port 3 pin 1
    IOPORT_PORT_03_PIN_02 = 0x0302,                        ///< IO port 3 pin 2
    IOPORT_PORT_03_PIN_03 = 0x0303,                        ///< IO port 3 pin 3
    IOPORT_PORT_03_PIN_04 = 0x0304,                        ///< IO port 3 pin 4
    IOPORT_PORT_03_PIN_05 = 0x0305,                        ///< IO port 3 pin 5
    IOPORT_PORT_03_PIN_06 = 0x0306,                        ///< IO port 3 pin 6
    IOPORT_PORT_03_PIN_07 = 0x0307,                        ///< IO port 3 pin 7
    IOPORT_PORT_03_PIN_08 = 0x0308,                        ///< IO port 3 pin 8
    IOPORT_PORT_03_PIN_09 = 0x0309,                        ///< IO port 3 pin 9
    IOPORT_PORT_03_PIN_10 = 0x030A,                        ///< IO port 3 pin 10
    IOPORT_PORT_03_PIN_11 = 0x030B,                        ///< IO port 3 pin 11
    IOPORT_PORT_03_PIN_12 = 0x030C,                        ///< IO port 3 pin 12
    IOPORT_PORT_03_PIN_13 = 0x030D,                        ///< IO port 3 pin 13
    IOPORT_PORT_03_PIN_14 = 0x030E,                        ///< IO port 3 pin 14
    IOPORT_PORT_03_PIN_15 = 0x030F,                        ///< IO port 3 pin 15

    IOPORT_PORT_04_PIN_00 = 0x0400,                        ///< IO port 4 pin 0
    IOPORT_PORT_04_PIN_01 = 0x0401,                        ///< IO port 4 pin 1
    IOPORT_PORT_04_PIN_02 = 0x0402,                        ///< IO port 4 pin 2
    IOPORT_PORT_04_PIN_03 = 0x0403,                        ///< IO port 4 pin 3
    IOPORT_PORT_04_PIN_04 = 0x0404,                        ///< IO port 4 pin 4
    IOPORT_PORT_04_PIN_05 = 0x0405,                        ///< IO port 4 pin 5
    IOPORT_PORT_04_PIN_06 = 0x0406,                        ///< IO port 4 pin 6
    IOPORT_PORT_04_PIN_07 = 0x0407,                        ///< IO port 4 pin 7
    IOPORT_PORT_04_PIN_08 = 0x0408,                        ///< IO port 4 pin 8
    IOPORT_PORT_04_PIN_09 = 0x0409,                        ///< IO port 4 pin 9
    IOPORT_PORT_04_PIN_10 = 0x040A,                        ///< IO port 4 pin 10
    IOPORT_PORT_04_PIN_11 = 0x040B,                        ///< IO port 4 pin 11
    IOPORT_PORT_04_PIN_12 = 0x040C,                        ///< IO port 4 pin 12
    IOPORT_PORT_04_PIN_13 = 0x040D,                        ///< IO port 4 pin 13
    IOPORT_PORT_04_PIN_14 = 0x040E,                        ///< IO port 4 pin 14
    IOPORT_PORT_04_PIN_15 = 0x040F,                        ///< IO port 4 pin 15

    IOPORT_PORT_05_PIN_00 = 0x0500,                        ///< IO port 5 pin 0
    IOPORT_PORT_05_PIN_01 = 0x0501,                        ///< IO port 5 pin 1
    IOPORT_PORT_05_PIN_02 = 0x0502,                        ///< IO port 5 pin 2
    IOPORT_PORT_05_PIN_03 = 0x0503,                        ///< IO port 5 pin 3
    IOPORT_PORT_05_PIN_04 = 0x0504,                        ///< IO port 5 pin 4
    IOPORT_PORT_05_PIN_05 = 0x0505,                        ///< IO port 5 pin 5
    IOPORT_PORT_05_PIN_06 = 0x0506,                        ///< IO port 5 pin 6
    IOPORT_PORT_05_PIN_07 = 0x0507,                        ///< IO port 5 pin 7
    IOPORT_PORT_05_PIN_08 = 0x0508,                        ///< IO port 5 pin 8
    IOPORT_PORT_05_PIN_09 = 0x0509,                        ///< IO port 5 pin 9
    IOPORT_PORT_05_PIN_10 = 0x050A,                        ///< IO port 5 pin 10
    IOPORT_PORT_05_PIN_11 = 0x050B,                        ///< IO port 5 pin 11
    IOPORT_PORT_05_PIN_12 = 0x050C,                        ///< IO port 5 pin 12
    IOPORT_PORT_05_PIN_13 = 0x050D,                        ///< IO port 5 pin 13
    IOPORT_PORT_05_PIN_14 = 0x050E,                        ///< IO port 5 pin 14
    IOPORT_PORT_05_PIN_15 = 0x050F,                        ///< IO port 5 pin 15

    IOPORT_PORT_06_PIN_00 = 0x0600,                        ///< IO port 6 pin 0
    IOPORT_PORT_06_PIN_01 = 0x0601,                        ///< IO port 6 pin 1
    IOPORT_PORT_06_PIN_02 = 0x0602,                        ///< IO port 6 pin 2
    IOPORT_PORT_06_PIN_03 = 0x0603,                        ///< IO port 6 pin 3
    IOPORT_PORT_06_PIN_04 = 0x0604,                        ///< IO port 6 pin 4
    IOPORT_PORT_06_PIN_05 = 0x0605,                        ///< IO port 6 pin 5
    IOPORT_PORT_06_PIN_06 = 0x0606,                        ///< IO port 6 pin 6
    IOPORT_PORT_06_PIN_07 = 0x0607,                        ///< IO port 6 pin 7
    IOPORT_PORT_06_PIN_08 = 0x0608,                        ///< IO port 6 pin 8
    IOPORT_PORT_06_PIN_09 = 0x0609,                        ///< IO port 6 pin 9
    IOPORT_PORT_06_PIN_10 = 0x060A,                        ///< IO port 6 pin 10
    IOPORT_PORT_06_PIN_11 = 0x060B,                        ///< IO port 6 pin 11
    IOPORT_PORT_06_PIN_12 = 0x060C,                        ///< IO port 6 pin 12
    IOPORT_PORT_06_PIN_13 = 0x060D,                        ///< IO port 6 pin 13
    IOPORT_PORT_06_PIN_14 = 0x060E,                        ///< IO port 6 pin 14
    IOPORT_PORT_06_PIN_15 = 0x060F,                        ///< IO port 6 pin 15

    IOPORT_PORT_07_PIN_00 = 0x0700,                        ///< IO port 7 pin 0
    IOPORT_PORT_07_PIN_01 = 0x0701,                        ///< IO port 7 pin 1
    IOPORT_PORT_07_PIN_02 = 0x0702,                        ///< IO port 7 pin 2
    IOPORT_PORT_07_PIN_03 = 0x0703,                        ///< IO port 7 pin 3
    IOPORT_PORT_07_PIN_04 = 0x0704,                        ///< IO port 7 pin 4
    IOPORT_PORT_07_PIN_05 = 0x0705,                        ///< IO port 7 pin 5
    IOPORT_PORT_07_PIN_06 = 0x0706,                        ///< IO port 7 pin 6
    IOPORT_PORT_07_PIN_07 = 0x0707,                        ///< IO port 7 pin 7
    IOPORT_PORT_07_PIN_08 = 0x0708,                        ///< IO port 7 pin 8
    IOPORT_PORT_07_PIN_09 = 0x0709,                        ///< IO port 7 pin 9
    IOPORT_PORT_07_PIN_10 = 0x070A,                        ///< IO port 7 pin 10
    IOPORT_PORT_07_PIN_11 = 0x070B,                        ///< IO port 7 pin 11
    IOPORT_PORT_07_PIN_12 = 0x070C,                        ///< IO port 7 pin 12
    IOPORT_PORT_07_PIN_13 = 0x070D,                        ///< IO port 7 pin 13

    IOPORT_PORT_08_PIN_00 = 0x0800,                        ///< IO port 8 pin 0
    IOPORT_PORT_08_PIN_01 = 0x0801,                        ///< IO port 8 pin 1
    IOPORT_PORT_08_PIN_02 = 0x0802,                        ///< IO port 8 pin 2
    IOPORT_PORT_08_PIN_03 = 0x0803,                        ///< IO port 8 pin 3
    IOPORT_PORT_08_PIN_04 = 0x0804,                        ///< IO port 8 pin 4
    IOPORT_PORT_08_PIN_05 = 0x0805,                        ///< IO port 8 pin 5
    IOPORT_PORT_08_PIN_06 = 0x0806,                        ///< IO port 8 pin 6
    IOPORT_PORT_08_PIN_07 = 0x0807,                        ///< IO port 8 pin 7
    IOPORT_PORT_08_PIN_08 = 0x0808,                        ///< IO port 8 pin 8
    IOPORT_PORT_08_PIN_09 = 0x0809,                        ///< IO port 8 pin 9
    IOPORT_PORT_08_PIN_10 = 0x080A,                        ///< IO port 8 pin 10
    IOPORT_PORT_08_PIN_11 = 0x080B,                        ///< IO port 8 pin 11
    IOPORT_PORT_08_PIN_12 = 0x080C,                        ///< IO port 8 pin 12
    IOPORT_PORT_08_PIN_13 = 0x080D,                        ///< IO port 8 pin 13

    IOPORT_PORT_09_PIN_00 = 0x0900,                        ///< IO port 9 pin 0
    IOPORT_PORT_09_PIN_01 = 0x0901,                        ///< IO port 9 pin 1
    IOPORT_PORT_09_PIN_02 = 0x0902,                        ///< IO port 9 pin 2
    IOPORT_PORT_09_PIN_03 = 0x0903,                        ///< IO port 9 pin 3
    IOPORT_PORT_09_PIN_04 = 0x0904,                        ///< IO port 9 pin 4
    IOPORT_PORT_09_PIN_05 = 0x0905,                        ///< IO port 9 pin 5
    IOPORT_PORT_09_PIN_06 = 0x0906,                        ///< IO port 9 pin 6
    IOPORT_PORT_09_PIN_07 = 0x0907,                        ///< IO port 9 pin 7
    IOPORT_PORT_09_PIN_08 = 0x0908,                        ///< IO port 9 pin 8
    IOPORT_PORT_09_PIN_09 = 0x0909,                        ///< IO port 9 pin 9
    IOPORT_PORT_09_PIN_10 = 0x090A,                        ///< IO port 9 pin 10
    IOPORT_PORT_09_PIN_11 = 0x090B,                        ///< IO port 9 pin 11
    IOPORT_PORT_09_PIN_12 = 0x090C,                        ///< IO port 9 pin 12
    IOPORT_PORT_09_PIN_13 = 0x090D,                        ///< IO port 9 pin 13
    IOPORT_PORT_09_PIN_14 = 0x090E,                        ///< IO port 9 pin 14
    IOPORT_PORT_09_PIN_15 = 0x090F,                        ///< IO port 9 pin 15

    IOPORT_PORT_10_PIN_00 = 0x0A00,                        ///< IO port 10 pin 0
    IOPORT_PORT_10_PIN_01 = 0x0A01,                        ///< IO port 10 pin 1
    IOPORT_PORT_10_PIN_02 = 0x0A02,                        ///< IO port 10 pin 2
    IOPORT_PORT_10_PIN_03 = 0x0A03,                        ///< IO port 10 pin 3
    IOPORT_PORT_10_PIN_04 = 0x0A04,                        ///< IO port 10 pin 4
    IOPORT_PORT_10_PIN_05 = 0x0A05,                        ///< IO port 10 pin 5
    IOPORT_PORT_10_PIN_06 = 0x0A06,                        ///< IO port 10 pin 6
    IOPORT_PORT_10_PIN_07 = 0x0A07,                        ///< IO port 10 pin 7
    IOPORT_PORT_10_PIN_08 = 0x0A08,                        ///< IO port 10 pin 8
    IOPORT_PORT_10_PIN_09 = 0x0A09,                        ///< IO port 10 pin 9
    IOPORT_PORT_10_PIN_10 = 0x0A0A,                        ///< IO port 10 pin 10
    IOPORT_PORT_10_PIN_11 = 0x0A0B,                        ///< IO port 10 pin 11
    IOPORT_PORT_10_PIN_12 = 0x0A0C,                        ///< IO port 10 pin 12
    IOPORT_PORT_10_PIN_13 = 0x0A0D,                        ///< IO port 10 pin 13
    IOPORT_PORT_10_PIN_14 = 0x0A0E,                        ///< IO port 10 pin 14
    IOPORT_PORT_10_PIN_15 = 0x0A0F,                        ///< IO port 10 pin 15

    IOPORT_PORT_11_PIN_00 = 0x0B00,                        ///< IO port 11 pin 0
    IOPORT_PORT_11_PIN_01 = 0x0B01,                        ///< IO port 11 pin 1
    IOPORT_PORT_11_PIN_02 = 0x0B02,                        ///< IO port 11 pin 2
    IOPORT_PORT_11_PIN_03 = 0x0B03,                        ///< IO port 11 pin 3
    IOPORT_PORT_11_PIN_04 = 0x0B04,                        ///< IO port 11 pin 4
    IOPORT_PORT_11_PIN_05 = 0x0B05,                        ///< IO port 11 pin 5
    IOPORT_PORT_11_PIN_06 = 0x0B06,                        ///< IO port 11 pin 6
    IOPORT_PORT_11_PIN_07 = 0x0B07,                        ///< IO port 11 pin 7

    IOPORT_PIN_END                                         ///< Marks end of enum - used by parameter checking
} ioport_port_pin_t;

/** Peripheral functions supported by this device  */
typedef enum e_ioport_peripheral
{
    IOPORT_PERIPHERAL_IO              = 0x00,                                   ///< Pin will functions as an IO pin
    IOPORT_PERIPHERAL_DEBUG           = (0x00UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a DEBUG pin
    IOPORT_PERIPHERAL_AGT             = (0x01UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an AGT
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_GPT0            = (0x02UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a GPT
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_GPT1            = (0x03UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a GPT
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_SCI0_2_4_6_8    = (0x04UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an SCI
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_SCI1_3_5_7_9    = (0x05UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an SCI
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_RSPI            = (0x06UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a RSPI
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_RIIC            = (0x07UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a RIIC
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_KEY             = (0x08UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a KEY
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_CLKOUT_COMP_RTC = (0x09UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a
                                                                                // clock/comparator/RTC peripheral pin
    IOPORT_PERIPHERAL_CAC_AD          = (0x0AUL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a CAC/ADC
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_BUS             = (0x0BUL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a BUS
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_CTSU            = (0x0CUL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a CTSU
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_LCDC            = (0x0DUL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a segment LCD
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_CAN             = (0x10UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a CAN
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_QSPI            = (0x11UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a QSPI
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_SSI             = (0x12UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an SSI
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_USB_FS          = (0x13UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a USB
                                                                                // full speed peripheral pin
    IOPORT_PERIPHERAL_USB_HS          = (0x14UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a USB
                                                                                // high speed peripheral pin
    IOPORT_PERIPHERAL_SDHI_MMC        = (0x15UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an SD/MMC
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_ETHER_MII       = (0x16UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an Ethernet
                                                                                // MMI peripheral pin
    IOPORT_PERIPHERAL_ETHER_RMII      = (0x17UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an Ethernet
                                                                                // RMMI peripheral pin
    IOPORT_PERIPHERAL_PDC             = (0x18UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a PDC
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_LCD_GRAPHICS    = (0x19UL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a graphics
                                                                                // LCD peripheral pin
    IOPORT_PERIPHERAL_TRACE           = (0x1AUL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as a debug trace
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_SDRAM           = (0x1BUL << IOPORT_PRV_PFS_PSEL_OFFSET), ///< Pin will function as an SDRAM
                                                                                // peripheral pin
    IOPORT_PERIPHERAL_END                                                       ///< Marks end of enum - used by
                                                                                // parameter checking
} ioport_peripheral_t;

/** Ethernet channels supported by this device */
typedef enum e_ioport_eth_ch
{
    IOPORT_ETHERNET_CHANNEL_0 = 0x10,                   ///< Used to select Ethernet channel 0
    IOPORT_ETHERNET_CHANNEL_1 = 0x20,                   ///< Used to select Ethernet channel 1
    IOPORT_ETHERNET_CHANNEL_END                         ///< Marks end of enum - used by parameter checking
} ioport_ethernet_channel_t;

/** Ethernet PHY modes supported by this device */
typedef enum e_ioport_eth_mode
{
    IOPORT_ETHERNET_MODE_MII = 0,                       ///< Ethernet PHY mode set to MII
    IOPORT_ETHERNET_MODE_RMII,                          ///< Ethernet PHY mode set to RMII
    IOPORT_ETHERNET_MODE_END                            ///< Marks end of enum - used by parameter checking
} ioport_ethernet_mode_t;

/** Options to configure pin functions  */
typedef enum e_ioport_cfg_options
{
    IOPORT_CFG_PORT_DIRECTION_INPUT  = 0x00000000,      ///< Sets the pin direction to input (default)
    IOPORT_CFG_PORT_DIRECTION_OUTPUT = 0x00000004,      ///< Sets the pin direction to output
    IOPORT_CFG_PULLUP_ENABLE         = 0x00000010,      ///< Enables the pin's internal pull-up
    IOPORT_CFG_NMOS_ENABLE           = 0x00000040,      ///< Enables the pin's NMOS open-drain output
    IOPORT_CFG_PMOS_ENABLE           = 0x00000080,      ///< Enables the pin's PMOS open-drain ouput
    IOPORT_CFG_DRIVE_MID             = 0x00000400,      ///< Sets pin drive output to medium
    IOPORT_CFG_DRIVE_HIGH            = 0x00000C00,      ///< Sets pin drive output to high
    IOPORT_CFG_EVENT_RISING_EDGE     = 0x00001000,      ///< Sets pin event trigger to rising edge
    IOPORT_CFG_EVENT_FALLING_EDGE    = 0x00002000,      ///< Sets pin event trigger to falling edge
    IOPORT_CFG_EVENT_BOTH_EDGES      = 0x00003000,      ///< Sets pin event trigger to both edges
    IOPORT_CFG_IRQ_ENABLE            = 0x00004000,      ///< Sets pin as an IRQ pin
    IOPORT_CFG_ANALOG_ENABLE         = 0x00008000,      ///< Enables pin to operate as an analog pin
    IOPORT_CFG_PERIPHERAL_PIN        = 0x00010000       ///< Enables pin to operate as a peripheral pin
} ioport_cfg_options_t;
#endif /* HW_IOPORT_S7G2_H_ */
