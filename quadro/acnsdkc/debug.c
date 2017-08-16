#include "debug.h"
#include <stdarg.h>
#include <string.h>
#include "wiced.h"

static char dbg_buf[DBG_LINE_SIZE];

void dbg_line(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    dbg_buf[0] = '\0';
    vsnprintf(dbg_buf, DBG_LINE_SIZE-2, fmt, args);
    strcat(dbg_buf, "\r\n");
    //int dbg_len = strlen(dbg_buf);
    WPRINT_APP_INFO( (dbg_buf) );
    va_end(args);
}

