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

#include "bsd/socket.h"
#include "winc_main.h"

//#define DEBUG
#include "debug.h"

#include <string.h>

struct hostent s_hostent;
char * host_ips[2] = {0,0};
uint32_t addr0;

void sdk_socket_init()
{
	host_ips[0] = (char*)&addr0;
	s_hostent.h_addr_list = (char**)host_ips;
	s_hostent.h_addr = (char*)&addr0;
	
}

struct hostent* gethostbyname(const char *name)
{
	uint32_t tmp32;
	uint32_t * ptruint32;
	
	tmp32 = winc_gethostbyname(name);
	//printf("Ret %d\n",tmp32);
	if(tmp32==0) return NULL;
	
	addr0 =  tmp32;
	
	//ptruint32 = (uint32_t*)(s_hostent.h_addr_list[0]);
	//*(ptruint32) = tmp32; 
	return &s_hostent;
}

int socket(int protocol_family, int socket_type, int protocol)
{
	int sock;
	
	// IF the sockets are TCP, then request 
	if(socket_type==SOCK_STREAM)
		sock = winc_socket_TLS(protocol_family,socket_type,protocol);
	else
		sock = winc_socket(protocol_family,socket_type,protocol);
	
	return sock;
}

void soc_close(int socket)
{
	winc_close(socket);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
	//printf("Sock send %d bytes\n",len);
	return (ssize_t)winc_send(sockfd, (void*)buf, (uint16_t)len, (uint16_t)flags);
}

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen)
{
	struct winc_sockaddr tmp;
	
	// struct sockaddr has len, winc_sockaddr does not, so
	// copy the data
	tmp.sa_family = dest_addr->sa_family;
	memcpy(&(tmp.sa_data),&(dest_addr->sa_data),sizeof(tmp.sa_data));
	
	//printf("Sock sendto %d bytes\n",len);
	
	// TODO: Handle DGRAM and STREAM socks
	//if(type(sockfd)==SOCK_STREAM){
	//return (ssize_t)winc_send(sockfd, (void*)buf, (uint16_t)len, (uint16_t)flags);
	//}else{
	return (ssize_t)winc_sendto(sockfd, (void *)buf, (uint16_t)len, (uint16_t)flags,
		&tmp, (uint8_t)addrlen);
	//}
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
	int ret;
	//printf("Enter RECV\n");
	ret =  winc_recv(sockfd, buf, len, flags);
	//printf("Exit RECV\n");
	return ret;
}

ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen)
{
	//return (ssize_t) winc_recvfrom(sock, buf, size, 2000);
	return (ssize_t)winc_recv(sock, buf, size, flags);
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
	struct winc_sockaddr tmp;
		
	// struct sockaddr has len, winc_sockaddr does not, so
	// copy the data
	tmp.sa_family = addr->sa_family;
	memcpy(&(tmp.sa_data),&(addr->sa_data),sizeof(tmp.sa_data));
	
	return winc_connect(sockfd,&tmp,addrlen);
}

// TODO: Fix
int setsockopt(int sockfd, int level, int optname,
               const void *optval, socklen_t optlen) {
  // FIXME implementation
  return 0;
}

int bind(int sock, const struct sockaddr *addr, socklen_t addrlen) {
  // FIXME implementation
  return 0;
}

int listen(int sock, int backlog) {
  // FIXME implementation
  return 0;
}

int accept(int sock, struct sockaddr *addr, socklen_t *addrlen) {
  // FIXME implementation
  return 0;
}

// EOF
