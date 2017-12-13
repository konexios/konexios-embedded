/*
 * WiFi_c.h
 *
 *  Created on: 12 апр. 2017 г.
 *      Author: ddemidov
 */

#ifndef WIFI_C_H_
#define WIFI_C_H_

#include <bsd/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/type.h>

int get_wifi_mac_address(char *mac);

int wifi_gethostbyname(const char *addr, uint32_t *ip);

int wifi_socket(int protocol_family, int socket_type, int protocol);

int wifi_socket_close(int socket);

int wifi_socket_send(int socket, const void *buf, size_t len);

int wifi_socket_recv(int sockfd, void *buf, size_t len);

int wifi_socket_sendto(int socket, const void *buf, size_t len,
                       const struct sockaddr *dest_addr, socklen_t addrlen);

int wifi_socket_recvfrom(int socket, void *buf, size_t len,
                         struct sockaddr *src_addr, socklen_t *addrlen);

int wifi_socket_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int wifi_socket_setopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);

#if defined(USE_QUADRO_SHIELD)

int wifi_ssl_socket_connect(int sock);

int wifi_ssl_socket_read(int sock, char *data, int len);

int wifi_ssl_socket_write(int sock, char *data, int len);

int wifi_ssl_socket_close(int sock);

#endif // USE_QUADRO_SHIELD

#ifdef __cplusplus
}
#endif


#endif /* WIFI_C_H_ */
