/* generated configuration header file - do not edit */
#ifndef BSP_CFG_H_
#define BSP_CFG_H_
#define BSP_BOARD_ARIS
#define BSP_CFG_MCU_PART_PACKAGE (0x7)
#define BSP_CFG_MCU_PART_MEMORY_SIZE (0x11)
#define BSP_CFG_MCU_PART_CORE (0x10)
#define BSP_CFG_MCU_PART_SERIES (0x7)
#define BSP_CFG_STACK_MAIN_BYTES (0x2000)
#define BSP_CFG_STACK_PROCESS_BYTES (0)
#define BSP_CFG_HEAP_BYTES (0x9000)
#define OFS_SEQ1 0xA001A001 | (1 << 1) | (3 << 2)
#define OFS_SEQ2 (15 << 4) | (3 << 8) | (3 << 10)
#define OFS_SEQ3 (1 << 12) | (1 << 14) | (1 << 17)
#define OFS_SEQ4 (3 << 18) |(15 << 20) | (3 << 24) | (3 << 26)
#define OFS_SEQ5 (1 << 28) | (1 << 30)           
#define BSP_CFG_ROM_REG_OFS0 (OFS_SEQ1 | OFS_SEQ2 | OFS_SEQ3 | OFS_SEQ4 | OFS_SEQ5)           
#define BSP_CFG_ROM_REG_OFS1 (0xFFFFFEF8 | (1 << 2) | (3) |  (1 << 8))
#define BSP_CFG_ID_CODE_LONG_1 (0xFFFFFFFF)
#define BSP_CFG_ID_CODE_LONG_2 (0xFFFFFFFF)
#define BSP_CFG_ID_CODE_LONG_3 (0xFFFFFFFF)
#define BSP_CFG_ID_CODE_LONG_4 (0xFFFFFFFF)
#define BSP_CFG_MCU_VCC_MV (3300)
#define BSP_CFG_PARAM_CHECKING_ENABLE (1)
#define BSP_CFG_RTOS (1)
#define BSP_CFG_ASSERT (0)
#define BSP_CFG_ERROR_LOG (0)
#endif /* BSP_CFG_H_ */
