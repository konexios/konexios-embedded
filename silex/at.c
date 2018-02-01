/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <config.h>
#include <uart2.h>
#include <time/time.h>
#include <time/watchdog.h>
#include <bsd/socket.h>
#include <sys/mac.h>
#include <arrow/utf8.h>
#include <arrow_wlan.h>
#include <ssl/ssl.h>
#include <qcom/qcom_gpio.h>
#include <debug.h>
#include <arrow/storage.h>

#if defined(AT_COMMAND)
# if defined(AP_AT)
#  define CRLF "\r\n"
# else
#  define CRLF "\r\n"
# endif
#else
# define CRLF "\r\n\r\n"
#endif
#define CRLF_LEN (sizeof(CRLF)-1)
#define NLINE "\r\n"

#define POWERON NLINE "+WIND:1:Poweron" NLINE NLINE
#define WIFION "+WIND:2:WiFi" NLINE

#define BUFFER_LENGTH     (1524)

extern A_UINT8 currentDeviceId;

uint8_t tmp_buffer[BUFFER_LENGTH];

static int get_socket(const char *str, int *sock) {
    int ret = sscanf(str, "%d", sock);
    if ( ret != 1 ) return -1;
    return 0;
}

// find the ",<ip>,<port>" pattern
 char *get_ip_addr(const char *str, struct sockaddr_in *servaddr) {
    char *ip_str = strstr(str, ",");
    if ( !ip_str ) return NULL;
    ip_str++;
    unsigned int ip_addr[4];
    uint32_t ip = 0;
    int ret = sscanf(ip_str, "%u.%u.%u.%u", ip_addr, ip_addr+1, ip_addr+2, ip_addr+3);
    if ( ret != 4 ) return NULL;
    ip |= (ip_addr[0] & 0x00ff);
    ip |= ((ip_addr[1] & 0x00ff) << 8);
    ip |= ((ip_addr[2] & 0x00ff) << 16);
    ip |= ((ip_addr[3] & 0x00ff) << 24);
    char *port_str = strstr(ip_str, ",");
    port_str++;
    unsigned int port = 0;
    ret = sscanf(port_str, "%u", &port);
    if ( ret != 1 ) return NULL;
    servaddr->sin_addr.s_addr = htonl(ip);
    servaddr->sin_family = AF_INET;
    servaddr->sin_port = htons(port);
    return port_str;
}

static int get_len(char *str, uint32_t *len) {
    int ret = sscanf(str, "%u", (unsigned int*)len);
    if ( ret != 1 ) return -1;
    return 0;
}

#define DATALEN "DATALEN:"

#define INIT_SOCK_AND_CHECK(s, cmd) \
        int s = -1; \
        if ( get_socket(cmd, &s) < 0 ) return -1;

#define INIT_LEN_AND_CHECK(s, cmd) \
        uint32_t s = 0; \
        if ( get_len(cmd, &s) < 0 ) return -1;

#define INIT_PTR_AFTER_COMMA_AND_CHECK(s, cmd) \
        char *s = strstr(cmd, ","); \
        if ( !s ) return -1; \
        s++;

#define DECODE_DATA_AND_CHECK(tmp, cmd, datalen) \
        char *data_str = strstr(cmd, "\r"); \
        if ( !data_str ) return -1; \
        data_str++; \
        hex_decode((char*)tmp, data_str, datalen);

#define ADD_DATALEN(ans, len) \
        len = sprintf(ans, DATALEN "%d\r\n", (int)len); \
        ans[len] = 0x0;

#define ADD_DATA(ans, data, len) \
        uint32_t l = len; \
        ADD_DATALEN(ans, l); \
        ans += l; \
        hex_encode(ans, (char*)data, (int)len); \
        ans += len * 2; \
        strcpy(ans, "\r\n");

#define ADD_DATALEN2(ans, qnt, len) \
        qnt = sprintf(ans + qnt, "%s%d\r\n", DATALEN, (int)len); \
        ans[qnt] = 0x0;

#define ADD_DATA2(ans, qnt, data, len) \
        ADD_DATALEN2(ans, qnt, len); \
        memcpy(ans + qnt, (char*)data, (int)len); \
        strcpy(ans + qnt + len, "\r\n"); \
        qnt += len + 2;

int create_socket(const char *arg, char *ans, int *num) {
    int sock = -1;
    char *type = strstr(arg, ",");
    if ( !type ) {
        DBG("There is no sock type");
        return -1;
    }
    type++;
    if ( strncmp(type, "UDP", 3) == 0 ) {
        // create UDP
        sock = socket(AF_INET, SOCK_DGRAM, 0);
    } else if ( strncmp(type, "TCP", 3) == 0 ) {
        // create TCP
        sock = socket(AF_INET, SOCK_STREAM, 0);
    } else return -1;
    if ( sock < 0 ) return sock;
    int ret = sprintf(ans, "ID:%d", sock);
    ans[ret] = 0x0;
    if (num) *num = ret;
    return 0;
}

int close_socket(const char *cmd, char *ans, int *num) {
    SSP_PARAMETER_NOT_USED(ans);
    if ( num ) *num = 0;
    INIT_SOCK_AND_CHECK(sock, cmd);
    if ( sock >= 0 ) {
        DBG("close = %d", sock);
        soc_close(sock);
    }
    return 0;
}

int sendto_socket(const char *cmd, char *ans, int *num) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    struct sockaddr_in serveraddr;
    char *last_str = NULL;
    if ( ! ( last_str = get_ip_addr(cmd, &serveraddr) ) ) return -1;
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, last_str);
    INIT_LEN_AND_CHECK(datalen, len_str);
    DECODE_DATA_AND_CHECK(tmp_buffer, len_str, datalen);
    socklen_t serverlen = sizeof(serveraddr);
    int ret = sendto(sock, tmp_buffer, datalen, 0, (struct sockaddr*)&serveraddr, serverlen);
    if ( ret < 0 ) return -1;
    ADD_DATALEN(ans, ret);
    if ( num ) *num = ret;
    return 0;
}

int recvfrom_socket(const char *cmd, char *ans, int *num) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    struct sockaddr addr;
    socklen_t serverlen = sizeof(addr);
    int ret = recvfrom(sock, tmp_buffer, datalen, 0, &addr, &serverlen);
    if ( ret < 0 ) return -1;
    datalen = ret;
    ADD_DATA(ans, tmp_buffer, datalen);
    if ( num ) *num = datalen;
    return 0;
}


int get_host_by_name(const char *cmd, char *ans, int *num) {
    char *name_end = strstr(cmd, CRLF);
    char buffer[100];
    strncpy(buffer, cmd, name_end - cmd);
    buffer[name_end - cmd] = 0x0;
    struct hostent *serv = gethostbyname(buffer);
    if (serv) {
        int ret = sprintf(ans, "%08x", *(int*)serv->h_addr);
        ans[ret] = 0x0;
        if ( num ) *num = ret;
        return 0;
    }
    return -1;
}

int get_mac_addr(const char *cmd, char *ans, int *num) {
    SSP_PARAMETER_NOT_USED(cmd);
    unsigned char mac[8];
    if ( get_mac_address((char*)mac) < 0 ) { DBG("no mac"); return -1; }
    int ret = sprintf(ans, "MAC:%02x.%02x.%02x.%02x.%02x.%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    ans[ret] = 0;
    if ( num ) *num = ret;
    return 0;
}

int socket_connect(const char *cmd, char *ans, int *num) {
    SSP_PARAMETER_NOT_USED(ans);
    if ( num ) *num = 0;
    INIT_SOCK_AND_CHECK(sock, cmd);
    struct sockaddr_in serveraddr;
    if ( ! get_ip_addr(cmd, &serveraddr) ) return -1;
    socklen_t serverlen = sizeof(serveraddr);
    return connect(sock, (struct sockaddr*)&serveraddr, serverlen);
}

int socket_send(const char *cmd, char *ans, int *num) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    DECODE_DATA_AND_CHECK(tmp_buffer, len_str, datalen);
//    DBG("send [%d](%d)", sock, datalen);
    int ret = send(sock, tmp_buffer, datalen, 0);
    if ( ret < 0 ) return ret;
    ADD_DATALEN(ans, ret);
    if ( num ) *num = ret;
    return 0;
}

int socket_recv(const char *cmd, char *ans, int *num) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
//    DBG("recv [%d](%d)", sock, datalen);
    int ret = recv(sock, tmp_buffer, datalen, 0);
    if ( ret < 0 ) return -1;
    datalen = ret;
    ADD_DATA(ans, tmp_buffer, datalen);
    if ( num ) *num = datalen;
    return 0;
}

int socket_timeout(const char *cmd, char *ans, int *num) {
    SSP_PARAMETER_NOT_USED(ans);
    if ( num ) *num = 0;
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(time_str, cmd);
    INIT_LEN_AND_CHECK(timeout, time_str);
    struct timeval tv;
    tv.tv_sec =     (time_t)        ( timeout / 1000 );
    tv.tv_usec =    (suseconds_t)   (( timeout % 1000 ) * 1000);
    return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
}

int wifi_connect(const char *cmd, char *ans, int *num) {
    char ssid[32];
    char passphrase[32];
    A_INT32 security;
//    DBG("wc:-%s-", cmd);
    char *ssid_end = strstr(cmd, ",");
    strncpy(ssid, cmd, (size_t)(ssid_end - cmd));
    ssid[(ssid_end - cmd)] = 0x0;
    char *pass_start = ssid_end + 1;
    char *pass_end = strstr(pass_start, ",");
    strncpy(passphrase, pass_start, (pass_end - pass_start));
    passphrase[(pass_end - pass_start)] = 0x0;
    char *sec_start = pass_end + 1;
    sscanf(sec_start, "%d", (int*)&security);
    struct sec_t {
      A_UINT16 encr;
      A_UINT16 auth;
    } sec;
    if ( security == 4194308) {
        // FIXME quadro hack
        DBG("convert secure setting quadro -> qca")
        security = 262147;
    }
    memcpy(&sec, &security, 4);
    qcom_sec_set_passphrase(currentDeviceId, passphrase);
    qcom_sec_set_auth_mode(currentDeviceId, (A_UINT32)sec.auth);
    qcom_sec_set_encrypt_mode(currentDeviceId, (A_UINT32)sec.encr);
    A_UINT32 test;
    qcom_sec_get_auth_mode(currentDeviceId, &test);
    DBG("auth %d", test);

    qcom_sec_get_encrypt_mode(currentDeviceId, &test);
    DBG("secr %d", test);

    DBG("SSID: [%s](%d)", ssid, (int)(ssid_end - cmd));

    qcom_sec_get_passphrase(currentDeviceId, passphrase);
    DBG("PASS: [%s](%d)", passphrase, (int)(pass_end - pass_start));
    arrow_connect_ssid(currentDeviceId, ssid);

    qcom_get_ssid(currentDeviceId, ssid);
    DBG("SSID: [%s]", ssid);

    int count = 100; // 10 sec
    A_UINT32 ip = 0;

    do {
        if ( count-- <= 0 ) break;
      A_UINT32 pr;
      A_STATUS st = qcom_ip_address_get(currentDeviceId, &ip, &pr, &pr);
      (void)(st);
      if (!ip) qcom_thread_msleep(100);
    } while(!ip);

    if ( ip ) {
        strcpy(ans, "CONNECT");
        if ( num ) *num = strlen(ans);
        DBG(WIFION);
    } else {
        qcom_disconnect(currentDeviceId);
//        qcom_sta_reconnect_stop(currentDeviceId);
        return -1;
    }
    return 0;
}

int ssl_connect_sock(const char *cmd, char *ans, int *num) {
    SSP_PARAMETER_NOT_USED(ans);
    if ( num ) *num = 0;
    INIT_SOCK_AND_CHECK(sock, cmd);
    return ssl_connect(sock);
}

int ssl_send_sock(const char *cmd, char *ans, int *num) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    DECODE_DATA_AND_CHECK(tmp_buffer, len_str, datalen);
    int ret = ssl_send(sock, (char*)tmp_buffer, datalen);
    if ( ret < 0 ) return ret;
    ADD_DATALEN(ans, ret);
    if ( num ) *num = ret;
    return 0;
}

int ssl_recv_sock(const char *cmd, char *ans, int *num) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    int ret = ssl_recv(sock, (char*)tmp_buffer, datalen);
    if ( ret < 0 ) {
        DBG("------ ssl error!");
        return -1;
    }
    datalen = ret;
    ret = 0;
    ADD_DATA2(ans, ret, tmp_buffer, datalen);
    if ( num ) *num = ret;
    DBG("ssl recv sock %d [%d]", datalen, ret);
    return 0;
}

int ssl_close_sock(const char *cmd, char *ans, int *num) {
    SSP_PARAMETER_NOT_USED(ans);
    if ( num ) *num = 0;
    INIT_SOCK_AND_CHECK(sock, cmd);
    if ( sock >= 0 ) {
        DBG("ssl close = %d", sock);
        return ssl_close(sock);
    }
    return 0;
}

static void reset_callback( void* arg ) {
    SSP_PARAMETER_NOT_USED(arg);
    DBG("RESET QUADRO");
    qcom_sys_reset();
}

int reset(const char *cmd, char *ans, int *num) {
    SSP_PARAMETER_NOT_USED(cmd);
    SSP_PARAMETER_NOT_USED(ans);
    SSP_PARAMETER_NOT_USED(num);
    DBG("call reset");
    reset_callback(NULL);
    return 0;
}

int set_wifi(const char *cmd, char *ans, int *num) {
    char ssid[32];
    char passphrase[32];
    A_INT32 security;
    char *ssid_end = strstr(cmd, ",");
    strncpy(ssid, cmd, (size_t)(ssid_end - cmd));
    ssid[(ssid_end - cmd)] = 0x0;
    char *pass_start = ssid_end + 1;
    char *pass_end = strstr(pass_start, ",");
    strncpy(passphrase, pass_start, (pass_end - pass_start));
    passphrase[(pass_end - pass_start)] = 0x0;
    char *sec_start = pass_end + 1;
    sscanf(sec_start, "%d", (int*)&security);
    save_wifi_setting(ssid, passphrase, security);
    strcpy(ans, "SAVE");
    if ( num ) *num = 4;
    return 0;
}

int set_keys(const char *cmd, char *ans, int *num) {
    char api_key[66];
    char sec_key[44];
    char *api_key_end = strstr(cmd, ",");
    strncpy(api_key, cmd, (size_t)(api_key_end - cmd));
    api_key[(api_key_end - cmd)] = 0x0;
    char *sec_key_start = api_key_end + 1;
    char *sec_key_end = strstr(sec_key_start , CRLF);
    strncpy(sec_key, sec_key_start, (size_t)(sec_key_end - sec_key_start));
    sec_key[(sec_key_end - sec_key_start)] = 0x0;
    save_key_setting(api_key, sec_key);
    strcpy(ans, "SAVE");
    if ( num ) *num = 4;
    return 0;
}

typedef int(*at_cmd_eq)(const char *, char *, int *);

typedef struct _at_cmd_ {
    const char *name;
    at_cmd_eq func;
} at_cmd_t;

at_cmd_t cmds[] = {
        { "S.SOCK",     create_socket },
        { "S.SOCKC",    close_socket },
        { "S.RHOST",    get_host_by_name },
        { "S.SOCKST",   sendto_socket },
        { "S.SOCKRF",   recvfrom_socket },
        { "S.MAC",      get_mac_addr },
        { "S.SOCKCON",  socket_connect },
        { "S.SOCKSD",   socket_send },
        { "S.SOCKRV",   socket_recv },
        { "S.SOCKTM",   socket_timeout },
        { "WIFI",       wifi_connect },
        { "S.SSLCON",   ssl_connect_sock },
        { "S.SSLSND",   ssl_send_sock },
        { "S.SSLRCV",   ssl_recv_sock },
        { "S.SSLCLS",   ssl_close_sock },
        { "SWIFI",      set_wifi },
        { "SKEYS",      set_keys },
        { "RESET",      reset }
};

at_cmd_t *find_cmd(const char *name, int size) {
//    char n_b[100];
//    strncpy(n_b, name, size);
//    n_b[size] = 0;
//    DBG("<%s>", n_b);
    uint32_t i = 0;
    for ( i = 0; i < sizeof(cmds)/sizeof(at_cmd_t); i++ ) {
        if ( strncmp(cmds[i].name, name, size) == 0 ) {
            return cmds + i;
        }
    }
    DBG("There is no CMDs handler");
    return NULL;
}

// input buffer
static char uart_tx_buffer[BUFFER_LENGTH];
static uint8_t uart_rx_buffer[BUFFER_LENGTH];

int exec_cmd(const char *cmd, int size) {
    SSP_PARAMETER_NOT_USED(size);
    char *eq = strstr(cmd, "=");
    if ( !eq ) return -1;
    int ret = -1;
    int tx_len = 0;
    uart_tx_buffer[0] = 0x0;
    at_cmd_t *at = find_cmd(cmd, (int)(eq-cmd));
    if ( at ) {
        if (at->func) ret = at->func(
                    eq + 1,
                    uart_tx_buffer,// + strlen(uart_tx_buffer),
                    &tx_len);
    }
    if (ret < 0)
        strcat(uart_tx_buffer, NLINE "ERROR" NLINE);
    else
        strcpy(uart_tx_buffer + tx_len, NLINE "OK" NLINE);
//    DBG("-->[%s]", uart_tx_buffer);
    int tot_size = tx_len + strlen(uart_tx_buffer + tx_len);
//    uart2_write(uart_tx_buffer, tot_size );
    int tr_size = 0;
#define SEND_MAX 50
    DBG("send to uart %d", tot_size);
    while (tot_size > 0) {
        int chunk = (tot_size > SEND_MAX ? SEND_MAX : tot_size);
        uart2_write(uart_tx_buffer + tr_size, chunk );
        tr_size += chunk;
        tot_size -= chunk;
        msleep(1);
    }
    return ret;
}

int purse_cmd(const char *buf, int size) {
    if ( size < 5 ) return -1;
    char *ptr_at = strstr(buf, "AT+");
    if ( !ptr_at ) return -1;
    char *ptr_cmd = ptr_at + 3;
    return exec_cmd(ptr_cmd, size);
}

int at_go(void) {
    uart2_init();
    qcom_gpio_interrupt_info_t	gpio_interrupt;
    gpio_interrupt.pin = 8;
    gpio_interrupt.gpio_pin_int_handler_fn = reset_callback;
    gpio_interrupt.arg = NULL;
    if (qcom_gpio_interrupt_register(&gpio_interrupt) != A_OK) {
        DBG("ERROR:gpio interrupt register error");
    }
    if (qcom_gpio_interrupt_mode(&gpio_interrupt, QCOM_GPIO_PIN_INT_RISING_EDGE) != A_OK) {
        DBG("ERROR:gpio interrupt mode error");
    }
    qcom_gpio_interrupt_start(&gpio_interrupt);
    uart2_write(POWERON, sizeof(POWERON)-1);
    DBG(POWERON);

    int status = 0;
    int offset = 0;
    while (1) {
        char c = get_char(&status, 1);
        wdt_feed();
        if ( status == A_OK ) {
            uart_rx_buffer[offset] = c;
            offset ++;
            uart_rx_buffer[offset] = 0x0;
            if ( offset > (int)CRLF_LEN &&
                 strncmp((char*)uart_rx_buffer+offset-CRLF_LEN, CRLF, CRLF_LEN) == 0 ) {
//                DBG("receive %d [%s]", offset, uart_rx_buffer);
                purse_cmd((char *)uart_rx_buffer, offset + 1);
                offset = 0;
            }
        }
    }
    return 0;
}
