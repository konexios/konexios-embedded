#include <debug.h>
#include <qcom_common.h>
#include <stdarg.h>
#include <qcom_utils.h>
#include <arrow/mem.h>

static char dbg_buffer[DBG_LINE_SIZE];

void dbg_line(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  *dbg_buffer = 0x0;
  qcom_vsnprintf(dbg_buffer, DBG_LINE_SIZE-2, fmt, args);
  strcat(dbg_buffer, "\r\n");
  A_PRINTF(dbg_buffer);
  va_end(args);
}

