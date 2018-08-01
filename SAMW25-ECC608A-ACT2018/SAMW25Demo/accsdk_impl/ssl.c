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
#include "ssl/ssl.h"
#include "winc_main.h"

#include <string.h>
//#define DEBUG
#include "debug.h"

#define strset(B,S)		(strncpy(B,S,sizeof(S)-1),0)


int ssl_connect(int sock)
{
	// Do nothing since the socket takes care of everything
	return 1;
}

int ssl_recv(int sock, char *data, int len)
{
	int ret;
	// Just pass to recv
	ret = (int)recv(sock, (void *)data, (size_t)len,0);
	//printf("SSL recv %d bytes\n",ret);	
	return ret;
}

int ssl_send(int sock, char* data, int len)
{
	// just pass to send
	//printf("SSL Send %d bytes\n",len);
	return (int)send(sock, (void*)data, (ssize_t)len, 0);
}

int ssl_close(int sock)
{
	// Do nothing since the socket will be closed with soc_close()
	return 0;
}
