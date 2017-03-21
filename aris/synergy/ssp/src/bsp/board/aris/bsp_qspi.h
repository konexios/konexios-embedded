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

/***********************************************************************************************************************
 * File Name    : bsp_qspi.h
 * Description  : Configures QSPI bus and device access.
 **********************************************************************************************************************/



/*******************************************************************************************************************//**
 * @ingroup BSP_BOARD_ARIS
 * @defgroup BSP_ARIS_QSPI Board QSPI
 * @brief QSPI configuration setup for this board.
 *
 * This is code specific to the ARIS board.
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_QSPI_H_
#define BSP_QSPI_H_

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* ID and type information for the S25FL512S Spansion Serial NOR Flash Memory */
#define BSP_PRV_QSPI_MANUFACTURER_ID        0x01
#define BSP_PRV_QSPI_DEVICE_ID              0x19
#define BSP_PRV_QSPI_MEMORY_CAPACITY        0x1A    //512Mbit

#define BSP_PRV_QSPI_MANUFACTER_ID_ADDR     0x000000
#define BSP_PRV_QSPI_MEMORY_CAPACITY_ADDR   0x001027

/** Physical address the QSPI device is mapped to */
#define BSP_PRV_QSPI_DEVICE_PHYSICAL_ADDRESS (0x60000000)

#define BSP_PRV_QSPI_CFG1_MASK_OTP_BITS     0x28    /* OTP bit value to NOT write! */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Available QSPI clock rates for  Spansion S25FL512S quad SPI flash device */
typedef enum e_qspi_clk
{
    /** Reset Operations */
    QSPI_CLK_60MHZ=0,       ///< SPI CLK runs at 60.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_40MHZ,         ///< SPI CLK runs at 40.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_30MHZ,         ///< SPI CLK runs at 30.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_24MHZ,         ///< SPI CLK runs at 24.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_20MHZ,         ///< SPI CLK runs at 20.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_17MHZ,         ///< SPI CLK runs at 17.14 MHz when PCLKA is set to 120MHz
    QSPI_CLK_15MHZ,         ///< SPI CLK runs at 15.00 MHz when PCLKA is set to 120MHz
    QSPI_CLK_13MHZ,         ///< SPI CLK runs at 13.33 MHz when PCLKA is set to 120MHz
    QSPI_CLK_12MHZ,         ///< SPI CLK runs at 12.00 MHz when PCLKA is set to 120MHz
} qspi_clk;

/** Available QSPI read modes for  Spansion S25FL512S quad SPI flash device */
typedef enum e_qspi_op_mode
{
    QSPI_OPERATION_SERIAL=0,
    QSPI_OPERATION_DUAL,
    QSPI_OPERATION_QUAD,
} qspi_op_mode;

/** Available QSPI commands for the Spansion S25FL512S quad SPI flash device */
typedef enum e_qspi_command
{
    /** Reset Operations */
    QSPI_COMMAND_RESET                      = 0xF0,     ///<* Software Reset Command
    QSPI_COMMAND_MBR                        = 0xFF,     ///<* Mode Bit Reset
    
    /** Read Operations */
    QSPI_COMMAND_READ                       = 0x03,     ///<* Read memory
    QSPI_COMMAND_4BYTE_READ                 = 0x13,     ///<* Read memory with 4byte address
    QSPI_COMMAND_FAST_READ                  = 0x0B,     ///<* Fast Read memory
    
    /** Write Operations */
    QSPI_COMMAND_WRITE_ENABLE               = 0x06,     ///<* Write Enable
    QSPI_COMMAND_WRITE_DISABLE              = 0x04,     ///<* Write Disable
    QSPI_COMMAND_PAGE_PROGRAM               = 0x02,     ///<* Program a page
    QSPI_COMMAND_4BYTE_PAGE_PROGRAM         = 0x12,     ///<* Program a page using a 4 byte address
    
    /** Register Operations */
    QSPI_COMMAND_READ_STATUS_REGISTER       = 0x05,     ///<* Read Status register
    QSPI_COMMAND_CLEAR_STATUS_REGISTER      = 0x30,     ///<* Clear Status register
    QSPI_COMMAND_READ_ID                    = 0x90,     ///<* Read the chip ID
    QSPI_COMMAND_READ_RSFDP                 = 0x5A,     ///<* Read Serial Flash Discoverable Parameters
    QSPI_COMMAND_READ_CR1                   = 0x35,     ///<* Read Configuration Register 1
    QSPI_COMMAND_WRITE_ST_CR1               = 0x01,     ///<* Write Status register and Configuration register 1
    QSPI_COMMAND_READ_BANK_REGISTER         = 0x16,     ///<* Read Bank register
    QSPI_COMMAND_WRITE_BANK_REGISTER        = 0x17,     ///<* Write Bank register
    QSPI_COMMAND_READ_ASP_REGISTER          = 0x2B,     ///<* Read Advanced Sector Protection register
    
    /** Erase Operations */
    QSPI_COMMAND_SECTOR_ERASE              = 0xD8,      ///<* Erase a sector
    QSPI_COMMAND_4BYTE_SECTOR_ERASE        = 0xDC,      ///<* Erase a sector using a 4 byte address
    QSPI_COMMAND_SUBSECTOR_ERASE           = 0x20,         ///< Erase a subsector
    QSPI_COMMAND_4BYTE_SUBSECTOR_ERASE     = 0x21,         ///< Erase a subsector using a 4 byte address
    QSPI_COMMAND_CHIP_ERASE                = 0xC7,         ///< Erase the entire chip
} qspi_command;

/** Configuration register 1 for the Spansion S25FL512S quad SPI flash device */
typedef struct st_s25fl512s_cfg1
{
    union
    {
        uint8_t    entire_cfg;
        struct
        {
        uint8_t freeze                      : 1;    ///< Lock current state of BP2-0 bits in Status Register, TBPROT in
                                                    //   Configuration Register, and OTP regions
                                                    //   0 = Unlocked
                                                    //   1 = Locked
        uint8_t quad_operation              : 1;    ///< Puts te device into Quad I/O operation.
                                                    //   0 = Dual or Serial
                                                    //   1 = Quad
        uint8_t reserved1                   : 1;
        uint8_t bpnv                        : 1;    ///< Make BP2-0 bits volatile or not.
                                                    //   0 = Non-Volatile
                                                    //   1 = Volatile
        uint8_t reserved2                   : 1;
        uint8_t block_prot_start            : 1;    ///< TBPROT, Configures Start of Block Protection
                                                    //   0 = BP starts at bottom    ('Upper' mode)
                                                    //   1 = BP starts at top       ('Lower' mode)
        uint8_t latency_code                : 2;    ///< Defines the number of address bytes for a command
        };
    };
} s25fl512s_cfg1;

/** Status register for the Spansion S25FL512S quad SPI flash device */
typedef struct st_s25fl512s_status
{
    union
    {
        uint8_t    entire_cfg;
        struct
        {
        uint8_t write_in_progress           : 1;    ///< 0 = Standby, 1 = Busy
        uint8_t write_enable                : 1;    ///< 0 = Enabled, 1 = Disabled
        uint8_t block_protect               : 3;    ///< Defines memory to be software protected against PROGRAM or ERASE
                                                    ///< operations. See TBPROT of the configuration register for the
                                                    ///< Upper or Lower selecion.
                                                    ///< b000 = No protection
                                                    ///< b001 = Upper/Lower 1024 kbytes
                                                    ///< b010 = Upper/Lower 2048 kbytes
                                                    ///< b011 = Upper/Lower 4096 kbytes
                                                    ///< b100 = Upper/Lower 8192 kbytes
                                                    ///< b101 = Upper/Lower 16384 kbytes
                                                    ///< b110 = Upper/Lower 32768 kbytes
                                                    ///< b111 = Upper/Lower 65536 kbytes
        uint8_t erase_error                 : 1;    ///< 0 = No Error, 1 = Error Occurred
        uint8_t programming_error           : 1;    ///< 0 = No Error, 1 = Error Occurred
        uint8_t srwd                        : 1;    ///< Status Register Write Disable
        };
    };
} s25fl512s_status;

/** Bank Address register for the Spansion S25FL512S quad SPI flash device */
typedef struct st_s25fl512s_bank_address
{
    union
    {
        uint32_t    entire_cfg;
        struct
        {
        uint8_t ba24                        : 1;    ///< Address bit 24
        uint8_t ba25                        : 1;    ///< Address bit 25
        uint8_t reserved                    : 5;
        uint8_t ext_addr                    : 1;    ///< 0 = 3 bytes addressing, 1 = 4 bytes addressing
        };
    };
} s25fl512s_bank_address;

typedef enum qspi_protocol
{
    QSPI_EXTENDED_SPI_PROTOCOL = 0,
    QSPI_DUAL_SPI_PROTOCOL     = 1,
    QSPI_QUAD_SPI_PROTOCOL     = 2,
} qspi_protocol;

/** characteristics of the flash device */
typedef struct st_S25FL512S_characteristics
{
    uint8_t  manufacturer_id;           ///< Manufacturer ID
    uint8_t  device_id;                 ///< Device ID
    uint8_t  memory_capacity;           ///< Memory capacity
} S25FL512S_characteristics;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void bsp_qspi_init (void);

void bsp_qspi_status_get (bool * p_write_in_progress);

void bsp_qspi_xip_enter (void);

void bsp_qspi_xip_exit (void);

void bsp_qspi_config_get (uint8_t  * p_manufacturer_id,
                          uint8_t  * p_device_id,
                          uint8_t  * p_memory_capacity,
                          uint32_t * p_max_eraseable_size,
                          uint32_t * p_num_address_bytes,
                          uint32_t * p_spi_mode,
                          uint32_t * p_page_size,
                          bool     * p_xip_mode);

#endif /* BSP_QSPI_H_ */
/** @} (end defgroup BSP_ARIS_QSPI) */
