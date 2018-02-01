/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <silex/silex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <debug.h>
#include <arrow/utf8.h>

static char static_buffer[QUADRO_SIZE];
const char *basic_pattern[] = { "OK", "ERROR" };

static int silex_data_mode = 0;

int silex_read_block(
        char *buf,
        int chunk,
        const char **pattern,
        int pttn_qnt,
        int timeout_ms) {
    int offset = 0;
    int c = 0;
    if ( silex_data_mode ) {
        int size = chunk < silex_data_mode ? chunk : silex_data_mode;
        offset = uart_recv_timeout(
                    buf,
                    size,
                    timeout_ms);
        silex_data_mode -= offset;
    } else {
        while ( ( c = silex_read_line(buf + offset, chunk - offset, timeout_ms) ) >= 0 ) {
            if ( c > 0 ) {
                offset += c;
                buf[offset] = 0x0;
                for( int i = 0; i< pttn_qnt; i++ ) {
                    if ( strstr(buf, pattern[i] ) ) return offset;
                }
                if ( offset >= chunk ) return offset;
            }
        }
    }
    return offset?offset:-1;
}

int silex_write(const char *buf, int size) {
    uart_send("AT+", 3);
    uart_send(buf, size);
    uart_send("\r\n", 2);
    return size;
}


int silex_read(char *buf, int chunk, int timeout_ms) {
    int offset = 0;
    offset = uart_recv_timeout(buf, chunk, timeout_ms);
    if ( offset <= 0 )
        return -1;
    else
        return offset;
}

int silex_gethostbyname(const char *host, uint32_t *ip) {
    int ret = snprintf(static_buffer, 256, "%s=%s", GETHOSTBYNAME, host);
    silex_write(static_buffer, ret);
    if ( silex_read_block(static_buffer, 256, basic_pattern, 2, 5000) > 0 ) {
        int ret = sscanf(static_buffer, "%8x",
                         (unsigned int*)(ip));
        if ( ret != 1 ) return -1;
        return 0;
    }
    return -1;
}

int silex_getmacaddress(char *mac) {
    strcpy(static_buffer, GETMACADDR "=?");
    silex_write(static_buffer, strlen(static_buffer));
    if ( silex_read_block(static_buffer, 256, basic_pattern, 2, 5000) > 0 ) {
        char *mac_str = strstr(static_buffer, "MAC:");
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

int silex_read_line(char *buf, int chunk, int timeout_ms) {
    if ( silex_data_mode ) return -1;
    int i = 0;
    buf[0] = 0x0;
    while ( silex_read(buf+i, 1, timeout_ms) == 1 ) {
        buf[++i] = 0;
        if ( ( i > 2 &&
               buf[i-2] == '\r' &&
               buf[i-1] == '\n' ) || i == chunk )
            return i;
    }
    return ( i ? i : -1 );
}

int silex_tcp_socket() {
    strcpy(static_buffer, SOCKET_CREATE "=?,TCP");
    silex_write(static_buffer, strlen(static_buffer));
    if ( silex_read_block(static_buffer, QUADRO_SIZE, basic_pattern, 2, 5000) > 0 ) {
        char *id = strstr(static_buffer, "ID");
        int sock = -1;
        int ret = sscanf(id, "ID:%d", &sock);
        if ( ret != 1 ) {
            return -1;
        }
        return sock;
    }
    return -1;
}

int silex_wait_wifi_connect(const char *SSID, const char *pass, int sec) {
    const char *power_pattern[] = { "UART" };
    int ret = snprintf(static_buffer, QUADRO_SIZE,
                       SILEX_RESET "=?" );
    static_buffer[ret] = 0;
    silex_write(static_buffer, strlen(static_buffer));
    if ( silex_read_block(static_buffer, 256,
                          power_pattern, 1, 30000) < 0 ) {
        return -2;
    }
    DBG("SSID: %s", SSID);
    DBG("pass: %s", pass);
    ret = snprintf(static_buffer, QUADRO_SIZE,
                       "WIFI=%s,%s,%d",
                       SSID, pass, sec);
    static_buffer[ret] = 0;
    silex_write(static_buffer, strlen(static_buffer));
    if ( silex_read_block(static_buffer, 256, basic_pattern, 2, 30000) > 0 ) {
        if ( strstr(static_buffer, "CONNECT") ) return 0;
    }
    return -1;
}

int silex_tcp_connect(int sock, const char *ip, uint16_t port, int timeout_ms) {
    int ret = snprintf(static_buffer, QUADRO_SIZE, SOCKET_CONNECT "=%d,%s,%d\r\n", sock, ip, port);
    silex_write(static_buffer, ret);
    if ( silex_read_block(static_buffer,
                          QUADRO_SIZE,
                          basic_pattern,
                          2,
                          timeout_ms) > 0 ) {
        return 0;
    }
    return -1;
}

static int silex_get_msg_len(const char *buf) {
    char *datalen = strstr(buf, "DATALEN:");
    if ( !datalen ) return -1;
    int dl = -1;
    datalen += 8;
    if ( sscanf(datalen, "%d", &dl) != 1 ) return -1;
    return dl;
}

int silex_get_msg_data(char *buf, int tot_size) {
    int dl = silex_get_msg_len(static_buffer);
    char *data = strstr(static_buffer, "\r\n");
    if ( !data ) return -1;
    data += 2;
    if ( tot_size < (data - static_buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/ ) {
        DBG("WARN: msg is corrupted! [%d|%d]", tot_size, (data - static_buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/);
        return -1;
    }
    hex_decode(buf, data, dl);
    buf[dl] = 0x0;
    printf("dl %d\r\n", dl);
    return dl;
}

static int silex_get_msg_len2(const char *buf) {
    char *datalen = strstr(buf, "DATALEN:");
    if ( !datalen ) return -1;
    int dl = -1;
    datalen += 8;
    if ( sscanf(datalen, "%d", &dl) != 1 ) return -1;
    return dl;
}

int silex_get_msg_data2(char *buf, int tot_size) {
    int dl = silex_get_msg_len2(static_buffer);
    char *data = strstr(static_buffer, "\r\n");
    if ( !data ) return -1;
    data += 2;
    if ( tot_size < (data - static_buffer) + dl + 2/*\r\n*/ + 6 /*ok*/ ) {
        DBG("WARN2: msg is corrupted! [%d|%d]", tot_size, (data - static_buffer) + dl + 2/*\r\n*/ + 6 /*ok*/);
        return -1;
    }
    memcpy(buf, data, dl);
    buf[dl] = 0x0;
    printf("dl %d\r\n", dl);
    return dl;
}


int silex_tcp_send(int sock, const char *buf, int len, int timeout_ms) {
    int ret = snprintf(static_buffer, QUADRO_SIZE,
                       SOCKET_SEND "=%d,%d\r",
                       sock, len);
    hex_encode(static_buffer + ret, buf, len);
    len *= 2;
    memcpy(static_buffer + ret + len, "\r\n", 2);
    silex_write(static_buffer, ret + len +2);
    if ( silex_read_block(static_buffer, QUADRO_SIZE,
                              basic_pattern, 2, timeout_ms) > 0 ) {
        ret = silex_get_msg_len(static_buffer);
        if ( strstr(static_buffer, "OK") ) return ret;
    }
    return -1;
}

int silex_tcp_receive(int sock, char *buf, int len, int timeout_ms) {
    printf("recv %d\r\n", len);
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SOCKET_RECV "=%d,%d\r\n",
                       sock, len);
    silex_write(static_buffer, ret);
    int tot_size = 0;
    if ( ( tot_size = silex_read_block(static_buffer, QUADRO_SIZE, basic_pattern, 2, timeout_ms) ) > 0 ) {
        return silex_get_msg_data(buf, tot_size);
    }
    return -1;
}

int silex_tcp_receive_all(int sock, char *buf, int len, int timeout_ms) {
    if ( len > RECV_LIMIT ) {
        int tot_size = len;
        int offset = 0;
        while ( tot_size > 0 ) {
            int chunk = tot_size > RECV_LIMIT ? RECV_LIMIT : tot_size ;
            int ret = silex_tcp_receive(sock, buf + offset, chunk, timeout_ms);
            if ( ret < 0 ) return offset;
            offset += ret;
            tot_size -= ret;
        }
        return offset;
    }
    return silex_tcp_receive(sock, buf, len, timeout_ms);
}

int silex_close(int sock) {
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SOCKET_CLOSE "=%d", sock);
    silex_write(static_buffer, ret);
    if ( silex_read_block(static_buffer, QUADRO_SIZE, basic_pattern, 2, 5000) < 0 ) {
        DBG("QUADRO: close sock %d error", sock);
        return -1;
    }
    return 0;
}

void silex_set_timeout(int sock, int timeout_ms) {
    timeout_ms += 500;
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SETTIMEOUT "=%d,%d",
                       sock, timeout_ms);
    silex_write(static_buffer, ret);
    if ( silex_read_block(static_buffer,
                          QUADRO_SIZE,
                          basic_pattern, 2, timeout_ms) < 0 ) {
        DBG("QUADRO: set timeout %d error", timeout_ms);
    }
}

int silex_ssl_connect(int sock, int timeout_ms) {
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SSL_CONNECT "=%d",
                       sock);
    silex_write(static_buffer, ret);
    if ( silex_read_block(static_buffer,
                          QUADRO_SIZE,
                          basic_pattern,
                          2,
                          timeout_ms) > 0 ) {
        return 0;
    }
    return -1;
}

int silex_ssl_read(int sock, char *buf, int len, int timeout_ms) {
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SSL_RECV "=%d,%d",
                       sock,
                       len);
    silex_write(static_buffer, ret);
    int tot_size = 0;
    int datalen = 0;
    if ( ( tot_size = silex_read_line(static_buffer, 20, timeout_ms) ) > 0 ) {
        datalen = silex_get_msg_len(static_buffer);
        if ( datalen < 0 ) return -1;
        int need_to_reed = datalen;
        int red = 0;
        while ( need_to_reed > 0) {
            int r = uart_recv_timeout(
                        static_buffer + red,
                        need_to_reed,
                        timeout_ms);
            if ( r <= 0 ) {
                DBG("uart %d %d", need_to_reed, datalen);
                return -2;
            }
            need_to_reed -= r;
            red += r;
        }
        memcpy(buf, static_buffer, datalen);
        buf[datalen] = 0x0;
    }
    if ( silex_read_block(static_buffer,
                          RECV_LIMIT,
                          basic_pattern,
                          2, timeout_ms) > 0 ) {
        if ( strstr(static_buffer, "OK") ) return datalen;
        return -1;
    }
    return -1;
}

int silex_ssl_read_all(int sock, char *data, int len, int timeout_ms) {
    if ( len > RECV_LIMIT ) {
        int tot_size = len;
        int offset = 0;
        while ( tot_size > 0 ) {
            int chunk = tot_size > RECV_LIMIT ? RECV_LIMIT : tot_size ;
            int ret = silex_ssl_read(sock, data + offset, chunk, timeout_ms);
            if ( ret < 0 ) return offset;
            offset += ret;
            tot_size -= ret;
        }
        printf("offset = %d\r\n", offset);
        return offset;
    }
    return silex_ssl_read(sock, data, len, timeout_ms);
}

int silex_ssl_send(int sock, char* data, int length, int timeout_ms) {
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SSL_SEND "=%d,%d\r",
                       sock,
                       length);
    hex_encode(static_buffer + ret, data, length);
    length *= 2;
    memcpy(static_buffer + ret + length, "\r\n", 2);
    silex_write(static_buffer, ret + length + 2);
    if ( silex_read_block(static_buffer,
                          QUADRO_SIZE,
                          basic_pattern,
                          2, timeout_ms) > 0 ) {
        ret = silex_get_msg_len(static_buffer);
        if ( strstr(static_buffer, "OK") ) return ret;
    }
    return -1;
}

int silex_ssl_close(int sock) {
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SSL_CLOSE "=%d",
                       sock);
    silex_write(static_buffer, ret);
    if ( silex_read_block(static_buffer,
                          QUADRO_SIZE,
                          basic_pattern,
                          2, 5000) > 0 ) {
        return 0;
    }
    return -1;
}

int silex_udp_socket() {
    int sock = -1;
    strcpy(static_buffer, SOCKET_CREATE "=?,UDP");
    silex_write(static_buffer, strlen(static_buffer));
    if ( silex_read_block(static_buffer,
                          QUADRO_SIZE,
                          basic_pattern,
                          2, 5000) > 0 ) {
        char *id = strstr(static_buffer, "ID");
        int ret = sscanf(id, "ID:%d", &sock);
        if ( ret != 1 ) {
            sock = -1;
        }
    }
    return sock;
}

int silex_sendto(int sock,
                 const char *dst_ip,
                 uint16_t dst_port,
                 const char *buf,
                 int len,
                 int timeout_ms) {
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SOCKET_SENDTO "=%d,%s,%d,%d\r",
            sock, dst_ip, dst_port, len);
    hex_encode(static_buffer + ret, buf, len);
    len *= 2;
    memcpy(static_buffer + ret + len, "\r\n", 2);
    silex_write(static_buffer, ret + len +2);
    if ( silex_read_block(static_buffer,
                              QUADRO_SIZE,
                              basic_pattern,
                              2, timeout_ms) > 0 ) {
        if ( strstr(static_buffer, "OK") ) return len;
    }
    return -1;
}

int silex_receivefrom(int sock,
                      char *dst_ip,
                      uint16_t dst_port,
                      char *buf,
                      int len,
                      int timeout_ms) {
    int ret = snprintf(static_buffer,
                       QUADRO_SIZE,
                       SOCKET_RECVFROM "=%d,%d\r\n",
                       sock,
                       len);
    silex_write(static_buffer, ret);
    int tot_size = 0;
    if ( ( tot_size = silex_read_block(static_buffer,
                                       QUADRO_SIZE,
                                       basic_pattern,
                                       2, timeout_ms ) ) > 0 ) {
        char *datalen = strstr(static_buffer, "DATALEN:");
        if ( !datalen ) return -1;
        datalen += 8;
        int dl = 0;
        sscanf(datalen, "%d", &dl);
        char *data = strstr(datalen, "\r\n");
        if ( !data ) return -1;
        data+=2;
        if ( tot_size < (data - static_buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/ ) {
            DBG("WARN: msg is corrupted! [%d|%d]", tot_size, (data - static_buffer) + 2*dl + 2/*\r\n*/ + 6 /*ok*/);
            return -1;
        }
        hex_decode(buf, data, dl);
        return dl;
    }
    return -1;
}
