/*******************************************************************************
* Copyright (c) 2018 Arrow Electronics, Inc.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Apache License 2.0
* which accompanies this distribution, and is available at
* http://apache.org/licenses/LICENSE-2.0
*
* Contributors:
*     IoT Engineering Group - Arrow Electronics, Inc.
*******************************************************************************/
#include "defs.h"
#include <string.h>
#include "system.h"
#include "driver_init.h"
#include "hal_io.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "ac_data.h"
#include "console.h"
#include "winc_main.h"
#include "app.h"

// Types and definitions
// ---------------------------------------------------------------------------

// Variables
// ---------------------------------------------------------------------------

// Tick rate is configTICK_RATE_HZ
static uint32_t m_ticks=0;
static SemaphoreHandle_t xSerialWriteMutex;
static SemaphoreHandle_t xSerialReadMutex;

static uint32_t ticks_timing_start = 0;

uint32_t g_timestamp = 0;


// Local prototypes
// ---------------------------------------------------------------------------
static void gpio_init(void);
static void _sys_flash_cb_ready(struct flash_descriptor * descr);
static void _sys_flash_cb_error(struct flash_descriptor * descr);


// Public functions
// ---------------------------------------------------------------------------
void system_init()
{
	m_ticks = 0;
	
	xSerialWriteMutex = xSemaphoreCreateMutex();
	xSerialReadMutex = xSemaphoreCreateMutex();
			
	//flash_register_callback(&FLASH0, FLASH_CB_READY, _sys_flash_cb_ready);
	//flash_register_callback(&FLASH0, FLASH_CB_ERROR, _sys_flash_cb_error);

	gpio_init();
	
	return;
}
uint32_t system_getticks()
{
	return m_ticks;
}
uint8_t system_char_read(int fd)
{
	uint8_t tmp;
	if(fd!=1) return 0;
	xSemaphoreTake(xSerialReadMutex, portMAX_DELAY );
	while (ERR_TIMEOUT == io_read(&USART0.io, &tmp, 1)) {
		;
	}
	xSemaphoreGive(xSerialReadMutex);
	return tmp;
}

// Pulled from core_cm0plus.h
void system_SystemReset(void)
{
  __DSB();                                                          /* Ensure all outstanding memory accesses included
                                                                       buffered write are completed before reset */
  SCB->AIRCR  = ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) |
                 SCB_AIRCR_SYSRESETREQ_Msk);
  __DSB();                                                          /* Ensure completion of memory access */

  for(;;)                                                           /* wait until reset */
  {
    __NOP();
  }
}


// NOTE.NOTE.NOTE.NOTE.NOTE.NOTE.NOTE
//
// SAMD21G18A
// #define FLASH_SIZE            _UL_(0x00040000) /* 256 kB */
// #define FLASH_PAGE_SIZE       64
// #define FLASH_NB_OF_PAGES     4096
// #define FLASH_USER_PAGE_SIZE  64
// #define HMCRAMC0_SIZE         _UL_(0x00008000) /* 32 kB */
// #define FLASH_ADDR            _UL_(0x00000000) /**< FLASH base address */
// #define FLASH_USER_PAGE_ADDR  _UL_(0x00800000) /**< FLASH_USER_PAGE base address */
// #define HMCRAMC0_ADDR         _UL_(0x20000000) /**< HMCRAMC0 base address */
//
// NOTE.NOTE.NOTE.NOTE.NOTE.NOTE.NOTE

#define DATA_BLOCK_SIZE		512
#define DATA_BLOCK_PAGES	(DATA_BLOCK_SIZE/FLASH_PAGE_SIZE)
#define DATA_SAVE_ADDRESS	(FLASH_SIZE-((DATA_BLOCK_PAGES)*FLASH_PAGE_SIZE))
#define DATA_VALID_KEY		0xAB
#define DATA_VERSION		1
typedef struct{
	uint8_t key;
	uint8_t version;
	uint16_t size;
	uint32_t timestamp;	
}data_header_t ;
typedef struct{
	data_header_t header;
	char gw_name[32];
	char gw_HID[45];
	char gw_UID[32];
	bool gw_registered;

	char dev_name[32];
	char dev_HID[45];
	char dev_UID[32];
	bool dev_registered;

	char sdk_apikey[65];
	char sdk_secret[65];
	
	char wifi_ssid[64];
	char wifi_key[64];
	
	struct flags{
		unsigned debug_options	:1;	
		unsigned wifi_wpa		:1;
		unsigned autostart		:1;
	}flags_s;
	
}system_data_t;

uint8_t system_data_block[DATA_BLOCK_SIZE];

#if (DATA_BLOCK_SIZE&(FLASH_PAGE_SIZE-1)) != 0
#error "Must be mutltiple of page size"
#endif

#define pbuf(B,S)	do{int x; printf("Buffer at x%X:\n",(int)(B)); for(x=1;x<=(S);x++){ printf("%X,",(B)[x]); if(x&&(x%16)==0) printf("\n"); } printf("\n"); }while(0)
void printbuf(uint8_t * buf,int size){
	int x;
	printf("Buffer at x%X:\n",(int)(buf));
	for(x=1;x<=size;x++)
	{
		printf("%X,",buf[x]);
		if(x&&(x%16)==0) printf("\n");
	}
	printf("\n");
}

void system_loadvars()
{
	system_data_t * dataptr;
	int page=0;
	uint8_t * ptr;
	uint32_t ret;
			
	// SANITY CHECK
	if(sizeof(system_data_block) < sizeof(system_data_t))
	{
		printf("ERROR!!!! system_data_block < system_data_t. %d < %d\n",sizeof(system_data_block),sizeof(system_data_t));
		return;
	}
	printf("++ Load vars, block size %d, from addr x%X\n",DATA_BLOCK_SIZE,DATA_SAVE_ADDRESS);
	

	memset(system_data_block,0,sizeof(system_data_block));
	
	// Load the page here!!!
	for(page=0;page<DATA_BLOCK_PAGES;page++)
	{
		ptr = &(system_data_block[page*FLASH_PAGE_SIZE]);
		ret = flash_read(&FLASH0, (DATA_SAVE_ADDRESS+(page*FLASH_PAGE_SIZE)), ptr, FLASH_PAGE_SIZE);
		//printf("Read page %d, ret %d\n",page,ret);
	}
	if(console_engineering_get())
		printbuf(system_data_block,sizeof(system_data_block));
	// END LOAD
	
	// Copy over the data to the real values	
	dataptr = (system_data_t *)&system_data_block;
	
	if(dataptr->header.key != DATA_VALID_KEY)
	{
		printf("Error, data valid key failed, aborting load (x%X!=x%X)\n",dataptr->header.key,DATA_VALID_KEY);
		return;
	}
	
	printf("Data valid, version %d, size %d\n",dataptr->header.version,dataptr->header.size);

	ac_data_gw_name_set(dataptr->gw_name);
	ac_data_gw_HID_set(dataptr->gw_HID);
	ac_data_gw_UID_set(dataptr->gw_UID);
	ac_data_gw_registered_set(dataptr->gw_registered);

	ac_data_dev_name_set(dataptr->dev_name);
	ac_data_dev_HID_set(dataptr->dev_HID);
	ac_data_dev_UID_set(dataptr->dev_UID);
	ac_data_dev_registered_set(dataptr->dev_registered);
	
	ac_data_apikey_set(dataptr->sdk_apikey);
	ac_data_secret_set(dataptr->sdk_secret);
	
	winc_set_ssid(dataptr->wifi_ssid);
	winc_set_key(dataptr->wifi_key);
	if(dataptr->flags_s.wifi_wpa) winc_set_WPA2();
	else winc_set_OPEN();
	app_autostart_set(dataptr->flags_s.autostart);
		
	console_engineering_set(dataptr->flags_s.debug_options);

	return;
}

void system_savevars()
{
	system_data_t * dataptr;
	int page=0;
	uint8_t * ptr;
	uint32_t ret;
	
	// SANITY CHECK
	if(sizeof(system_data_block) < sizeof(system_data_t))
	{
		printf("ERROR!!!! system_data_block < system_data_t. %d < %d\n",sizeof(system_data_block),sizeof(system_data_t));
		return;
	}
	printf("++ Save vars, block size %d, to addr x%X\n",DATA_BLOCK_SIZE,DATA_SAVE_ADDRESS);
	
	// Copy all data from real values
	dataptr = (system_data_t *)&system_data_block;
	
	dataptr->header.key = DATA_VALID_KEY;
	dataptr->header.version = DATA_VERSION;
	dataptr->header.size = DATA_BLOCK_SIZE;
	dataptr->header.timestamp = 0x12345678;
		
	strncpy(dataptr->gw_name,ac_data_gw_name_get(),sizeof(dataptr->gw_name));
	strncpy(dataptr->gw_HID,ac_data_gw_HID_get(),sizeof(dataptr->gw_HID));
	strncpy(dataptr->gw_UID,ac_data_gw_UID_get(),sizeof(dataptr->gw_UID));
	dataptr->gw_registered = ac_data_gw_registered_get();
	
	strncpy(dataptr->dev_name,ac_data_dev_name_get(),sizeof(dataptr->dev_name));
	strncpy(dataptr->dev_HID,ac_data_dev_HID_get(),sizeof(dataptr->dev_HID));
	strncpy(dataptr->dev_UID,ac_data_dev_UID_get(),sizeof(dataptr->dev_UID));
	dataptr->dev_registered = ac_data_dev_registered_get();
	
	strncpy(dataptr->sdk_apikey,ac_data_apikey_get(),sizeof(dataptr->sdk_apikey));
	strncpy(dataptr->sdk_secret,ac_data_secret_get(),sizeof(dataptr->sdk_secret));
	
	
	strncpy(dataptr->wifi_ssid,winc_get_ssid(),sizeof(dataptr->wifi_ssid));
	strncpy(dataptr->wifi_key,winc_get_key(),sizeof(dataptr->wifi_key));
	dataptr->flags_s.wifi_wpa = winc_isset_WPA2();
	dataptr->flags_s.autostart = app_autostart_get();
	
	dataptr->flags_s.debug_options = console_engineering_get();
	
	// Write the data to the flash
	for(page=0;page<DATA_BLOCK_PAGES;page++)
	{
		ptr = &(system_data_block[page*FLASH_PAGE_SIZE]);
		ret = flash_write(&FLASH0, (DATA_SAVE_ADDRESS+(page*FLASH_PAGE_SIZE)), ptr, FLASH_PAGE_SIZE);
		//printf("Write page %d, ret %d\n",page,ret);
	}
	if(console_engineering_get())
		printbuf(system_data_block,sizeof(system_data_block));
	// END SAVE
	

	return;
}

uint32_t last_ticks = 0;
void system_update_time()
{
	uint32_t t = m_ticks;
	if( (t-last_ticks) >= 1000)
	{
		last_ticks=t;
		g_timestamp+=1;
	}
	return;
}

void system_timing_start()
{
	ticks_timing_start = m_ticks;
}
uint32_t system_timing_delta()
{
	return (m_ticks-ticks_timing_start);
	
}

void system_print_info()
{
	printf("System:\n");
	printf(" Firmware: %s\n",FIRMWARE_VERSION);
	printf(" Build: %s %s\n",__DATE__,__TIME__);
	printf(" m_ticks: %d\n",m_ticks);
	printf(" Data block: %d\n",DATA_BLOCK_SIZE);
	printf(" Data struct: %d\n",sizeof(system_data_t));
	return;
}


// Print memory map
void system_printmem(uint8_t * buf,int size)
{
	int x,y;
	// Header
	printf("Memory Dump:\n");
	for(x=0;x<52;x++){printf("-");} printf("\n");
	printf("Adr:");
	for(x=0;x<16;x++)
	{
		printf("%02X,",x);
	}
	printf("\n");
	for(x=0;x<52;x++){printf("-");} printf("\n");
	// Memory
	for(y=0;y<size;y+=16)
	{
		printf("x%02X:",y);
		for(x=0;x<16;x++)
		{
			printf("%02X,",buf[y+x]);
		}
		printf("\n");
	}
	
}

// Private functions
// ---------------------------------------------------------------------------
static void gpio_init(void)
{
	/* Configure the button*/
	gpio_set_pin_pull_mode(SW0, GPIO_PULL_UP);
	
	/* Configure the LED*/
	gpio_set_pin_level(LED0, 1);
}

static void _sys_flash_cb_ready(struct flash_descriptor *const descr)
{
	printf("Got flash ready callback\n");
	return;
}
static void _sys_flash_cb_error(struct flash_descriptor *const descr)
{
	printf("Got flash error callback\n");
	return;
}

int _write(int fd, char *str, int len)
{
	int32_t ret;
	if(fd!=1) return 0;
	
	xSemaphoreTake(xSerialWriteMutex, portMAX_DELAY );
	ret = io_write(&USART0.io, (const uint8_t *)str, len);
	xSemaphoreGive(xSerialWriteMutex);
	
	return ret;
}

void vApplicationTickHook( void )
{
	m_ticks++;
	return;
}
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName)
{
	printf("\n\n***Stack overflow '%s'\n",pcTaskName);
	while(1)
		nop();	
}
void vApplicationMallocFailedHook(void)
{
	char msg[] = "Failed malloc\n";
	_write(1,msg,sizeof(msg));
	while(1)
		nop();
}


// ISRs
// ---------------------------------------------------------------------------
void NMI_Handler( void )
{
	while(1)
		nop();
}
void HardFault_Handler( void )
{
	volatile uint32_t VTOR;
	volatile uint32_t PC;
	volatile uint32_t res;
	volatile uint32_t * StackPointer;
	
	char msg[] = "Hard fault\n";
	_write(1,msg,sizeof(msg));
	
	 __asm ("MOV %[result], R13"
	 : [result] "=r" (res)
     );
	
	StackPointer = (uint32_t*)res;
	
	VTOR =  SCB->VTOR;
	PC = *((uint32_t*)(res+0x18));
	
	while(1)
		nop();
		
	__ASM volatile("BKPT #01");
}


// EOF

