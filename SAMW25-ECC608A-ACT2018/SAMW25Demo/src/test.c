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
#include <ctype.h>

#include "test.h"
#include "driver_init.h"
#include "hal_i2c_m_sync.h"
#include "hpl_i2c_m_sync.h"

#include "bsd/socket.h"
#include "ssl/ssl.h"

#include "winc_main.h"

#include "ssl/crypt.h"

// Types and definitions
// ---------------------------------------------------------------------------

// Variables
// ---------------------------------------------------------------------------
//static char m_test_host[50] = "www.google.com";
static char m_test_host[50] = "api.arrowconnect.io";
static uint16_t m_test_port = 443;
uint8_t get_msg[255];
uint8_t rx_msg[50];
int mysock=-1;
static bool m_print_data=false;

// Local prototypes
// ---------------------------------------------------------------------------
static void _test_i2c_errors(int8_t err);

// Public functions
// ---------------------------------------------------------------------------

void test_i2cbus()
{
	uint32_t ret;
	struct _i2c_m_msg msg;
	uint8_t buf[1];
	uint8_t x;
	uint16_t y;
	uint8_t devices[10];
	uint8_t devices_index=0;
	
	printf("Testing I2C bus\n");
	
	memset(devices,0,sizeof(devices));
	devices_index=0;
	
	buf[0] = 0;
	msg.addr = 0x00;
	msg.buffer = buf;
	msg.flags = I2C_M_RD;
	msg.len = sizeof(buf);
	
	// Print header
	printf("Addr:");
	for(x=0;x<16;x+=2)
	{
		printf(" %X",x);	
	}
	printf("\n");
	
	// Print each address by blocks of 16
	for(y=0x00;y<0x100;y+=0x10)
	{
		printf("0x%X:",y);
		for(x=0;x<16;x+=2)
		{
			msg.addr = ( ((uint8_t)y)+x )>>1;
			#if 0
			ret = i2c_m_sync_transfer(&I2C_0, &msg);
			#else
			i2c_m_sync_set_slaveaddr(&I2C_0,msg.addr,I2C_M_SEVEN);
			ret = i2c_m_sync_cmd_read(&I2C_0,0,buf,sizeof(buf));
			#endif
			printf(" %s", ((ret==I2C_OK || ret==I2C_ACK)?" A": ((ret==I2C_NACK)?" .":" ?")  ) );
			
			// Save to list
			if(ret==I2C_OK)
			{
				devices[devices_index] = (msg.addr<<1);
				devices_index++;
				if(devices_index>=sizeof(devices))
				{
					devices_index = sizeof(devices)-1;
					printf("device list overflow!!\n");
				}
			}
		}
		printf("\n");
	}
	
	for(x=0;x<devices_index;x++)
	{
		printf("Device %d: x%X (dec: %d)\n",x,devices[x],devices[x]);
	}
	
	
	return;
}

void test_info()
{
	printf("Test server info:\n");
	printf(" host: %s\n",m_test_host);
	printf(" port: %d\n",m_test_port);
	printf(" sock: %d\n",mysock);
	printf(" print: %d\n",m_print_data);
	return;
}

void test_set_host(char * newhost)
{
	strncpy(m_test_host,newhost,sizeof(m_test_host));
	printf("New host is: %s\n",m_test_host);
	return;
}
void test_set_port(uint16_t port)
{
	m_test_port = port;
	printf("New port is: %d\n",m_test_port);
	return;
}
void test_toggle_print()
{
	m_print_data = !m_print_data;
	printf("Print data is %s\n", ((m_print_data)?"ON":"OFF") );
}
int test_network()
{
	struct hostent * hostents;
	winc_states_e st;
	uint32_t addr;
	struct sockaddr_in sin;
	int8_t ret;
	int len,y;
	int total_bytes=0;
	
	
	winc_rx_reset();
	
	printf("Testing network...\n");
	
	// Make sure connected and socket is closed
	st = winc_get_state();
	if(st!=WINC_CONNECTED){
		printf("Must be connected first!\n");
		return -1;
	}
	test_closenetwork();
	
	printf("Socket GET / from %s:%d\n",m_test_host,m_test_port);
	
	// Get the IP for this host
	hostents = gethostbyname(m_test_host);
	if(hostents==NULL)
	{
		printf("Can't look up host %s\n",m_test_host);
		test_closenetwork();
		return -1;
	}
	addr = *((uint32_t*)(hostents->h_addr));
	printf("Host %s IP ",m_test_host);
	winc_print_ip(addr);
	
	// Get a new socket
	mysock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	printf("Socket is %d\n",mysock);
	if(mysock<0)
	{
		printf("Error with sockets\n");
		test_closenetwork();
		return -1;
	}
	
	// Connect to the IP address
	memset(&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = *((uint32_t*)(hostents->h_addr));
	sin.sin_port = htons(m_test_port);
	sin.sin_family = AF_INET;
#if 0
	// Print buffer
	{
		int x;
		printf("struct sockaddr: ");
		for(x=0;x<sizeof(struct sockaddr);x++) printf("x%X,", ((uint8_t*)&sin)[x] ); printf("\n");
	}
#endif
	
	ret = connect(mysock, (struct sockaddr*)&sin,  sizeof(struct sockaddr_in));
	if(ret!=0)
	{
		printf("Connect error %d\n",ret);
		test_closenetwork();
		return -1;
	}
	ssl_connect(mysock);
	printf("Connect successful\n");
	
	// Format an HTTP GET request	
	len = sprintf((char*)get_msg,"GET / HTTP/1.1\r\nHost:%s\r\n",m_test_host);
	ret = ssl_send(mysock,(char*)get_msg,len);
	if(ret<0){
		// TODO: Did send really send?
		printf("Sending error, %d\n",ret);
		return -1;
	}
	
#if 1
	len = sprintf((char*)get_msg,"x-myinfo-api: %s\r\n","5501f50fdc62aee5d04dbd6a58b68b781ee2aaade8ad1eb24b1e4e77cb282ae2");
	ret = ssl_send(mysock,(char*)get_msg,len);
	if(ret<0){
		// TODO: Did send really send?
		printf("Sending error, %d\n",ret);
		return -1;
	}
#endif

#if 1
	len = sprintf((char*)get_msg,"\r\n");
	ret = ssl_send(mysock,(char*)get_msg,len);
	if(ret<0){
		// TODO: Did send really send?
		printf("Sending error, %d\n",ret);
		return -1;
	}
#endif	
	
	// Read all the data
	#if 1
	memset(rx_msg, 0, sizeof(rx_msg));
	do 
	{
		rx_msg[0]=0;
		len = ssl_recv(mysock, (char*)rx_msg, sizeof(rx_msg)-1);
		if(len>0) rx_msg[len]=0;
		total_bytes += len;
		// Print the buffer?
		if(m_print_data && len>0)
		{
			#if 1
			printf("%s",rx_msg);
			#else
			printf("Got %d bytes\n",len);
			#endif
		}
		
	}
	while(len>0);
	printf("Last status: %d\n",winc_get_last_rx_status());
	#endif
	
	test_closenetwork();
	
	printf("Total bytes %d\n",total_bytes);
	
	
	return total_bytes;
}
void test_closenetwork()
{
	if(mysock>=0)
	{
		printf("Closing socket %d\n",mysock);
		ssl_close(mysock);
		soc_close(mysock);
		mysock=-1;
	}
	return;
}

char teststr[256];
char sha256_out[32];
uint8_t hmac_out[32];
char hmac_out_string[65];
char demoapi[] = "5501f50fdc62aee5d04dbd6a58b68b781ee2aaade8ad1eb24b1e4e77cb282ae2";
char demosecret[] = "ARAzUzRzekFwRTNACBQYUx89LlZyImhKFVloHUVMDw8EGRxxSCckFgdFPysAAWJCLDgMdkstZzw3GGVqNHxXcno5Iz54LRBSKy0TaCBwNndkfQNdD38KAA==";
void test_signature()
{
	int x;
	char timestamp[] =  "2016-04-12T14:28:36.218Z";
	char ver[] = "1";
	
	printf("Testing signature\n");	

// Test 1 - Full string
	memset(teststr,0,sizeof(teststr));
	strcat(teststr,"POST\n");
	strcat(teststr,"/api/v1/kronos/gateways\n");
	strcat(teststr,"age=30\n");
	strcat(teststr,"firstname=Jane\n");
	strcat(teststr,"lastname=Doe\n");
	strcat(teststr,"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
	
	printf("Input string>>>>%s<<<<\n",teststr);
	
	sha256(sha256_out,teststr,strlen(teststr));
	sha256_out[sizeof(sha256_out)-1]=0;
	printf("Hash: "); for(x=0;x<sizeof(sha256_out);x++){ printf("%02X",sha256_out[x]); }printf("\n");

// Test 2 - Piecemeal string
#define ARG(S)		S,sizeof(S)-1
	memset(teststr,0,sizeof(teststr));
	sha256_init();
	sha256_chunk(ARG("POST\n"));
	sha256_chunk(ARG("/api/v1/kronos/gateways\n"));
	sha256_chunk(ARG("age=30\n"));
	sha256_chunk(ARG("firstname=Jane\n"));
	sha256_chunk(ARG("lastname=Doe\n"));
	sha256_chunk(ARG("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"));
	sha256_fin(sha256_out);
	printf("Hash: "); for(x=0;x<sizeof(sha256_out);x++){ printf("%02X",sha256_out[x]); }printf("\n");
		
	printf("API key: %s\n",demoapi);
	printf("Secret: %s\n",demosecret);
	
	
	printf("Key1: %s\n",demosecret);
	
// Test 3 - "Step 2.1" of signature
	hmac256((char*)hmac_out, demoapi,strlen(demoapi), demosecret, strlen(demosecret));
	for(x=0;x<sizeof(hmac_out);x++) sprintf(&(hmac_out_string[x*2]),"%02x",hmac_out[x]);
	hmac_out_string[sizeof(hmac_out_string)-1]=0;
	printf("Key2: %s\n",hmac_out_string);

	//printf("strlen(hmac_out_string) = %d\n",strlen(hmac_out_string));

// Test 4 - "Step 2.2" of signature
	hmac256((char*)hmac_out, timestamp,strlen(timestamp), hmac_out_string, strlen(hmac_out_string));
	for(x=0;x<sizeof(hmac_out);x++) sprintf(&(hmac_out_string[x*2]),"%02x",hmac_out[x]);
	hmac_out_string[sizeof(hmac_out_string)-1]=0;
	printf("Key3: %s\n",hmac_out_string);

// Test 5 - "Step 2.3" of signature piecemeal
	hmac256_init((char*)ver, strlen(ver));
	
	hmac256_chunk((char*)hmac_out_string, 20);
	hmac256_chunk((char*)&(hmac_out_string[20]), 20);
	hmac256_chunk((char*)&(hmac_out_string[40]), 20);
	hmac256_chunk((char*)&(hmac_out_string[60]), 4);
	
	hmac256_fin((char *)hmac_out);
	for(x=0;x<sizeof(hmac_out);x++) sprintf(&(hmac_out_string[x*2]),"%02x",hmac_out[x]);
	hmac_out_string[sizeof(hmac_out_string)-1]=0;
	printf("Key4: %s\n",hmac_out_string);


// Test 6 - "Step 2.3" of signature piecemeal
	memset(teststr,0,sizeof(teststr));
	strcat(teststr,"5a2d3589ffb15fab720069fbd26fd8e8311a1c7047e5899608faff450df6d7dc\n");
	strcat(teststr,"5501f50fdc62aee5d04dbd6a58b68b781ee2aaade8ad1eb24b1e4e77cb282ae2\n");
	strcat(teststr,timestamp);
	strcat(teststr,"\n");
	strcat(teststr,ver);

#if 0
	hmac256_init((char*)teststr, strlen(teststr));

	hmac256_chunk((char*)hmac_out_string, 20);
	hmac256_chunk((char*)&(hmac_out_string[20]), 20);
	hmac256_chunk((char*)&(hmac_out_string[40]), 20);
	hmac256_chunk((char*)&(hmac_out_string[60]), 4);
#else
	hmac256_init((char*)hmac_out_string, strlen(hmac_out_string));
	hmac256_chunk((char*)teststr, 30);
	hmac256_chunk((char*)&(teststr[30]), strlen(teststr)-30);
#endif

	hmac256_fin((char *)hmac_out);
	for(x=0;x<sizeof(hmac_out);x++) sprintf(&(hmac_out_string[x*2]),"%02x",hmac_out[x]);
	hmac_out_string[sizeof(hmac_out_string)-1]=0;
	printf("Result: %s\n",hmac_out_string);

// Test 7 - Fixed
	memset(teststr,0,sizeof(teststr));
	strcat(teststr,"08730ac4e59c42f5ddd93d782cfdf960a67c5e819c49dfef602a30cfac414412\n");
	strcat(teststr,"c445596cfd3fd6bc40b4e78691674361ab2450c9a03c55603e32dbdb334d8480\n");
	strcat(teststr,"2000-01-00T01:01:01.000Z\n");
	strcat(teststr,"1");
	
	
	hmac256_init("b095c62b1336e85a4a31b82e66414fca2c0a936dd9809ef36bfc4cd531349e9f", 64);
	hmac256_chunk((char*)teststr, 30);
	hmac256_chunk((char*)&(teststr[30]), strlen(teststr)-30);
	hmac256_fin((char *)hmac_out);
	for(x=0;x<sizeof(hmac_out);x++) sprintf(&(hmac_out_string[x*2]),"%02x",hmac_out[x]);
	hmac_out_string[sizeof(hmac_out_string)-1]=0;
	printf("TEST: %s\n",hmac_out_string);
	
}


// Private functions
// ---------------------------------------------------------------------------


static void _test_i2c_errors(int8_t err)
{
	 switch(err){
	 case I2C_OK: printf("I2C_OK\n"); return;
	 case I2C_ACK: printf("I2C_ACK\n"); return;
	 case I2C_NACK: printf("I2C_NACK\n"); return;
	 case I2C_ERR_ARBLOST: printf("I2C_ERR_ARBLOST\n"); return;
	 case I2C_ERR_BAD_ADDRESS: printf("I2C_ERR_BAD_ADDRESS\n"); return;
	 case I2C_ERR_BUS: printf("I2C_ERR_BUS\n"); return;
	 case I2C_ERR_BUSY: printf("I2C_ERR_BUSY\n"); return;
	 case I2c_ERR_PACKAGE_COLLISION: printf("I2c_ERR_PACKAGE_COLLISION\n"); return;
	 default: break;
	 }
	 printf("Unknown error %d\n",err);	
}


// EOF
