/* generated thread source file - do not edit */
#include "console_thread.h"

TX_THREAD console_thread;
void console_thread_create(void);
static void console_thread_func(ULONG thread_input);
/** Alignment requires using pragma for IAR. GCC is done through attribute. */
#if defined(__ICCARM__)
#pragma data_alignment = BSP_STACK_ALIGNMENT
#endif
static uint8_t console_thread_stack[6000] BSP_PLACE_IN_SECTION(".stack.console_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
sf_comms_ctrl_t g_sf_trace_ctrl;
sf_el_ux_comms_on_comms_ctrl_t g_sf_trace_ctrl_extend;
sf_el_ux_comms_on_comms_cfg_t g_sf_trace_cfg_extend =
{ .p_ctrl = &g_sf_trace_ctrl_extend, };
const sf_comms_cfg_t g_sf_trace_cfg =
{ .p_extend = &g_sf_trace_cfg_extend };
/* Instance structure to use this module. */
const sf_comms_instance_t g_sf_trace =
{ .p_ctrl = &g_sf_trace_ctrl, .p_cfg = &g_sf_trace_cfg, .p_api = &g_sf_el_ux_comms_on_sf_comms };
void console_thread_create(void)
{
    /* Initialize each kernel object. */

    tx_thread_create (&console_thread, (CHAR *) "Console Thread", console_thread_func, (ULONG) NULL,
                      &console_thread_stack, 6000, 6, 6, 1, TX_AUTO_START);
}

static void console_thread_func(ULONG thread_input)
{
    /* Not currently using thread_input. */
    SSP_PARAMETER_NOT_USED (thread_input);

    /* Initialize each module instance. */
    ssp_err_t ssp_err_g_sf_trace;
    ssp_err_g_sf_trace = g_sf_trace.p_api->open (g_sf_trace.p_ctrl, g_sf_trace.p_cfg);
    if (SSP_SUCCESS != ssp_err_g_sf_trace)
    {
        while (1)
            ;
    }

    /* Enter user code for this thread. */
    console_thread_entry ();
}
