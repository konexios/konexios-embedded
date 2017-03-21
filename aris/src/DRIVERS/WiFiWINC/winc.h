/*
 * winc.h
 *
 *  Created on: 9 февр. 2017 г.
 *      Author: ddemidov
 */

#ifndef DRIVERS_WIFIWINC_WINC_H_
#define DRIVERS_WIFIWINC_WINC_H_

#include <unint.h>
#include "socket/include/socket.h"
typedef size_t socklen_t;

int winc1500_init();
int net_sta_init();
int net_ap_init();

struct hostent *gethostbyname(char *host);

int socket(int protocol_family, int socket_type, int protocol);
void soc_close(int socket);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t recvfrom(int sock, void *buf, size_t size, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int setsockopt(int sockfd, int level, int optname,
             const void *optval, socklen_t optlen);

int bind(int sock, const struct sockaddr *addr, socklen_t addrlen);
int listen(int sock, int backlog);
int accept(int sock, struct sockaddr *addr, socklen_t *addrlen);

#endif /* DRIVERS_WIFIWINC_WINC_H_ */
