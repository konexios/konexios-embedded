/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#include <hal_i2c_m_sync.h>
#include <hal_usart_sync.h>

extern struct i2c_m_sync_desc I2C_0;
extern struct usart_sync_descriptor USART_0;
extern struct usart_sync_descriptor EDBG_COM;

void driver_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
