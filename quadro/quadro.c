
#include <stdlib.h>
#include "wiced.h"
#include "wiced_tls.h"

#include <bsd/socket.h>
#include <time/time.h>
#include <ntp/ntp.h>
#include <arrow/routine.h>
#include <arrow/utf8.h>
#include <arrow/software_release.h>
#include <arrow/events.h>
#include <sys/mac.h>
#include <ssl/ssl.h>
#include <debug.h>

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define BUFFER_LENGTH     (2048)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/
#define RX_BUFFER_SIZE    512

wiced_uart_config_t uart_config =
{
    .baud_rate    = 115200,
    .data_width   = DATA_WIDTH_8BIT,
    .parity       = NO_PARITY,
    .stop_bits    = STOP_BITS_1,
    .flow_control = FLOW_CONTROL_DISABLED,
};

wiced_ring_buffer_t rx_buffer;
uint8_t             rx_data[RX_BUFFER_SIZE];


/******************************************************
 *               Function Definitions
 ******************************************************/

#define POWERON "\r\n+WIND:1:Poweron\r\n\r\n"
#define WIFION "+WIND:2:WiFi\r\n"

#define UART_PORT WICED_UART_2

uint8_t uart_rx_buffer[BUFFER_LENGTH];
uint8_t tmp_buffer[BUFFER_LENGTH];

static int get_socket(const char *str, int *sock) {
    int ret = sscanf(str, "%d", sock);
    if ( ret != 1 ) return -1;
    return 0;
}

// find the ",<ip>,<port>" pattern
static char *get_ip_addr(const char *str, struct sockaddr_in *servaddr) {
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

int create_socket(const char *arg, char *ans) {
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
    sprintf(ans, "ID:%d", sock);
    return 0;
}

int close_socket(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    if ( sock >= 0 ) {
        DBG("close = %d", sock);
        soc_close(sock);
    }
    return 0;
}

int sendto_socket(const char *cmd, char *ans) {
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
    return 0;
}

int recvfrom_socket(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    struct sockaddr addr;
    socklen_t serverlen = sizeof(addr);
    int ret = recvfrom(sock, tmp_buffer, datalen, 0, &addr, &serverlen);
    if ( ret < 0 ) return -1;
    datalen = ret;
    ADD_DATA(ans, tmp_buffer, datalen);
    return 0;
}

int get_host_by_name(const char *cmd, char *ans) {
    char *name_end = strstr(cmd, "\r\n");
    char buffer[100];
    strncpy(buffer, cmd, name_end - cmd);
    buffer[name_end - cmd] = 0x0;
    struct hostent *serv = gethostbyname(buffer);
    if (serv) {
        sprintf(ans, "%08x", *(int*)serv->h_addr);
        return 0;
    }
    return -1;
}

int get_mac_addr(const char *cmd, char *ans) {
    SSP_PARAMETER_NOT_USED(cmd);
    unsigned char mac[8];
    if ( get_mac_address((char*)mac) < 0 ) { DBG("no mac"); return -1; }
    int ret = sprintf(ans, "MAC:%02x.%02x.%02x.%02x.%02x.%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    ans[ret] = 0;
    return 0;
}

int socket_connect(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    struct sockaddr_in serveraddr;
    if ( ! get_ip_addr(cmd, &serveraddr) ) return -1;
    socklen_t serverlen = sizeof(serveraddr);
    return connect(sock, (struct sockaddr*)&serveraddr, serverlen);
}

int socket_send(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    DECODE_DATA_AND_CHECK(tmp_buffer, len_str, datalen);
    int ret = send(sock, tmp_buffer, datalen, 0);
    if ( ret < 0 ) return ret;
    ADD_DATALEN(ans, ret);
    return 0;
}

int socket_recv(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    int ret = recv(sock, tmp_buffer, datalen, 0);
    if ( ret < 0 ) return -1;
    datalen = ret;
    ADD_DATA(ans, tmp_buffer, datalen);
    return 0;
}

int socket_timeout(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(time_str, cmd);
    INIT_LEN_AND_CHECK(timeout, time_str);
    struct timeval tv;
    tv.tv_sec =     (time_t)        ( timeout / 1000 );
    tv.tv_usec =    (suseconds_t)   (( timeout % 1000 ) * 1000);
    return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
}

int wifi_connect(const char *cmd, char *ans) {
    struct {
        wiced_bool_t             device_configured;
        wiced_config_ap_entry_t  ap_entry;
    } temp_config;

    wiced_ssid_t ssid;
    char passphrase[32];
    DBG("wc: %s", cmd);
    wiced_security_t security = WICED_SECURITY_WPA2_AES_PSK;
    char *ssid_end = strstr(cmd, ",");
    strncpy((char *)ssid.value, cmd, (size_t)(ssid_end - cmd));
    ssid.value[(ssid_end - cmd)] = 0x0;
    ssid.length = (ssid_end - cmd);
    char *pass_start = ssid_end + 1;
    char *pass_end = strstr(pass_start, ",");
    strncpy(passphrase, pass_start, (pass_end - pass_start));
    passphrase[(pass_end - pass_start)] = 0x0;
    char *sec_start = pass_end + 1;
    sscanf(sec_start, "%d", (int*)&security);

    DBG("SSID: %s", ssid.value);
    DBG("PASS: %s", passphrase);

    memset(&temp_config, 0, sizeof(temp_config));
    memcpy(&temp_config.ap_entry.details.SSID,     &ssid, sizeof(wiced_ssid_t));
    memcpy(&temp_config.ap_entry.details.security, &security, sizeof(wiced_security_t));
    memcpy(temp_config.ap_entry.security_key,       passphrase, strlen(passphrase));
    temp_config.ap_entry.security_key_length = strlen(passphrase);
    temp_config.device_configured = WICED_TRUE;
    wiced_dct_write( &temp_config, DCT_WIFI_CONFIG_SECTION, 0, sizeof(temp_config) );
    wiced_result_t r = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);
    if ( r == WICED_SUCCESS ) {
        strcpy(ans, "CONNECT");
        DBG(WIFION);
    } else {
        strcpy(ans, "ERROR");
        return -1;
    }
    return 0;
}

int ssl_connect_sock(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    return ssl_connect(sock);
}

int ssl_send_sock(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    DECODE_DATA_AND_CHECK(tmp_buffer, len_str, datalen);
    int ret = ssl_send(sock, (char*)tmp_buffer, datalen);
    if ( ret < 0 ) return ret;
    ADD_DATALEN(ans, ret);
    return 0;
}

int ssl_recv_sock(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    INIT_PTR_AFTER_COMMA_AND_CHECK(len_str, cmd);
    INIT_LEN_AND_CHECK(datalen, len_str);
    int ret = ssl_recv(sock, (char*)tmp_buffer, datalen);
    if ( ret < 0 ) return -1;
    datalen = ret;
    ADD_DATA(ans, tmp_buffer, datalen);
    return 0;
}

int ssl_close_sock(const char *cmd, char *ans) {
    INIT_SOCK_AND_CHECK(sock, cmd);
    if ( sock >= 0 ) {
        DBG("ssl close = %d", sock);
        return ssl_close(sock);
    }
    return 0;
}


typedef int(*at_cmd_eq)(const char *, char *);

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
        { "S.SSLCLS",   ssl_close_sock }

};

at_cmd_t *find_cmd(const char *name, int size) {
//    char n_b[100];
//    strncpy(n_b, name, size);
//    n_b[size] = 0;
//    DBG("<%s>", n_b);
    for ( int i = 0; i < sizeof(cmds)/sizeof(at_cmd_t); i++ ) {
        if ( strncmp(cmds[i].name, name, size) == 0 ) {
            return cmds + i;
        }
    }
    DBG("There is no CMDs handler");
    return NULL;
}


static char uart_tx_buffer[BUFFER_LENGTH];

int exec_cmd(const char *cmd, int size) {
    char *eq = strstr(cmd, "=");
    if ( !eq ) return -1;
    int ret = -1;
    uart_tx_buffer[0] = 0x0;
    at_cmd_t *at = find_cmd(cmd, (int)(eq-cmd));
    if ( at ) {
        if (at->func) ret = at->func( eq + 1, uart_tx_buffer + strlen(uart_tx_buffer));
    }
    if (ret < 0) strcat(uart_tx_buffer, "\r\nERROR\r\n");
    else strcat(uart_tx_buffer, "\r\nOK\r\n");
//    DBG("-->[%s]", uart_tx_buffer);
    int tot_size = strlen(uart_tx_buffer);
    int tr_size = 0;
//    DBG("send to uart %d", tot_size);
    while (tot_size > 0) {
        int chunk = (tot_size > 1 ? 1 : tot_size);
        wiced_uart_transmit_bytes(WICED_UART_2, uart_tx_buffer + tr_size, chunk );
        tr_size += chunk;
        tot_size -= chunk;
        wiced_rtos_delay_microseconds(400);
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

static void reset_callback( void* arg )
{
    SSP_PARAMETER_NOT_USED(arg);
    DBG("RESET QUADRO");
    wiced_deinit( );
    wiced_rtos_delay_milliseconds( 100 );
    wiced_framework_reboot( );
}

#define RESET_PIN WICED_GPIO_14

typedef struct _data_ {
    int x;
} data_t;

int get_telemetry_data(void *d) {
    data_t *data = (data_t *)d;
    data->x = 10;
    return 0;
}

int reset_cmd(void *arg) {
    reset_callback(arg);
    return 0;
}

int test_cmd(void *arg) {
    DBG("--- test command ---");
    msleep(5000);
    return 0;
}

void application_start( )
{
    DBG("--- Quadro Demo ---");
    wiced_result_t r_button;
    r_button = wiced_gpio_init( WICED_GPIO_49, INPUT_HIGH_IMPEDANCE);
    DBG("gpio51 %d", r_button);
    r_button = wiced_gpio_input_get( WICED_GPIO_49 );
    DBG("gpio51 %d", r_button);

    r_button = wiced_gpio_init( RESET_PIN, INPUT_HIGH_IMPEDANCE);
    if ( r_button != WICED_SUCCESS ) {
        DBG("gpio init failed %d %d", RESET_PIN, r_button);
    }

    r_button = wiced_gpio_input_irq_enable( RESET_PIN, IRQ_TRIGGER_RISING_EDGE, reset_callback, NULL );
    if ( r_button != WICED_SUCCESS ) {
        DBG("reset irq enable failed %d", r_button);
    }

    uint32_t expected_data_size = 1;
    /* Initialise ring buffer */
    ring_buffer_init(&rx_buffer, rx_data, RX_BUFFER_SIZE );

    /* Initialise UART. A ring buffer is used to hold received characters */
    wiced_result_t r_uart = wiced_uart_init( UART_PORT, &uart_config, &rx_buffer );
    DBG("uart %d", r_uart);

    wiced_init();

    wiced_uart_transmit_bytes(WICED_UART_2, POWERON, sizeof( POWERON ) - 1 );
    DBG(POWERON);
#if defined(QUADRO_HOSTLESS)
    int offset = 0;
    while ( 1 ) {
        expected_data_size = 1;
        r_uart = wiced_uart_receive_bytes( WICED_UART_2, uart_rx_buffer + offset, &expected_data_size, -1 );
        offset += expected_data_size;
        uart_rx_buffer[offset] = 0x0;
        switch (r_uart) {
        case WICED_SUCCESS: {
            // normal cmd
            if ( offset > 2 && strncmp((char*)uart_rx_buffer+offset-2, "\r\n", 2) == 0 ) {
//                DBG("receive %d [%s]", offset, uart_rx_buffer);
                purse_cmd((char *)uart_rx_buffer, offset + expected_data_size);
                offset = 0;
            }
        } break;
        default:
            DBG("receive fail %d %d", r_uart, expected_data_size);
        }
    }
#else

    struct {
            wiced_bool_t             device_configured;
            wiced_config_ap_entry_t  ap_entry;
        } temp_config;

        wiced_ssid_t ssid;
        wiced_security_t security = WICED_SECURITY_WPA2_AES_PSK;
        strcpy(ssid.value, DEFAULT_WIFI_SSID);
        ssid.length = strlen(ssid.value);

        DBG("SSID: %s", ssid.value);
        DBG("PASS: %s", DEFAULT_WIFI_PASS);

        memset(&temp_config, 0, sizeof(temp_config));
        memcpy(&temp_config.ap_entry.details.SSID,     &ssid, sizeof(wiced_ssid_t));
        memcpy(&temp_config.ap_entry.details.security, &security, sizeof(wiced_security_t));
        memcpy(temp_config.ap_entry.security_key,       DEFAULT_WIFI_PASS, strlen(DEFAULT_WIFI_PASS));
        temp_config.ap_entry.security_key_length = strlen(DEFAULT_WIFI_PASS);
        temp_config.device_configured = WICED_TRUE;
        wiced_dct_write( &temp_config, DCT_WIFI_CONFIG_SECTION, 0, sizeof(temp_config) );
        wiced_result_t r = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);
        if ( r == WICED_SUCCESS ) {
            DBG(WIFION);
        }


    extern int arrow_release_download_init();
    extern int arrow_release_download_payload(const char *payload, int size, int flags);
    extern int arrow_release_download_complete(int ota_result);

    arrow_software_release_dowload_set_cb(
            arrow_release_download_init,
            arrow_release_download_payload,
            arrow_release_download_complete
            );

    arrow_command_handler_add("reset", reset_cmd);
    arrow_command_handler_add("test", test_cmd);

    ntp_set_time_cycle();

    arrow_mqtt_events_init();

    arrow_initialize_routine();

    data_t test;

    while(1) {
        arrow_mqtt_connect_routine();
        if ( arrow_mqtt_has_events() ) {
            arrow_mqtt_event_proc();
        }
        int ret = arrow_mqtt_send_telemetry_routine(get_telemetry_data, &test);
        switch (ret) {
        case ROUTINE_RECEIVE_EVENT:
            arrow_mqtt_disconnect_routine();
            arrow_mqtt_event_proc();
            break;
        default:
            arrow_mqtt_disconnect_routine();
        };
    }


    arrow_mqtt_events_done();
#endif

    DBG("Done!");

    wiced_deinit();
}
