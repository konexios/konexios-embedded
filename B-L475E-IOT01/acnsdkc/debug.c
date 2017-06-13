#include "debug.h"
#include "FreeRTOS.h"
#include "usb_device.h"
#include <stdarg.h>
#include <string.h>
#include <semphr.h>
#include <usbd_cdc_if.h>

static char dbg_buf[DBG_LINE_SIZE];
static SemaphoreHandle_t xMutex = NULL;

/*void DBG_Init() {
  if ( !xMutex ) {
    if( (xMutex = xSemaphoreCreateMutex()) == NULL ) {
      //Error_Handler();
      xMutex = NULL;
    } else {
      xSemaphoreTake(xMutex, 10);
      xSemaphoreGive(xMutex);
    }

  }
}*/

void dbg_line(const char *fmt, ...) {
  if ( !xMutex ) {
    xMutex = xSemaphoreCreateMutex();
  }
  if ( !xMutex ) return;
//  if( xSemaphoreTake( xMutex, (TickType_t)10 ) == pdTRUE ) {
    va_list args;
    va_start(args, fmt);
    dbg_buf[0] = '\0';
    vsnprintf(dbg_buf, DBG_LINE_SIZE-2, fmt, args);
    strcat(dbg_buf, "\r\n");
    int dbg_len = strlen(dbg_buf);
    CDC_Transmit_FS((uint8_t*)dbg_buf, dbg_len);
    va_end(args);
//    xSemaphoreGive( xMutex );
//  }
}

