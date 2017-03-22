/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#ifdef _ARIS_
#include "miniserver.h"
#include <http/request.h>
#include "TRACE_USE.h"
#include <tx_mutex.h>
#include "flasharis.h"
#include <bsd/socket.h>
#include <arrow/utf8.h>
#include <arrow/gateway.h>
#include <arrow/device.h>

#define CHUNK_SIZE 512

#define nnet_send(sock, str) send((sock), (uint8_t*)(str), (uint16_t)strlen(str), 0)

int receive_request(SOCKET sock, http_request_t *res, char *buf, size_t *bufsize, size_t *len) {
    int ret;
    if ( (ret = recv(sock, (uint8_t*)buf, 20, 0)) < 0 ) return ret;
    *len = (size_t)ret;
    *bufsize -= (size_t)ret;
    buf[ret] = 0;
    // find begin of the request XXX /xxx HTTP/1.X
    char* crlfPtr = strstr(buf, "\r\n");
    while( crlfPtr == NULL ) {
        if( *len < CHUNK_SIZE - 1 ) {
            size_t newTrfLen;
            if ( (ret = recv(sock, (uint8_t*)buf + *len, 10, 0)) < 0 ) return ret;
            newTrfLen = (size_t)ret;
            *len += newTrfLen;
            *bufsize -= newTrfLen;
        } else {
            return -1;
        }
        buf[*len] = 0;
        crlfPtr = strstr(buf, "\r\n");
        if ( crlfPtr ) { // check HTTP
            char *http_start = strstr(buf, "HTTP");
            if ( !http_start && http_start < crlfPtr ) {
                DBG("No HTTP");
                size_t offset = crlfPtr + 2 - buf;
                memmove(buf, crlfPtr + 2, offset);
                *len -= offset;
                *bufsize += offset;
                crlfPtr = NULL;
            }
        }
    }

    int crlfPos = crlfPtr - buf;
    if ( crlfPos < 0 && crlfPos > (int)(*len) ) return -1;
    buf[crlfPos] = '\0';
    DBG("request: {%s}", buf);
    char *first_space = strstr(buf, " ");

    if( ! first_space ) {
        DBG("Not a correct HTTP answer : %s", buf);
        return -1;
    }
    *first_space = '\0';

    ret = cmp_meth(buf);
    if ( ret < 0 ) {
        res->meth = NULL;
    } else {
        res->meth = (uint8_t*)get_meth(ret);
    }
    DBG("meth: {%s}", res->meth);
    if ( !res->meth ) return -1;

    memmove(buf, &buf[crlfPos+2], *len - (size_t)(crlfPos + 2) + 1 );
    *len -= (size_t)(crlfPos + 2);

    return 0;
}

struct wifi_conf {
    char *ssid;
    char *password;
    int secure;
};

static char *get_prm(char *str, const char *name, int *len) {
    char *prm;
    char *prm_start = strstr(str, name);
    if ( !prm_start ) return NULL;

    prm = prm_start+strlen(name);
    char *prm_end = strstr(prm_start, "&");
    if ( ! prm_end ) {
        DBG("wrong parameter {%s}", prm);
        return NULL;
    }
    if ( len ) *len = prm_end - prm;
    return prm;
}

int get_line(SOCKET sock, uint8_t *buf) {
    char *crlfptr = NULL;
    int nlen = 0;
    while( !crlfptr ) {
        int ret = recv(sock, buf+nlen, 1, 0);
        if ( ret < 0 ) return nlen?nlen:ret;
        nlen += (size_t)ret;
        buf[nlen] = '\0';
        crlfptr = strstr((char*)buf, "\r\n");
    }
    return 0;
}

static int receive_query(SOCKET sock, uint8_t *buf, struct wifi_conf *wfc) {
    char *crlfptr = NULL;
    size_t nlen = 0;
    size_t content_len = 0;
    buf[0] = '\0';
    while( get_line(sock, buf) == 0 ) {
        DBG("parse line [%s]", buf);
        if (!content_len) { // check content length
            char *contlen = strstr((char*)buf, "Content-Length: "); // len=16
            if ( contlen ) {
                content_len = (size_t)atoi(contlen+16);
                DBG("Content-Length: %u", content_len);
            }
        } else {
            if ( strcmp(buf, "\r\n") == 0 ) break;
        }
    }
    // parse the payload!
    nlen = recv(sock, buf, content_len, 0);
    if ( nlen > 0) {
        int len;
        char *ssid = get_prm((char*)buf, "SSID=", &len);
        if ( ssid ) {
            wfc->ssid = malloc(len+1);
            strncpy(wfc->ssid, ssid, len);
            wfc->password[len] = 0;
            DBG("ssid = %d{%s}", len, wfc->ssid);
        } else return -1;
        char *pass = get_prm((char*)buf, "PASS=", &len);
        if ( pass ) {
            wfc->password = malloc(len+1);
            strncpy(wfc->password, pass, len);
            wfc->password[len] = 0;
            fix_urldecode(wfc->password);
            DBG("pass = %d{%s}", len, wfc->password);
        } else return -1;
        char *sec = get_prm((char*)buf, "SECR=", &len);
        if ( sec ) {
            DBG("sec = {%s}", sec);
            if ( strncmp(sec, "OPEN", 4) == 0 ) {
                wfc->secure = M2M_WIFI_SEC_OPEN;
            } else if ( strncmp(sec, "WPA_PSK", 7) == 0 ) {
                wfc->secure = M2M_WIFI_SEC_WPA_PSK;
            } else if ( strncmp(sec, "WEP", 3) == 0 ) {
                wfc->secure = M2M_WIFI_SEC_WEP;
            } else if ( strncmp(sec, "802_1X", 6) == 0 ) {
                wfc->secure = M2M_WIFI_SEC_802_1X;
            } else {
                wfc->secure = M2M_WIFI_SEC_INVALID;
            }
            DBG("sec = {%d}", wfc->secure);
        } else return -1;
        return 0;
    } else return -1;

    return 0;
}

typedef struct _sock_event {
    SOCKET sock;
    size_t bufsize;
    struct _sock_event *next;
} sock_event_t;

static sock_event_t *socket_queue = NULL;
static TX_MUTEX mtx;


void process_client(SOCKET sock, size_t bufsize) {
    tx_mutex_get(&mtx, TX_WAIT_FOREVER);
    while ( socket_queue && socket_queue->next ) socket_queue = socket_queue->next;
    sock_event_t *ptr = malloc(sizeof(sock_event_t));
    ptr->bufsize = bufsize;
    ptr->sock = sock;
    ptr->next = NULL;
    if ( socket_queue ) {
        socket_queue->next = ptr;
    } else {
        socket_queue = ptr;
    }
    tx_mutex_put(&mtx);
}


static void send_simple_response(SOCKET sock, char *pl) {
    nnet_send(sock, "HTTP/1.0 200 OK\r\n");
    nnet_send(sock, "Server: nginx/0.6.31\r\n");
    nnet_send(sock, "Content-Type: text/html; charset=utf-8\r\n");
    char *tmp = malloc(30);
    sprintf(tmp, "Content-Length: %d\r\n", strlen(pl));
    send(sock, (uint8_t*)tmp, (uint16_t)strlen(tmp), 0);
    nnet_send(sock, "Connection: close\r\n");
    nnet_send(sock, "\r\n");
    send(sock, (uint8_t*)pl, (uint16_t)strlen(pl), 0);
    free(tmp);
}

void server_init() {
    socket_queue = NULL;
    tx_mutex_create(&mtx, "ap_server_mutex", TX_NO_INHERIT);
}

void server_run() {
    char *client_buf = malloc(CHUNK_SIZE);
    int need_read = CHUNK_SIZE;
    http_request_t req;
    int parentfd; /* parent socket */
    int childfd = -1; /* child socket */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char *hostaddrp; /* dotted decimal host addr string */
    int n; /* message byte size */

    parentfd = socket(AF_INET, SOCK_STREAM, 0);
    if (parentfd < 0)
        DBG("ERROR opening socket");

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = 0x00;
    serveraddr.sin_port = htons(80);

    if (bind(parentfd, (struct sockaddr *) &serveraddr,
             sizeof(serveraddr)) < 0)
        DBG("ERROR on binding");


    if (listen(parentfd, 5) < 0)
        DBG("ERROR on listen");

    clientlen = sizeof(clientaddr);

    while(childfd < 0) {
        childfd = accept(parentfd, (struct sockaddr *) &clientaddr, &clientlen);
        if (childfd < 0) DBG("ERROR on accept");
    }
    DBG("+++ on accept %d", childfd);

    while(1) {
        req.meth = NULL;
        size_t buf_len;
        if ( receive_request(childfd, &req, client_buf, &need_read, &buf_len) < 0) {
            TRACE("unknown request\r\n");
            tx_thread_sleep(CONV_MS_TO_TICK(100));
        }

        switch( cmp_meth((char*)req.meth) ) {
            case GET: {
                TRACE("send response\r\n");
                char *pl =
                        "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>"
                        "<style>body{font-size:xx-large;font-family:'Arial';} input{font-size:xx-large;width:100%;padding:12px 20px;margin:8px 0;box-sizing:border-box;} button{background-color:white;color:black;border:2px solid #blue;padding:15px 32px;font-size:xx-large;}</style></head>"
                        "<body><div style='padding: 20px;'><h1>Arrow Connect</h1><h3>ARIS Board Wi-Fi settings</h3><br>"
                        "<form>SSID:<br><input type=\"text\" name=\"SSID\"><p>"
                        "Security:<br><select size=4 multiple name=\"SECR\">"
                        "<option disabled>Security</option>"
                        "<option value='OPEN'>OPEN</option>"
                        "<option selected value='WPA_PSK'>WPA_PSK</option>"
                        "<option value='WEP'>WEP</option>"
                        "<option value='802_1X'>802_1X</option>"
                        "</select></p>"
                        "Password:<br><input type=\"text\" name=\"PASS\"><p>"
                        "<button type='submit' formmethod='post' formaction='' name='act' value='save'>Save</button>"
                        "</form></div></body></html>";
                send_simple_response(childfd, pl);
                TRACE("send response done\r\n");
            } break;
            case POST: {
                TRACE("POST response\r\n");
                struct wifi_conf wfc = {NULL, NULL, 0};
                if ( receive_query(childfd, (uint8_t*)client_buf, &wfc) < 0 ) {
                    char *pl =
                            "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>"
                            "<style>body{font-size:xx-large;font-family:'Arial';}</style></head>"
                            "<body><div style='padding: 20px;'><h1>Something wrong</h1><h3>Wi-Fi configuration was not saved</h3>"
                            "</div></body></html>";
                    send_simple_response(childfd, pl);
                } else {
                    DBG("ssid: %s", wfc.ssid);
                    DBG("pass: %s", wfc.password);
                    DBG("sec:  %d", wfc.secure);
                    save_wifi_setting(wfc.ssid, wfc.password, wfc.secure);
                    arrow_gateway_t dummy_gate;
                    arrow_device_t dummy_dev;
                    arrow_gateway_init(&dummy_gate);
                    arrow_device_init(&dummy_dev);
                    save_gateway_info(&dummy_gate);
                    save_device_info(&dummy_dev);
                    if ( wfc.ssid ) free(wfc.ssid);
                    if ( wfc.password ) free(wfc.password);
                    char *pl =
                            "<html><head><meta http-equiv='Cache-Control' content='no-cache, no-store, must-revalidate'/>"
                            "<style>body{font-size:xx-large;font-family:'Arial';}</style></head>"
                            "<body><div style='padding: 20px;'><h1>DONE</h1><h3>Wi-Fi configuration was saved</h3>"
                            "</div></body></html>";
                    send_simple_response(childfd, pl);
                    tx_thread_sleep ( CONV_MS_TO_TICK(1000) );
                    Reset_Handler();
                }
            } break;
        } // switch
    }
    soc_close(childfd);
}

#endif
