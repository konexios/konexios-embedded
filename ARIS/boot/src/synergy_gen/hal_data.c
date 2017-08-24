/* generated HAL source file - do not edit */
#include "hal_data.h"
#if FLASH_CALLBACK_USED
#if defined(__ICCARM__)
#define NULL_WEAK_ATTRIBUTE
#pragma weak NULL                            = NULL_internal
#elif defined(__GNUC__)
#define NULL_WEAK_ATTRIBUTE       __attribute__ ((weak, alias("NULL_internal")))
#endif
void NULL(flash_callback_args_t * p_args) NULL_WEAK_ATTRIBUTE;
#endif
flash_ctrl_t g_flash0_ctrl;
const flash_cfg_t g_flash0_cfg =
{ .data_flash_bgo = false, .p_callback = NULL, .p_context = &g_flash0 };
/* Instance structure to use this module. */
const flash_instance_t g_flash0 =
{ .p_ctrl = &g_flash0_ctrl, .p_cfg = &g_flash0_cfg, .p_api = &g_flash_on_flash_hp };

#if FLASH_CALLBACK_USED
/*******************************************************************************************************************//**
 * @brief      This is a weak example callback function.  It should be overridden by defining a user callback function 
 *             with the prototype below.
 *               - void NULL(flash_callback_args_t * p_args)
 *
 *
 * @param[in]  p_args  Callback arguments used to identify what caused the callback.
 **********************************************************************************************************************/
void NULL_internal(flash_callback_args_t * p_args)
{
    /** Do nothing. */
}
#endif
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
const elc_instance_t g_elc =
{ .p_api = &g_elc_on_elc, .p_cfg = NULL };
const cgc_instance_t g_cgc =
{ .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
void g_hal_init(void)
{
    g_common_init ();
}
