
#include "wifi_module.h"

extern volatile Wifi_Status_Var status_flag;

extern wifi_bool Uartx_Rx_Processing;
extern buffer_td big_buff;
extern wifi_bool resume_receive_data;
extern wifi_bool AT_Cmd_Processing;
extern WiFi_AT_CMD_Response_t WiFi_Module_State;
extern volatile WiFi_WIND_State_TypeDef WiFi_WIND_State;
extern volatile uint8_t wifi_connected;
extern volatile uint32_t WIND64_count;
extern volatile uint8_t wifi_client_connected;
extern wifi_bool WiFi_Enabled;
extern wifi_bool Standby_Enabled;
extern wifi_bool Deep_Sleep_Enabled;
extern int write_size;
extern uint8_t WiFi_AT_Cmd_Buff[];

extern void callSpwfSADevice_init(void* object);
extern char callSpwfSADevice_getChar(void* object);
extern int  callSpwfSADevice_write(void* object, const char * cmd, uint16_t size);
extern void callSpwfSADevice_wakeup(void* object, int wake);
extern void callSpwfSADevice_reset(void* object, int reset); 
extern void callSpwfSADevice_rts(void* object, int rts);
extern void callSpwfSADevice_attach(wifi_bool attach);
extern void callSpwfSADevice_debug(void* object, const char * string);
extern void destroySpwfSADevice(void);
extern int callSpwfSADevice_read_rts(void* object);

extern void Stop_Timer(void);
extern void Start_Timer(void);

extern void Wifi_TIM_Handler(void);
extern void Wifi_SysTick_Isr(void);

void *spwf_dev;

void Wifi_scheduler(void);
void Rx_irq_handler(void);
void Wifi_ticker(void);

