#include "debug.h"
#include "FreeRTOS.h"
#include "usb_device.h"
#include <stdarg.h>
#include <string.h>
#include <semphr.h>
#include <usbd_cdc_if.h>
#include "cmsis_os.h"

static char dbg_buf[DBG_LINE_SIZE];
extern osMutexId consoleMutexHandle;

extern UART_HandleTypeDef console_uart;

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
//  if ( !xMutex ) {
//    xMutex = xSemaphoreCreateMutex();
//  }
//  if ( !xMutex ) return;
    if ( !consoleMutexHandle ) return;
    osMutexWait(consoleMutexHandle, -1);
//  if( xSemaphoreTake( xMutex, (TickType_t)10 ) == pdTRUE ) {
    va_list args;
    va_start(args, fmt);
    dbg_buf[0] = '\0';
    int dbg_len = vsnprintf(dbg_buf, DBG_LINE_SIZE-2, fmt, args);
    strcpy(dbg_buf+dbg_len, "\r\n");
    dbg_len += 2;
#if defined(USB_OTG_DEBUG)
    CDC_Transmit_FS((uint8_t*)dbg_buf, dbg_len);
#else
    while (HAL_OK != HAL_UART_Transmit(&console_uart,
                                       (uint8_t *)dbg_buf,
                                       dbg_len,
                                       30000))
      ;
#endif
    va_end(args);
    osMutexRelease(consoleMutexHandle);
//    xSemaphoreGive( xMutex );
//  }
}

