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

#include "driver_init.h"
#include "winc_config.h"
#include "winc_main.h"
#include "system.h"

#include "driver/source/nmbus.h"
#include "driver/source/nmspi.h"
//#include "driver/source/nmasic.h"
#include "driver/include/m2m_wifi.h"
#include "driver/include/m2m_ssl.h"

// WINC M2M socket functions
#include "socket/include/socket.h"

// FreeRTOS headers for semaphores
#include "FreeRTOS.h"
#include "semphr.h"

// Should be in bus_wrapper.h
extern void set_winc_spi_descriptor(struct spi_m_sync_descriptor *spi_inst);

// Types and definitions
// ---------------------------------------------------------------------------
#if 0
#define DEFAULT_SSID		"IoT_Lab"
#define DEFAULT_PSK_KEY		"DryCr33k$"
#else
#define DEFAULT_SSID		"Arrow Connect"
#define DEFAULT_PSK_KEY		"Arrowdem0123!"
#endif
// Variables
// ---------------------------------------------------------------------------
static char m_wifi_ssid[64] = DEFAULT_SSID;
static tuniM2MWifiAuth wifi_security_union;
static bool m_wifi_wpa = true;


// Flags for callbacks to allow network task to start
// new operations (connect, recv, etc)
static bool req_connect = false;
static bool req_recv = false;
static int req_recv_sock = -1;
static int current_packet = 0;

static winc_states_e m_winc_state = WINC_INIT;

static uint32_t last_hostname_ip = 0;

volatile static SemaphoreHandle_t m_hostname_sem;
static SemaphoreHandle_t m_socket_sem;
static SemaphoreHandle_t m_rx_sem;

// TODO: if we use the m_callback_mutex to block multiple calls
// to winc functions, do we reallt need this mutex for the 
// RX buffer???? We could add a take(m_callback_mutex) to the
// 
static SemaphoreHandle_t m_rx_mutex;

static SemaphoreHandle_t m_tx_sem;

static SemaphoreHandle_t m_callback_mutex;

// Since the RECV callback from the WINC does not
// buffer data, we need to have our own circular buffer
// Lots taken from https://github.com/pthrasher/c-generic-ring-buffer
#define RECV_BUFFER_SIZE	1536
typedef struct {
	 int size;
	 int start;
	 int end;
	 uint8_t elems[RECV_BUFFER_SIZE];
}ring_buffer_t;
#define BUFFINIT(BUFF)			do{memset((BUFF),0,sizeof(ring_buffer_t)); (BUFF)->size = RECV_BUFFER_SIZE;(BUFF)->start=0;(BUFF)->end=0;}while(0)
#define NEXTSTARTINDEX(BUFF)	(((BUFF)->start + 1) % ((BUFF)->size + 1))
#define NEXTENDINDEX(BUFF)		(((BUFF)->end + 1) % ((BUFF)->size + 1))
#define ISBUFFEREMPTY(BUFF)		((BUFF)->end == (BUFF)->start)
#define ISBUFFERFULL(BUFF)		(NEXTENDINDEX(BUFF) == (BUFF)->start)
#define BUFWRITE(BUFF, ELEM)	do{ (BUFF)->elems[(BUFF)->end] = (ELEM); (BUFF)->end = NEXTENDINDEX(BUFF); \
	    							if(ISBUFFEREMPTY(BUFF)){ (BUFF)->start = NEXTSTARTINDEX(BUFF); } }while(0)
#define BUFREAD(BUFF, ELEM)		do{(ELEM) = (BUFF)->elems[(BUFF)->start];(BUFF)->start = NEXTSTARTINDEX(BUFF);}while(0)
#define BUFSIZE(BUFF)			((BUFF)->size)
static int BUFFELEMENTS(ring_buffer_t * buf){
	if(ISBUFFEREMPTY(buf)) return 0;
	if(buf->end < buf->start) 
		return ((buf->end-buf->start)+buf->size);
	return (buf->end-buf->start);
}

static uint8_t m_tmp_rx_buffer[100];
static ring_buffer_t recv_buffer;
static int rx_bytes=0;
static int16_t last_rx_status=0;

static uint8_t m_connect_status=0;

#if 0
#define TAKE()	do{xSemaphoreTake(m_callback_mutex,portMAX_DELAY);printf("TAKE %s:%d\n",__FUNCTION__,__LINE__);}while(0)
#define GIVE()	do{printf("GIVE %s:%d\n",__FUNCTION__,__LINE__);xSemaphoreGive(m_callback_mutex);}while(0)
#else
#define TAKE()	xSemaphoreTake(m_callback_mutex,portMAX_DELAY)
#define GIVE()	xSemaphoreGive(m_callback_mutex)
#endif

// Local prototypes
// ---------------------------------------------------------------------------
static void _winc_set_io();
static void _winc_state_change(winc_states_e newstate);
static void _winc_wifi_cb(uint8 u8MsgEvent, void *pvMsg);
static void _winc_socket_callback(SOCKET sock, uint8 u8Msg, void *pvMsg);
static void _winc_DNS_resolve_callback(uint8 *pu8DomainName, uint32 u32ServerIP);
static void _winc_SSL_callback(uint8_t u8MsgType, void *pvMsg);
static void _winc_conn_err_str(uint8_t err);
static char * _winc_sock_err_str(int16_t err);

// Public Functions
// ---------------------------------------------------------------------------
void winc_init()
{
	tstrWifiInitParam params;
	int ret;
	
	m_hostname_sem = xSemaphoreCreateBinary();
	m_socket_sem = xSemaphoreCreateBinary();
	m_rx_sem =  xSemaphoreCreateBinary();
	m_rx_mutex = xSemaphoreCreateMutex();
	m_tx_sem = xSemaphoreCreateBinary();
	
	m_callback_mutex = xSemaphoreCreateMutex();
	
	req_recv = false;
	
	winc_rx_reset();
	
	memset(&wifi_security_union,0,sizeof(wifi_security_union));
	sprintf((char*)(wifi_security_union.au8PSK),DEFAULT_PSK_KEY);
	
	_winc_set_io();
	
	set_winc_spi_descriptor(&SPI_0);
	
	/* Initialize the BSP. */
	nm_bsp_init();
	
	/* Initialize Wi-Fi parameters structure. */
	memset((uint8_t *)&params, 0, sizeof(tstrWifiInitParam));

	params.pfAppWifiCb = _winc_wifi_cb;
		
	// Init library
	ret= m2m_wifi_init(&params);
	if(M2M_SUCCESS != ret)
	{
		while (1) {
			nop(); //ERROR
		}
	}
	
	socketDeinit();
	socketInit();
	registerSocketCallback(_winc_socket_callback, _winc_DNS_resolve_callback);
	
	
	// Configure the SSL stuff
	m2m_ssl_init(_winc_SSL_callback);
	//m2m_ssl_set_active_ciphersuites(SSL_ECC_ALL_CIPHERS)
	
	
	return;
}

winc_states_e winc_get_state()
{
	return m_winc_state;
}

// Primary task for network operations
void winc_network_task(void *p)
{
	while(1)
	{
		m2m_wifi_handle_events(NULL);
		
		
		switch(m_winc_state){
		case WINC_INIT:
			//printf("Handle wifi init\n");
			_winc_state_change(WINC_DISCONNECTED);
			break;
		case WINC_DISCONNECTED:
			if(req_connect)
			{
				uint8_t sec=M2M_WIFI_SEC_OPEN;
				void * pvAuthInfo = &wifi_security_union;
				if(m_wifi_wpa) sec=M2M_WIFI_SEC_WPA_PSK;
				_winc_state_change(WINC_CONNECTING);
				m2m_wifi_connect(m_wifi_ssid,strlen(m_wifi_ssid),sec,pvAuthInfo,M2M_WIFI_CH_ALL);
				req_connect = false;
			}
			break;
		case WINC_CONNECTED:
			if(req_recv)
			{
				int8_t ret;
				req_recv = false;
				//printf("New RECV requested\n");
				if(req_recv_sock>=0)
				{
					current_packet++;
					// Pass in a temporary RX buffer to the winc socket driver, so it gets loaded
					// locally
					TAKE();
					ret = wincsock_recv(req_recv_sock, m_tmp_rx_buffer, sizeof(m_tmp_rx_buffer), (1*MS_PER_SEC));
					GIVE();
				}
			}
			break;
		default: break;
		}
	}
}

void winc_wifi_connect()
{
	if(req_connect) return;
	if(m_winc_state!=WINC_DISCONNECTED)
	{
		printf("Already connected, disconnect first\n");
		return;
	}
	printf(" Connecting to %s...\n",m_wifi_ssid);
	req_connect = true;
	return;
}
void winc_wifi_disconnect()
{
	printf("WiFi disconnecting\n");
	TAKE();
	m2m_wifi_disconnect();
	GIVE();
	
	_winc_state_change(WINC_DISCONNECTED);
}
void winc_get_rssi()
{
	printf(" Requesting RSSI...\n");
	TAKE();
	m2m_wifi_req_curr_rssi();
	GIVE();
}
void winc_get_mac()
{
	uint8_t mac[6];
	int8_t x;
	TAKE();
	x = m2m_wifi_get_mac_address(mac);
	GIVE();
	
	printf("Mac ret %d\n",x);
	printf("MAC: ");
	for(x=0;x<sizeof(mac);x++) printf("%X:",mac[x]);
	printf("\n");
}
void winc_rx_reset()
{
	BUFFINIT(&recv_buffer);
	rx_bytes=0;
}
void winc_print_info()
{
	printf("WIFI:\n");
	printf(" SSID: %s\n",m_wifi_ssid);
	printf(" Key : %s\n",wifi_security_union.au8PSK);
	printf(" WPA2: %c\n",((m_wifi_wpa)?'Y':'N') );
	printf(" State: %d\n",m_winc_state);
}

void winc_print_ip(uint32_t addr)
{
	int x;
	printf("Address: ");
	for(x=0;x<4;x++)
	{
		if(x<3) printf("%d.", (addr>>(8*x))&0xff );
		else printf("%d", (addr>>(8*x))&0xff );
	}
	printf("\n");
	return;
}

// WINC Socket functions
// ---------------------------------------------------------------------------

uint32_t winc_gethostbyname(const char *name)
{
	int8_t ret;
	TickType_t ticks = (TICKS_PER_SEC*5);
	last_hostname_ip = 0;

	//system_timing_start();
	TAKE();
	//printf("winc_gethostbyname(%s)\n",name);
	// Do the async gethostname
	ret = wincsock_gethostbyname((uint8_t*)name);
	//printf("Waiting for gethostname\n");
	if(ret!=SOCK_ERR_NO_ERROR || xSemaphoreTake(m_hostname_sem, ticks )==pdFALSE)
	{
		GIVE();
		printf("winc_gethostbyname Error or timeout, ret = %d\n",ret);
		return 0;
	}
	//printf("winc_gethostbyname(%s) = x%08X\n",name,last_hostname_ip);
	//winc_print_ip(last_hostname_ip);
	GIVE();
	//printf("Timing took %d msec\n",system_timing_delta());
	
	// We have the hostname
	//return 0x11223344;
	return last_hostname_ip;
}

int winc_socket(int protocol_family, int socket_type, int protocol)
{
	int sock;
	
	TAKE();
	sock = wincsock_socket(protocol_family,socket_type,protocol);
	GIVE();
	
	// Clear the RX buffer
	// Take the mutex so the RX callback doesn't add more data
	xSemaphoreTake(m_rx_mutex,portMAX_DELAY);
	BUFFINIT(&recv_buffer);
	xSemaphoreGive(m_rx_mutex);
	
	return sock;
}

int winc_socket_TLS(int protocol_family, int socket_type, int protocol)
{
	int sock;
	int value;
	int8_t ret;
	
	TAKE();
	
	// TLS sockets
	sock = wincsock_socket(protocol_family,socket_type,protocol|SOCKET_FLAGS_SSL);
	if(sock<0)
	{
		printf("TLS socket create error\n");
		goto socket_TLS_exit;
	}
	value = 1;
	ret = wincsock_setsockopt(sock,SOL_SSL_SOCKET,SO_SSL_BYPASS_X509_VERIF,&value,sizeof(value));
	if(ret!=SOCK_ERR_NO_ERROR)
	{
		printf("setsockopt error %d\n",ret);
	}
	
	// Clear the RX buffer
	// Take the mutex so the RX callback doesn't add more data
	xSemaphoreTake(m_rx_mutex,portMAX_DELAY);
	BUFFINIT(&recv_buffer);
	xSemaphoreGive(m_rx_mutex);
	
socket_TLS_exit:
	GIVE();

	return sock;
}

int8_t winc_close(int sock)
{
	int8_t ret;
	
	TAKE();
	//printf("wincsock_close ret %d\n",sock);
	ret = wincsock_close((SOCKET)sock);
	GIVE();
	
	return ret;
}
int8_t winc_connect(int sock, struct winc_sockaddr *pstrAddr, uint8_t u8AddrLen)
{
	int8_t ret;
	TickType_t ticks = (TICKS_PER_SEC*5);
	
	
	TAKE();
	m_connect_status=0;
	ret = wincsock_connect(sock,(struct wincsock_sockaddr *)pstrAddr, u8AddrLen);
	if(ret!=SOCK_ERR_NO_ERROR)
		nop();
	GIVE();
	
	// Wait until the callback is done
	if(!xSemaphoreTake(m_socket_sem,ticks))
	{
		printf("wincsock_connect failed\n");
		return -1;
	}
	//printf("wincsock_connect ret %d\n",sock);
	return m_connect_status;
}
int winc_recv(int socksfd, void *buf, int len, int flags)
{
	int x,rxlen;
	int8_t ret;
	uint8_t * ptr;
	//TickType_t ticks = TICKS_PER_SEC;
	TickType_t ticks = portMAX_DELAY;

	//printf("Enter RECV\n");

	// Request a new packet
	req_recv = true;
	req_recv_sock = socksfd;
	
	ptr = (uint8_t*)buf;
	
	// Typically the recv(), recvfrom() functions are blocking, so
	// mimic that method


	// This is a bit confusing.  First we Take() the mutex to stop the
	// callback from loading new data, then we see if the ring buffer
	// is empty.  If it is, we need to wait for a new Give() on the
	// semaphore, HOWEVER we want to ignore any previous Give() of
	// the semaphore.  Thus, inside the mutex, we Take() the semaphore
	// with wait time of 0, so it will not block.  This will
	// 'reset' the semaphore, then we Give() the mutex, allowing
	// the callback to load the buffer, then we can truely Take()
	// the semaphore.
	//
	// The else condition is the buffer is >0, so just keep
	// processing.

// CRITICAL

	// Take the mutex so the RX callback doesn't add more data
	xSemaphoreTake(m_rx_mutex,portMAX_DELAY);
	
	// If the buffer is empty we need to wait
	if(ISBUFFEREMPTY(&recv_buffer))
	{
		// Since the buffer is empty, clear any earlier Give() of
		// the binary semaphore (Ticks wait = 0 to not block)
		#if 1
		xSemaphoreTake(m_rx_sem, 0 );
		#else
		printf("--Old semaphore clear was %d\n",xSemaphoreTake(m_rx_sem, 0 ));
		#endif
		
		// Release the mutex so the RX callback can
		// load the buffer
		xSemaphoreGive(m_rx_mutex);
		
		// Wait for the RX callback to Give() the semaphore, which
		// tells us there is more data
		if(xSemaphoreTake(m_rx_sem, ticks )==pdFALSE)
		{
			//printf("--Receive error or timeout\n");
			return -1;
		}
		//printf("--Took semaphore\n");
		
		// If the buffer is empty then the semaphore was given
		// because of an error code.
		if(ISBUFFEREMPTY(&recv_buffer))
		{
			//printf("--Empty buffer, is socket now closed?\n");
			return 0;	
		}
		
		// If there is no error		
		// Take the semaphore again since we are going
		// to remove data from buffer
		xSemaphoreTake(m_rx_mutex,portMAX_DELAY);		
	}
	
	// At this point we hold mutex no matter the case
	
	// Load all the data in to the real receiver buffer
	//printf("--Available: %d bytes\n",BUFFELEMENTS(&recv_buffer));
	rxlen = BUFFELEMENTS(&recv_buffer);
	if(rxlen>len) rxlen = len;
	for(x=0;x<rxlen;x++)
	{
		BUFREAD(&recv_buffer,ptr[x]);
	}
	//printf("--RECV RX %d bytes\n",rxlen);
	
	// Release the mutex so the RX callback
	// can load the buffer
	xSemaphoreGive(m_rx_mutex);

// END CRITICAL

	return rxlen;
}

int16_t winc_recvfrom(int sock, void *pvRecvBuf, uint16_t u16BufLen, uint32_t u32Timeoutmsec)
{
	return 0;
}


#define TRANSMIT_BLOCK_SIZE	32
#define TICK_LIMIT	(TICKS_PER_SEC*5)
int16_t winc_send(int sock, void *pvSendBuffer, uint16_t u16SendLength, uint16_t u16Flags)
{
	int16_t ret=0;
	TickType_t ticks = (TICKS_PER_SEC*5);
	//TickType_t ticks = portMAX_DELAY;

	//printf("sending %d bytes\n",u16SendLength);
	//printf("sending %d: %s\n",u16SendLength,(char*)pvSendBuffer);

	// Clear the semaphore
	while(xSemaphoreTake(m_tx_sem, 0)) printf("clear\n");

	TAKE();
	ret = wincsock_send(sock, pvSendBuffer, u16SendLength, u16Flags);
	GIVE();

	// Wait for send complete semaphore?
	// Wait for the TX callback to Give() the semaphore, which
	// tells us there is more data
	if(xSemaphoreTake(m_tx_sem, TICK_LIMIT )==pdFALSE)
	{
		printf("--Send error or timeout\n");
		ret = -1;
		goto send_exit;
	}
	//printf("Send done\n");
	
send_exit:
	return ret;
}

int16_t winc_sendto(int sock, void *pvSendBuffer, uint16_t u16SendLength, uint16_t flags,
					struct winc_sockaddr *pstrDestAddr, uint8_t u8AddrLen)
{
	int16_t ret=0;
	TickType_t ticks = (TICKS_PER_SEC*5);
	//TickType_t ticks = portMAX_DELAY;

	//printf("sending %d bytes\n",u16SendLength);
	//printf("sending %d: %s\n",u16SendLength,(char*)pvSendBuffer);

	// Clear the semaphore
	while(xSemaphoreTake(m_tx_sem, 0)) printf("clear\n");

	TAKE();
	ret = wincsock_sendto(sock,pvSendBuffer, u16SendLength, flags,(struct wincsock_sockaddr *)pstrDestAddr, u8AddrLen);
	GIVE();

	// Wait for send complete semaphore?
	// Wait for the TX callback to Give() the semaphore, which
	// tells us there is more data
	if(xSemaphoreTake(m_tx_sem, TICK_LIMIT )==pdFALSE)
	{
		printf("--Send error or timeout\n");
		ret = -1;
		goto sendto_exit;
	}
	//printf("Send done\n");
	
sendto_exit:
	return ret;
}

int16_t winc_get_last_rx_status()
{
	return last_rx_status;
}

void winc_abort()
{
	
	printf("Abort any RECV\n");

	#if 1
	xSemaphoreTake(m_rx_mutex,portMAX_DELAY);
	BUFFINIT(&recv_buffer);
	BUFWRITE(&recv_buffer,'\r');
	BUFWRITE(&recv_buffer,'\n');
	xSemaphoreGive(m_rx_mutex);
	#endif

	xSemaphoreGive(m_rx_sem);
}

void winc_reset()
{
	gpio_set_pin_level(WINC_PIN_RESET,true);
	delay_ms(10);
	gpio_set_pin_level(WINC_PIN_RESET,false);

	nm_spi_reset();	
	
	nm_bus_reset();
	
}

// WINC variable functions
// ---------------------------------------------------------------------------
void winc_set_ssid(char * ssid)
{
	strncpy(m_wifi_ssid,ssid,sizeof(m_wifi_ssid));
	return;
}
void winc_set_key(char * key)
{
	strncpy((char *)(wifi_security_union.au8PSK),key,sizeof(wifi_security_union.au8PSK));
	return;
}
void winc_set_WPA2()
{
	m_wifi_wpa = true;
	return;
}
void winc_set_OPEN()
{
	m_wifi_wpa = false;
	return;
}
char * winc_get_ssid()
{
	return m_wifi_ssid;
}
char * winc_get_key()
{
	return (char*)(wifi_security_union.au8PSK);
}
bool winc_isset_WPA2()
{
	return m_wifi_wpa;
}
bool winc_isset_OPEN()
{
	return (m_wifi_wpa==false);
}



// Private functions
// ---------------------------------------------------------------------------
static void _winc_set_io()
{
	gpio_set_pin_direction(WINC_PIN_CHIP_SELECT, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(WINC_PIN_CHIP_SELECT,false);
	gpio_set_pin_function(WINC_PIN_CHIP_SELECT, GPIO_PIN_FUNCTION_OFF);


	gpio_set_pin_direction(WINC_PIN_RESET, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(WINC_PIN_RESET,false);
	gpio_set_pin_function(WINC_PIN_RESET, GPIO_PIN_FUNCTION_OFF);


	gpio_set_pin_direction(WINC_PIN_WAKE, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(WINC_PIN_WAKE,false);
	gpio_set_pin_function(WINC_PIN_WAKE, GPIO_PIN_FUNCTION_OFF);
	
	gpio_set_pin_direction(WINC_PIN_EN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(WINC_PIN_EN,false);
	gpio_set_pin_function(WINC_PIN_EN, GPIO_PIN_FUNCTION_OFF);
	
	return;
}

static void _winc_state_change(winc_states_e newstate)
{
	//printf("Entering WINC state %d\n",newstate);
	m_winc_state = newstate;
	return;
}

// This is the WIFI state callback function to tell us if
// the device connected/disconnected, etc.
static void _winc_wifi_cb(uint8 u8MsgEvent, void *pvMsg)
{
TAKE();
	//printf("** WiFi callback: %d\n",u8MsgEvent);

	switch(u8MsgEvent){
	case M2M_WIFI_RESP_CURRENT_RSSI:
	{
		// Handle the RSSI info from the device
		sint8	*rssi = (sint8*)pvMsg;
		M2M_INFO("ch rssi %d\n",*rssi);
	}
	break;
	
	case M2M_WIFI_RESP_CON_STATE_CHANGED:
	{
		// Handle connection state changes
		tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
		//printf("Connection state changed, %d\n",pstrWifiState->u8CurrState);
		if(pstrWifiState->u8CurrState!=M2M_WIFI_CONNECTED)
		{
			if(m_winc_state!=WINC_DISCONNECTED) _winc_state_change(WINC_DISCONNECTED);
			printf("Disconnected\n");
			if(pstrWifiState->u8ErrCode!=0) _winc_conn_err_str(pstrWifiState->u8ErrCode);
		}else{
			printf("Successfully connected to %s\n",m_wifi_ssid);
			_winc_state_change(WINC_CONNECTED);
		}
	}
	break;
	
	case M2M_WIFI_RESP_CONN_INFO:
	{
		// Handle the network info from the device
		tstrM2MConnInfo		*pstrConnInfo = (tstrM2MConnInfo*)pvMsg;
		int8_t sig;
		
		sig = ((pstrConnInfo->s8RSSI+80)*100)/40;
		if(sig<0) sig=0;
		if(sig>100) sig=100;

		printf("\nCONNECTED AP INFO\n");
		printf("SSID             : %s\n",pstrConnInfo->acSSID);
		//printf("SEC TYPE         : %d\n",pstrConnInfo->u8SecType);
		printf("Signal Strength  : %d%% (%d)\n", sig,pstrConnInfo->s8RSSI);
		printf("Local IP Address : %d.%d.%d.%d\n",
		pstrConnInfo->au8IPAddr[0] , pstrConnInfo->au8IPAddr[1], pstrConnInfo->au8IPAddr[2],
		pstrConnInfo->au8IPAddr[3]);
	}
	break;

	case M2M_WIFI_REQ_DHCP_CONF:
		// Get the current AP information.
		m2m_wifi_get_connection_info();
		break;
	
	case M2M_WIFI_MAX_STA_ALL:
		printf("STA error\n");
		break;
	case M2M_WIFI_MAX_CONFIG_ALL:
		printf("CFG error\n");
		break;
	default:break;
	 }

GIVE();

	return;
}

//
// This is the socket callback function for send, receive functions, etc
// The RX buffer MUST be consumed in this function.  Once the RECV callback
// returns from this function is can be immediately overwritten, so it must
// be copied to a persistent buffer.  Information can be found at:
// https://community.atmel.com/forum/winc1500-recv-function-need-some-clarifications
// https://www.avrfreaks.net/forum/winc1500-partial-socket-receive-calls
//
static void _winc_socket_callback(SOCKET sock, uint8 u8Msg, void *pvMsg)
{
TAKE();
//printf("** Enter socket cb\n");
	//printf("\n** Socket callback, sock %d, msg x%X\n",sock,u8Msg);
	
	switch (u8Msg) {
	// Socket actions
	case SOCKET_MSG_BIND:
	case SOCKET_MSG_LISTEN:
	case SOCKET_MSG_DNS_RESOLVE:
	case SOCKET_MSG_ACCEPT:
		break;
	case SOCKET_MSG_CONNECT:
	{
		tstrSocketConnectMsg * status;
		status = (tstrSocketConnectMsg*)pvMsg;
		//printf("Got connect callback\n");
		if(status->s8Error!=0)
			printf("Connect error: %d:%s\n",status->s8Error,_winc_sock_err_str(status->s8Error));
		m_connect_status=status->s8Error;
		xSemaphoreGive(m_socket_sem);
		
	}
	break;

	// Receive data
	case SOCKET_MSG_RECVFROM:
	case SOCKET_MSG_RECV:
	{
		uint8_t * pu8;
		tstrSocketRecvMsg * status;
		int16_t x;
		
		status =(tstrSocketRecvMsg*)pvMsg;
		
		#if 0
		printf("status->s16BufferSize = %d\n",status->s16BufferSize);
		printf("status->strRemoteAddr.sin_port = %d\n",status->strRemoteAddr.sin_port);
		printf("status->u16RemainingSize = %d\n",status->u16RemainingSize);
		winc_print_ip(status->strRemoteAddr.sin_addr.s_addr);
		#endif
		
		// If size is less than zero, then this
		// is not a data callback, it is a status callback	
		if(status->s16BufferSize<0)
		{
			last_rx_status = status->s16BufferSize;
			// Give the semaphore to indicate an new status
			//printf("==Give semaphore (for status %d)\n",status->s16BufferSize);
			//printf("**RX status %d:%s\n",status->s16BufferSize,_winc_sock_err_str(status->s16BufferSize));
			xSemaphoreGive(m_rx_sem);
			
			goto socket_callback_exit;
		}
		last_rx_status=0;
			
		// NOTE: Must consume the data inside the callback!!!!!
		// https://community.atmel.com/forum/winc1500-recv-function-need-some-clarifications
		
		// Add all the data to the receive buffer
		#if 1
		if(status->s16BufferSize)
		{
			// Keep a count of total bytes received
			rx_bytes += status->s16BufferSize;

			pu8 = status->pu8Buffer;
			xSemaphoreTake(m_rx_mutex,portMAX_DELAY);
			for(x=0;x<status->s16BufferSize;x++)
			{
				BUFWRITE(&recv_buffer,pu8[x]);
			}
			//printf("==SOCKET RX %d for packet %d. %d tot\n",status->s16BufferSize,current_packet,rx_bytes);
			//printf("==Ring buffer %d of %d\n",BUFFELEMENTS(&recv_buffer),recv_buffer.size);
			xSemaphoreGive(m_rx_mutex);
		}
		#endif
		
		// If this is the end of the packet, we need
		// to request a new packet
		if(status->u16RemainingSize==0)
		{
			req_recv = true;
			req_recv_sock = sock;
		}

		// Finally, give the semaphore to indicate we have data
		//printf("==Give semaphore (for data)\n");
		xSemaphoreGive(m_rx_sem);
	}
	break;
		
	// Send data
	case SOCKET_MSG_SEND:
	case SOCKET_MSG_SENDTO:
		//printf("Send callback, u8Msg %d\n",pvMsg);
		xSemaphoreGive(m_tx_sem);
		//printf("Sem TX give\n");
		break;
		
	default: break;
	}

socket_callback_exit:

GIVE();
	return;
}

static void _winc_DNS_resolve_callback(uint8 *pu8DomainName, uint32 u32ServerIP)
{
	//printf("** Got DNS callback, domain %s\n",(char*)pu8DomainName);
	last_hostname_ip = u32ServerIP;
	xSemaphoreGive(m_hostname_sem);
	return;
}


static void _winc_SSL_callback(uint8_t u8MsgType, void *pvMsg)
{
	printf("** SSL callback, u8MsgType %d\n",u8MsgType);
	
	return;
}


static void _winc_conn_err_str(uint8_t err)
{
	switch(err){
	case M2M_ERR_SCAN_FAIL:
		printf("Scan fail, does AP exist?\n");
		break;
	case M2M_ERR_JOIN_FAIL:
		printf("Join fail\n");
		break;
	case M2M_ERR_AUTH_FAIL:
		printf("Auth fail, is key right?\n");
		break;
	case M2M_ERR_ASSOC_FAIL:
		printf("Assoc fail\n");
		break;
	case M2M_ERR_CONN_INPROGRESS:
		printf("In Progress\n");
		break;
	default:
		printf("Unknown error %d\n",err);
		break;
	}
}

static char * _winc_sock_err_str(int16_t err)
{
	switch(err){
		case SOCK_ERR_NO_ERROR: return "SOCK_ERR_NO_ERROR";
		case SOCK_ERR_INVALID_ADDRESS: return "SOCK_ERR_INVALID_ADDRESS";
		case SOCK_ERR_ADDR_ALREADY_IN_USE: return "SOCK_ERR_ADDR_ALREADY_IN_USE";
		case SOCK_ERR_MAX_TCP_SOCK: return "SOCK_ERR_MAX_TCP_SOCK";
		case SOCK_ERR_MAX_UDP_SOCK: return "SOCK_ERR_MAX_UDP_SOCK";
		case SOCK_ERR_INVALID_ARG: return "SOCK_ERR_INVALID_ARG";
		case SOCK_ERR_MAX_LISTEN_SOCK: return "SOCK_ERR_MAX_LISTEN_SOCK";
		case SOCK_ERR_INVALID: return "SOCK_ERR_INVALID";
		case SOCK_ERR_ADDR_IS_REQUIRED: return "SOCK_ERR_ADDR_IS_REQUIRED";
		case SOCK_ERR_CONN_ABORTED: return "SOCK_ERR_CONN_ABORTED";
		case SOCK_ERR_TIMEOUT: return "SOCK_ERR_TIMEOUT";
		case SOCK_ERR_BUFFER_FULL: return "SOCK_ERR_BUFFER_FULL";
		default: break;
	}
	return "SOCK_ERR_UNKNOWN";
}

// EOF
