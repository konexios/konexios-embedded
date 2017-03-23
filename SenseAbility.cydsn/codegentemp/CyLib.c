/*******************************************************************************
* File Name: CyLib.c
* Version 5.30
*
*  Description:
*   Provides a system API for the Clocking, Interrupts, SysTick, and Voltage
*   Detect.
*
*  Note:
*   Documentation of the API's in this file is located in the PSoC 4 System
*   Reference Guide provided with PSoC Creator.
*
********************************************************************************
* Copyright 2010-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "CyLib.h"

/* CySysClkWriteImoFreq() || CySysClkImoEnableWcoLock() */
#if ((CY_IP_SRSSV2 && CY_IP_FMLT) || CY_IP_IMO_TRIMMABLE_BY_WCO)
    #include "CyFlash.h"
#endif /* (CY_IP_SRSSV2 && CY_IP_FMLT) */

/* Do not use these definitions directly in your application */
uint32 cydelayFreqHz  = CYDEV_BCLK__SYSCLK__HZ;
uint32 cydelayFreqKhz = (CYDEV_BCLK__SYSCLK__HZ + CY_DELAY_1K_MINUS_1_THRESHOLD) / CY_DELAY_1K_THRESHOLD;
uint8  cydelayFreqMhz = (uint8)((CYDEV_BCLK__SYSCLK__HZ + CY_DELAY_1M_MINUS_1_THRESHOLD) / CY_DELAY_1M_THRESHOLD);
uint32 cydelay32kMs   = CY_DELAY_MS_OVERFLOW * ((CYDEV_BCLK__SYSCLK__HZ + CY_DELAY_1K_MINUS_1_THRESHOLD) /
                        CY_DELAY_1K_THRESHOLD);


static cySysTickCallback CySysTickCallbacks[CY_SYS_SYST_NUM_OF_CALLBACKS];
static void CySysTickServiceCallbacks(void);
#if (CY_IP_SRSSV2 && CY_IP_PLL)
    static uint32 CySysClkPllGetBypassMode(uint32 pll);
    static cystatus CySysClkPllConfigChangeAllowed(uint32 pll);
#endif /* #if (CY_IP_SRSSV2 && CY_IP_PLL) */


/*******************************************************************************
* Indicates whether or not the SysTick has been initialized. The variable is
* initialized to 0 and set to 1 the first time CySysTickStart() is called.
*
* This allows the component to restart without reinitialization after the first
* call to the CySysTickStart() routine.
*
* If reinitialization of the SysTick is required, call CySysTickInit() before
* calling CySysTickStart(). Alternatively, the SysTick can be reinitialized by
* calling the CySysTickInit() and CySysTickEnable() functions.
*******************************************************************************/
uint32 CySysTickInitVar = 0u;


#if(CY_IP_SRSSV2)
        /* Conversion between CySysClkWriteImoFreq() parameter and register's value */
        const uint8 cyImoFreqMhz2Reg[CY_SYS_CLK_IMO_FREQ_TABLE_SIZE] = {
            /*  3 MHz */ 0x03u,  /*  4 MHz */ 0x04u,  /*  5 MHz */ 0x05u,  /*  6 MHz */ 0x06u,
            /*  7 MHz */ 0x07u,  /*  8 MHz */ 0x08u,  /*  9 MHz */ 0x09u,  /* 10 MHz */ 0x0Au,
            /* 11 MHz */ 0x0Bu,  /* 12 MHz */ 0x0Cu,  /* 13 MHz */ 0x0Eu,  /* 14 MHz */ 0x0Fu,
            /* 15 MHz */ 0x10u,  /* 16 MHz */ 0x11u,  /* 17 MHz */ 0x12u,  /* 18 MHz */ 0x13u,
            /* 19 MHz */ 0x14u,  /* 20 MHz */ 0x15u,  /* 21 MHz */ 0x16u,  /* 22 MHz */ 0x17u,
            /* 23 MHz */ 0x18u,  /* 24 MHz */ 0x19u,  /* 25 MHz */ 0x1Bu,  /* 26 MHz */ 0x1Cu,
            /* 27 MHz */ 0x1Du,  /* 28 MHz */ 0x1Eu,  /* 29 MHz */ 0x1Fu,  /* 30 MHz */ 0x20u,
            /* 31 MHz */ 0x21u,  /* 32 MHz */ 0x22u,  /* 33 MHz */ 0x23u,  /* 34 MHz */ 0x25u,
            /* 35 MHz */ 0x26u,  /* 36 MHz */ 0x27u,  /* 37 MHz */ 0x28u,  /* 38 MHz */ 0x29u,
            /* 39 MHz */ 0x2Au,  /* 40 MHz */ 0x2Bu,  /* 41 MHz */ 0x2Eu,  /* 42 MHz */ 0x2Fu,
            /* 43 MHz */ 0x30u,  /* 44 MHz */ 0x31u,  /* 45 MHz */ 0x32u,  /* 46 MHz */ 0x33u,
            /* 47 MHz */ 0x34u,  /* 48 MHz */ 0x35u };
#endif /* (CY_IP_SRSSV2) */

#if (CY_IP_IMO_TRIMMABLE_BY_WCO)
        /* Conversion between IMO frequency and WCO DPLL max offset steps */
        const uint8 cyImoFreqMhz2DpllOffset[CY_SYS_CLK_IMO_FREQ_WCO_DPLL_TABLE_SIZE] = {
            /* 26 MHz */  238u, /* 27 MHz */  219u, /* 28 MHz */  201u, /* 29 MHz */  185u,
            /* 30 MHz */  170u, /* 31 MHz */  155u, /* 32 MHz */  142u, /* 33 MHz */  130u,
            /* 34 MHz */  118u, /* 35 MHz */  107u, /* 36 MHz */   96u, /* 37 MHz */  86u,
            /* 38 MHz */   77u, /* 39 MHz */   68u, /* 40 MHz */   59u, /* 41 MHz */  51u,
            /* 42 MHz */   44u, /* 43 MHz */   36u, /* 44 MHz */   29u, /* 45 MHz */  23u,
            /* 46 MHz */   16u, /* 47 MHz */   10u, /* 48 MHz */   4u };
#endif /* (CY_IP_IMO_TRIMMABLE_BY_WCO) */

/* Stored CY_SYS_CLK_IMO_TRIM4_REG when modified for USB lock */
#if (CY_IP_IMO_TRIMMABLE_BY_USB)
        uint32 CySysClkImoTrim4 = 0u;
#endif /* (CY_IP_IMO_TRIMMABLE_BY_USB) */


/*******************************************************************************
* Function Name: CySysClkImoStart
********************************************************************************
*
* Summary:
*  Enables the IMO.
*
*  For PSoC 4100M / PSoC 4200M devices, this function will also enable WCO lock
*  if selected in the Design Wide Resources tab.
*
*  For PSoC 4200L devices, this function will also enable USB lock if selected
*  in the Design Wide Resources tab.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySysClkImoStart(void)
{
    CY_SYS_CLK_IMO_CONFIG_REG |= CY_SYS_CLK_IMO_CONFIG_ENABLE;

    #if (CY_IP_IMO_TRIMMABLE_BY_WCO)
        #if (CYDEV_IMO_TRIMMED_BY_WCO == 1u)
            CySysClkImoEnableWcoLock();
        #endif  /* (CYDEV_IMO_TRIMMED_BY_WCO == 1u) */
    #endif  /* (CY_IP_IMO_TRIMMABLE_BY_WCO) */


    #if (CY_IP_IMO_TRIMMABLE_BY_USB)
        #if (CYDEV_IMO_TRIMMED_BY_USB == 1u)
            CySysClkImoEnableUsbLock();
        #endif  /* (CYDEV_IMO_TRIMMED_BY_USB == 1u) */
    #endif  /* (CY_IP_IMO_TRIMMABLE_BY_USB) */

}


/*******************************************************************************
* Function Name: CySysClkImoStop
********************************************************************************
*
* Summary:
*  Disables the IMO.
*
*  For PSoC 4100M/PSoC 4200M devices, this function will also disable WCO lock.
*
*  For PSoC PSoC 4200L devices, this function will also disable USB lock.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySysClkImoStop(void)
{
    #if (CY_IP_IMO_TRIMMABLE_BY_WCO)
        CySysClkImoDisableWcoLock();
    #endif  /* (CY_IP_IMO_TRIMMABLE_BY_WCO) */

    #if (CY_IP_IMO_TRIMMABLE_BY_USB)
        CySysClkImoDisableUsbLock();
    #endif  /* (CY_IP_IMO_TRIMMABLE_BY_USB) */

    CY_SYS_CLK_IMO_CONFIG_REG &= ( uint32 ) ( ~( uint32 )CY_SYS_CLK_IMO_CONFIG_ENABLE);
}

#if (CY_IP_IMO_TRIMMABLE_BY_WCO)

    /*******************************************************************************
    * Function Name: CySysClkImoEnableWcoLock
    ********************************************************************************
    *
    * Summary:
    *  Enables the IMO to WCO lock feature. This function works only if the WCO is
    *  already enabled. If the WCO is not enabled then this function returns
    *  without enabling the lock feature.
    *
    *  It takes up to 20 ms for the IMO to stabilize. The delay is implemented with
    *  CyDelay() function. The delay interval is measured based on the system
    *  frequency defined by PSoC Creator at build time. If System clock frequency
    *  is changed in runtime, the CyDelayFreq() with the appropriate parameter
    *  should be called.
    *
    *  For PSoC 4200L devices, note that the IMO can lock to either WCO or USB
    *  but not both.
    *
    *  This function is applicable for PSoC 4100M, PSoC 4200M and  PSoC 4200L.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkImoEnableWcoLock(void)
    {
        uint32 i;
        uint32 freq = CY_SYS_CLK_IMO_MIN_FREQ_MHZ;
        uint8  interruptState;
        uint32 regTmp;
        uint32 lfLimit = 0u;
        volatile uint32 flashCtlReg;

        if (0u != CySysClkWcoEnabled())
        {
            interruptState = CyEnterCriticalSection();

            /* Set oscillator interface control port to WCO */
            #if (CY_IP_IMO_TRIMMABLE_BY_WCO && CY_IP_IMO_TRIMMABLE_BY_USB)
                CY_SYS_CLK_OSCINTF_CTL_REG =
                    (CY_SYS_CLK_OSCINTF_CTL_REG & (uint32) ~CY_SYS_CLK_OSCINTF_CTL_PORT_SEL_MASK) |
                    CY_SYS_CLK_OSCINTF_CTL_PORT_SEL_WCO;
            #endif /* (CY_IP_IMO_TRIMMABLE_BY_WCO && CY_IP_IMO_TRIMMABLE_BY_USB) */

            /* Get current IMO frequency based on the register value */
            for(i = 0u; i < CY_SYS_CLK_IMO_FREQ_TABLE_SIZE; i++)
            {
                if ((uint8) (CY_SYS_CLK_IMO_TRIM2_REG & CY_SYS_CLK_IMO_FREQ_BITS_MASK) == cyImoFreqMhz2Reg[i])
                {
                    freq = i + CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET;
                    break;
                }
            }

            /* For the WCO locking mode, the IMO gain needs to be CY_SYS_CLK_IMO_TRIM4_GAIN */
            if ((CY_SYS_CLK_IMO_TRIM4_REG & CY_SYS_CLK_IMO_TRIM4_GAIN_MASK) == 0u)
            {
			    CY_SYS_CLK_IMO_TRIM4_REG = (CY_SYS_CLK_IMO_TRIM4_REG & (uint32) ~CY_SYS_CLK_IMO_TRIM4_GAIN_MASK) |
			             				    CY_SYS_CLK_IMO_TRIM4_WCO_GAIN;
            }

            regTmp  = CY_SYS_CLK_WCO_DPLL_REG & ~(CY_SYS_CLK_WCO_DPLL_MULT_MASK |
                                                  CY_SYS_CLK_WCO_CONFIG_DPLL_LF_IGAIN_MASK |
                                                  CY_SYS_CLK_WCO_CONFIG_DPLL_LF_PGAIN_MASK |
                                                  CY_SYS_CLK_WCO_CONFIG_DPLL_LF_LIMIT_MASK);

            /* Set multiplier to determine IMO frequency in multiples of the WCO frequency */
            regTmp |= (CY_SYS_CLK_WCO_DPLL_MULT_VALUE(freq) & CY_SYS_CLK_WCO_DPLL_MULT_MASK);

            /* Set DPLL Loop Filter Integral and Proportional Gains Setting */
            regTmp |= (CY_SYS_CLK_WCO_CONFIG_DPLL_LF_IGAIN | CY_SYS_CLK_WCO_CONFIG_DPLL_LF_PGAIN);

            /* Set maximum allowed IMO offset */
            if (freq < CY_SYS_CLK_IMO_FREQ_WCO_DPLL_SAFE_POINT)
            {
                regTmp |= (CY_SYS_CLK_WCO_CONFIG_DPLL_LF_LIMIT_MAX << CY_SYS_CLK_WCO_CONFIG_DPLL_LF_LIMIT_SHIFT);
            }
            else
            {
                lfLimit = CY_SFLASH_IMO_TRIM_REG(freq - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) +
                    cyImoFreqMhz2DpllOffset[freq - CY_SYS_CLK_IMO_FREQ_WCO_DPLL_TABLE_OFFSET];

                lfLimit = (lfLimit > CY_SYS_CLK_WCO_CONFIG_DPLL_LF_LIMIT_MAX) ?
                    CY_SYS_CLK_WCO_CONFIG_DPLL_LF_LIMIT_MAX : lfLimit;

                regTmp |= (lfLimit << CY_SYS_CLK_WCO_CONFIG_DPLL_LF_LIMIT_SHIFT);
            }

            CY_SYS_CLK_WCO_DPLL_REG = regTmp;

            flashCtlReg = CY_FLASH_CTL_REG;
            CySysFlashSetWaitCycles(CY_SYS_CLK_IMO_MAX_FREQ_MHZ);
            CY_SYS_CLK_WCO_CONFIG_REG |= CY_SYS_CLK_WCO_CONFIG_DPLL_ENABLE;
            CyDelay(CY_SYS_CLK_WCO_IMO_TIMEOUT_MS);
            CY_FLASH_CTL_REG = flashCtlReg;

            CyExitCriticalSection(interruptState);
        }
    }


    /*******************************************************************************
    * Function Name: CySysClkImoDisableWcoLock
    ********************************************************************************
    *
    * Summary:
    *  Disables the IMO to WCO lock feature.
    *
    *  This function is applicable for PSoC 4100M, PSoC 4200M and  PSoC 4200L.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkImoDisableWcoLock(void)
    {
        CY_SYS_CLK_WCO_CONFIG_REG &= (uint32) ~CY_SYS_CLK_WCO_CONFIG_DPLL_ENABLE;
    }


    /*******************************************************************************
    * Function Name: CySysClkImoGetWcoLock
    ********************************************************************************
    *
    * Summary:
    *  Reports the IMO to WCO lock enable state.
    *
    *  This function is applicable for PSoC 4100M, PSoC 4200M and  PSoC 4200L.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  1 if IMO to WCO lock is enabled, and 0 if IMO to WCO lock is disabled.
    *
    *******************************************************************************/
    uint32 CySysClkImoGetWcoLock(void)
    {
        return ((0u != (CY_SYS_CLK_WCO_CONFIG_REG & CY_SYS_CLK_WCO_CONFIG_DPLL_ENABLE)) ?
                (uint32) 1u :
                (uint32) 0u);
    }

#endif /* (CY_IP_IMO_TRIMMABLE_BY_WCO) */


#if (CY_IP_IMO_TRIMMABLE_BY_USB)

    /*******************************************************************************
    * Function Name: CySysClkImoEnableUsbLock
    ********************************************************************************
    *
    * Summary:
    *  Enables the IMO to USB lock feature.
    *
    *  This function must be called before CySysClkWriteImoFreq().
    *
    *  This function is called from CySysClkImoStart() function if USB lock
    *  selected in the Design Wide Resources tab.
    *
    *  This is applicable for PSoC 4200L family of devices only. For PSoC 4200L
    *  devices, the IMO can lock to either WCO or USB, but not both.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkImoEnableUsbLock(void)
    {
        /* Set oscillator interface control port to USB */
        #if (CY_IP_IMO_TRIMMABLE_BY_WCO && CY_IP_IMO_TRIMMABLE_BY_USB)
            CY_SYS_CLK_OSCINTF_CTL_REG = (CY_SYS_CLK_OSCINTF_CTL_REG & (uint32) ~CY_SYS_CLK_OSCINTF_CTL_PORT_SEL_MASK) |
                                          CY_SYS_CLK_OSCINTF_CTL_PORT_SEL_USB;
        #endif /* (CY_IP_IMO_TRIMMABLE_BY_WCO && CY_IP_IMO_TRIMMABLE_BY_USB) */

        /* Save CY_SYS_CLK_IMO_TRIM4_REG and set IMO gain for USB lock */
        CySysClkImoTrim4 = CY_SYS_CLK_IMO_TRIM4_REG & (uint32) ~CY_SYS_CLK_IMO_TRIM4_GAIN_MASK;
        CY_SYS_CLK_IMO_TRIM4_REG = (CY_SYS_CLK_IMO_TRIM4_REG & (uint32) ~CY_SYS_CLK_IMO_TRIM4_GAIN_MASK) |
                                    CY_SYS_CLK_IMO_TRIM4_USB_GAIN;

        CY_SYS_CLK_USBDEVv2_CR1_REG |= CY_SYS_CLK_USBDEVv2_CR1_ENABLE_LOCK;
    }


    /*******************************************************************************
    * Function Name: CySysClkImoDisableUsbLock
    ********************************************************************************
    *
    * Summary:
    *  Disables the IMO to USB lock feature.
    *
    *  This function is called from CySysClkImoStop() function if USB lock selected
    *  in the Design Wide Resources tab.
    *
    *  This is applicable for PSoC 4200L family of devices only.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkImoDisableUsbLock(void)
    {
        CY_SYS_CLK_USBDEVv2_CR1_REG &= (uint32) ~CY_SYS_CLK_USBDEVv2_CR1_ENABLE_LOCK;

        /* Restore CY_SYS_CLK_IMO_TRIM4_REG */
        CY_SYS_CLK_IMO_TRIM4_REG = (CY_SYS_CLK_IMO_TRIM4_REG & (uint32) ~CY_SYS_CLK_IMO_TRIM4_GAIN_MASK) |
                                    CySysClkImoTrim4;
    }


    /*******************************************************************************
    * Function Name: CySysClkImoGetUsbLock
    ********************************************************************************
    *
    * Summary:
    *  Reports the IMO to USB lock enable state.
    *
    *  This function is applicable for PSoC 4200L devices only.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  1 if IMO to USB lock is enabled, and 0 if IMO to USB lock is disabled.
    *
    *******************************************************************************/
    uint32 CySysClkImoGetUsbLock(void)
    {
        return ((0u != (CY_SYS_CLK_USBDEVv2_CR1_REG & CY_SYS_CLK_USBDEVv2_CR1_ENABLE_LOCK)) ?
                (uint32) 1u :
                (uint32) 0u);
    }
#endif  /* (CY_IP_IMO_TRIMMABLE_BY_USB) */



/*******************************************************************************
* Function Name: CySysClkWriteHfclkDirect
********************************************************************************
*
* Summary:
*  Selects the direct source for the HFCLK.
*
* Parameters:
*  clkSelect: One of the available HFCLK direct sources:
*   Define                    Source
*    CY_SYS_CLK_HFCLK_IMO     IMO
*    CY_SYS_CLK_HFCLK_EXTCLK  External clock pin
*    CY_SYS_CLK_HFCLK_ECO     External crystal oscillator
*                             (applicable only for PSoC 4100-BL / PSoC 4200-BL)
*    CY_SYS_CLK_HFCLK_PLL0    PLL#0
*                             (applicable only for PSoC 4200L)
*    CY_SYS_CLK_HFCLK_PLL1    PLL#1
*                             (applicable only for PSoC 4200L)
*
* Return:
*  None
*
* Side Effects:
*  The new source must be running and stable before calling this function.
*
*  PSoC 4000:
*  The SYSCLK has a maximum speed of 16 MHz, so HFCLK and SYSCLK dividers should
*  be selected in a way to not to exceed 16 MHz for the System clock.
*
*  If the SYSCLK clock frequency increases during device operation, call
*  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
*  of clock cycles the cache will wait before sampling data comes back from
*  Flash. If the SYSCLK clock frequency decreases, you can call
*  CySysFlashSetWaitCycles() to improve the CPU performance. See
*  CySysFlashSetWaitCycles() description for more information.
*
*  Do not select PLL as the source for HFCLK if PLL output frequency exceeds
*  maximum permissible value for HFCLK.
*
*******************************************************************************/
void CySysClkWriteHfclkDirect(uint32 clkSelect)
{
    uint8  interruptState;
    uint32 tmpReg;

    interruptState = CyEnterCriticalSection();


    tmpReg = CY_SYS_CLK_SELECT_REG & ~(CY_SYS_CLK_SELECT_DIRECT_SEL_MASK |
                                       CY_SYS_CLK_SELECT_HFCLK_SEL_MASK);

#if (CY_IP_SRSSV2 && CY_IP_PLL)
    if ((CY_SYS_CLK_HFCLK_PLL0 == clkSelect) || (CY_SYS_CLK_HFCLK_PLL1 == clkSelect))
    {
        tmpReg |= (clkSelect & CY_SYS_CLK_SELECT_HFCLK_SEL_MASK);
    }
    else
#endif /* (CY_IP_SRSSV2 && CY_IP_PLL) */
    {
        tmpReg |= (clkSelect & CY_SYS_CLK_SELECT_DIRECT_SEL_MASK);
    }

    CY_SYS_CLK_SELECT_REG = tmpReg;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CySysClkGetSysclkSource
********************************************************************************
*
* Summary:
*  Returns the source of the System clock.
*
* Parameters:
*  None
*
* Return:
*  The same as CySysClkWriteHfclkDirect() function parameters.
*
*******************************************************************************/
uint32 CySysClkGetSysclkSource(void)
{
    uint8  interruptState;
    uint32 sysclkSource;

    interruptState = CyEnterCriticalSection();

#if (CY_IP_SRSSV2 && CY_IP_PLL)
    if ((CY_SYS_CLK_SELECT_REG & CY_SYS_CLK_SELECT_HFCLK_SEL_MASK) != 0u)
    {
        sysclkSource = (CY_SYS_CLK_SELECT_REG & CY_SYS_CLK_SELECT_HFCLK_SEL_MASK);
    }
    else
#endif /* (CY_IP_SRSSV2 && CY_IP_PLL) */
    {
        sysclkSource = (CY_SYS_CLK_SELECT_REG & CY_SYS_CLK_SELECT_DIRECT_SEL_MASK);
    }

    CyExitCriticalSection(interruptState);

    return (sysclkSource);
}


/*******************************************************************************
* Function Name: CySysClkWriteSysclkDiv
********************************************************************************
*
* Summary:
*  Selects the prescaler divide amount for SYSCLK from HFCLK.
*
* Parameters:
*  divider: Power of 2 prescaler selection
*
*   Define                        Description
*   CY_SYS_CLK_SYSCLK_DIV1        SYSCLK = HFCLK / 1
*   CY_SYS_CLK_SYSCLK_DIV2        SYSCLK = HFCLK / 2
*   CY_SYS_CLK_SYSCLK_DIV4        SYSCLK = HFCLK / 4
*   CY_SYS_CLK_SYSCLK_DIV8        SYSCLK = HFCLK / 8
*   CY_SYS_CLK_SYSCLK_DIV16       SYSCLK = HFCLK / 16  (N/A for 4000 Family)
*   CY_SYS_CLK_SYSCLK_DIV32       SYSCLK = HFCLK / 32  (N/A for 4000 Family)
*   CY_SYS_CLK_SYSCLK_DIV64       SYSCLK = HFCLK / 64  (N/A for 4000 Family)
*   CY_SYS_CLK_SYSCLK_DIV128      SYSCLK = HFCLK / 128 (N/A for 4000 Family)
*
* Return:
*  None
*
* Side Effects:
*  4000 Family:
*  The SYS_CLK has the speed of 16 MHz, so dividers HF_CLK and SYS_CLK
*  dividers should be selected in a way, not to exceed 16 MHz for SYS_CLK.
*
*  If the SYSCLK clock frequency increases during the device operation, call
*  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
*  of clock cycles the cache will wait before sampling data comes back from
*  Flash. If the SYSCLK clock frequency decreases, you can call
*  CySysFlashSetWaitCycles() to improve the CPU performance. See
*  CySysFlashSetWaitCycles() description for more information.
*
*******************************************************************************/
void CySysClkWriteSysclkDiv(uint32 divider)
{
    uint8  interruptState;

    interruptState = CyEnterCriticalSection();

    CY_SYS_CLK_SELECT_REG = ((uint32)(((uint32)divider & CY_SYS_CLK_SELECT_SYSCLK_DIV_MASK) <<
                                    CY_SYS_CLK_SELECT_SYSCLK_DIV_SHIFT)) |
                            (CY_SYS_CLK_SELECT_REG & ((uint32)(~(uint32)(CY_SYS_CLK_SELECT_SYSCLK_DIV_MASK <<
                                    CY_SYS_CLK_SELECT_SYSCLK_DIV_SHIFT))));

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CySysClkWriteImoFreq
********************************************************************************
*
* Summary:
*  Sets the frequency of the IMO.
*
*  If IMO is currently driving the HFCLK, and if the HFCLK frequency decreases,
*  you can call CySysFlashSetWaitCycles () to improve CPU performance. See
*  CySysFlashSetWaitCycles () for more information.
*
*  For PSoC 4000 family of devices, maximum HFCLK frequency is 16 MHz. If IMO is
*  configured to frequencies above 16 MHz, ensure to set the appropriate HFCLK
*  predivider value first.
*
*  For PSoC 4200M and PSoC 4200L device families, if WCO lock feature is enabled
*  then this API will disable the lock, write the new IMO frequency and then
*  re-enable the lock.
*
*  For PSoC 4200L device families, this function enables the USB lock when 24 or
*  48 MHz passed as a parameter if the USB lock option is enabled in Design Wide
*  Resources tab or CySysClkImoEnableUsbLock() was called before. Note the USB
*  lock is disabled during IMO frequency change.
*
*  Note: The CPU is halted if new frequency is invalid and project is compiled
*  in debug mode.
*
* Parameters:
*  All PSoC 4 families excluding PSoC 4000: Valid range [3-48] with step size
*  equals 1.
*
*  PSoC 4000: Valid range [24-48] with step size equals 4.
*
*  Note: The CPU is halted if new frequency is invalid and project is
*  compiled in debug mode.
*
* Return:
*  None
*
* Side Effects:
*  If the SYSCLK clock frequency increases during the device operation, call
*  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
*  of clock cycles the cache will wait before sampling data comes back from
*  Flash. If the SYSCLK clock frequency decreases, you can call
*  CySysFlashSetWaitCycles() to improve the CPU performance. See
*  CySysFlashSetWaitCycles() description for more information.
*
*  PSoC 4000: The System Clock (SYSCLK) has maximum speed of 16 MHz, so HFCLK
*  and SYSCLK dividers should be selected in a way, to not to exceed 16 MHz for
*  the System clock.
*
*******************************************************************************/
#if(CY_IP_SRSSV2)
    void CySysClkWriteImoFreq(uint32 freq)
    {
        #if (CY_IP_FMLT)
            volatile uint32   parameters[2u];
            volatile uint32   regValues[4u];
        #else
            uint8  bgTrim4;
            uint8  bgTrim5;
            uint8  newImoTrim2Value;
            uint8  currentImoTrim2Value;
            uint8  imoTrim1Value;
        #endif /* (CY_IP_FM) */

        #if (CY_IP_IMO_TRIMMABLE_BY_WCO)
            uint32 wcoLock = 0u;
        #endif  /* (CY_IP_IMO_TRIMMABLE_BY_WCO) */

        #if (CY_IP_IMO_TRIMMABLE_BY_USB)
            uint32 usbLock = 0u;
        #endif  /* (CY_IP_IMO_TRIMMABLE_BY_USB) */

        uint8  interruptState;


        interruptState = CyEnterCriticalSection();

        #if (CY_IP_IMO_TRIMMABLE_BY_WCO)
            if(0u != CySysClkImoGetWcoLock())
            {
                wcoLock = 1u;
                CySysClkImoDisableWcoLock();
            }
        #endif  /* (CY_IP_IMO_TRIMMABLE_BY_WCO) */


        #if (CY_IP_IMO_TRIMMABLE_BY_USB)

            #if (CYDEV_IMO_TRIMMED_BY_USB == 0u)
                if(0u != CySysClkImoGetUsbLock())
                {
            #endif  /* (CYDEV_IMO_TRIMMED_BY_USB == 0u) */

                if ((24u == freq) || (48u == freq))
                {
                    usbLock = 1u;
                    CySysClkImoDisableUsbLock();
                }

            #if (CYDEV_IMO_TRIMMED_BY_USB == 0u)
                }
            #endif  /* (CYDEV_IMO_TRIMMED_BY_USB == 0u) */

        #endif  /* (CYDEV_IMO_TRIMMED_BY_USB == 0u) */


        #if (CY_IP_FMLT)

            /* FM-Lite Clock Restore */
            regValues[0u] = CY_SYS_CLK_IMO_CONFIG_REG;
            regValues[1u] = CY_SYS_CLK_SELECT_REG;
            regValues[2u] = cyImoFreqMhz2Reg[freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];
            regValues[3u] = CY_FLASH_CTL_REG;

            parameters[0u] =
                (uint32) ((CY_FLASH_KEY_TWO(CY_FLASH_API_OPCODE_CLK_RESTORE) <<  CY_FLASH_PARAM_KEY_TWO_OFFSET) |
                            CY_FLASH_KEY_ONE);
            parameters[1u] = (uint32) &regValues[0u];

            CY_FLASH_CPUSS_SYSARG_REG = (uint32) &parameters[0u];
            CY_FLASH_CPUSS_SYSREQ_REG = CY_FLASH_CPUSS_REQ_START | CY_FLASH_API_OPCODE_CLK_RESTORE;
            (void) CY_FLASH_CPUSS_SYSARG_REG;

        #else /* (CY_IP_FM) */

        if ((freq >= CY_SYS_CLK_IMO_MIN_FREQ_MHZ) && (freq <= CY_SYS_CLK_IMO_MAX_FREQ_MHZ))
        {
            if(freq <= CY_SFLASH_IMO_MAXF0_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS0_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO0_REG;
            }
            else if(freq <= CY_SFLASH_IMO_MAXF1_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS1_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO1_REG;
            }
            else if(freq <= CY_SFLASH_IMO_MAXF2_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS2_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO2_REG;
            }
            else if(freq <= CY_SFLASH_IMO_MAXF3_REG)
            {
                bgTrim4 = CY_SFLASH_IMO_ABS3_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO3_REG;
            }
            else
            {
                bgTrim4 = CY_SFLASH_IMO_ABS4_REG;
                bgTrim5 = CY_SFLASH_IMO_TMPCO4_REG;
            }

            /* Get IMO_TRIM2 value for the new frequency */
            newImoTrim2Value = cyImoFreqMhz2Reg[freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];


            /****************************************************************************
            * The IMO can have a different trim per frequency. To avoid possible corner
            * cases where a trim change can exceed the maximum frequency, the trim must
            * be applied at a frequency that is low enough.
            *
            * Comparing IMO_TRIM2 values for the current and new frequencies, since
            * IMO_TRIM2 value as a function of IMO frequency is a strictly increasing
            * function and is time-invariant.
            ***************************************************************************/
            if ((newImoTrim2Value >= CY_SYS_CLK_IMO_BOUNDARY_FREQ_TRIM2) && (freq >= CY_SYS_CLK_IMO_BOUNDARY_FREQ_MHZ))
            {
                /* Set boundary IMO frequency: safe for IMO above 48 MHZ trimming */
                CY_SYS_CLK_IMO_TRIM2_REG = (uint32) cyImoFreqMhz2Reg[CY_SYS_CLK_IMO_TEMP_FREQ_MHZ -
                                                                     CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];

                CyDelayCycles(CY_SYS_CLK_IMO_FREQ_TIMEOUT_CYCLES);

                currentImoTrim2Value = CY_SYS_CLK_IMO_TEMP_FREQ_TRIM2;
            }
            else
            {
                currentImoTrim2Value = (uint8) (CY_SYS_CLK_IMO_TRIM2_REG & CY_SYS_CLK_IMO_FREQ_BITS_MASK);
            }


            /***************************************************************************
            * A trim change needs to be allowed to settle (within 5us) before the Freq
            * can be changed to a new frequency.
            *
            * Comparing IMO_TRIM2 values for the current and new frequencies, since
            * IMO_TRIM2 value as a function of IMO frequency is a strictly increasing
            * function and is time-invariant.
            ***************************************************************************/
            if (newImoTrim2Value < currentImoTrim2Value)
            {
                /* Set new IMO frequency */
                CY_SYS_CLK_IMO_TRIM2_REG = cyImoFreqMhz2Reg[freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];
                CyDelayCycles(CY_SYS_CLK_IMO_FREQ_TIMEOUT_CYCLES);
            }

            /* Set trims for the new IMO frequency */

            #if (CY_IP_IMO_TRIMMABLE_BY_USB)
                if ((1u == usbLock) && (48u == freq))
                {
                    imoTrim1Value = CY_SFLASH_IMO_TRIM_USBMODE_48_REG;
                }
                else if ((1u == usbLock) && (24u == freq))
                {
                    imoTrim1Value = CY_SFLASH_IMO_TRIM_USBMODE_24_REG;
                }
                else
                {
                    imoTrim1Value = (uint8) CY_SFLASH_IMO_TRIM_REG(freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET);
                }
            #else
                imoTrim1Value = (uint8) CY_SFLASH_IMO_TRIM_REG(freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET);
            #endif  /* (CY_IP_IMO_TRIMMABLE_BY_USB) */


            CY_SYS_CLK_IMO_TRIM1_REG = (uint32) imoTrim1Value;
            CY_PWR_BG_TRIM4_REG  = bgTrim4;
            CY_PWR_BG_TRIM5_REG  = bgTrim5;
            CyDelayUs(CY_SYS_CLK_IMO_TRIM_TIMEOUT_US);

            if (newImoTrim2Value > currentImoTrim2Value)
            {
                /* Set new IMO frequency */
                CY_SYS_CLK_IMO_TRIM2_REG = cyImoFreqMhz2Reg[freq - CY_SYS_CLK_IMO_FREQ_TABLE_OFFSET];
                CyDelayCycles(CY_SYS_CLK_IMO_FREQ_TIMEOUT_CYCLES);
            }
        }
        else
        {
            /* Halt CPU in debug mode if new frequency is invalid */
            CYASSERT(0u != 0u);
        }

        #endif /* (CY_IP_FMLT) */

        #if (CY_IP_IMO_TRIMMABLE_BY_WCO)
            if (1u == wcoLock)
            {
                CySysClkImoEnableWcoLock();
            }
        #endif  /* (CY_IP_IMO_TRIMMABLE_BY_WCO) */

        #if (CY_IP_IMO_TRIMMABLE_BY_USB)
            if (1u == usbLock)
            {
                CySysClkImoEnableUsbLock();
            }
        #endif  /* (CY_IP_IMO_TRIMMABLE_BY_USB) */

        CyExitCriticalSection(interruptState);
    }

#else

    void CySysClkWriteImoFreq(uint32 freq)
    {
        uint8  interruptState;

        if ((freq == 24u) || (freq == 32u) || (freq == 48u))
        {
            interruptState = CyEnterCriticalSection();

            /* Set IMO to 24 MHz - CLK_IMO_SELECT.FREQ = 0 */
            CY_SYS_CLK_IMO_SELECT_REG &= ((uint32) ~CY_SYS_CLK_IMO_SELECT_FREQ_MASK);

            /* Apply coarse trim */
            CY_SYS_CLK_IMO_TRIM1_REG = (CY_SYS_CLK_IMO_TRIM1_REG & ((uint32) ~CY_SYS_CLK_IMO_TRIM1_OFFSET_MASK)) |
                (CY_SFLASH_IMO_TRIM_REG(freq - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) & CY_SYS_CLK_IMO_TRIM1_OFFSET_MASK);

            /* Zero out fine trim */
            CY_SYS_CLK_IMO_TRIM2_REG = CY_SYS_CLK_IMO_TRIM2_REG & ((uint32) ~CY_SYS_CLK_IMO_TRIM2_FSOFFSET_MASK);

            /* Apply TC trim */
            CY_SYS_CLK_IMO_TRIM3_REG = (CY_SYS_CLK_IMO_TRIM3_REG & ((uint32) ~CY_SYS_CLK_IMO_TRIM3_VALUES_MASK)) |
                (CY_SFLASH_IMO_TCTRIM_REG(freq - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) & CY_SYS_CLK_IMO_TRIM3_VALUES_MASK);

            CyDelayCycles(CY_SYS_CLK_IMO_TRIM_DELAY_US);

            if (freq > CY_SYS_CLK_IMO_MIN_FREQ_MHZ)
            {
                /* Select nearby intermediate frequency */
                CY_SYS_CLK_IMO_SELECT_REG = (CY_SYS_CLK_IMO_SELECT_REG & ((uint32) ~CY_SYS_CLK_IMO_SELECT_FREQ_MASK)) |
                    (((freq - 4u - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) >> 2u) & CY_SYS_CLK_IMO_SELECT_FREQ_MASK);

                CyDelayCycles(CY_SYS_CLK_IMO_TRIM_DELAY_US);

                /* Make small step to final frequency */
                /* Select nearby intermediate frequency */
                CY_SYS_CLK_IMO_SELECT_REG = (CY_SYS_CLK_IMO_SELECT_REG & ((uint32) ~CY_SYS_CLK_IMO_SELECT_FREQ_MASK)) |
                    (((freq - CY_SYS_CLK_IMO_MIN_FREQ_MHZ) >> 2u) & CY_SYS_CLK_IMO_SELECT_FREQ_MASK);
            }

            CyExitCriticalSection(interruptState);
        }
        else
        {
            /* Halt CPU in debug mode if new frequency is invalid */
            CYASSERT(0u != 0u);
        }
    }

#endif /* (CY_IP_SRSSV2) */


#if(CY_IP_SRSSLT)
    /*******************************************************************************
    * Function Name: CySysClkWriteHfclkDiv
    ********************************************************************************
    *
    * Summary:
    *  Selects the pre-scaler divider value for HFCLK from IMO.
    *
    *  The HFCLK predivider allows the device to divide the HFCLK selection mux
    *  input before use as HFCLK. The predivider is capable of dividing the HFCLK by
    *  powers of 2 between 1 and 8.
    *
    *  Note HFCLK's frequency cannot exceed 16 MHz. This API is applicable for
    *  PSoC 4000 family of devices only.
    *
    *
    * Parameters:
    *  divider: HF clock divider value
    *   Define                        Description
    *   CY_SYS_CLK_HFCLK_DIV_NODIV    Transparent mode (w/o dividing)
    *   CY_SYS_CLK_HFCLK_DIV_2        Divide selected clock source by 2
    *   CY_SYS_CLK_HFCLK_DIV_4        Divide selected clock source by 4
    *   CY_SYS_CLK_HFCLK_DIV_8        Divide selected clock source by 8
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  If the SYSCLK clock frequency increases during the device operation, call
    *  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
    *  of clock cycles the cache will wait before sampling data comes back from
    *  Flash. If the SYSCLK clock frequency decreases, you can call
    *  CySysFlashSetWaitCycles() to improve the CPU performance. See
    *  CySysFlashSetWaitCycles() description for more information.
    *
    *******************************************************************************/
    void CySysClkWriteHfclkDiv(uint32 divider)
    {
        uint8  interruptState;

        interruptState = CyEnterCriticalSection();

        CY_SYS_CLK_SELECT_REG = ((CY_SYS_CLK_SELECT_REG & ((uint32) (~(CY_SYS_CLK_SELECT_HFCLK_DIV_MASK <<
                                        CY_SYS_CLK_SELECT_HFCLK_DIV_SHIFT)))) |
                    ((uint32)((divider & CY_SYS_CLK_SELECT_HFCLK_DIV_MASK) << CY_SYS_CLK_SELECT_HFCLK_DIV_SHIFT)));

        CyExitCriticalSection(interruptState);
    }
#endif /* (CY_IP_SRSSLT) */


#if (CY_IP_ECO)

    /*******************************************************************************
    * Function Name: CySysClkEcoStart
    ********************************************************************************
    *
    * Summary:
    *  Starts the External Crystal Oscillator (ECO). Refer to the device datasheet
    *  for the ECO startup time.
    *
    *  The timeout interval is measured based on the system frequency defined by
    *  PSoC Creator at build time. If System clock frequency is changed in
    *  runtime, the CyDelayFreq() with the appropriate parameter should be called.
    *
    * Parameters:
    *  timeoutUs: Timeout in microseconds. If zero is specified, the function
    *             starts the crystal and returns CYRET_SUCCESS. If non-zero
    *             value is passed, the CYRET_SUCCESS is returned once crystal
    *             is oscillating and amplitude reached 60% and it does not mean
    *             24 MHz crystal is within 50 ppm. If it is not oscillating or
    *             amplitude didn't reach 60% after specified amount of time, the
    *             CYRET_TIMEOUT is returned.
    *
    * Return:
    *   CYRET_SUCCESS - Completed successfully. The ECO is oscillating and amplitude
    *                   reached 60% and it does not mean 24 MHz crystal is within
    *                   50 ppm.
    *   CYRET_TIMEOUT - Timeout occurred.
    *
    *******************************************************************************/
    cystatus CySysClkEcoStart(uint32 timeoutUs)
    {
        cystatus returnStatus = CYRET_SUCCESS;

        #if (CY_IP_WCO_BLESS)
            /* Enable the RF oscillator band gap */
            CY_SYS_XTAL_BLESS_RF_CONFIG_REG |= CY_SYS_XTAL_BLESS_RF_CONFIG_RF_ENABLE;

            /* Update trimming register */
            CY_SYS_XTAL_BLERD_BB_XO_REG = CY_SYS_XTAL_BLERD_BB_XO_TRIM;

            /* Enable the Crystal */
            CY_SYS_XTAL_BLERD_DBUS_REG |= CY_SYS_XTAL_BLERD_DBUS_XTAL_ENABLE;
        #else /* CY_IP_WCO_SRSSV2 */
            CY_SYS_CLK_ECO_CONFIG_REG |= CY_SYS_CLK_ECO_CONFIG_ENABLE;
            CyDelayUs(CY_SYS_CLK_ECO_CONFIG_CLK_EN_TIMEOUT_US);
            CY_SYS_CLK_ECO_CONFIG_REG |= CY_SYS_CLK_ECO_CONFIG_CLK_EN;
        #endif /* (CY_IP_WCO_BLESS) */

        if(timeoutUs > 0u)
        {
            returnStatus = CYRET_TIMEOUT;

            for( ; timeoutUs > 0u; timeoutUs--)
            {
                CyDelayUs(1u);

                if(0u != CySysClkEcoReadStatus())
                {
                    returnStatus = CYRET_SUCCESS;
                    break;
                }
            }

        }

        return(returnStatus);
    }


    /*******************************************************************************
    * Function Name: CySysClkEcoStop
    ********************************************************************************
    *
    * Summary:
    *  Stops the megahertz crystal.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkEcoStop(void)
    {
        #if (CY_IP_WCO_BLESS)
            /* Disable the RF oscillator band gap */
            CY_SYS_XTAL_BLESS_RF_CONFIG_REG &= (uint32) ~CY_SYS_XTAL_BLESS_RF_CONFIG_RF_ENABLE;

            /* Disable the Crystal */
            CY_SYS_XTAL_BLERD_DBUS_REG &= (uint32) ~CY_SYS_XTAL_BLERD_DBUS_XTAL_ENABLE;
        #else
            CY_SYS_CLK_ECO_CONFIG_REG &= (uint32) ~(CY_SYS_CLK_ECO_CONFIG_ENABLE | CY_SYS_CLK_ECO_CONFIG_CLK_EN);
        #endif /* (CY_IP_WCO_BLESS) */
    }


    /*******************************************************************************
    * Function Name: CySysClkEcoReadStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status bit for the megahertz crystal.
    *
    *  For PSoC 4100 BLE/PSoC 4200 BLE devices, the status bit is the XO_AMP_DETECT
    *  bit in FSM register.
    *
    *  For PSoC 4200L devices, the error status bit is the WATCHDOG_ERROR bit in
    *  ECO_STATUS register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  PSoC 4100 BLE/PSoC 4200 BLE:
    *  Non-zero indicates that ECO output reached 50 ppm and is oscillating in valid
    *  range.
    *
    *  PSoC 4200L:
    *  Non-zero indicates that ECO is running.
    *
    *******************************************************************************/
    uint32 CySysClkEcoReadStatus(void)
    {
        uint32 returnValue;

        #if (CY_IP_WCO_BLESS)
            returnValue = CY_SYS_XTAL_BLERD_FSM_REG & CY_SYS_XTAL_BLERD_FSM_XO_AMP_DETECT;
        #else
            returnValue = (0u != (CY_SYS_CLK_ECO_STATUS_REG & CY_SYS_CLK_ECO_STATUS_WATCHDOG_ERROR)) ? 0u : 1u;
        #endif /* (CY_IP_WCO_BLESS) */

        return (returnValue);
    }

    #if (CY_IP_ECO_BLESS)
        /*******************************************************************************
        * Function Name: CySysClkWriteEcoDiv
        ********************************************************************************
        *
        * Summary:
        *  Selects value for the ECO divider.
        *
        *  The ECO must not be the HFCLK clock source when this function is called.
        *  The HFCLK source can be changed to the other clock source by call to the
        *  CySysClkWriteHfclkDirect() function. If the ECO sources the HFCLK this
        *  function will not have any effect if compiler in release mode, and halt the
        *  CPU when compiler in debug mode.
        *
        * Parameters:
        *  divider: Power of 2 divider selection.
        *
        *   Define                        Description
        *   CY_SYS_CLK_ECO_DIV1             HFCLK = ECO / 1
        *   CY_SYS_CLK_ECO_DIV2             HFCLK = ECO / 2
        *   CY_SYS_CLK_ECO_DIV4             HFCLK = ECO / 4
        *   CY_SYS_CLK_ECO_DIV8             HFCLK = ECO / 8
        *
        * Return:
        *  None
        *
        * Side Effects:
        *  If the SYSCLK clock frequency increases during the device operation, call
        *  CySysFlashSetWaitCycles() with the appropriate parameter to adjust the number
        *  of clock cycles the cache will wait before sampling data comes back from
        *  Flash. If the SYSCLK clock frequency decreases, you can call
        *  CySysFlashSetWaitCycles() to improve the CPU performance. See
        *  CySysFlashSetWaitCycles() description for more information.
        *
        *******************************************************************************/
        void CySysClkWriteEcoDiv(uint32 divider)
        {
            uint8  interruptState;

            if (CY_SYS_CLK_HFCLK_ECO != (CY_SYS_CLK_SELECT_REG & CY_SYS_CLK_SELECT_DIRECT_SEL_MASK))
            {
                interruptState = CyEnterCriticalSection();

                CY_SYS_CLK_XTAL_CLK_DIV_CONFIG_REG =  (divider & CY_SYS_CLK_XTAL_CLK_DIV_MASK) |
                                                      (CY_SYS_CLK_XTAL_CLK_DIV_CONFIG_REG & ((uint32) ~CY_SYS_CLK_XTAL_CLK_DIV_MASK));

                CyExitCriticalSection(interruptState);
            }
            else
            {
                /* Halt CPU in debug mode if ECO sources HFCLK */
                CYASSERT(0u != 0u);
            }
        }

    #else

        /*******************************************************************************
        * Function Name: CySysClkConfigureEcoTrim
        ********************************************************************************
        *
        * Summary:
        *  Selects trim setting values for ECO. This API is available only for PSoC
        *  4200L devices only.
        *
        *  The following parameters can be trimmed for ECO. The affected registers are
        *  ECO_TRIM0 and ECO_TRIM1.
        *
        *  Watchdog trim - This bit field sets the error threshold below the steady
        *  state amplitude level.
        *
        *  Amplitude trim - This bit field is to set the crystal drive level when
        *  ECO_CONFIG.AGC_EN = 1. WARNING: use care when setting this field because
        *  driving a crystal beyond its rated limit can permanently damage the crystal.
        *
        *  Filter frequency trim - This bit field sets LPF frequency trim and affects
        *  the 3rd harmonic content.
        *
        *  Feedback resistor trim - This bit field sets the feedback resistor trim and
        *  impacts the oscillation amplitude.
        *
        *  Amplifier gain trim - This bit field sets the amplifier gain trim and affects
        *  the startup time of the crystal.
        *
        * Parameters:
        *  wDTrim: Watchdog trim
        *       Parameter                   Value Description
        *   CY_SYS_CLK_ECO_WDTRIM0      Error threshold is 0.05 V
        *   CY_SYS_CLK_ECO_WDTRIM1      Error threshold is 0.10 V
        *   CY_SYS_CLK_ECO_WDTRIM2      Error threshold is 0.15 V
        *   CY_SYS_CLK_ECO_WDTRIM3      Error threshold is 0.20 V
        *
        *  aTrim: Amplitude trim
        *       Parameter                   Value Description
        *   CY_SYS_CLK_ECO_ATRIM0       Amplitude is 0.3 Vpp
        *   CY_SYS_CLK_ECO_ATRIM1       Amplitude is 0.4 Vpp
        *   CY_SYS_CLK_ECO_ATRIM2       Amplitude is 0.5 Vpp
        *   CY_SYS_CLK_ECO_ATRIM3       Amplitude is 0.6 Vpp
        *   CY_SYS_CLK_ECO_ATRIM4       Amplitude is 0.7 Vpp
        *   CY_SYS_CLK_ECO_ATRIM5       Amplitude is 0.8 Vpp
        *   CY_SYS_CLK_ECO_ATRIM6       Amplitude is 0.9 Vpp
        *   CY_SYS_CLK_ECO_ATRIM7       Amplitude is 1.0 Vpp
        *
        *  fTrim: Filter frequency trim
        *       Parameter                   Value Description
        *   CY_SYS_CLK_ECO_FTRIM0       Crystal frequency > 30 MHz
        *   CY_SYS_CLK_ECO_FTRIM1       24 MHz < Crystal frequency <= 30 MHz
        *   CY_SYS_CLK_ECO_FTRIM2       17 MHz < Crystal frequency <= 24 MHz
        *   CY_SYS_CLK_ECO_FTRIM3       Crystal frequency <= 17 MHz
        *
        *  rTrim: Feedback resistor trim
        *       Parameter                   Value Description
        *   CY_SYS_CLK_ECO_RTRIM0       Crystal frequency > 30 MHz
        *   CY_SYS_CLK_ECO_RTRIM1       24 MHz < Crystal frequency <= 30 MHz
        *   CY_SYS_CLK_ECO_RTRIM2       17 MHz < Crystal frequency <= 24 MHz
        *   CY_SYS_CLK_ECO_RTRIM3       Crystal frequency <= 17 MHz        *
        *
        *  gTrim: Amplifier gain trim. Calculate the minimum required gm
        *         (trans-conductance value). Divide the calculated gm value by
        *         4.5 to obtain an integer value 'result'. For more information
        *         please refer to the device TRM.
        *       Parameter                   Value Description
        *   CY_SYS_CLK_ECO_GTRIM0       If result = 1
        *   CY_SYS_CLK_ECO_GTRIM1       If result = 0
        *   CY_SYS_CLK_ECO_GTRIM2       If result = 2
        *   CY_SYS_CLK_ECO_GTRIM2       If result = 3

        * Return:
        *  None
        *
        * Side Effects:
        *  Use care when setting the amplitude trim field because driving a crystal
        *  beyond its rated limit can permanently damage the crystal.
        *******************************************************************************/
        void CySysClkConfigureEcoTrim(uint32 wDTrim, uint32 aTrim, uint32 fTrim, uint32 rTrim, uint32 gTrim)
        {
            uint8  interruptState;
            uint32 regTmp;

            interruptState = CyEnterCriticalSection();

            regTmp  = CY_SYS_CLK_ECO_TRIM0_REG & ~(CY_SYS_CLK_ECO_TRIM0_WDTRIM_MASK | CY_SYS_CLK_ECO_TRIM0_ATRIM_MASK);
            regTmp |= ((uint32) (wDTrim << CY_SYS_CLK_ECO_TRIM0_WDTRIM_SHIFT) & CY_SYS_CLK_ECO_TRIM0_WDTRIM_MASK);
            regTmp |= ((uint32) (aTrim << CY_SYS_CLK_ECO_TRIM0_ATRIM_SHIFT) & CY_SYS_CLK_ECO_TRIM0_ATRIM_MASK);
            CY_SYS_CLK_ECO_TRIM0_REG = regTmp;

            regTmp  = CY_SYS_CLK_ECO_TRIM1_REG & ~(CY_SYS_CLK_ECO_TRIM1_FTRIM_MASK |
                                                   CY_SYS_CLK_ECO_TRIM1_RTRIM_MASK |
                                                   CY_SYS_CLK_ECO_TRIM1_GTRIM_MASK);
            regTmp |= ((uint32) (fTrim << CY_SYS_CLK_ECO_TRIM1_FTRIM_SHIFT) & CY_SYS_CLK_ECO_TRIM1_FTRIM_MASK);
            regTmp |= ((uint32) (rTrim << CY_SYS_CLK_ECO_TRIM1_RTRIM_SHIFT) & CY_SYS_CLK_ECO_TRIM1_RTRIM_MASK);
            regTmp |= ((uint32) (gTrim << CY_SYS_CLK_ECO_TRIM1_GTRIM_SHIFT) & CY_SYS_CLK_ECO_TRIM1_GTRIM_MASK);

            CY_SYS_CLK_ECO_TRIM1_REG = regTmp;

            CyExitCriticalSection(interruptState);
        }


        /*******************************************************************************
        * Function Name: CySysClkConfigureEcoDrive
        ********************************************************************************
        *
        * Summary:
        *  Selects trim setting values for ECO based on crystal parameters. Use care
        *  when setting the driveLevel parameter because driving a crystal beyond its
        *  rated limit can permanently damage the crystal.
        *
        *  This API is available only for PSoC 4200L devices only.
        *
        * Parameters:
        *  freq: frequency of the crystal in kHz.
        *  cLoad: crystal load capacitance in pF.
        *  esr: equivalent series resistance of the crystal in ohm.
        *  maxAmplitude: maximum amplitude level in mV. Calculate as
        *  ((sqrt(driveLevel in uW / 2 / esr))/(3.14 * freq * cLoad)) * 10^9.
        *
        *  The Automatic Gain Control (AGC) is disabled when the specified maximum
        *  amplitude level equals or above 2. In this case the amplitude is not
        *  explicitly controlled and will grow until it saturates to the supply rail
        *  (1.8V nom). WARNING: use care when disabling AGC because driving a crystal
        *  beyond its rated limit can permanently damage the crystal.
        *
        * Return:
        *  status:
        *  CYRET_SUCCESS - ECO configuration completed successfully.
        *  CYRET_BAD_PARAM - One or more invalid parameters
        *
        *******************************************************************************/
        cystatus CySysClkConfigureEcoDrive(uint32 freq, uint32 cLoad, uint32 esr, uint32 maxAmplitude)
        {
            cystatus returnStatus = CYRET_SUCCESS;

            uint32 wDTrim;
            uint32 aTrim;
            uint32 fTrim;
            uint32 rTrim;
            uint32 gTrim;

            uint32 gmMin;


            if ((maxAmplitude < CY_SYS_CLK_ECO_MAX_AMPL_MIN_mV) ||
                (freq < CY_SYS_CLK_ECO_FREQ_KHZ_MIN) || (freq > CY_SYS_CLK_ECO_FREQ_KHZ_MAX))
            {
                returnStatus = CYRET_BAD_PARAM;
            }
            else
            {
                /* Calculate amplitude trim */
                aTrim = (maxAmplitude < CY_SYS_CLK_ECO_TRIM_BOUNDARY) ? ((maxAmplitude/100u) - 4u) : 7u;

                if (maxAmplitude < CY_SYS_CLK_ECO_AMPL_FOR_ATRIM0)
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM0;
                }
                else if (maxAmplitude < CY_SYS_CLK_ECO_AMPL_FOR_ATRIM1)
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM1;
                }
                else if (maxAmplitude < CY_SYS_CLK_ECO_AMPL_FOR_ATRIM2)
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM2;
                }
                else if (maxAmplitude < CY_SYS_CLK_ECO_AMPL_FOR_ATRIM3)
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM3;
                }
                else if (maxAmplitude < CY_SYS_CLK_ECO_AMPL_FOR_ATRIM4)
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM4;
                }
                else if (maxAmplitude < CY_SYS_CLK_ECO_AMPL_FOR_ATRIM5)
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM5;
                }
                else if (maxAmplitude < CY_SYS_CLK_ECO_AMPL_FOR_ATRIM6)
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM6;
                }
                else
                {
                    aTrim = CY_SYS_CLK_ECO_ATRIM7;
                }

                /* Calculate Watchdog trim. */
                wDTrim = (maxAmplitude < CY_SYS_CLK_ECO_TRIM_BOUNDARY) ? ((maxAmplitude/200u) - 2u) : 3u;

                /* Calculate amplifier gain trim. */
                gmMin = (uint32) (((((CY_SYS_CLK_ECO_GMMIN_COEFFICIENT * freq * cLoad) / 1000) * ((freq * cLoad * esr) / 1000)) / 100u) / 4500000u);
                if (gmMin > 3u)
                {
                    returnStatus = CYRET_BAD_PARAM;
                    gTrim = 0u;
                }
                else if (gmMin > 1u)
                {
                    gTrim = gmMin;
                }
                else
                {
                    gTrim = (gmMin == 1u) ? 0u : 1u;
                }

                /* Calculate feedback resistor trim */
                if (freq > CY_SYS_CLK_ECO_FREQ_FOR_FTRIM0)
                {
                    rTrim = CY_SYS_CLK_ECO_FTRIM0;
                }
                else if (freq > CY_SYS_CLK_ECO_FREQ_FOR_FTRIM1)
                {
                    rTrim = CY_SYS_CLK_ECO_FTRIM1;
                }
                else if (freq > CY_SYS_CLK_ECO_FREQ_FOR_FTRIM2)
                {
                    rTrim = CY_SYS_CLK_ECO_FTRIM2;
                }
                else
                {
                    rTrim = CY_SYS_CLK_ECO_FTRIM3;
                }

                /* Calculate filter frequency trim */
                fTrim = rTrim;

                CySysClkConfigureEcoTrim(wDTrim, aTrim, fTrim, rTrim, gTrim);

                /* Automatic Gain Control (AGC) enable */
                if (maxAmplitude < 2u)
                {
                    /* The oscillation amplitude is controlled to the level selected by amplitude trim */
                    CY_SYS_CLK_ECO_CONFIG_REG |= CY_SYS_CLK_ECO_CONFIG_AGC_EN;
                }
                else
                {
                    /* The amplitude is not explicitly controlled and will grow until it saturates to the
                    * supply rail (1.8V nom).
                    */
                    CY_SYS_CLK_ECO_CONFIG_REG &= (uint32) ~CY_SYS_CLK_ECO_CONFIG_AGC_EN;
                }
            }

            return (returnStatus);
        }

    #endif /* CY_IP_ECO_BLESS */

#endif /* (CY_IP_ECO) */


#if (CY_IP_SRSSV2 && CY_IP_PLL)
    /*******************************************************************************
    * Function Name: CySysClkPllStart
    ********************************************************************************
    *
    * Summary:
    *  Enables the PLL. Optionally waits for it to become stable. Waits at least
    *  250 us or until it is detected that the PLL is stable.
    *
    *  Clears the unlock occurred status bit by calling CySysClkPllGetUnlockStatus(),
    *  once the PLL is locked if the wait parameter is 1).
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *  0   PLL#0
    *  1   PLL#1
    *
    *  wait:
    *  0 - Return immediately after configuration.
    *  1 - Wait for PLL lock or timeout. This API shall use the CyDelayUs() to
    *  implement the timeout feature.
    *
    * Return:
    *  CYRET_SUCCESS - Completed successfully.
    *
    *  CYRET_TIMEOUT - The timeout occurred without detecting a stable clock.  If the
    *  input source of the clock is jittery, then the lock indication may not occur.
    *  However, after the timeout has expired, the generated PLL clock can still be
    *  used.
    *
    *  CYRET_BAD_PARAM - Either the PLL or wait parameter is invalid.
    *
    *******************************************************************************/
    cystatus CySysClkPllStart(uint32 pll, uint32 wait)
    {
        uint32 counts = CY_SYS_CLK_PLL_MAX_STARTUP_US;
        uint8  interruptState;
        cystatus returnStatus = CYRET_SUCCESS;

        if((pll < CY_IP_PLL_NR) && (wait <= 1u))
        {
            interruptState = CyEnterCriticalSection();

            /* Isolate PLL outputs */
            CY_SYS_CLK_PLL_BASE.pll[pll].config &= (uint32) ~CY_SYS_CLK_PLL_CONFIG_ISOLATE;

            /* Enable PLL */
            CY_SYS_CLK_PLL_BASE.pll[pll].config |= CY_SYS_CLK_PLL_CONFIG_ENABLE;

            CyExitCriticalSection(interruptState);

            /* De-isolate >= CY_SYS_CLK_PLL_MIN_STARTUP_US after PLL enabled */
            CyDelayUs(CY_SYS_CLK_PLL_MIN_STARTUP_US);
            interruptState = CyEnterCriticalSection();
            CY_SYS_CLK_PLL_BASE.pll[pll].config |= CY_SYS_CLK_PLL_CONFIG_ISOLATE;
            CyExitCriticalSection(interruptState);

            if(wait != 0u)
            {
                returnStatus = CYRET_TIMEOUT;

                while(0u != counts)
                {

                    if(0u != CySysClkPllGetLockStatus(pll))
                    {
                        returnStatus = CYRET_SUCCESS;
                        (void) CySysClkPllGetUnlockStatus(pll);
                        break;
                    }

                    CyDelayUs(1u);
                    counts--;
                }
            }
        }
        else
        {
            returnStatus = CYRET_BAD_PARAM;
        }

        return (returnStatus);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllGetLockStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns non-zero if the output of the specified PLL output is locked.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters: PLL:
    *  0   PLL#0
    *  1   PLL#1
    *
    * Return:
    *  A non-zero value when the specified PLL is locked.
    *
    *******************************************************************************/
    uint32 CySysClkPllGetLockStatus(uint32 pll)
    {
        uint8  interruptState;
        uint32 returnStatus;

        CYASSERT(pll < CY_IP_PLL_NR);

        interruptState = CyEnterCriticalSection();

        /* PLL is locked if reported so for two consecutive read. */
        returnStatus = CY_SYS_CLK_PLL_BASE.pll[pll].status & CY_SYS_CLK_PLL_STATUS_LOCKED;
        if(0u != returnStatus)
        {
            returnStatus = CY_SYS_CLK_PLL_BASE.pll[pll].status & CY_SYS_CLK_PLL_STATUS_LOCKED;
        }

        CyExitCriticalSection(interruptState);

        return (returnStatus);
    }

    /*******************************************************************************
    * Function Name: CySysClkPllStop
    ********************************************************************************
    *
    * Summary:
    *  Disables the PLL and isolates its outputs.
    *
    *  Ensures that either PLL is not the source of HFCLK before it is disabled,
    *  otherwise, the CPU will halt.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters: PLL:
    *  0   PLL#0
    *  1   PLL#1
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkPllStop(uint32 pll)
    {
        uint8  interruptState;

        if (pll < CY_IP_PLL_NR)
        {
            interruptState = CyEnterCriticalSection();
            CY_SYS_CLK_PLL_BASE.pll[pll].config &= (uint32) ~(CY_SYS_CLK_PLL_CONFIG_ISOLATE | CY_SYS_CLK_PLL_CONFIG_ENABLE);
            CyExitCriticalSection(interruptState);
        }
    }


    /*******************************************************************************
    * Function Name: CySysClkPllSetPQ
    ********************************************************************************
    *
    * Summary:
    *  Sets feedback (P) and reference the (Q) divider value. This API also sets the
    *  programmable charge pump current value. Note that the PLL has to be disabled
    *  before calling this API. If this function is called while any PLL is sourcing,
    *  the SYSCLK will return an error.
    *
    *  The PLL must not be the system clock source when calling this function. The
    *  PLL output will glitch during this function call.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *   0   PLL#0
    *   1   PLL#1
    *
    *  feedback: P divider
    *   Range 4 - 259. Control bits for the feedback divider
    *
    *  reference: The Q divider.
    *   Range 1 - 64. Divide by the reference.
    *
    *  current: Charge the pump current in uA. The 2 uA for output frequencies
    *  of 67 MHz or less, and 3 uA for higher output frequencies. The default
    *  value is 2 uA.
    *
    * Return:
    *  CYRET_SUCCESS - Completed successfully.
    *  CYRET_BAD_PARAM - The parameters are out of range or the specified PLL sources
    *  the system clock.
    *
    *******************************************************************************/
    cystatus CySysClkPllSetPQ(uint32 pll, uint32 feedback, uint32 reference, uint32 current)
    {
        uint32   regTmp;
        cystatus tmp;
        uint8    interruptState;
        cystatus returnStatus = CYRET_BAD_PARAM;

        interruptState = CyEnterCriticalSection();

        tmp = CySysClkPllConfigChangeAllowed(pll);

        if ((pll < CY_IP_PLL_NR) &&
            (feedback  >= CY_SYS_CLK_PLL_CONFIG_FEEDBACK_DIV_MIN)  && (feedback  <= CY_SYS_CLK_PLL_CONFIG_FEEDBACK_DIV_MAX)  &&
            (reference >= CY_SYS_CLK_PLL_CONFIG_REFERENCE_DIV_MIN) && (reference <= CY_SYS_CLK_PLL_CONFIG_REFERENCE_DIV_MAX) &&
            (current   >= CY_SYS_CLK_PLL_CONFIG_ICP_SEL_MIN )      && (current   <= CY_SYS_CLK_PLL_CONFIG_ICP_SEL_MAX) &&
            (CYRET_SUCCESS == tmp))
        {
            /* Set new feedback, reference and current values */
            regTmp  = CY_SYS_CLK_PLL_BASE.pll[pll].config & (uint32) ~(CY_SYS_CLK_PLL_CONFIG_FEEDBACK_DIV_MASK  |
                                                                       CY_SYS_CLK_PLL_CONFIG_REFERENCE_DIV_MASK |
                                                                       CY_SYS_CLK_PLL_CONFIG_ICP_SEL_MASK);

            regTmp |= ((feedback << CY_SYS_CLK_PLL_CONFIG_FEEDBACK_DIV_SHIFT) & CY_SYS_CLK_PLL_CONFIG_FEEDBACK_DIV_MASK);
            regTmp |= (((reference - 1u) << CY_SYS_CLK_PLL_CONFIG_REFERENCE_DIV_SHIFT) & CY_SYS_CLK_PLL_CONFIG_REFERENCE_DIV_MASK);
            regTmp |= ((current << CY_SYS_CLK_PLL_CONFIG_ICP_SEL_SHIFT) & CY_SYS_CLK_PLL_CONFIG_ICP_SEL_MASK);

            CY_SYS_CLK_PLL_BASE.pll[pll].config = regTmp;

            returnStatus = CYRET_SUCCESS;
        }

        CyExitCriticalSection(interruptState);

        return (returnStatus);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllSetBypassMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the bypass mode for the specified PLL.
    *
    *  The PLL must not be the system clock source when calling this function.
    *  The PLL output will glitch during this function call.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *   0   PLL#0
    *   1   PLL#1
    *
    *  bypass: The bypass mode.
    *   CY_SYS_PLL_BYPASS_AUTO - Automatic usage of the lock indicator. When unlocked,
    *   automatically selects PLL the reference input (bypass mode). When locked,
    *   automatically selects the PLL output.
    *
    *   CY_SYS_PLL_BYPASS_PLL_REF - Selects the PLL reference input (bypass mode).
    *   Ignores the lock indicator.
    *
    *   CY_SYS_PLL_BYPASS_PLL_OUT - Selects the PLL output.  Ignores the lock indicator.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkPllSetBypassMode(uint32 pll, uint32 bypass)
    {
        uint32 regTmp;
        uint8  interruptState;

        interruptState = CyEnterCriticalSection();

        if ((pll < CY_IP_PLL_NR) && (bypass <= CY_SYS_PLL_BYPASS_PLL_OUT))
        {
            regTmp  = CY_SYS_CLK_PLL_BASE.pll[pll].config & (uint32) ~CY_SYS_CLK_PLL_CONFIG_BYPASS_SEL_MASK;
            regTmp |=  (uint32)(bypass << CY_SYS_CLK_PLL_CONFIG_BYPASS_SEL_SHIFT);
            CY_SYS_CLK_PLL_BASE.pll[pll].config = regTmp;
        }

        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllGetBypassMode
    ********************************************************************************
    *
    * Summary:
    *  Gets the bypass mode for the specified PLL.
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *   0   PLL#0
    *   1   PLL#1
    *
    *  Return
    *  bypass: Bypass mode.
    *   The same as the parameter of the CySysClkPllSetBypassMode().
    *
    *******************************************************************************/
    static uint32 CySysClkPllGetBypassMode(uint32 pll)
    {
        uint32 returnValue;
        uint8  interruptState;

        CYASSERT(pll < CY_IP_PLL_NR);

        interruptState = CyEnterCriticalSection();

        returnValue = CY_SYS_CLK_PLL_BASE.pll[pll].config & CY_SYS_CLK_PLL_CONFIG_BYPASS_SEL_MASK;
        returnValue =  returnValue >> CY_SYS_CLK_PLL_CONFIG_BYPASS_SEL_SHIFT;

        CyExitCriticalSection(interruptState);

        return (returnValue);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllConfigChangeAllowed
    ********************************************************************************
    *
    * Summary:
    *  The function returns non-zero value if the specified PLL sources the System
    *  clock and the PLL is not in the bypass mode.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *   0   PLL#0
    *   1   PLL#1
    *
    * Return:
    *  Non-zero value when the specified PLL sources the System clock and the PLL
    *  is not in the bypass mode.
    *
    *******************************************************************************/
    static cystatus CySysClkPllConfigChangeAllowed(uint32 pll)
    {
        uint32   pllBypassMode;
        uint32   sysclkSource;
        cystatus returnValue = CYRET_LOCKED;

        sysclkSource  = CySysClkGetSysclkSource();
        pllBypassMode = CySysClkPllGetBypassMode(pll);

        if ((CY_SYS_PLL_BYPASS_PLL_REF == pllBypassMode)             ||
            ((CY_SYS_CLK_HFCLK_PLL0 != sysclkSource) && (0u == pll)) ||
            ((CY_SYS_CLK_HFCLK_PLL1 != sysclkSource) && (1u == pll)))
        {
            returnValue = CYRET_SUCCESS;
        }

        return (returnValue);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllGetUnlockStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns a non-zero value if the specified PLL output was unlocked.
    *  The unlock status is an indicator that the PLL has lost a lock at least once
    *  during its operation. The unlock status is cleared once it is read using
    *  this API.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *   0   PLL#0
    *   1   PLL#1
    *
    * Return:
    *  Non-zero value when the specified PLL was unlocked.
    *
    *******************************************************************************/
    uint32 CySysClkPllGetUnlockStatus(uint32 pll)
    {
        uint32 returnStatus = 0u;
        uint8  interruptState;

        interruptState = CyEnterCriticalSection();

        returnStatus = CY_SYS_CLK_PLL_BASE.pll[pll].test & CY_SYS_CLK_PLL_TEST_UNLOCK_OCCURRED_MASK;
        CY_SYS_CLK_PLL_BASE.pll[pll].test |= CY_SYS_CLK_PLL_TEST_UNLOCK_OCCURRED_MASK;

        CyExitCriticalSection(interruptState);

        return (returnStatus);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllSetFrequency
    ********************************************************************************
    *
    * Summary:
    *  Configures either PLL#0 or PLL#1 for the requested input/output frequencies.
    *  The input frequency is the frequency of the source to the PLL. The source is
    *  set using the CySysClkPllSetSource() function.
    *
    *  The PLL must not be the system clock source when calling this function. The
    *  PLL output will glitch during this function call.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *   0   PLL#0
    *   1   PLL#1
    *
    *  inputFreq:
    *   The reference frequency in KHz. The valid range is from 1000 to 49152 KHz.
    *
    *  pllFreq:
    *   The target frequency in KHz. The valid range is from 22500 to 49152 KHz.
    *
    * divider:
    *  The output clock divider for the PLL:
    *  CY_SYS_PLL_OUTPUT_DIVPASS    Pass Through
    *  CY_SYS_PLL_OUTPUT_DIV2       Divide by 2
    *  CY_SYS_PLL_OUTPUT_DIV4       Divide by 4
    *  CY_SYS_PLL_OUTPUT_DIV8       Divide by 8
    *
    * freqTol:
    *  The tolerance in ppm, 10 ppm is equal to 0.001%.
    *
    * Return:
    *  CYRET_SUCCESS - The PLL was successfully configured for the requested
    *  frequency.
    *
    *  CYRET_BAD_PARAM - The PLL was not able to successfully configure for the
    *  requested frequency.
    *
    *******************************************************************************/
    cystatus CySysClkPllSetFrequency(uint32 pll, uint32 inputFreq, uint32 pllFreq, uint32 divider, uint32 freqTol)
    {
        uint32 qMin;
        uint32 qMax;

        uint32 qVal = CY_SYS_CLK_PLL_INVALID;
        uint32 pVal = CY_SYS_CLK_PLL_INVALID;

        uint32 q;
        uint32 p;

        uint32 fvco;
        int32 ferr;

        cystatus tmp;
        cystatus returnStatus = CYRET_BAD_PARAM;


        tmp = CySysClkPllConfigChangeAllowed(pll);

        if ((pll < CY_IP_PLL_NR) &&
            (inputFreq  >= CY_SYS_CLK_PLL_INPUT_FREQ_MIN )  && (inputFreq  <= CY_SYS_CLK_PLL_INPUT_FREQ_MAX) &&
            (pllFreq >= CY_SYS_CLK_PLL_OUTPUT_FREQ_MIN ) && (pllFreq <= CY_SYS_CLK_PLL_OUTPUT_FREQ_MAX) &&
            (divider <= CY_SYS_PLL_OUTPUT_DIV8) &&
            (CYRET_SUCCESS == tmp))
        {

            /* Minimum feed forward loop divisor */
            qMin = (inputFreq + (CY_SYS_CLK_PLL_FPFDMAX - 1u)) / CY_SYS_CLK_PLL_FPFDMAX;
            qMin = (qMin < CY_SYS_CLK_PLL_QMINIP) ? CY_SYS_CLK_PLL_QMINIP : qMin;

            /* Maximum feed forward loop divisor */
            qMax = inputFreq / CY_SYS_CLK_PLL_FPFDMIN;
            qMax = (qMax > CY_SYS_CLK_PLL_QMAXIP) ? CY_SYS_CLK_PLL_QMAXIP : qMax;

            if (qMin <= qMax)
            {
                for(q = qMin; q <= qMax; q++)
                {
                    /* Solve for the feedback divisor value */

                    /* INT((pllFreq * q ) / inputFreq), where INT is normal rounding  */
                    p = ((pllFreq * q) + (inputFreq / 2u)) / inputFreq;

                    /* Calculate the actual VCO frequency (FVCO) */
                    fvco = ((inputFreq * p) / q);

                    /* Calculate the frequency error (FERR) */
                    ferr = ((1000000 * ((int32) fvco - (int32) pllFreq))/ (int32) pllFreq);

                    /* Bound check the frequency error and decide next action */
                    if ((( -1 * (int32) freqTol) <= ferr) && (ferr <= (int32) freqTol))
                    {
                        qVal = q;
                        pVal = p;
                        break;
                    }
                }


                if ((pVal != CY_SYS_CLK_PLL_INVALID) && (qVal != CY_SYS_CLK_PLL_INVALID))
                {
                    if (CySysClkPllSetPQ(pll, pVal, qVal, CY_SYS_CLK_PLL_CURRENT_DEFAULT) == CYRET_SUCCESS)
                    {
                        returnStatus = CySysClkPllSetOutputDivider(pll, divider);
                    }
                }
            }

        }

        return (returnStatus);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllSetSource
    ********************************************************************************
    *
    * Summary:
    *  Sets the input clock source to the PLL. The PLL must not be the system clock
    *  source when calling this function. The PLL output will glitch during this
    *  function call.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *  0   PLL#0
    *  1   PLL#1
    *
    * source:
    *  CY_SYS_PLL_SOURCE_IMO        IMO
    *  CY_SYS_PLL_SOURCE_EXTCLK     External Clock
    *  CY_SYS_PLL_SOURCE_ECO        ECO
    *  CY_SYS_PLL_SOURCE_DSI0       DSI_OUT[0]
    *  CY_SYS_PLL_SOURCE_DSI1       DSI_OUT[1]
    *  CY_SYS_PLL_SOURCE_DSI2       DSI_OUT[2]
    *  CY_SYS_PLL_SOURCE_DSI3       DSI_OUT[3]
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysClkPllSetSource(uint32 pll, uint32 source)
    {
        uint32 regTmp;
        uint8  interruptState;

        interruptState = CyEnterCriticalSection();

        if (pll < CY_IP_PLL_NR)
        {
            regTmp = CY_SYS_CLK_SELECT_REG & (uint32) ~CY_SYS_CLK_SELECT_PLL_MASK(pll);
            regTmp |= ((source << CY_SYS_CLK_SELECT_PLL_SHIFT(pll)) & CY_SYS_CLK_SELECT_PLL_MASK(pll));
            CY_SYS_CLK_SELECT_REG = regTmp;
        }

        CyExitCriticalSection(interruptState);
    }


    /*******************************************************************************
    * Function Name: CySysClkPllSetOutputDivider
    ********************************************************************************
    *
    * Summary:
    *  Sets the output clock divider for the PLL.
    *
    *  The PLL must not be the System Clock source when calling this function. The
    *  PLL output will glitch during this function call.
    *
    *  This API is available only for PSoC 4200L devices.
    *
    * Parameters:
    *  PLL:
    *   0   PLL#0
    *   1   PLL#1
    *
    * divider:
    *  CY_SYS_PLL_OUTPUT_DIVPASS    Pass through
    *  CY_SYS_PLL_OUTPUT_DIV2       Divide by 2
    *  CY_SYS_PLL_OUTPUT_DIV4       Divide by 4
    *  CY_SYS_PLL_OUTPUT_DIV8       Divide by 8
    *
    * Return:
    *  CYRET_SUCCESS   - Completed successfully.
    *  CYRET_BAD_PARAM - The parameters are out of range or the specified PLL
    *                    sources the System clock.
    *
    *******************************************************************************/
    cystatus CySysClkPllSetOutputDivider(uint32 pll, uint32 divider)
    {
        uint32   tmpReg;
        uint8    interruptState;
        cystatus returnStatus = CYRET_BAD_PARAM;
        cystatus tmp;


        interruptState = CyEnterCriticalSection();

        tmp = CySysClkPllConfigChangeAllowed(pll);

        if ((pll < CY_IP_PLL_NR) && (CYRET_SUCCESS == tmp) && (divider <= CY_SYS_PLL_OUTPUT_DIV8))
        {
            tmpReg  = CY_SYS_CLK_PLL_BASE.pll[pll].config & (uint32) ~(CY_SYS_CLK_PLL_CONFIG_OUTPUT_DIV_MASK);
            tmpReg |= ((divider << CY_SYS_CLK_PLL_CONFIG_OUTPUT_DIV_SHIFT) & CY_SYS_CLK_PLL_CONFIG_OUTPUT_DIV_MASK);

            CY_SYS_CLK_PLL_BASE.pll[pll].config = tmpReg;

            returnStatus = CYRET_SUCCESS;
        }

        CyExitCriticalSection(interruptState);

        return (returnStatus);
    }
#endif /* (CY_IP_SRSSV2 && CY_IP_PLL) */


#if(CY_IP_SRSSV2)

    /*******************************************************************************
    * Function Name: CySysLvdEnable
    ********************************************************************************
    *
    * Summary:
    *  Enables the output of the low-voltage monitor when Vddd is at or below the
    *  trip point, configures the device to generate an interrupt, and sets the
    *  voltage trip level.
    *
    * Parameters:
    *  threshold: Threshold selection for Low Voltage Detect circuit.
    *  Threshold variation is +/- 2.5% from these typical voltage choices.
    *         Define                     Voltage threshold
    *    CY_LVD_THRESHOLD_1_75_V       1.7500 V
    *    CY_LVD_THRESHOLD_1_80_V       1.8000 V
    *    CY_LVD_THRESHOLD_1_90_V       1.9000 V
    *    CY_LVD_THRESHOLD_2_00_V       2.0000 V
    *    CY_LVD_THRESHOLD_2_10_V       2.1000 V
    *    CY_LVD_THRESHOLD_2_20_V       2.2000 V
    *    CY_LVD_THRESHOLD_2_30_V       2.3000 V
    *    CY_LVD_THRESHOLD_2_40_V       2.4000 V
    *    CY_LVD_THRESHOLD_2_50_V       2.5000 V
    *    CY_LVD_THRESHOLD_2_60_V       2.6000 V
    *    CY_LVD_THRESHOLD_2_70_V       2.7000 V
    *    CY_LVD_THRESHOLD_2_80_V       2.8000 V
    *    CY_LVD_THRESHOLD_2_90_V       2.9000 V
    *    CY_LVD_THRESHOLD_3_00_V       3.0000 V
    *    CY_LVD_THRESHOLD_3_20_V       3.2000 V
    *    CY_LVD_THRESHOLD_4_50_V       4.5000 V
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysLvdEnable(uint32 threshold)
    {
        /* Prevent propagating a false interrupt */
        CY_LVD_PWR_INTR_MASK_REG   &= (uint32) ~CY_LVD_PROPAGATE_INT_TO_CPU;

        /* Set specified threshold */
        CY_LVD_PWR_VMON_CONFIG_REG = (CY_LVD_PWR_VMON_CONFIG_REG & ~CY_LVD_PWR_VMON_CONFIG_LVD_SEL_MASK) |
                ((threshold << CY_LVD_PWR_VMON_CONFIG_LVD_SEL_SHIFT) & CY_LVD_PWR_VMON_CONFIG_LVD_SEL_MASK);

        /* Enable the LVD. This may cause a false LVD event. */
        CY_LVD_PWR_VMON_CONFIG_REG |= CY_LVD_PWR_VMON_CONFIG_LVD_EN;

        /* Wait for the circuit to stabilize */
        CyDelayUs(CY_LVD_STABILIZE_TIMEOUT_US);

        /* Clear the false event */
        CySysLvdClearInterrupt();

        /* Unmask the interrupt */
        CY_LVD_PWR_INTR_MASK_REG   |= CY_LVD_PROPAGATE_INT_TO_CPU;
    }


    /*******************************************************************************
    * Function Name: CySysLvdDisable
    ********************************************************************************
    *
    * Summary:
    *  Disables the low voltage detection. A low voltage interrupt is disabled.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysLvdDisable(void)
    {
        CY_LVD_PWR_INTR_MASK_REG   &= ~CY_LVD_PROPAGATE_INT_TO_CPU;
        CY_LVD_PWR_VMON_CONFIG_REG &= ~CY_LVD_PWR_VMON_CONFIG_LVD_EN;
    }


    /*******************************************************************************
    * Function Name: CySysLvdGetInterruptSource
    ********************************************************************************
    *
    * Summary:
    *  Gets the low voltage detection interrupt status (without clearing).
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Interrupt request value:
    *    CY_SYS_LVD_INT - Indicates an Low Voltage Detect interrupt
    *
    *******************************************************************************/
    uint32 CySysLvdGetInterruptSource(void)
    {
        return (CY_LVD_PWR_INTR_REG & CY_SYS_LVD_INT);
    }


    /*******************************************************************************
    * Function Name: CySysLvdClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears the low voltage detection interrupt status.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysLvdClearInterrupt(void)
    {
        CY_LVD_PWR_INTR_REG = CY_SYS_LVD_INT;
    }

#endif /* (CY_IP_SRSSV2) */


/*******************************************************************************
* Function Name: CySysGetResetReason
********************************************************************************
*
* Summary:
*  Reports the cause for the latest reset(s) that occurred in the system. All
*  the bits in the RES_CAUSE register assert when the corresponding reset cause
*  occurs and must be cleared by the firmware. These bits are cleared by the
*  hardware only during XRES, POR, or a detected brown-out.
*
* Parameters:
*  reason: bits in the RES_CAUSE register to clear.
*   CY_SYS_RESET_WDT       - WDT caused a reset
*   CY_SYS_RESET_PROTFAULT - Occured protection violation that requires reset
*   CY_SYS_RESET_SW        - Cortex-M0 requested a system reset.
*
* Return:
*  Status. Same enumerated bit values as used for the reason parameter.
*
*******************************************************************************/
uint32 CySysGetResetReason(uint32 reason)
{
    uint32 returnStatus;

    reason &= (CY_SYS_RESET_WDT | CY_SYS_RESET_PROTFAULT | CY_SYS_RESET_SW);
    returnStatus = CY_SYS_RES_CAUSE_REG &
                  (CY_SYS_RESET_WDT | CY_SYS_RESET_PROTFAULT | CY_SYS_RESET_SW);
    CY_SYS_RES_CAUSE_REG = reason;

    return (returnStatus);
}


/*******************************************************************************
* Function Name: CyDisableInts
********************************************************************************
*
* Summary:
*  Disables all interrupts.
*
* Parameters:
*  None
*
* Return:
*  32 bit mask of previously enabled interrupts.
*
*******************************************************************************/
uint32 CyDisableInts(void)
{
    uint32 intState;

    /* Get current interrupt state. */
    intState = CY_INT_CLEAR_REG;

    /* Disable all interrupts. */
    CY_INT_CLEAR_REG = CY_INT_CLEAR_DISABLE_ALL;

    return (intState);
}


/*******************************************************************************
* Function Name: CyEnableInts
********************************************************************************
*
* Summary:
*  Enables interrupts to a given state.
*
* Parameters:
*   mask, 32 bit mask of interrupts to enable.
*
* Return:
*  None
*
*******************************************************************************/
void CyEnableInts(uint32 mask)
{
    CY_INT_ENABLE_REG = mask;
}


/*******************************************************************************
* Function Name: CyIntSetSysVector
********************************************************************************
*
* Summary:
*  Sets the interrupt vector of the specified system interrupt number. These
*  interrupts are for SysTick, PendSV and others.
*
* Parameters:
*  number: System interrupt number:
*    CY_INT_NMI_IRQN        - Non Maskable Interrupt
*    CY_INT_HARD_FAULT_IRQN - Hard Fault Interrupt
*    CY_INT_SVCALL_IRQN     - SV Call Interrupt
*    CY_INT_PEND_SV_IRQN    - Pend SV Interrupt
*    CY_INT_SYSTICK_IRQN    - System Tick Interrupt
*
*  address: Pointer to an interrupt service routine.
*
* Return:
*  The old ISR vector at this location.
*
*******************************************************************************/
cyisraddress CyIntSetSysVector(uint8 number, cyisraddress address)
{
    cyisraddress oldIsr;
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_INT_IRQ_BASE);

    /* Save old Interrupt service routine. */
    oldIsr = ramVectorTable[number];

    /* Set new Interrupt service routine. */
    ramVectorTable[number] = address;

    return(oldIsr);
}


/*******************************************************************************
* Function Name: CyIntGetSysVector
********************************************************************************
*
* Summary:
*   Gets the interrupt vector of the specified system interrupt number. These
*   interrupts are for SysTick, PendSV and others.
*
* Parameters:
*  number: System interrupt number:
*    CY_INT_NMI_IRQN        - Non Maskable Interrupt
*    CY_INT_HARD_FAULT_IRQN - Hard Fault Interrupt
*    CY_INT_SVCALL_IRQN     - SV Call Interrupt
*    CY_INT_PEND_SV_IRQN    - Pend SV Interrupt
*    CY_INT_SYSTICK_IRQN    - System Tick Interrupt
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress CyIntGetSysVector(uint8 number)
{
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_INT_IRQ_BASE);

    return(ramVectorTable[number]);
}


/*******************************************************************************
* Function Name: CyIntSetVector
********************************************************************************
*
* Summary:
*  Sets the interrupt vector of the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number
*  address: Pointer to an interrupt service routine
*
* Return:
*   Previous interrupt vector value.
*
*******************************************************************************/
cyisraddress CyIntSetVector(uint8 number, cyisraddress address)
{
    cyisraddress oldIsr;
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_NUM_INTERRUPTS);

    /* Save old Interrupt service routine. */
    oldIsr = ramVectorTable[CY_INT_IRQ_BASE + number];

    /* Set new Interrupt service routine. */
    ramVectorTable[CY_INT_IRQ_BASE + number] = address;

    return(oldIsr);
}


/*******************************************************************************
* Function Name: CyIntGetVector
********************************************************************************
*
* Summary:
*  Gets the interrupt vector of the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number
*
* Return:
*  Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress CyIntGetVector(uint8 number)
{
    cyisraddress *ramVectorTable = (cyisraddress *) CY_INT_VECT_TABLE;

    CYASSERT(number < CY_NUM_INTERRUPTS);

    return (ramVectorTable[CY_INT_IRQ_BASE + number]);
}

/*******************************************************************************
* Function Name: CyIntSetPriority
********************************************************************************
*
* Summary:
*  Sets the priority of the interrupt.
*
* Parameters:
*  priority: Priority of the interrupt. 0 - 3, 0 being the highest.
*  number: The number of the interrupt, 0 - 31.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntSetPriority(uint8 number, uint8 priority)
{
    uint8 interruptState;
    uint32 shift;
    uint32 value;

    CYASSERT(priority <= CY_MIN_PRIORITY);
    CYASSERT(number < CY_NUM_INTERRUPTS);

    shift = CY_INT_PRIORITY_SHIFT(number);

    interruptState = CyEnterCriticalSection();

    value = CY_INT_PRIORITY_REG(number);
    value &= (uint32)(~((uint32)(CY_INT_PRIORITY_MASK << shift)));
    value |= ((uint32)priority << shift);
    CY_INT_PRIORITY_REG(number) = value;

    CyExitCriticalSection(interruptState);
}


/*******************************************************************************
* Function Name: CyIntGetPriority
********************************************************************************
*
* Summary:
*  Gets the priority of the interrupt.
*
* Parameters:
*  number: The number of the interrupt, 0 - 31.
*
* Return:
*  Priority of the interrupt. 0 - 3, 0 being the highest.
*
*******************************************************************************/
uint8 CyIntGetPriority(uint8 number)
{
    uint8 priority;

    CYASSERT(number < CY_NUM_INTERRUPTS);

    priority = (uint8) (CY_INT_PRIORITY_REG(number) >> CY_INT_PRIORITY_SHIFT(number));

    return (priority & (uint8) CY_INT_PRIORITY_MASK);
}


/*******************************************************************************
* Function Name: CyIntEnable
********************************************************************************
*
* Summary:
*  Enables the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number
*
* Return:
*  None
*
*******************************************************************************/
void CyIntEnable(uint8 number)
{
    CY_INT_ENABLE_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}


/*******************************************************************************
* Function Name: CyIntGetState
********************************************************************************
*
* Summary:
*  Gets the enable state of the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number.
*
* Return:
*  Enable status: 1 if enabled, 0 if disabled
*
*******************************************************************************/
uint8 CyIntGetState(uint8 number)
{
    /* Get state of interrupt. */
    return ((0u != (CY_INT_ENABLE_REG & ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number)))) ? 1u : 0u);
}


/*******************************************************************************
* Function Name: CyIntDisable
********************************************************************************
*
* Summary:
*   Disables the specified interrupt number.
*
* Parameters:
*   number: Valid range [0-31].  Interrupt number.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntDisable(uint8 number)
{
    CY_INT_CLEAR_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}

/*******************************************************************************
* Function Name: CyIntSetPending
********************************************************************************
*
* Summary:
*  Forces the specified interrupt number to be pending.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntSetPending(uint8 number)
{
    CY_INT_SET_PEND_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}


/*******************************************************************************
* Function Name: CyIntClearPending
********************************************************************************
*
* Summary:
*  Clears any pending interrupt for the specified interrupt number.
*
* Parameters:
*  number: Valid range [0-31].  Interrupt number.
*
* Return:
*  None
*
*******************************************************************************/
void CyIntClearPending(uint8 number)
{
    CY_INT_CLR_PEND_REG = ((uint32) 0x01u << (CY_INT_ENABLE_RANGE_MASK & number));
}


/*******************************************************************************
* Function Name: CyHalt
********************************************************************************
*
* Summary:
*  Halts the CPU.
*
* Parameters:
*  reason: Value to be used during debugging.
*
* Return:
*  None
*
*******************************************************************************/
void CyHalt(uint8 reason)
{
    if(0u != reason)
    {
        /* To remove unreferenced local variable warning */
    }

    #if defined (__ARMCC_VERSION)
        __breakpoint(0x0);
    #elif defined(__GNUC__) || defined (__ICCARM__)
        __asm("    bkpt    1");
    #elif defined(__C51__)
        CYDEV_HALT_CPU;
    #endif  /* (__ARMCC_VERSION) */
}


/*******************************************************************************
* Function Name: CySoftwareReset
********************************************************************************
*
* Summary:
*  Forces a software reset of the device.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySoftwareReset(void)
{
    /***************************************************************************
    * Setting the system reset request bit. The vector key value must be written
    * to the register, otherwise the register write is unpredictable.
    ***************************************************************************/
    CY_SYS_CM0_AIRCR_REG = (CY_SYS_CM0_AIRCR_REG & (uint32)(~CY_SYS_CM0_AIRCR_VECTKEY_MASK)) |
                            CY_SYS_CM0_AIRCR_VECTKEY | CY_SYS_CM0_AIRCR_SYSRESETREQ;
}


/*******************************************************************************
* Function Name: CyDelay
********************************************************************************
*
* Summary:
*  Blocks for milliseconds.
*
* Parameters:
*  milliseconds: number of milliseconds to delay.
*
* Return:
*  None
*
*******************************************************************************/
void CyDelay(uint32 milliseconds)
{
    while (milliseconds > CY_DELAY_MS_OVERFLOW)
    {
        /* This loop prevents overflow.
         * At 100MHz, milliseconds * cydelayFreqKhz overflows at about 42 seconds
         */
        CyDelayCycles(cydelay32kMs);
        milliseconds -= CY_DELAY_MS_OVERFLOW;
    }

    CyDelayCycles(milliseconds * cydelayFreqKhz);
}


/*******************************************************************************
* Function Name: CyDelayUs
********************************************************************************
* Summary:
*  Blocks for microseconds.
*
* Parameters:
*  microseconds: number of microseconds to delay.
*
* Return:
*  None
*
*******************************************************************************/
void CyDelayUs(uint16 microseconds)
{
    CyDelayCycles((uint32)microseconds * cydelayFreqMhz);
}


/*******************************************************************************
* Function Name: CyDelayFreq
********************************************************************************
* Summary:
*  Sets clock frequency for CyDelay.
*
* Parameters:
*  freq: Frequency of bus clock in Hertz.
*
* Return:
*  None
*
*******************************************************************************/
void CyDelayFreq(uint32 freq)
{
    if (freq != 0u)
    {
        cydelayFreqHz = freq;
    }
    else
    {
        cydelayFreqHz = CYDEV_BCLK__SYSCLK__HZ;
    }

    cydelayFreqMhz = (uint8)((cydelayFreqHz + CY_DELAY_1M_MINUS_1_THRESHOLD) / CY_DELAY_1M_THRESHOLD);
    cydelayFreqKhz = (cydelayFreqHz + CY_DELAY_1K_MINUS_1_THRESHOLD) / CY_DELAY_1K_THRESHOLD;
    cydelay32kMs   = CY_DELAY_MS_OVERFLOW * cydelayFreqKhz;
}


/*******************************************************************************
* Function Name: CySysTickStart
********************************************************************************
*
* Summary:
*  Configures the SysTick timer to generate an interrupt every 1 ms and enables
*  the interrupt. Refer to the corresponding function description for details.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set.
*
*******************************************************************************/
void CySysTickStart(void)
{
    if (0u == CySysTickInitVar)
    {
        CySysTickInit();
        CySysTickInitVar = 1u;
    }

    CySysTickEnable();
}


/*******************************************************************************
* Function Name: CySysTickInit
********************************************************************************
*
* Summary:
    *  Initializes the callback addresses with pointers to NULL, associates the
    *  SysTick system vector with the function that is responsible for calling
    *  registered callback functions, configures SysTick timer to generate interrupt
    *  every 1 ms.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  Clears SysTick count flag if it was set.
    *
    *  The 1 ms interrupt interval is configured based on the frequency determined
    *  by PSoC Creator at build time. If System clock frequency is changed in
    *  runtime, the CyDelayFreq() with the appropriate parameter should be called.
    *
    *******************************************************************************/
    void CySysTickInit(void)
    {
        uint32 i;

    for (i = 0u; i<CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
    {
        CySysTickCallbacks[i] = (void *) 0;
    }

    (void) CyIntSetSysVector(CY_INT_SYSTICK_IRQN, &CySysTickServiceCallbacks);

    #if(CY_SYSTICK_LFCLK_SOURCE)
        CySysTickSetClockSource(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK);
    #endif /* (CY_SYSTICK_LFCLK_SOURCE) */

    CySysTickSetReload(cydelayFreqHz/1000u);
    CySysTickClear();
    CyIntEnable(CY_INT_SYSTICK_IRQN);
}


/*******************************************************************************
* Function Name: CySysTickEnable
********************************************************************************
*
* Summary:
*  Enables the SysTick timer and its interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickEnable(void)
{
    CySysTickEnableInterrupt();
    CY_SYS_SYST_CSR_REG |= CY_SYS_SYST_CSR_ENABLE;
}


/*******************************************************************************
* Function Name: CySysTickStop
********************************************************************************
*
* Summary:
*  Stops the system timer (SysTick).
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickStop(void)
{
    CY_SYS_SYST_CSR_REG &= ((uint32) ~(CY_SYS_SYST_CSR_ENABLE));
}


/*******************************************************************************
* Function Name: CySysTickEnableInterrupt
********************************************************************************
*
* Summary:
*  Enables the SysTick interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickEnableInterrupt(void)
{
    CY_SYS_SYST_CSR_REG |= CY_SYS_SYST_CSR_ENABLE_INT;
}


/*******************************************************************************
* Function Name: CySysTickDisableInterrupt
********************************************************************************
*
* Summary:
*  Disables the SysTick interrupt.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  Clears SysTick count flag if it was set
*
*******************************************************************************/
void CySysTickDisableInterrupt(void)
{
    CY_SYS_SYST_CSR_REG &= ((uint32) ~(CY_SYS_SYST_CSR_ENABLE_INT));
}


/*******************************************************************************
* Function Name: CySysTickSetReload
********************************************************************************
*
* Summary:
*  Sets value the counter is set to on startup and after it reaches zero. This
*  function do not change or reset current sysTick counter value, so it should
*  be cleared using CySysTickClear() API.
*
* Parameters:
*  value: Valid range [0x0-0x00FFFFFF]. Counter reset value
*
* Return:
*  None
*
*******************************************************************************/
void CySysTickSetReload(uint32 value)
{
    CY_SYS_SYST_RVR_REG = (value & CY_SYS_SYST_RVR_CNT_MASK);
}


/*******************************************************************************
* Function Name: CySysTickGetReload
********************************************************************************
*
* Summary:
*  Sets value the counter is set to on startup and after it reaches zero.
*
* Parameters:
*  None
*
* Return:
*  Counter reset value.
*
*******************************************************************************/
uint32 CySysTickGetReload(void)
{
    return(CY_SYS_SYST_RVR_REG & CY_SYS_SYST_RVR_CNT_MASK);
}


/*******************************************************************************
* Function Name: CySysTickGetValue
********************************************************************************
*
* Summary:
*  Gets current SysTick counter value.
*
* Parameters:
*  None
*
* Return:
*  Current SysTick counter value
*
*******************************************************************************/
uint32 CySysTickGetValue(void)
{
    return(CY_SYS_SYST_CVR_REG & CY_SYS_SYST_CVR_CNT_MASK);
}


#if(CY_SYSTICK_LFCLK_SOURCE)
    /*******************************************************************************
    * Function Name: CySysTickSetClockSource
    ********************************************************************************
    *
    * Summary:
    *  Sets the clock source for the SysTick counter.
    *
    *  Clears SysTick count flag if it was set. If clock source is not ready this
    *  function call will have no effect. After changing clock source to the low
    *  frequency clock the counter and reload register values will remain unchanged
    *  so time to the interrupt will be significantly longer and vice versa.
    *
    *  The function is not available on PSoC 4000, PSoC 4100, and PSoC 42000
    *  devices. The SysTick timer clocked by the System clock on these devices.
    *
    * Parameters:
    *  clockSource: Clock source for SysTick counter
    *         Define                     Clock Source
    *   CY_SYS_SYST_CSR_CLK_SRC_SYSCLK     SysTick is clocked by CPU clock.
    *   CY_SYS_SYST_CSR_CLK_SRC_LFCLK      SysTick is clocked by the low frequency
    *                                      clock. (ILO 100 KHz for PSoC 5LP, and
    *                                      LFCLK for PSoC 4).
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void CySysTickSetClockSource(uint32 clockSource)
    {
        if (clockSource == CY_SYS_SYST_CSR_CLK_SRC_SYSCLK)
        {
            CY_SYS_SYST_CSR_REG |= (uint32)(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK << CY_SYS_SYST_CSR_CLK_SOURCE_SHIFT);
        }
        else
        {
            CY_SYS_SYST_CSR_REG &= ((uint32) ~((uint32)(CY_SYS_SYST_CSR_CLK_SRC_SYSCLK << CY_SYS_SYST_CSR_CLK_SOURCE_SHIFT)));
        }
    }
#endif /* (CY_SYSTICK_LFCLK_SOURCE) */


/*******************************************************************************
* Function Name: CySysTickGetCountFlag
********************************************************************************
*
* Summary:
*  The count flag is set once SysTick counter reaches zero.
*   The flag cleared on read.
*
* Parameters:
*  None
*
* Return:
    *  Returns non-zero value if flag is set, otherwise zero is returned.
*
*
* Side Effects:
*  Clears SysTick count flag if it was set.
*
*******************************************************************************/
uint32 CySysTickGetCountFlag(void)
{
    return ((CY_SYS_SYST_CSR_REG>>CY_SYS_SYST_CSR_COUNTFLAG_SHIFT) & 0x01u);
}


/*******************************************************************************
* Function Name: CySysTickClear
********************************************************************************
*
* Summary:
*  Clears the SysTick counter for well-defined startup.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void CySysTickClear(void)
{
    CY_SYS_SYST_CVR_REG = 0u;
}


/*******************************************************************************
* Function Name: CySysTickSetCallback
********************************************************************************
*
* Summary:
*  This function allows up to five user-defined interrupt service routine
*  functions to be associated with the SysTick interrupt. These are specified
*  through the use of pointers to the function.
*
*  To set a custom callback function without the overhead of the system provided
*  one, use CyIntSetSysVector(CY_INT_SYSTICK_IRQN, cyisraddress <address>),
*  where <address> is address of the custom defined interrupt service routine.
*  Note: a custom callback function overrides the system defined callback
*  functions.
*
* Parameters:
*  number: The number of the callback function addresses to be set. The valid
*          range is from 0 to 4.
*
*  void(*CallbackFunction(void): A pointer to the function that will be
*                                associated with the SysTick ISR for the
*                                specified number.
*
* Return:
*  Returns the address of the previous callback function.
*  The NULL is returned if the specified address in not set.
*
* Side Effects:
*  The registered callback functions will be executed in the interrupt.
*
*******************************************************************************/
cySysTickCallback CySysTickSetCallback(uint32 number, cySysTickCallback function)
{
    cySysTickCallback retVal;

    retVal = CySysTickCallbacks[number];
    CySysTickCallbacks[number] = function;
    return (retVal);
}


/*******************************************************************************
* Function Name: CySysTickGetCallback
********************************************************************************
*
* Summary:
*  The function get the specified callback pointer.
*
* Parameters:
*  number: The number of callback function address to get. The valid
*          range is from 0 to 4.
*
* Return:
*  Returns the address of the specified callback function.
*  The NULL is returned if the specified address in not initialized.
*
*******************************************************************************/
cySysTickCallback CySysTickGetCallback(uint32 number)
{
    return ((cySysTickCallback) CySysTickCallbacks[number]);
}


/*******************************************************************************
* Function Name: CySysTickServiceCallbacks
********************************************************************************
*
* Summary:
*  System Tick timer interrupt routine
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
static void CySysTickServiceCallbacks(void)
{
    uint32 i;

    /* Verify that tick timer flag was set */
    if (1u == CySysTickGetCountFlag())
    {
        for (i=0u; i < CY_SYS_SYST_NUM_OF_CALLBACKS; i++)
        {
            if (CySysTickCallbacks[i] != (void *) 0)
            {
                (void)(CySysTickCallbacks[i])();
            }
        }
    }
}


/*******************************************************************************
* Function Name: CyGetUniqueId
********************************************************************************
*
* Summary:
*  Returns the 64-bit unique ID of the device. The uniqueness of the number is
*  guaranteed for 10 years due to the die lot number having a cycle life of 10
*  years and even after 10 years, the probability of getting two identical
*  numbers is very small.
*
* Parameters:
*  uniqueId: The pointer to a two element 32-bit unsigned integer array. Returns
*  the 64-bit unique ID of the device by loading them into the integer array
*  pointed to by uniqueId.
*
* Return:
*  None
*
*******************************************************************************/
void CyGetUniqueId(uint32* uniqueId)
{
#if(CY_PSOC4)
    uniqueId[0u]  =  (uint32)(* (reg8 *) CYREG_SFLASH_DIE_LOT0  );
    uniqueId[0u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_LOT1  ) <<  8u);
    uniqueId[0u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_LOT2  ) << 16u);
    uniqueId[0u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_WAFER ) << 24u);

    uniqueId[1u]  =  (uint32)(* (reg8 *) CYREG_SFLASH_DIE_X     );
    uniqueId[1u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_Y     ) <<  8u);
    uniqueId[1u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_SORT  ) << 16u);
    uniqueId[1u] |= ((uint32)(* (reg8 *) CYREG_SFLASH_DIE_MINOR ) << 24u);
#else
    uniqueId[0u]  =  (uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_LOT_LSB   ));
    uniqueId[0u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_LOT_MSB   )) <<  8u);
    uniqueId[0u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_MLOGIC_REV_ID                )) << 16u);
    uniqueId[0u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_WAFER_NUM )) << 24u);

    uniqueId[1u]  =  (uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_X_LOC     ));
    uniqueId[1u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_Y_LOC     )) <<  8u);
    uniqueId[1u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_WRK_WK    )) << 16u);
    uniqueId[1u] |= ((uint32) CY_GET_XTND_REG8((void CYFAR *) (CYREG_FLSHID_CUST_TABLES_FAB_YR    )) << 24u);
#endif  /* (CY_PSOC4) */
}


#if (CY_IP_DMAC_PRESENT)
    /*******************************************************************************
    * Function Name: CySysSetRamAccessArbPriority
    ****************************************************************************//**
    *
    * Summary:
    *  Sets RAM access priority between CPU and DMA. The RAM_CTL register is
    *  configured to set the priority. Please refer to the device TRM for more
    *  details.
    *
    *  This API is applicable for PSoC 4200M and PSoC 4200L devices only.
    *
    *  Parameters:
    *  source:
    *   CY_SYS_ARB_PRIORITY_CPU              CPU has priority (Default)
    *   CY_SYS_ARB_PRIORITY_DMA              DMA has priority
    *   CY_SYS_ARB_PRIORITY_ROUND            Round robin
    *   CY_SYS_ARB_PRIORITY_ROUND_STICKY     Round robin sticky
    *
    *******************************************************************************/
    void CySysSetRamAccessArbPriority(uint32 source)
    {
        uint32 regTmp;

        regTmp  = CY_SYS_CPUSS_RAM_CTL_REG & ~CY_SYS_CPUSS_RAM_CTL_ARB_MASK;
        regTmp |= ((uint32) (source << CY_SYS_CPUSS_RAM_CTL_ARB_SHIFT) & CY_SYS_CPUSS_RAM_CTL_ARB_MASK);
        CY_SYS_CPUSS_RAM_CTL_REG = regTmp;
    }


    /*******************************************************************************
    * Function Name: CySysSetFlashAccessArbPriority
    ****************************************************************************//**
    *
    * Summary:
    *  Sets flash access priority between CPU and DMA. The FLASH_CTL register is
    *  configured to set the priority. Please refer to the device TRM for more
    *  details.
    *
    *  This API is applicable for PSoC 4200M and PSoC 4200L devices only.
    *
    *  Parameters:
    *  source:
    *   CY_SYS_ARB_PRIORITY_CPU              CPU has priority (Default)
    *   CY_SYS_ARB_PRIORITY_DMA              DMA has priority
    *   CY_SYS_ARB_PRIORITY_ROUND            Round robin
    *   CY_SYS_ARB_PRIORITY_ROUND_STICKY     Round robin sticky
    *
    *******************************************************************************/
    void CySysSetFlashAccessArbPriority(uint32 source)
    {
        uint32 regTmp;

        regTmp  = CY_SYS_CPUSS_FLASH_CTL_REG & ~CY_SYS_CPUSS_FLASH_CTL_ARB_MASK;
        regTmp |= ((uint32) (source << CY_SYS_CPUSS_FLASH_CTL_ARB_SHIFT) & CY_SYS_CPUSS_FLASH_CTL_ARB_MASK);
        CY_SYS_CPUSS_FLASH_CTL_REG = regTmp;
    }


    /*******************************************************************************
    * Function Name: CySysSetDmacAccessArbPriority
    ****************************************************************************//**
    *
    * Summary:
    *  Sets DMAC slave interface access priority between CPU and DMA. The DMAC_CTL
    *  register is configured to set the priority. Please refer to the device TRM
    *  for more details.
    *
    *  This API is applicable for PSoC 4200M and PSoC 4200L devices only.
    *
    *  Parameters:
    *  source:
    *   CY_SYS_ARB_PRIORITY_CPU              CPU has priority (Default)
    *   CY_SYS_ARB_PRIORITY_DMA              DMA has priority
    *   CY_SYS_ARB_PRIORITY_ROUND            Round robin
    *   CY_SYS_ARB_PRIORITY_ROUND_STICKY     Round robin sticky
    *
    *******************************************************************************/
    void CySysSetDmacAccessArbPriority(uint32 source)
    {
        uint32 regTmp;

        regTmp  = CY_SYS_CPUSS_DMAC_CTL_REG & ~CY_SYS_CPUSS_DMAC_CTL_ARB_MASK;
        regTmp |= ((uint32) (source << CY_SYS_CPUSS_DMAC_CTL_ARB_SHIFT) & CY_SYS_CPUSS_DMAC_CTL_ARB_MASK);
        CY_SYS_CPUSS_DMAC_CTL_REG = regTmp;
    }


    /*******************************************************************************
    * Function Name: CySysSetPeripheralAccessArbPriority
    ****************************************************************************//**
    *
    * Summary:
    *  Sets slave peripheral interface access priority between CPU and DMA.
    *  The SL_CTL register is configured to set the priority. Please refer to the
    *  device TRM for more details.
    *
    *  This API is applicable for PSoC 4200M and PSoC 4200L devices only.
    *
    *  Parameters:
    *  interfaceNumber: the slave interface number. Please refer to the device TRM
    *  for more details.
    *
    *  source:
    *   CY_SYS_ARB_PRIORITY_CPU              CPU has priority (Default)
    *   CY_SYS_ARB_PRIORITY_DMA              DMA has priority
    *   CY_SYS_ARB_PRIORITY_ROUND            Round robin
    *   CY_SYS_ARB_PRIORITY_ROUND_STICKY     Round robin sticky
    *
    *******************************************************************************/
    void CySysSetPeripheralAccessArbPriority(uint32 interfaceNumber, uint32 source)
    {
        uint32 regTmp;

        if (interfaceNumber == 0u)
        {
            regTmp  = CY_SYS_CPUSS_SL_CTL0_REG & ~CY_SYS_CPUSS_SL_CTL_ARB_MASK;
            regTmp |= ((uint32) (source << CY_SYS_CPUSS_SL_CTL_ARB_SHIFT) & CY_SYS_CPUSS_SL_CTL_ARB_MASK);
            CY_SYS_CPUSS_SL_CTL0_REG = regTmp;
        } else
    #if (CY_IP_SL_NR >= 2)
        if (interfaceNumber == 1u)
        {
            regTmp  = CY_SYS_CPUSS_SL_CTL1_REG & ~CY_SYS_CPUSS_SL_CTL_ARB_MASK;
            regTmp |= ((uint32) (source << CY_SYS_CPUSS_SL_CTL_ARB_SHIFT) & CY_SYS_CPUSS_SL_CTL_ARB_MASK);
            CY_SYS_CPUSS_SL_CTL1_REG = regTmp;
        } else
    #endif /* (CY_IP_SL_NR >= 1) */
    #if (CY_IP_SL_NR >= 3)
        if (interfaceNumber == 2u)
        {
            regTmp  = CY_SYS_CPUSS_SL_CTL2_REG & ~CY_SYS_CPUSS_SL_CTL_ARB_MASK;
            regTmp |= ((uint32) (source << CY_SYS_CPUSS_SL_CTL_ARB_SHIFT) & CY_SYS_CPUSS_SL_CTL_ARB_MASK);
            CY_SYS_CPUSS_SL_CTL2_REG = regTmp;
        } else
    #endif /* (CY_IP_SL_NR >= 1) */
        {
            /* Halt CPU in debug mode if interface is invalid */
            CYASSERT(0u != 0u);
        }
    }

#endif /* (CY_IP_DMAC_PRESENT) */


/* [] END OF FILE */
