/***********************************************************************************************************************
 * Copyright [2016] RELOC s.r.l. - All rights strictly reserved
 * This Software is provided for evaluation purposes; any other use — including reproduction, modification, use for
 * a commercial product, distribution, or republication — without the prior written permission of the Copyright holder
 * is strictly prohibited. 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name    : bsp_qspi.c
 * Description  : QSPI initialization.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_ARIS_QSPI
 * @brief QSPI initialization
 *
 * This file contains code that initializes the QSPI flash controller connected to a S25FL512S Spansion Serial NOR Flash
 * Memory mounted on a ARIS development board.
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

#include "bsp_qspi.h"

#if defined(BSP_BOARD_ARIS)

/***********************************************************************************************************************
 * Configuration parameters
 **********************************************************************************************************************/
//DEFINE THIS IN THE PROJECT CONFIGURATION, IF NEEDED
//#define QSPI_SRWD_BIT_LOCKED            /* Enable this define only if SRWD bit in the Status Register 1 has been set to 1 and WP# pin must be handled */
//#define QSPI_ROM_PREFTECH_MODE          /* Is prefetch used for ROM access mode */
//#define QSPI_XIP_MODE_AFTER_INIT        /* Enter XIP mode after bsp_qspi_init is called. */

/* Number of bytes to be used for the address (3 or 4) */
#define BSP_PRV_QSPI_NUM_ADDRESS_BYTES          4
#if BSP_PRV_QSPI_NUM_ADDRESS_BYTES != 3 && BSP_PRV_QSPI_NUM_ADDRESS_BYTES != 4
#error BSP_PRV_QSPI_NUM_ADDRESS_BYTES must be equal to 3 or 4
#endif

/* QSPI Clock rate */
#define BSP_PRV_QSPI_CLOCK_RATE                 QSPI_CLK_40MHZ
#define BSP_PRV_QSPI_CLOCK_RATE_MHZ             40

/* Latency Code which select the number of mode and dummy cycles between
the end of address and the start of read data output for all read commands */
#define BSP_PRV_QSPI_S25FL512S_LATENCY_CODE     0x03


/* Read mode to operate the device in */
#define BSP_PRV_QSPI_READ_MODE                  QSPI_OPERATION_QUAD
/* Number of dummy clocks to set on the flash device for FAST QUAD I/O READ operations */
#define BSP_PRV_QSPI_S25FL512S_NUM_DUMMY_CLOCKS 3


/* QSSL high between cycles */
/* Spansion QSPI needs 50ns de-select  (QSSL high between cycles) for nonREAD commands */
#define BSP_PRV_QSPI_S25FL512S_DE_SELECT_DELAY_NS     50
#define BSP_PRV_QSPI_S25FL512S_DE_SELECT_DELAY_CLOCKS (((BSP_PRV_QSPI_CLOCK_RATE_MHZ*50)/1000)+1)

/* XIP entry and exit confirmation codes for the flash device */
#define BSP_PRV_QSPI_S25FL512S_XIP_ENTRY_CODE   0xA0
#define BSP_PRV_QSPI_S25FL512S_XIP_EXIT_CODE    0x00

/* Flash device page size */
#define BSP_PRV_QSPI_S25FL512S_PAGE_SIZE        512

/* Default values of device registers */
#define BSP_PRV_SR1_DEFAULT_VALUE               0x00
#define BSP_PRV_CR1_DEFAULT_VALUE               0x00
#define BSP_PRV_BAR_DEFAULT_VALUE               0x00


/***********************************************************************************************************************
 * Private variables
 **********************************************************************************************************************/
static s25fl512s_cfg1               mem_cfg;        /* Configuration register 1*/
static s25fl512s_status             mem_sts;        /* Status register */
static s25fl512s_bank_address       mem_bank;       /* Bank Address register */

/* Device ID and characteristics */
static S25FL512S_characteristics    device_characteristics = { 0, 0, 0 };


/***********************************************************************************************************************
 * Static functions declaration
 **********************************************************************************************************************/
static void bsp_qspi_device_reset (void);
static void bsp_qspi_write_enable (void);
static void bsp_qspi_write_disable (void);
static void bsp_qspi_read_status_reg (s25fl512s_status * p_status);
static void bsp_qspi_read_cfg_reg_1 (s25fl512s_cfg1 * p_cfg1);
static void bsp_qspi_read_bank_addr_reg (s25fl512s_bank_address * p_bank);
static void bsp_qspi_write_status_reg (s25fl512s_status * p_status);
static void bsp_qspi_write_cfg_reg_1 (s25fl512s_cfg1 * p_cfg1);
static void bsp_qspi_write_bank_addr_reg (s25fl512s_bank_address * p_bank);
static bool bsp_qspi_check_memory_id (S25FL512S_characteristics * dev_ids);
static void bsp_qspi_clear_errors (void);
static void bsp_qspi_device_xip_mode (bool enable_mode);
static void bsp_qspi_xip_mode (bool enter_mode);
static bool bsp_qspi_is_write_in_progress (void);


/*******************************************************************************************************************//**
 * @brief   Initializes QSPI.
 *
 * This function initializes QSPI and Spansion Serial Flash Memory device on the board.
 *
 * @note This function is executed once after reset.
 **********************************************************************************************************************/
void bsp_qspi_init (void)
{
#ifdef QSPI_SRWD_BIT_LOCKED
uint32_t wr_pin_cfg;
#endif
    /* Turn QSPI module ON */
    R_MSTP->MSTPCRB_b.MSTPB6 = 0;
    
    /* Initialized unused bits */
    R_QSPI->SFMSPC = 0x10;
    R_QSPI->SFMCST = 0;
    R_QSPI->SFMSIC = 0;
    R_QSPI->SFMPMD = 0;
    R_QSPI->SFMCNT1 = 0;
    R_QSPI->SFMSMD_b.SFMMD3 = 0;
    
    /* Set the SPI clock rate */
    R_QSPI->SFMSKC_b.SFMDV = BSP_PRV_QSPI_CLOCK_RATE;
    
    /* Enter direct communication mode */
    R_QSPI->SFMCMD_b.DCOM = 1;
    
#ifdef QSPI_SRWD_BIT_LOCKED
    wr_pin_cfg = ((uint32_t) ((((((uint32_t)IOPORT_PORT_05_PIN_04 >> 8) & 0x00FF) * (16 * 4))) + (((uint32_t)IOPORT_PORT_05_PIN_04 & 0x00FF) * 4) + ((uint32_t) R_PFS_BASE)));
    wr_pin_cfg = *((uint32_t*)wr_pin_cfg);
    g_ioport.p_api->pinCfg (IOPORT_PORT_05_PIN_04, (IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_PERIPHERAL_IO) );
    g_ioport.p_api->pinWrite (IOPORT_PORT_05_PIN_04, IOPORT_LEVEL_HIGH );
#endif
    /* Reset the flash device */
    bsp_qspi_device_reset();
    
    /* Clear error flags */
    bsp_qspi_clear_errors();
    
    /* Reset register to default values */
    mem_sts.entire_cfg = BSP_PRV_SR1_DEFAULT_VALUE;
    bsp_qspi_write_status_reg (&mem_sts);
    mem_cfg.entire_cfg = BSP_PRV_CR1_DEFAULT_VALUE;
    bsp_qspi_write_cfg_reg_1 (&mem_cfg);
    mem_bank.entire_cfg = BSP_PRV_BAR_DEFAULT_VALUE;
    bsp_qspi_write_bank_addr_reg (&mem_bank);
    
#ifdef QSPI_SRWD_BIT_LOCKED
    g_ioport.p_api->pinCfg (IOPORT_PORT_05_PIN_04, wr_pin_cfg );
#endif
    
    /* Read back the device's registers */
    bsp_qspi_read_cfg_reg_1 (&mem_cfg);
    bsp_qspi_read_status_reg (&mem_sts);
    bsp_qspi_read_bank_addr_reg (&mem_bank);
    
    /* Check memory id */
    if (bsp_qspi_check_memory_id (&device_characteristics) == false)
    {
        while(1);
        return;
    }
    
    /* Change the configuration of the device if it differs from the configuration specified above */
    if ((((BSP_PRV_QSPI_READ_MODE == QSPI_OPERATION_QUAD) ? 1 : 0)  != mem_cfg.quad_operation   ) ||
          (BSP_PRV_QSPI_S25FL512S_LATENCY_CODE                      != mem_cfg.latency_code     ))
    {
        mem_cfg.quad_operation = ((BSP_PRV_QSPI_READ_MODE == QSPI_OPERATION_QUAD) ? 1 : 0);
        mem_cfg.latency_code = BSP_PRV_QSPI_S25FL512S_LATENCY_CODE;
        
        bsp_qspi_write_cfg_reg_1 (&mem_cfg);
    }
    
    /* Set the number of address bytes */
    mem_bank.entire_cfg = 0;
    if (BSP_PRV_QSPI_NUM_ADDRESS_BYTES == 4)
    {
        /* 4-byte address */
        mem_bank.ext_addr = 1;
        bsp_qspi_write_bank_addr_reg (&mem_bank);
        /* Configure the QSPI block that way as well */
        R_QSPI->SFMSAC_b.SFMAS  = 3; /* 4 byte address */
        R_QSPI->SFMSAC_b.SFM4BC = 0; /* Select default instruction code */
    }
    else
    {
        /* 3-byte address */
        mem_bank.ext_addr = 0;
        bsp_qspi_write_bank_addr_reg (&mem_bank);
        /* Configure the QSPI block that way as well */
        R_QSPI->SFMSAC_b.SFMAS  = 2; /* 3 byte address */
        R_QSPI->SFMSAC_b.SFM4BC = 0; /* Select default instruction code */
    }
    
    /* Set the number of dummy cycles in QSPI peripheral */
    R_QSPI->SFMSDC_b.SFMDN = (BSP_PRV_QSPI_S25FL512S_NUM_DUMMY_CLOCKS - 2);

    /* Set the minimum number of de-select */
    R_QSPI->SFMSSC_b.SFMSW = BSP_PRV_QSPI_S25FL512S_DE_SELECT_DELAY_CLOCKS;

    /* Set the read mode */
    switch (BSP_PRV_QSPI_READ_MODE)
    {
        case QSPI_OPERATION_QUAD    : R_QSPI->SFMSMD_b.SFMRM = 0x05; break;
        case QSPI_OPERATION_DUAL    : R_QSPI->SFMSMD_b.SFMRM = 0x03; break;
        case QSPI_OPERATION_SERIAL  :
        default                     : R_QSPI->SFMSMD_b.SFMRM = 0x00; break;
    }

#ifdef BSP_PRV_QSPI_ROM_PREFTECH_MODE
    R_QSPI->SFMSMD_b.SFMPFE = 1;
#endif
    
#ifdef BSP_PRV_QSPI_XIP_MODE_AFTER_INIT
    /* Enable XIP mode on the flash device */
    bsp_qspi_xip_enter();
#else
    /* Enter ROM access mode */
    R_QSPI->SFMCMD_b.DCOM = 0;
#endif
    
}

/*******************************************************************************************************************//**
 * @brief   Enter XIP mode
 *
 * This function enters XIP mode in both the QSPI controller block and on the flash device.
 *
 **********************************************************************************************************************/
void bsp_qspi_xip_enter (void)
{
    /* Check there is no serial transfer in progress */
    while (R_QSPI->SFMCST_b.COMBSY == 1);
    
    /* Configure memory for XIP mode */
    bsp_qspi_device_xip_mode (true);
    
    /* Drive the XIP confirmation bit to 0 during the next FAST-READ cycle */
    bsp_qspi_xip_mode (true);
}

/*******************************************************************************************************************//**
 * @brief   Exit XIP mode
 *
 * This function exits XIP mode in both the QSPI controller block and on the flash device.
 *
 **********************************************************************************************************************/
void bsp_qspi_xip_exit (void)
{
    /* Check there is no serial transfer in progress */
    while (R_QSPI->SFMCST_b.COMBSY == 1);
    
    bsp_qspi_xip_mode (false);
}

/*******************************************************************************************************************//**
 * @brief   Check the status of the QSPI device.
 *
 * This function reads the status register from the device which indicates status and errors that occurred during various
 * operations like erasing and programming.
 *
 **********************************************************************************************************************/
void bsp_qspi_status_get (bool * p_write_in_progress)
{
    *p_write_in_progress = bsp_qspi_is_write_in_progress();
}

/*******************************************************************************************************************//**
 * @brief   Get the current configuration of the QSPI device.
 *
 * This function reads the volatile and non-volatile registers of the device and returns portions of these to the QSPI
 * module driver for use in direct communication mode.
 *
 * @param[out] num_address_bytes      Number of bytes used for the address - 3 bytes or 4 bytes
 * @param[out] spi_mode               SPI mode used - 0 = Extended, 1 = Dual, 2 = Quad
 *
 **********************************************************************************************************************/
void bsp_qspi_config_get (uint8_t  * p_manufacturer_id,
                          uint8_t  * p_device_id,
                          uint8_t  * p_memory_capacity,
                          uint32_t * p_max_eraseable_size,
                          uint32_t * p_num_address_bytes,
                          uint32_t * p_spi_mode,
                          uint32_t * p_page_size,
                          bool     * p_xip_mode)
{
    *p_manufacturer_id    = device_characteristics.manufacturer_id;
    *p_device_id          = device_characteristics.device_id;
    *p_memory_capacity    = device_characteristics.memory_capacity;
    *p_max_eraseable_size = 4; /* 4k bytes */
    *p_num_address_bytes  = BSP_PRV_QSPI_NUM_ADDRESS_BYTES;
    *p_spi_mode           = BSP_PRV_QSPI_READ_MODE;
    *p_page_size          = BSP_PRV_QSPI_S25FL512S_PAGE_SIZE;
#ifdef BSP_PRV_QSPI_XIP_MODE_AFTER_INIT
    *p_xip_mode           = 1;
#else
    *p_xip_mode           = 0;
#endif
}





/*******************************************************************************************************************//**
 * Static functions
 **********************************************************************************************************************/


/*******************************************************************************************************************//**
 * @brief   Reset the flash device
 **********************************************************************************************************************/
static void bsp_qspi_device_reset (void)
{
    volatile uint32_t i = 0;
    
    
    /* Reset the flash device. */
    R_QSPI->SFMCOM        = QSPI_COMMAND_MBR;           /* Write the command */
    R_QSPI->SFMCMD_b.DCOM = 1;                          /* Close the SPI bus cycle */
    R_QSPI->SFMCOM        = QSPI_COMMAND_RESET;         /* Write the command */
    R_QSPI->SFMCMD_b.DCOM = 1;                          /* Close the SPI bus cycle */
    
    /* Must wait 35us ( 1000 = 54us @240MHz)*/
    while( i < 1000 ) i++;
}

/*******************************************************************************************************************//**
 * @brief   Enable write on registers.
 **********************************************************************************************************************/
static void bsp_qspi_write_enable (void)
{
    R_QSPI->SFMCOM        = QSPI_COMMAND_WRITE_ENABLE;              /* Enable writing */
    R_QSPI->SFMCMD_b.DCOM = 1;                                      /* Close the SPI bus cycle */
}

/*******************************************************************************************************************//**
 * @brief   Disable write on registers.
 **********************************************************************************************************************/
static void bsp_qspi_write_disable (void)
{
    R_QSPI->SFMCOM        = QSPI_COMMAND_WRITE_DISABLE;             /* Disable writing */
    R_QSPI->SFMCMD_b.DCOM = 1;                                      /* Close the SPI bus cycle */
}

/*******************************************************************************************************************//**
 * This function reads the status register from the device which indicates status and errors that occurred during various
 * operations like erasing and programming.
 **********************************************************************************************************************/
static void bsp_qspi_read_status_reg (s25fl512s_status * p_status)
{
    R_QSPI->SFMCOM = QSPI_COMMAND_READ_STATUS_REGISTER;         /* Send command to read status */
    p_status->entire_cfg = R_QSPI->SFMCOM_b.SFMD;               /* Read the device status register */
    R_QSPI->SFMCMD_b.DCOM = 1;                                  /* Close the SPI bus cycle */
}

/*******************************************************************************************************************//**
 * This function reads the configuration register 1 from the device.
 **********************************************************************************************************************/
static void bsp_qspi_read_cfg_reg_1 (s25fl512s_cfg1 * p_cfg1)
{
    R_QSPI->SFMCOM = QSPI_COMMAND_READ_CR1;                     /* Send command to read configuration register 1 */
    p_cfg1->entire_cfg = R_QSPI->SFMCOM_b.SFMD;                 /* Read the device configuration register 1 */
    R_QSPI->SFMCMD_b.DCOM = 1;                                  /* Close the SPI bus cycle */
}

/*******************************************************************************************************************//**
 * This function reads the bank address register from the device which contain configuration about address mode.
 **********************************************************************************************************************/
static void bsp_qspi_read_bank_addr_reg (s25fl512s_bank_address * p_bank)
{
    R_QSPI->SFMCOM = QSPI_COMMAND_READ_BANK_REGISTER;           /* Send command to read bank address register */
    p_bank->entire_cfg = R_QSPI->SFMCOM_b.SFMD;                 /* Read the device bank address register */
    R_QSPI->SFMCMD_b.DCOM = 1;                                  /* Close the SPI bus cycle */
}

/*******************************************************************************************************************//**
 * This function writes the status register of the device.
 **********************************************************************************************************************/
static void bsp_qspi_write_status_reg (s25fl512s_status * p_status)
{
    s25fl512s_cfg1 cfg1_reg;
    
    
    bsp_qspi_read_cfg_reg_1 (&cfg1_reg);
    bsp_qspi_write_enable ();
    if (cfg1_reg.quad_operation == 0)
    {
        R_QSPI->SFMCOM        = QSPI_COMMAND_WRITE_ST_CR1;              /* Write the command */
        R_QSPI->SFMCOM        = p_status->entire_cfg;                   /* Write the status register */
        R_QSPI->SFMCMD_b.DCOM = 1;                                      /* Close the SPI bus cycle */
    }
    else
    {
        /* In quad mode both status and cfg1 registers must be written! */
        R_QSPI->SFMCOM        = QSPI_COMMAND_WRITE_ST_CR1;              /* Write the command */
        R_QSPI->SFMCOM        = p_status->entire_cfg;                   /* Write the status register */
        R_QSPI->SFMCOM        = cfg1_reg.entire_cfg;                    /* Write back the configuration register */
        R_QSPI->SFMCMD_b.DCOM = 1;                                      /* Close the SPI bus cycle */
    }
    /* Wait write end */
    while( bsp_qspi_is_write_in_progress() );
    
    bsp_qspi_write_disable ();
}

/*******************************************************************************************************************//**
 * This function writes the configuration register 1 of the device.
 **********************************************************************************************************************/
static void bsp_qspi_write_cfg_reg_1 (s25fl512s_cfg1 * wanted_cfg1_reg)
{
    s25fl512s_status status_reg_backup;
    s25fl512s_cfg1 cfg1_reg;
    
    
    /* Back up Status register value */
    bsp_qspi_read_status_reg (&status_reg_backup);
    /* Read current configuration register value */
    bsp_qspi_read_cfg_reg_1 (&cfg1_reg);
    
    /* Leave OTP bits at the current value!!! */
    cfg1_reg.entire_cfg = (uint8_t)( (cfg1_reg.entire_cfg & BSP_PRV_QSPI_CFG1_MASK_OTP_BITS) | (wanted_cfg1_reg->entire_cfg & (~BSP_PRV_QSPI_CFG1_MASK_OTP_BITS)) );
    
    bsp_qspi_write_enable();
    
    R_QSPI->SFMCOM        = QSPI_COMMAND_WRITE_ST_CR1;              /* Write the command */
    R_QSPI->SFMCOM        = status_reg_backup.entire_cfg;           /* Write the status register */
    R_QSPI->SFMCOM        = cfg1_reg.entire_cfg;                    /* Write the value */
    R_QSPI->SFMCMD_b.DCOM = 1;                                      /* Close the SPI bus cycle */
    
    /* Wait write end */
    while( bsp_qspi_is_write_in_progress() );
    
    bsp_qspi_write_disable();
}

/*******************************************************************************************************************//**
 * This function writes the bank address register of the device.
 **********************************************************************************************************************/
static void bsp_qspi_write_bank_addr_reg (s25fl512s_bank_address * p_bank)
{
    bsp_qspi_write_enable();
    
    R_QSPI->SFMCOM        = QSPI_COMMAND_WRITE_BANK_REGISTER;           /* Write the command */
    R_QSPI->SFMCOM        = p_bank->entire_cfg;                         /* Write the value */
    R_QSPI->SFMCMD_b.DCOM = 1;                                          /* Close the SPI bus cycle */
    
    /* Wait write end */
    while( bsp_qspi_is_write_in_progress() );
    
    bsp_qspi_write_disable();
}

/*******************************************************************************************************************//**
 * This function read the device information.
 **********************************************************************************************************************/
static bool bsp_qspi_check_memory_id (S25FL512S_characteristics * dev_ids)
{
    bool ret;
    
    
    /* Read the ID of the device. Confirm it is the correct device. */
    R_QSPI->SFMCOM          = QSPI_COMMAND_READ_ID;                                     /* Write the command */
    R_QSPI->SFMCOM          = ((BSP_PRV_QSPI_MANUFACTER_ID_ADDR & 0x00FF0000) >> 16);   /* Write bits 23 to 16 of the address */
    R_QSPI->SFMCOM          = ((BSP_PRV_QSPI_MANUFACTER_ID_ADDR & 0x0000FF00) >> 8);    /* Write bits 15 to 8 of the address  */
    R_QSPI->SFMCOM          = ((BSP_PRV_QSPI_MANUFACTER_ID_ADDR & 0x000000FF) >> 0);    /* Write bits 7 to 0 of the address   */
    dev_ids->manufacturer_id= R_QSPI->SFMCOM_b.SFMD;                                    /* Read the manufacturer ID */
    dev_ids->device_id      = R_QSPI->SFMCOM_b.SFMD;                                    /* Read the device ID */
    R_QSPI->SFMCMD_b.DCOM   = 1;                                                        /* Close the SPI bus cycle */
    
    R_QSPI->SFMCOM          = QSPI_COMMAND_READ_RSFDP;                                  /* Write the command */
    R_QSPI->SFMCOM          = ((BSP_PRV_QSPI_MEMORY_CAPACITY_ADDR & 0x00FF0000) >> 16); /* Write bits 23 to 16 of the address */
    R_QSPI->SFMCOM          = ((BSP_PRV_QSPI_MEMORY_CAPACITY_ADDR & 0x0000FF00) >> 8);  /* Write bits 15 to 8 of the address  */
    R_QSPI->SFMCOM          = ((BSP_PRV_QSPI_MEMORY_CAPACITY_ADDR & 0x000000FF) >> 0);  /* Write bits 7 to 0 of the address   */
    dev_ids->memory_capacity= R_QSPI->SFMCOM_b.SFMD;                                    /* Dummy read */
    dev_ids->memory_capacity= R_QSPI->SFMCOM_b.SFMD;                                    /* Read the memory capacity */
    R_QSPI->SFMCMD_b.DCOM   = 1;                                                        /* Close the SPI bus cycle */
    
    if ((BSP_PRV_QSPI_MANUFACTURER_ID   != dev_ids->manufacturer_id  ) ||
        (BSP_PRV_QSPI_DEVICE_ID         != dev_ids->device_id        ) ||
        (BSP_PRV_QSPI_MEMORY_CAPACITY   != dev_ids->memory_capacity  ))
    {
        ret = false;
    }
    else
    {
        ret = true;
    }
    
    return ret;
}

/*******************************************************************************************************************//**
 * This function clear the error flash in the status register of the device.
 **********************************************************************************************************************/
static void bsp_qspi_clear_errors (void)
{
    R_QSPI->SFMCOM = QSPI_COMMAND_CLEAR_STATUS_REGISTER;
    R_QSPI->SFMCMD_b.DCOM                  = 1;                     /* Close the SPI bus cycle */
}


/*******************************************************************************************************************//**
 * @brief   Enable or disable XIP mode on the flash device
 *
 * @param[in]  enable_mode  0 = disable XIP mode, 1 = enable XIP mode
 *
 **********************************************************************************************************************/
static void bsp_qspi_device_xip_mode (bool enable_mode)
{
    /* No operation needed */
    SSP_PARAMETER_NOT_USED(enable_mode);
}

/*******************************************************************************************************************//**
 * @brief   Enter or exit XIP mode
 *
 * @param[in]  enter_mode  0 = exit XIP mode, 1 = enter XIP mode
 *
 **********************************************************************************************************************/
static void bsp_qspi_xip_mode (bool enter_mode)
{
    volatile uint32_t i = 0;
    volatile uint32_t timeout;
    
    
    SSP_PARAMETER_NOT_USED(i);
    
    R_QSPI->SFMCMD_b.DCOM = 0;      /* Enter ROM mode */
    
    if (enter_mode)
    {/* Enter XIP Mode */
        R_QSPI->SFMSDC_b.SFMXD  = BSP_PRV_QSPI_S25FL512S_XIP_ENTRY_CODE;    /* Set the XIP entry confirmation code */
        R_QSPI->SFMSDC_b.SFMXEN = 1;                                        /* Enter XIP mode in QSPI controller */
        i = *((uint32_t *) BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS);           /* Reading from the device set the code */
        
        /* Wait for the controller to enter XIP mode */
        timeout = 0xFFF;
        while (0 == R_QSPI->SFMSDC_b.SFMXST)
        {
            timeout--;
            if (0 == timeout)
            {
                return;
            }
        }
    }
    else
    {/* Release XIP Mode */
        R_QSPI->SFMSDC_b.SFMXD  = BSP_PRV_QSPI_S25FL512S_XIP_EXIT_CODE;     /* Set the XIP exit confirmation code */
        R_QSPI->SFMSDC_b.SFMXEN = 0;                                        /* Exit XIP mode in the QSPI controller block */
        i = *((uint32_t *) BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS);           /* Read from the device to set the code */
        
        /* Wait for the controller to exit XIP mode */
        timeout = 0xFFF;
        while (1 == R_QSPI->SFMSDC_b.SFMXST)
        {
            timeout--;
            if (0 == timeout)
            {
                return;
            }
        }
    }
}

/*******************************************************************************************************************//**
 * @brief   Check if a write process is in progress.
 *
 * @retval false            Device in standby.
 * @retval true             Write in progress.
 *
 **********************************************************************************************************************/
static bool bsp_qspi_is_write_in_progress (void)
{
    s25fl512s_status status;
    bsp_qspi_read_status_reg (&status);
    return status.write_in_progress;
}

#endif /* if defined(BSP_BOARD_ARIS) */

/** @} (end addtogroup BSP_ARIS_QSPI) */
