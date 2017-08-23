/*
 * Quadro.cpp
 *
 *  Created on: 7 рту. 2017 у.
 *      Author: ddemidov
 */

#include "Quadro.h"
#include <time/time.h>

#if !defined(USE_QUADRO_SHIELD)
typedef int __dummy_type;
#else
#include <debug.h>
extern "C" {
#include <arrow/utf8.h>
}

static char static_buffer[QUADRO_SIZE];
static Quadro *main_eth = NULL;
const char *basic_pattern[] = { "OK", "ERROR" };

Quadro::Quadro(): uart(p0, p1), buffer(static_buffer), _reset(p15) {
	uart.baud(115200);
	uart.format(8, SerialBase::None, 1);

	if ( !main_eth ) main_eth = this;
	else DBG("Quadro denied", false);
}

Quadro::~Quadro() {
	// TODO Auto-generated destructor stub
}

int Quadro::reset() {
	int wait_reboot = 1;
	_reset.write(1);
	msleep(100);
	_reset.write(0);
	while( wait_reboot ) {
		if ( read_line(this->buffer, 256, 30000) > 0 ) {
			if ( strstr(this->buffer, "Poweron") ) wait_reboot = 0;
		} else {
			DBG("reset quadro");
			_reset.write(1);
			msleep(100);
			_reset.write(0);
		}
	}
	return 0;
}

int Quadro::write(const char *buf, int size) {
//	DBG("write to uart AT+%s|r|n", buf);
	uart.puts("AT+");
	for (int i=0; i< size; i++) {
		uart.putc((int)buf[i]);
	}
	uart.puts("\r\n");
	return size;
}

int Quadro::read(char *buf, int chunk, int timeout_ms) {
	Timer t;
	int offset = 0;
	buf[offset] = 0x0;
	t.start();
	while( t.read_ms() < timeout_ms ) {
		while( uart.readable() ) {
			int c = uart.getc();
			buf[offset++] = (char)c;
//			DBG("q [%d, %c]", offset, (char)c);
			if ( offset == chunk ) return offset;
		}
		if ( !uart.readable() && offset ) return offset;
	}
//	DBG("q read %d", offset);
//	DBG("q read [%s]", buf);
	return ( offset ? offset : -1);
}

int Quadro::read_line(char *buf, int chunk, int timeout_ms) {
	int i = 0;
	buf[0] = 0x0;
	Timer t;
	t.start();
	while( t.read_ms() < timeout_ms ) {
//		if ( uart.scanf("%s", buf) == 1 ) return strlen(buf);
		while( uart.readable() ) {
			t.reset();
			buf[i++] = (char)uart.getc();
			buf[i] = 0;
			if ( ( i > 2 && buf[i-2] == '\r' && buf[i-1] == '\n' ) || i == chunk ) return i;
		}
	}
	return ( i ? i : -1 );
}

int Quadro::read_block(char *buf, int chunk, const char** pattern, int pttn_qnt, int timeout_ms) {
	int offset = 0;
	int c = 0;
	while ( ( c = read_line(buf + offset, chunk - offset, timeout_ms) ) >= 0 ) {
		if ( c > 0 ) {
			offset += c;
			buf[offset] = 0x0;
			for( int i = 0; i< pttn_qnt; i++ ) {
				if ( strstr(buf, pattern[i] ) ) return offset;
			}
			if ( offset >= chunk ) return offset;
		}
	}
	return offset?offset:-1;
}

int Quadro::gethostbyname(const char *host, uint32_t *ip) {
	int ret = snprintf(this->buffer, 256, "%s=%s", GETHOSTBYNAME, host);
	this->write(this->buffer, ret);
	if ( read_block(this->buffer, 256, basic_pattern, 2, 5000) > 0 ) {
		int ret = sscanf(this->buffer, "%8x",
				(unsigned int*)(ip));
		if ( ret != 1 ) return -1;
		return 0;
	}
	return -1;
}

int Quadro::getmacaddress(char *mac) {
	strcpy(this->buffer, GETMACADDR "=?");
	this->write(this->buffer, strlen(this->buffer));
	if ( read_block(this->buffer, 256, basic_pattern, 2, 5000) > 0 ) {
		char *mac_str = strstr(this->buffer, "MAC:");
		if ( !mac_str ) return -1;
		mac_str += 4;
		unsigned int mac32[6];
		int ret = sscanf(mac_str, "%2x.%2x.%2x.%2x.%2x.%2x", mac32+0, mac32+1, mac32+2, mac32+3, mac32+4, mac32+5);
		if ( ret != 6 ) return -1;
		for (int i=0; i < 6; i++) mac[i] = (char)(mac32[i]&0x00ff);
		return 0;
	}
	return -1;
}

int Quadro::wait_wifi_connect(const char *SSID, const char *pass, int sec) {
	DBG("SSID: %s", SSID);
	DBG("pass: %s", pass);
	int ret = snprintf(this->buffer, QUADRO_SIZE, "WIFI=%s,%s,%d",
				SSID, pass, sec);
	this->write(this->buffer, strlen(this->buffer));
	if ( read_block(this->buffer, 256, basic_pattern, 2, 30000) > 0 ) {
		if ( strstr(this->buffer, "CONNECT") ) return 0;
	}
	return -1;
}

// endpoint
int QUADRO_Endpoint::set_address(const char *ip, int port) {
	strcpy(this->ip, ip);
	this->port = port;
	return 0;
}

char *QUADRO_Endpoint::get_address() const {
	return (char*)this->ip;
}

uint16_t QUADRO_Endpoint::get_port() const {
	return this->port;
}

// socket
QUADRO_Socket::QUADRO_Socket() : sock(-1), buffer(main_eth->_buffer()), timeout(10000) {}
void QUADRO_Socket::init() {}

void QUADRO_Socket::set_blocking(bool, int timeout) {
	if ( sock < 0 ) return;
	this->timeout = timeout + 500;
	int ret = snprintf(this->buffer, QUADRO_SIZE, SETTIMEOUT "=%d,%d", this->sock, timeout);
	main_eth->write(this->buffer, ret);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) < 0 ) {
		DBG("QUADRO: set timeout %d error", this->timeout);
	}
}

QUADRO_Socket::~QUADRO_Socket() {
	if ( sock < 0 ) return;
	int ret = snprintf(this->buffer, QUADRO_SIZE, SOCKET_CLOSE "=%d", this->sock);
	main_eth->write(this->buffer, ret);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) < 0 ) {
		DBG("QUADRO: close sock %d error", this->sock);
	}
}

QUADRO_UDPSocket::QUADRO_UDPSocket() {
	this->init();
}

void QUADRO_UDPSocket::init() {
	if ( sock >= 0 ) return;
	strcpy(this->buffer, SOCKET_CREATE "=?,UDP");
	main_eth->write(this->buffer, strlen(this->buffer));
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		char *id = strstr(this->buffer, "ID");
		int ret = sscanf(id, "ID:%d", &this->sock);
		if ( ret != 1 ) {
			this->sock = -1;
		}
	}
}

int QUADRO_UDPSocket::sendTo(const QUADRO_Endpoint &ep, const char *buf, int len) {
	if ( sock < 0 ) return -1;
	int ret = snprintf(this->buffer, QUADRO_SIZE, SOCKET_SENDTO "=%d,%s,%d,%d\r",
			this->sock, ep.get_address(), ep.get_port(), len);
	hex_encode(this->buffer + ret, buf, len);
	len *= 2;
	memcpy(this->buffer + ret + len, "\r\n", 2);
	main_eth->write(this->buffer, ret + len +2);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		if ( strstr(this->buffer, "OK") ) return len;
	}
	return -1;
}

int QUADRO_UDPSocket::receiveFrom(QUADRO_Endpoint &ep, char *buf, int len) {
	if ( sock < 0 ) return -1;
	int ret = snprintf(this->buffer, QUADRO_SIZE, SOCKET_RECVFROM "=%d,%d\r\n", this->sock, len);
	main_eth->write(this->buffer, ret);
	int tot_size = 0;
	if ( ( tot_size = main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) ) > 0 ) {
		char *datalen = strstr(this->buffer, "DATALEN:");
		if ( !datalen ) return -1;
		datalen += 8;
		int dl = 0;
		sscanf(datalen, "%d", &dl);
		char *data = strstr(datalen, "\r\n");
		if ( !data ) return -1;
		data+=2;
		if ( tot_size < (data - this->buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/ ) {
			DBG("WARN: msg is corrupted! [%d|%d]", tot_size, (data - this->buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/);
			return -1;
		}
		hex_decode(buf, data, dl);
		return dl;
	}
	return -1;
}

QUADRO_TCPSocketConnection::QUADRO_TCPSocketConnection() {
	if ( sock >= 0 ) return;
	strcpy(this->buffer, SOCKET_CREATE "=?,TCP");
	main_eth->write(this->buffer, strlen(this->buffer));
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		char *id = strstr(this->buffer, "ID");
		int ret = sscanf(id, "ID:%d", &this->sock);
		if ( ret != 1 ) {
			this->sock = -1;
		}
	}
}

int QUADRO_TCPSocketConnection::get_msg_len() {
	char *datalen = strstr(this->buffer, "DATALEN:");
	if ( !datalen ) return -1;
	int dl = -1;
	datalen += 8;
	sscanf(datalen, "%d", &dl);
	return dl;
}

int QUADRO_TCPSocketConnection::get_msg_data(char *buf, int tot_size) {
	int dl = this->get_msg_len();
	char *data = strstr(this->buffer, "\r\n");
	if ( !data ) return -1;
	data+=2;
	if ( tot_size < (data - this->buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/ ) {
		DBG("WARN: msg is corrupted! [%d|%d]", tot_size, (data - this->buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/);
		return -1;
	}
	hex_decode(buf, data, dl);
	buf[dl] = 0x0;
	return dl;
}

int QUADRO_TCPSocketConnection::connect(const char *ip, uint16_t port) {
	int ret = snprintf(this->buffer, QUADRO_SIZE, SOCKET_CONNECT "=%d,%s,%d\r\n", this->sock, ip, port);
	main_eth->write(this->buffer, ret);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		return 0;
	}
	return -1;
}

int QUADRO_TCPSocketConnection::send(const char *buf, int len) {
	if ( sock < 0 ) return -1;
	int ret = snprintf(this->buffer, QUADRO_SIZE, SOCKET_SEND "=%d,%d\r",
			this->sock, len);
	hex_encode(this->buffer + ret, buf, len);
	len *= 2;
	memcpy(this->buffer + ret + len, "\r\n", 2);
	main_eth->write(this->buffer, ret + len +2);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		ret = get_msg_len();
		if ( strstr(this->buffer, "OK") ) return ret;
	}
	return -1;
}

int QUADRO_TCPSocketConnection::receive(char *buf, int len) {
	if ( sock < 0 ) return -1;
	int ret = snprintf(this->buffer, QUADRO_SIZE, SOCKET_RECV "=%d,%d\r\n", this->sock, len);
	main_eth->write(this->buffer, ret);
	int tot_size = 0;
	if ( ( tot_size = main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) ) > 0 ) {
		return get_msg_data(buf, tot_size);
	}
	return -1;
}

int QUADRO_TCPSocketConnection::receive_all(char *buf, int len) {
	if ( len > RECV_LIMIT ) {
		int tot_size = len;
		int offset = 0;
		while ( tot_size > 0 ) {
			int chunk = tot_size > RECV_LIMIT ? RECV_LIMIT : tot_size ;
			int ret = this->receive(buf + offset, chunk);
//			DBG("receive %d %d", offset, ret);
			if ( ret < 0 ) return offset;
			offset += ret;
			tot_size -= ret;
		}
		return offset;
	}
	return this->receive(buf, len);
}

int QUADRO_TCPSocketConnection::ssl_connect() {
	int ret = snprintf(this->buffer, QUADRO_SIZE, SSL_CONNECT "=%d", this->sock);
	main_eth->write(this->buffer, ret);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		return 0;
	}
	return -1;
}

int QUADRO_TCPSocketConnection::ssl_read(char *data, int len) {
	int ret = snprintf(this->buffer, QUADRO_SIZE, SSL_RECV "=%d,%d", this->sock, len);
	main_eth->write(this->buffer, ret);
	int tot_size = 0;
	if ( ( tot_size = main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) ) > 0 ) {
		return this->get_msg_data(data, tot_size);
	}
	return -1;
}

int QUADRO_TCPSocketConnection::ssl_read_all(char *data, int len) {
	if ( len > RECV_LIMIT ) {
		int tot_size = len;
		int offset = 0;
		while ( tot_size > 0 ) {
			int chunk = tot_size > RECV_LIMIT ? RECV_LIMIT : tot_size ;
			int ret = this->ssl_read(data + offset, chunk);
			if ( ret < 0 ) return offset;
			offset += ret;
			tot_size -= ret;
		}
		return offset;
	}
	return this->ssl_read(data, len);
}

int QUADRO_TCPSocketConnection::ssl_send(char* data, int length) {
	if ( sock < 0 ) return -1;
	int ret = snprintf(this->buffer, QUADRO_SIZE, SSL_SEND "=%d,%d\r", this->sock, length);
	hex_encode(this->buffer + ret, data, length);
	length *= 2;
	memcpy(this->buffer + ret + length, "\r\n", 2);
	main_eth->write(this->buffer, ret + length + 2);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		ret = get_msg_len();
		if ( strstr(this->buffer, "OK") ) return ret;
	}
	return -1;
}

int QUADRO_TCPSocketConnection::ssl_close() {
	int ret = snprintf(this->buffer, QUADRO_SIZE, SSL_CLOSE "=%d", this->sock);
	main_eth->write(this->buffer, ret);
	if ( main_eth->read_block(this->buffer, QUADRO_SIZE, basic_pattern, 2, this->timeout) > 0 ) {
		return 0;
	}
	return -1;
}

#endif

