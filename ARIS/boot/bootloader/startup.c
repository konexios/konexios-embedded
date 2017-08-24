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
* File Name    : startup_S7G2.c
* Description  : Startup for the S7G2
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bsp_api.h"

/* Only build this file if this board is chosen. */
#if defined(BSP_MCU_GROUP_S7G2)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define BSP_VECTOR_TABLE_ENTRIES    (112)
#define BSP_PLACE_IN_SECTION(x) __attribute__ ((section(x))) __attribute__ ((__used__))
#define BSP_SECTION_VECTOR ".vectors"
// #define BSP_CFG_HEAP_BYTES 0x400
// #define BSP_CFG_STACK_PROCESS_BYTES 0x100

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Defines function pointers to be used with vector table. */
typedef void (* exc_ptr_t)(void);

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
void Reset_Handler(void);
void Default_Handler(void);
void NMI_Handler(void);

#include <../synergy/ssp/src/bsp/cmsis/Device/RENESAS/S7G2/Include/R7FS7G2x.h>
#include <../synergy/ssp/inc/bsp/cmsis/Include/core_cm4.h>

static uint32_t addr = 0x4000;  
static uint32_t size_new_app = 0;


/***********************************************************************************************************************
* Function Name: Reset_Handler
* Description  : MCU starts executing here out of reset. Main stack pointer is setup already.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void Reset_Handler (void)
{
    /* Call user application. */
    size_new_app = *(uint32_t *)(0x40100000);
// jump to 0x200000
  __ASM volatile ("cpsid i" : : : "memory");
  __ASM volatile ("dmb 0xF":::"memory");
  int i = 0;
  for ( i = 0; i<96; i++ ) R_ICU->IELSRn[i] = 0;

  if ( size_new_app > 0 && size_new_app < 0xf8000 ) {
  // download the flasher
    addr = 0x200000;
    __ASM volatile ("MSR msp, %0\n" : : "r" (0x2002A118) : "sp");
    __ASM volatile ("MSR psp, %0\n" : : "r" (0x2002B770) : "sp");
  } else {
  // load the app
    addr = 0x4000;
//    __ASM volatile ("MSR msp, %0\n" : : "r" (0x20009CF0) : "sp");
    __ASM volatile ("MSR psp, %0\n" : : "r" (0x1FFE30D0) : "sp");
  }
  SCB->VTOR = addr;
  int msp_reg = *(int *)(addr + 0);
  int JumpAddress = *(int *)(addr + 4);
  exc_ptr_t JumpFunction = (exc_ptr_t)(JumpAddress);
//  __ASM volatile ("cpsie i" : : : "memory");
  __ASM volatile ("dsb 0xF":::"memory");

  __ASM volatile ("MSR msp, %0\n" : : "r" (msp_reg) : "sp");
  JumpFunction();

    while (1)
    {
        /* Infinite Loop. */
    }
}

/***********************************************************************************************************************
* Function Name: Default_Handler
* Description  : Default exception handler.
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void Default_Handler (void)
{
    while(1);
}

void NMI_Handler(void) {
  while(1)
    ;
}

/* Stacks. */
/* Main stack */
static uint8_t g_main_stack[BSP_CFG_STACK_MAIN_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK) \
                                                      BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);

/* Process stack */
#if (BSP_CFG_STACK_PROCESS_BYTES > 0)
BSP_DONT_REMOVE static uint8_t g_process_stack[BSP_CFG_STACK_PROCESS_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_STACK) \
                                                                            BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif

/* Heap */
#if (BSP_CFG_HEAP_BYTES > 0)
BSP_DONT_REMOVE static uint8_t g_heap[BSP_CFG_HEAP_BYTES] BSP_PLACE_IN_SECTION(BSP_SECTION_HEAP) \
                                                          BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif

/* All system exceptions in the vector table are weak references to Default_Handler. If the user wishes to handle
 * these exceptions in their code they should define their own function with the same name.
 */
#if defined(__GNUC__)
#define WEAK_REF_ATTRIBUTE      __attribute__ ((weak, alias("Default_Handler")))
#endif

void NMI_Handler(void);                      //NMI has many sources and is handled by BSP
void HardFault_Handler                       (void) WEAK_REF_ATTRIBUTE;
void MemManage_Handler                       (void) WEAK_REF_ATTRIBUTE;
void BusFault_Handler                        (void) WEAK_REF_ATTRIBUTE;
void UsageFault_Handler                      (void) WEAK_REF_ATTRIBUTE;

/* Vector table. */
BSP_DONT_REMOVE const exc_ptr_t __Vectors[BSP_VECTOR_TABLE_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_VECTOR) =
{
    (exc_ptr_t)(&g_main_stack[0] + BSP_CFG_STACK_MAIN_BYTES),           /*      Initial Stack Pointer     */
    Reset_Handler,                                                      /*      Reset Handler             */
    NMI_Handler,                                                        /*      NMI Handler               */
    HardFault_Handler,                                                  /*      Hard Fault Handler        */
    MemManage_Handler,                                                  /*      MPU Fault Handler         */
    BusFault_Handler,                                                   /*      Bus Fault Handler         */
    UsageFault_Handler,                                                 /*      Usage Fault Handler       */
};

#endif
