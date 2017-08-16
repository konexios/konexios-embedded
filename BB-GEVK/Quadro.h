/*
 * Quadro.h
 *
 *  Created on: 7 рту. 2017 у.
 *      Author: ddemidov
 */

#ifndef QUADRO_H_
#define QUADRO_H_

#if !defined(USE_QUADRO_SHIELD)
typedef int __dummy_type;
#else
#include "mbed.h"
#include "Serial.h"

#define SOCKET_CREATE	"S.SOCK"
#define SOCKET_CLOSE	"S.SOCKC"
#define SOCKET_SENDTO	"S.SOCKST"
#define SOCKET_RECVFROM	"S.SOCKRF"
#define SOCKET_CONNECT	"S.SOCKCON"
#define SOCKET_RECV		"S.SOCKRV"
#define SOCKET_SEND		"S.SOCKSD"
#define GETHOSTBYNAME	"S.RHOST"
#define GETMACADDR		"S.MAC"
#define SETTIMEOUT		"S.SOCKTM"
#define SSL_CONNECT		"S.SSLCON"
#define SSL_SEND		"S.SSLSND"
#define SSL_RECV		"S.SSLRCV"
#define SSL_CLOSE		"S.SSLCLS"

#define RECV_LIMIT 200
#define QUADRO_SIZE 512

class Quadro {
	Serial uart;
	char *buffer;
	DigitalInOut _reset;
public:
	Quadro();
	virtual ~Quadro();
	char *_buffer() { return buffer; }
	int read(char *buf, int chunk, int timeout);
	int read_line(char *buf, int chunk, int timeout_ms);
	int read_block(char *buf, int chunk, const char** pattern, int pttn_qnt, int timeout_ms);
	int write(const char *buf, int size);
	int gethostbyname(const char *host, uint32_t *ip);
	int getmacaddress(char *mac);
	int reset();
	int wait_wifi_connect(const char *SSID, const char *pass, int sec);
};

class QUADRO_Endpoint {
	char ip[16];
	uint16_t port;
public:
	int set_address(const char *ip, int port);
	char *get_address() const;
	uint16_t get_port() const;
};

class QUADRO_Socket {
protected:
	int sock;
	char *buffer;
	int timeout;
public:
	QUADRO_Socket();
	virtual ~QUADRO_Socket();
	virtual void init();
	void set_blocking(bool, int timeout);
};

class QUADRO_UDPSocket : public QUADRO_Socket {
public:
	QUADRO_UDPSocket();
	virtual ~QUADRO_UDPSocket(){}
	virtual void init();
	void bind(uint16_t port) {}
	int sendTo(const QUADRO_Endpoint &ep, const char *buf, int len);
	int receiveFrom(QUADRO_Endpoint &ep, char *buf, int len);
};
class QUADRO_TCPSocketConnection : public QUADRO_Socket {
	int get_msg_len();
	int get_msg_data(char *buf, int tot_size);
public:
	QUADRO_TCPSocketConnection();
	virtual ~QUADRO_TCPSocketConnection(){}
	int send(const char *buf, int len);
	int receive(char *buf, int len);
	int receive_all(char *buf, int len);
	int connect(const char *ip, uint16_t port);
	int ssl_connect();
	int ssl_read(char *data, int len);
	int ssl_read_all(char *data, int len);
	int ssl_send(char* data, int length);
	int ssl_close();
};
#endif

#endif /* QUADRO_H_ */
