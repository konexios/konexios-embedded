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
#ifndef __WINC_MAIN_H__
#define __WINC_MAIN_H__

typedef enum{
	WINC_INIT = 0,
	WINC_DISCONNECTED,
	WINC_CONNECTING,
	WINC_CONNECTED,
}winc_states_e;

struct winc_sockaddr {
	uint16_t sa_family;
	uint8_t sa_data[14];
};

void winc_init();
winc_states_e winc_get_state();
void winc_network_task(void *p);
void winc_wifi_connect();
void winc_wifi_disconnect();
void winc_get_rssi();
void winc_get_mac();
void winc_rx_reset();
void winc_print_info();
void winc_print_ip(uint32_t addr);

// WINC Socket functions
uint32_t winc_gethostbyname(const char *name);
int winc_socket(int protocol_family, int socket_type, int protocol);
int winc_socket_TLS(int protocol_family, int socket_type, int protocol);
int8_t winc_close(int sock);
int8_t winc_connect(int sock, struct winc_sockaddr *pstrAddr, uint8_t u8AddrLen);
int winc_recv(int sockfd, void *buf, int len, int flags);
int16_t winc_recvfrom(int sock, void *pvRecvBuf, uint16_t u16BufLen, uint32_t u32Timeoutmsec);
int16_t winc_send(int sock, void *pvSendBuffer, uint16_t u16SendLength, uint16_t u16Flags);
int16_t winc_sendto(int sock, void *pvSendBuffer, uint16_t u16SendLength, uint16_t flags,
						struct winc_sockaddr *pstrDestAddr, uint8_t u8AddrLen);
int16_t winc_get_last_rx_status();
void winc_abort();
void winc_reset();


// WINC variable functions
void winc_set_ssid(char * ssid);
void winc_set_key(char * password);
void winc_set_WPA2();
void winc_set_OPEN();
char * winc_get_ssid();
char * winc_get_key();
bool winc_isset_WPA2();
bool winc_isset_OPEN();


#endif


// EOF
