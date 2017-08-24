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
* File Name    : bsp_cfg_ref.h
* Description  : BSP configuration file for S7G2 device.
***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_CONFIG_ARIS
 * @defgroup BSP_CONFIG_ARIS_GENERAL Build Time Configurations - General
 *
 * This file contains compile-time configuration options for general BSP settings. Things such as stack sizes and ROM
 * registers are configured in this file.
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_BOARD_ARIS
#define BSP_BOARD_ARIS

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/**
 * @name Part Number Information
 *
 * Enter the product part number for your MCU. This information will be used to obtain information about your MCU such
 * as package and memory size.
 * To help parse this information, the part number will be defined using multiple macros.
 *
 * <PRE>
 * R7FS 7 G 2 7 H 3 A01 C FC
 * |    | | | | | | |   | |  Macro Name                      Description
 * |    | | | | | | |   | |__BSP_CFG_MCU_PART_PACKAGE      = Package type, number of pins
 * |    | | | | | | |   |____not used                      = Quality ID
 * |    | | | | | | |________not used                      = Software ID
 * |    | | | | | |__________not used                      = Operating range
 * |    | | | | |____________BSP_CFG_MCU_PART_MEMORY_SIZE  = ROM, RAM, and Data Flash Capacity
 * |    | | | |______________BSP_CFG_MCU_PART_FEATURE_SET  = Superset, no encryption
 * |    | | |________________not used                      = Document index
 * |    | |__________________BSP_CFG_MCU_PART_CORE         = Core & frequency (CM4, 240MHz)
 * |    |____________________BSP_CFG_MCU_PART_SERIES       = Performance category (High-performance)
 * |_________________________not used                      = Renesas Synergy MCU
 * </PRE>
 */

/** Package type. Set the macro definition based on values below:
 * <PRE>
 * Character(s) = Value for macro = Package Type/Number of Pins/Other Info
 * BJ           = 0x0             = BGA/121
 * BG           = 0x1             = BGA/176
 * BD           = 0x2             = BGA/224
 * FL           = 0x3             = LQFP/48
 * FM           = 0x4             = LQFP/64
 * FP           = 0x5             = LQFP/100
 * FB           = 0x6             = LQFP/144
 * FC           = 0x7             = LQFP/176
 * LM           = 0x8             = LGA/36
 * LA           = 0x9             = LGA/100/5.5x5.5
 * LJ           = 0xA             = LGA/100/7x7
 * LK           = 0xB             = LGA/145
 * NF           = 0xC             = WQFN/40
 * NE           = 0xD             = WQFN/48
 * NB           = 0xE             = WQFN/64
 * </PRE>
 */
#define BSP_CFG_MCU_PART_PACKAGE                    (0x7)

/** ROM, RAM, and Data Flash Capacity.
 * <PRE>
 * Character(s) = Value for macro = ROM Size/Ram Size/Data Flash Size
 * 6            = 0x06             = 64KB
 * 7            = 0x07             = 128KB
 * 8            = 0x08             = 256KB
 * A            = 0x0A             = 512KB
 * C            = 0x0C             = 1MB
 * E            = 0x0E             = 2MB
 * G            = 0x10             = 3MB
 * H            = 0x11             = 4MB/640KB/64KB
 * </PRE>
*/
#define BSP_CFG_MCU_PART_MEMORY_SIZE                (0x11)

/** Core & Frequency.
 * <PRE>
 * Character(s) = Value for macro = Description
 * 2            = 0x02            = CM0+, 32MHz
 * A            = 0x0A            = CM4,  48MHz
 * D            = 0x0D            = CM4, 120MHz
 * G            = 0x10            = CM4, 240MHz
 * </PRE>
*/
#define BSP_CFG_MCU_PART_CORE                       (0x10)

/** Series
 * <PRE>
 * Character(s) = Value for macro = Description
 * 1            = 0x1             = Ultra-low Power (up to 32MHz)
 * 3            = 0x3             = High-efficiency (33-100MHz)
 * 5            = 0x5             = High-integration (101MHz-200MHz)
 * 7            = 0x7             = High-performance (201MHz-300MHz)
 * </PRE>
*/
#define BSP_CFG_MCU_PART_SERIES                     (0x7)

/** @} (end of @name Part Number Information) */

/**
 * @name Stack & Heap Size Settings
 */

/** The Main Stack size in bytes. This is the stack that is used out of reset and is required. Exceptions always use
 * the main stack.
 */
#define BSP_CFG_STACK_MAIN_BYTES                    (0x1000)

/** The Process Stack size in bytes. Use of the Process Stack is optional. The BSP does not use the Process Stack.
 * If the user wishes to use the Process Stack in their application they will need to set the SPSEL bit in the
 * CONTROL register.
 */
#define BSP_CFG_STACK_PROCESS_BYTES                 (0)

/** Heap size in bytes. */
#define BSP_CFG_HEAP_BYTES                          (0x400)

/** @} (end of 'Stack & Heap Size Settings')*/

/**
 * @name Option-Setting Memory (ROM registers) Settings
 *
 * Certain registers are stored in ROM and used to configure the MCU out of reset.
 *
 * @note To use the default values for a register, leave the macro as all 0xFF's.
 * @note ALL OPTIONS ARE DISABLED BY DEFAULT.
*/

/** Configure WDT and IWDT settings.
 * OFS0 - Option Function Select Register 0
 * - b31     Reserved (set to 1)
 * - b30     WDTSTPCTL - WDT Stop Control - (0=counting continues, 1=stop when entering sleep mode)
 * - b29     Reserved (set to 1)
 * - b28     WDTRSTIRQS - WDT Reset Interrupt Request - What to do on underflow (0=take interrupt, 1=reset MCU)
 * - b27:b26 WDTRPSS - WDT Window Start Position Select - (0=25%, 1=50%, 2=75%, 3=100%,don't use)
 * - b25:b24 WDTRPES - WDT Window End Position Select - (0=75%, 1=50%, 2=25%, 3=0%,don't use)
 * - b23:b20 WDTCKS - WDT Clock Frequency Division Ratio - (1=/4, 4=/64, 0xF=/128, 6=/512, 7=/2048, 8=/8192)
 * - b19:b18 WDTTOPS - WDT Timeout Period Select - (0=1024 cycles, 1=4096, 2=8192, 3=16384)
 * - b17     WDTSTRT - WDT Start Mode Select - (0=auto-start after reset, 1=halt after reset)
 * - b16:b15 Reserved (set to 1)
 * - b14     IWDTSTPCTL - IWDT Sleep Stop Control - (0=counting continues, 1=stop w/selected low power modes)
 * - b13     Reserved (set to 1)
 * - b12     IWDTRSTIRQS - IWDT Reset Interrupt Request - What to do on underflow (0=take interrupt, 1=reset MCU)
 * - b11:b10 IWDTRPSS - IWDT Window Start Position Select - (0=25%, 1=50%, 2=75%, 3=100%,don't use)
 * - b9:b8   IWDTRPES - IWDT Window End Position Select - (0=75%, 1=50%, 2=25%, 3=0%,don't use)
 * - b7:b4   IWDTCKS - IWDT Clock Frequency Division Ratio - (0=none, 2=/16, 3 = /32, 4=/64, 0xF=/128, 5=/256)
 * - b3:b2   IWDTTOPS - IWDT Timeout Period Select - (0=128 cycles, 1=512, 2=1024, 3=2048)
 * - b1      IWDTSTRT - IWDT Start Mode Select - (0=auto-start after reset, 1=halt after reset)
 * - b0      Reserved (set to 1)
 *
 *  @note A value of 0xFFFFFFFF is the default and will disable all features
 */
#define BSP_CFG_ROM_REG_OFS0                    (0xFFFFFFFF)

/** Configure whether voltage detection 0 circuit and HOCO are enabled after reset.
 * OFS1 - Option Function Select Register 1
 * - b31:b9 Reserved (set to 1)
 * - b10:b9 HOCOFRQ0 - HOCO frequency setting (Set using BSP_CFG_HOCO_FREQUENCY in bsp_clock_cfg.h)
 * - b8     HOCOEN - Enable/disable HOCO oscillation after a reset (0=enable, 1=disable)
 * - b7:b3  Reserved (set to 1)
 * - b2     LVDAS - Choose to enable/disable Voltage Detection 0 Circuit after a reset (0=enable, 1=disable)
 * - b1:b0  VDSEL - Voltage Detection 0 Level Select (1=2.94V, 2=2.87V, 3=2.80V)
 *
 * @note A value of 0xFFFFFFFF is the default and will disable all features
 */
#define BSP_CFG_ROM_REG_OFS1                    (0xFFFFFFFF)

/** @} (end of @name 'Option-Setting Memory (ROM registers) Settings') */

/**
 * @name Security MPU Options
 *
 * Each Security MPU region below has 3 macros.
 * - Enable/Disable option. Use 1 = disabled, 0 = enabled
 * - Start address - The start address of this MPU area.
 * - End address - The end address of this MPU area.
 *
 * @note Different areas have different restrictions on the addresses that can be set.
 * @note ALL REGIONS ARE DISABLED BY DEFAULT.
*/
/** Enable or disable PC Region 0
 * - 0 = Enabled
 * - 1 = Disabled
 */
#define BSP_CFG_ROM_REG_MPU_PC0_ENABLE          (1)
/** Valid range: 0x00000000 - 0xFFFFFFFC */
#define BSP_CFG_ROM_REG_MPU_PC0_START           (0xFFFFFFFF)
/** Valid range: 0x00000003 - 0xFFFFFFFF */
#define BSP_CFG_ROM_REG_MPU_PC0_END             (0xFFFFFFFF)

/** Enable or disable PC Region 1
 * - 0 = Enabled
 * - 1 = Disabled
 */
#define BSP_CFG_ROM_REG_MPU_PC1_ENABLE          (1)
/** Valid range: 0x00000000 - 0xFFFFFFFC */
#define BSP_CFG_ROM_REG_MPU_PC1_START           (0xFFFFFFFF)
/** Valid range: 0x00000003 - 0xFFFFFFFF */
#define BSP_CFG_ROM_REG_MPU_PC1_END             (0xFFFFFFFF)

/** Enable or disable Memory Region 0
 * - 0 = Enabled
 * - 1 = Disabled
 */
#define BSP_CFG_ROM_REG_MPU_REGION0_ENABLE      (1)
/** Valid range: 0x00000000 - 0x00FFFFFC */
#define BSP_CFG_ROM_REG_MPU_REGION0_START       (0xFFFFFFFF)
/** Valid range: 0x00000003 - 0x00FFFFFF */
#define BSP_CFG_ROM_REG_MPU_REGION0_END         (0xFFFFFFFF)

/** Enable or disable Memory Region 1
 * - 0 = Enabled
 * - 1 = Disabled
 */
#define BSP_CFG_ROM_REG_MPU_REGION1_ENABLE      (1)
/** Valid range: 0x00000000 - 0xFFFFFFFC */
#define BSP_CFG_ROM_REG_MPU_REGION1_START       (0xFFFFFFFF)
/** Valid range: 0x00000003 - 0xFFFFFFFF */
#define BSP_CFG_ROM_REG_MPU_REGION1_END         (0xFFFFFFFF)

/** Enable or disable Memory Region 2
 * - 0 = Enabled
 * - 1 = Disabled
 */
#define BSP_CFG_ROM_REG_MPU_REGION2_ENABLE      (1)
/** Valid range: 0x40000000 - 0x407FFFFC */
#define BSP_CFG_ROM_REG_MPU_REGION2_START       (0xFFFFFFFF)
/** Valid range: 0x40000003 - 0x407FFFFF */
#define BSP_CFG_ROM_REG_MPU_REGION2_END         (0xFFFFFFFF)

/** Enable or disable Memory Region 3
 * - 0 = Enabled
 * - 1 = Disabled
 */
#define BSP_CFG_ROM_REG_MPU_REGION3_ENABLE      (1)
/** Valid range: 0x40000000 - 0x407FFFFC */
#define BSP_CFG_ROM_REG_MPU_REGION3_START       (0xFFFFFFFF)
/** Valid range: 0x40000003 - 0x407FFFFF */
#define BSP_CFG_ROM_REG_MPU_REGION3_END         (0xFFFFFFFF)

/** @} (end of @name Security MPU Options) */

/**
 * @name ID Code Protection
 *
 * Set your desired ID code. If you set this value and program it into the MCU then you will need to remember
 * the ID code because the debugger will ask for it when trying to connect. The ID Code is 16 bytes long.
 * The macro below define the ID Code in 4-byte sections.
 *
 * @note Leave at the default (all 0xFF's) if you do not wish to use an ID code (Protection Disabled).
 * @warning Please consult the hardware manual for your MCU on valid ID Code settings.
 */
/** Lowest 4-byte section, address 0x40120050. From MSB to LSB: ID Code 16, ID code 15, ID code 14, ID code 13. */
#define BSP_CFG_ID_CODE_LONG_1                      (0xFFFFFFFF)
/** 2nd ID Code section, address 0x40120054. From MSB to LSB: ID code 12, ID code 11, ID code 10, ID code 9. */
#define BSP_CFG_ID_CODE_LONG_2                      (0xFFFFFFFF)
/** 3rd ID Code section, address 0x40120058. From MSB to LSB: ID code 8, ID code 7, ID code 6, ID code 5. */
#define BSP_CFG_ID_CODE_LONG_3                      (0xFFFFFFFF)
/** 4th ID Code section, address 0x4012005C. From MSB to LSB: ID code 4, ID code 3, ID code 2, ID code 1. */
#define BSP_CFG_ID_CODE_LONG_4                      (0xFFFFFFFF)

/** @} (end of @name 'ID Code Protection') */

/**
 * @name Other Hardware Options
 */

/** This macro is used to define the voltage that is supplied to the MCU (Vcc). This macro is defined in millivolts.
 * This macro does not actually change anything on the MCU. Selected modules need this information so it is
 * defined here.
 */
#define BSP_CFG_MCU_VCC_MV                          (3300)

/** @} (end of @name 'ID Code Protection') */

/**
 *  @name Project-Wide Software Options
 */

/** By default SSP modules will check input parameters to be valid. This is helpful during development but certain users
 * will want to disable this for production code. The reason for this would be to save execution time and code space.
 * This macro is a global setting for enabling or disabling parameter checking. Each module will also have its
 * own local macro for this same purpose. By default the local macros will take the global value from here though
 * they can be overridden. Therefore, the local setting has priority over this global setting. Disabling parameter
 * checking should only used when inputs are known to be good and the increase in speed or decrease in code space is
 * needed.
 * - 0 = Global setting for parameter checking is disabled.
 * - 1 = Global setting for parameter checking is enabled (Default).
 */
#define BSP_CFG_PARAM_CHECKING_ENABLE               (1)

/** Specify which RTOS is being used
 * - 0 = No RTOS
 * - 1 = ThreadX
 * - 2 = Other
 */
#define BSP_CFG_RTOS                                (1)

/** Specify what to do if SSP_ASSERT fails
 * - 0 = Return SSP_ERR_ASSERTION.
 * - 1 = Call ssp_error_log, then return SSP_ERR_ASSERTION.  Note that ssp_error_log is a weak function and should be
 *       overridden in user code based on the prototype in ssp/src/bsp/mcu/all/bsp_common.h.
 * - 2 = Use standard assert library to halt execution.
 */
#define BSP_CFG_ASSERT                              (0)

/** Specify what to do when error codes are returned from SSP functions
 * - 0 = Return error code.
 * - 1 = Call ssp_error_log, then return error code.  Note that ssp_error_log is a weak function and should be
 *       overridden in user code based on the prototype in ssp/src/bsp/mcu/all/bsp_common.h.
 */
#define BSP_CFG_ERROR_LOG                           (0)

/** @} (end of @name 'Project-Wide Software Options') */

/** @} (end of defgroup BSP_CONFIG_ARIS_GENERAL) */

#endif /* BSP_BOARD_ARIS */
