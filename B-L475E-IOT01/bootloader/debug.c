#include "debug.h"
#include "usb_device.h"
#include <stdarg.h>
#include <string.h>
#include <usbd_cdc_if.h>


extern UART_HandleTypeDef console_uart;

static char dbg_buf[DBG_LINE_SIZE];

void dbg_line(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int dbg_len = vsnprintf(dbg_buf, DBG_LINE_SIZE-2, fmt, args);
    strcpy(dbg_buf + dbg_len, "\r\n");
    dbg_len += 2;
    if ( dbg_len > DBG_LINE_SIZE ) dbg_len = DBG_LINE_SIZE;
    dbg_buf[dbg_len] = 0x0;
    while (HAL_OK != HAL_UART_Transmit(&console_uart,
                                       dbg_buf,
                                       dbg_len,
                                       30000))
      ;
    va_end(args);
}

