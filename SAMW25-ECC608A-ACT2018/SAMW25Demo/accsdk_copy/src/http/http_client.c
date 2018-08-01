/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#define MODULE_NAME "HTTP_Client"

#include "http/http_client.h"

#define USE_STATIC
#include <data/chunk.h>

#include <debug.h>
#include <bsd/socket.h>
#include <time/time.h>
#include <arrow/utf8.h>

#include <ssl/ssl.h>

#define CHUNK_SIZE (RINGBUFFER_SIZE/2)

uint8_t tmpbuffer[CHUNK_SIZE];
#define MAX_TMP_BUF_SIZE (sizeof(tmpbuffer)-1)

int http_session_is_open(http_client_t *cli) {
    if ( cli->sock < 0 ) return 0;
    return 1;
}

void http_session_close_set(http_client_t *cli, bool mode) {
#if defined(HTTP_SOCK_KEEP_OPEN)
    mode = false;
#endif
  cli->flags._close = mode;
}

void http_session_set_protocol(http_client_t *cli, int prot) {
    cli->protocol = prot;
}

#define CHECK_CONN_ERR(ret) \
    if ( ret < 0 ) { \
      DBG("Connection error [%d] (%d)", __LINE__, ret); \
      return ret; \
    }

#define PRTCL_ERR() \
  { \
    DBG("Protocol error"); \
    return -1; \
  }

#define client_send(cli)                    simple_write((cli), NULL, 0)
#define client_send_direct(cli, buf, size)  simple_write((cli), (uint8_t*)(buf), (size))

static int client_recv(void *c, uint16_t len) {
    CREATE_CHUNK(tmp, MAX_TMP_BUF_SIZE);
    http_client_t *cli = (http_client_t *)c;
    if ( len > ringbuf_capacity(cli->queue) )
        len = ringbuf_capacity(cli->queue);
    int ret = -1;
    if ( cli->flags._cipher ) {
        ret = ssl_recv(cli->sock, tmp, (int)len);
    } else {
        ret = recv(cli->sock, tmp, len, 0);
    }
    if ( ret > 0 ) {
        if ( ringbuf_push(cli->queue, (uint8_t*)tmp, ret) < 0 ) {
            FREE_CHUNK(tmp);
            return -1;
        }
    }
    HTTP_DBG("%d|%s|", ret, tmp);
    FREE_CHUNK(tmp);
    return ret;
}

static int simple_write(void *c, uint8_t *buf, uint16_t len) {
    CREATE_CHUNK(tmp, MAX_TMP_BUF_SIZE);
    http_client_t *cli = (http_client_t *)c;
    if ( !buf ) {
        if ( !len ) len = ringbuf_size(cli->queue);
        if ( ringbuf_pop(cli->queue, (uint8_t*)tmp, len) < 0 )
            return -1;
        buf = (uint8_t*)tmp;
    } else {
        if ( !len ) len = strlen((char*)buf);
    }
    HTTP_DBG("%d|%s|", len, buf);
    int ret = -1;
    if ( cli->flags._cipher ) {
        ret = ssl_send(cli->sock, (char*)buf, (int)len);
    } else {
        ret = send(cli->sock, buf, len, 0);
    }
    FREE_CHUNK(tmp);
    return ret;
}

// allocated resources for the http client
int __attribute_weak__ http_client_init(http_client_t *cli) {
#if defined(STATIC_HTTP_CLIENT)
    cli->queue = &cli->static_queue;
#else
    cli->queue = alloc_type(ring_buffer_t);
#endif
    if ( !cli->queue ) {
        DBG("HTTP: queue alloc fail");
        return -1;
    }
    int ret = ringbuf_init(cli->queue, RINGBUFFER_SIZE);
    if ( ret < 0 ) {
        DBG("HTTP: ringbuffer init failed %d", ret);
        return ret;
    }
    cli->sock = -1;
    cli->flags._close = true;
    cli->flags._cipher = 0;
    cli->timeout = DEFAULT_API_TIMEOUT;
    cli->protocol = api_via_http;
    return 0;
}

int __attribute_weak__ http_client_free(http_client_t *cli) {
    ringbuf_free(cli->queue);
#if !defined(STATIC_HTTP_CLIENT)
    free(cli->queue);
#endif
    cli->queue = NULL;
    cli->protocol = api_via_http;
    return 0;
}

int default_http_client_open(http_client_t *cli, http_request_t *req);
int __attribute_weak__ http_client_open(http_client_t *cli, http_request_t *req) {
    return default_http_client_open(cli, req);
}

int default_http_client_open(http_client_t *cli, http_request_t *req) {
    cli->response_code = 0;
    cli->request = req;
    if ( cli->protocol != api_via_http ) return -1;
    if ( !cli->queue ) {
        DBG("HTTP: There is no queue");
        return -1;
    }
    ringbuf_clear(cli->queue);
    if ( cli->sock < 0 ) {
        DBG("new TCP connection");
        int ret = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if ( ret < 0 ) {
            DBG("HTTP: socket failed");
            return ret;
        }
        cli->sock = ret;
        // resolve the host
        struct sockaddr_in serv;
        struct hostent *serv_resolve;
        property_t null_term_host = property_as_null_terminated(&req->host);
        serv_resolve = gethostbyname(P_VALUE(null_term_host));
        property_free(&null_term_host);
        if (serv_resolve == NULL) {
            DBG("ERROR, no such host");
            return -1;
        }
        memset(&serv, 0, sizeof(serv));
        serv.sin_family = PF_INET;
		#if 0
        bcopy((char *)serv_resolve->h_addr,
              (char *)&serv.sin_addr.s_addr,
              (uint32_t)serv_resolve->h_length);
		#else
		serv.sin_addr.s_addr = *((uint32_t*)(serv_resolve->h_addr));
		#endif
        serv.sin_port = htons(req->port);

        // set timeout
        struct timeval tv;
        tv.tv_sec =     (time_t)        ( cli->timeout / 1000 );
        tv.tv_usec =    (suseconds_t)   (( cli->timeout % 1000 ) * 1000);
        setsockopt(cli->sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

        ret = connect(cli->sock, (struct sockaddr*)&serv, sizeof(serv));
        if ( ret < 0 ) {
            DBG("connect fail");
            soc_close(cli->sock);
            cli->sock = -1;
            return -1;
        }
        HTTP_DBG("connect done");
        if ( req->is_cipher ) {
            cli->flags._cipher = true;
            if ( ssl_connect(cli->sock) < 0 ) {
                HTTP_DBG("SSL connect fail");
                cli->flags._close = true;
                http_client_close(cli);
                return -1;
            }
        }
        if (socket_connect_done(cli->sock) < 0 ) {
            http_client_close(cli);
            return -1;
        }
    }
    return 0;
}

int default_http_client_close(http_client_t *cli);
int __attribute_weak__ http_client_close(http_client_t *cli) {
    return default_http_client_close(cli);
}

int default_http_client_close(http_client_t *cli) {
    cli->request = NULL;
    if ( cli->protocol != api_via_http ) return  -1;
    if ( cli->sock < 0 ) return -1;
    if ( !cli->flags._close ) return 1;
    if ( cli->flags._cipher ) {
        ssl_close(cli->sock);
        cli->flags._cipher = 0;
    }
    soc_close(cli->sock);
    cli->sock = -1;
    return 0;
}

static int send_start(http_client_t *cli, http_request_t *req, ring_buffer_t *buf) {
    ringbuf_clear(buf);
    int ret = snprintf((char*)tmpbuffer, MAX_TMP_BUF_SIZE,
                       "%s %s",
                       P_VALUE(req->meth), P_VALUE(req->uri));
    if ( ret < 0 ) return ret;
    if ( ringbuf_push(cli->queue, tmpbuffer, ret) < 0 ) return -1;
    if ( req->query ) {
        char *queryString = (char*)tmpbuffer;
        strcpy(queryString, "?");
        property_map_t *query = NULL;
        arrow_linked_list_for_each(query, req->query, property_map_t) {
          if ( (int)strlen(P_VALUE(query->key)) +
               (int)strlen(P_VALUE(query->value)) + 3 >
               (int)ringbuf_capacity(cli->queue) ) break;
            strcat(queryString, P_VALUE(query->key));
            strcat(queryString, "=");
            strcat(queryString, P_VALUE(query->value));
            if ( ringbuf_capacity(buf) - sizeof(HTTP_VERS)-1 < strlen(queryString) ) break;
            if ( ringbuf_push(buf, (uint8_t*)queryString, 0) < 0 )
                break;
            strcpy(queryString, "&");
        }
    }
    ret = ringbuf_push(buf, (uint8_t*)HTTP_VERS, 0);
    if ( ret < 0 ) return ret;
    if ( (ret = client_send(cli)) < 0 ) {
        return ret;
    }
    ret = snprintf((char*)tmpbuffer, MAX_TMP_BUF_SIZE, "Host: %s:%d\r\n", P_VALUE(req->host), req->port);
    if ( ret < 0 ) return ret;
    if ( ringbuf_push(cli->queue, tmpbuffer, ret) < 0) return -1;
    if ( (ret = client_send(cli)) < 0 ) {
        return ret;
    }
    return 0;
}

static int send_header(http_client_t *cli, http_request_t *req, ring_buffer_t *buf) {
    int ret;
    ringbuf_clear(buf);
    if ( !IS_EMPTY(req->payload) && property_size(&req->payload) ) {
        if ( req->is_chunked ) {
            ret = client_send_direct(cli, "Transfer-Encoding: chunked\r\n", 0);
        } else {
            ret = snprintf((char*)tmpbuffer,
                           ringbuf_capacity(cli->queue),
                           "Content-Length: %lu\r\n", (long unsigned int)property_size(&req->payload));
            if ( ret < 0 ) return ret;
            if ( ringbuf_push(cli->queue, tmpbuffer, ret) < 0 )
                return -1;
            ret = client_send(cli);
        }
        ringbuf_clear(buf);
        if ( ret < 0 ) return ret;
        ret = snprintf((char*)tmpbuffer,
                       ringbuf_capacity(cli->queue),
                       "Content-Type: %s\r\n", P_VALUE(req->content_type.value));
        if ( ringbuf_push(cli->queue, tmpbuffer, ret) < 0 ) return -1;
        if ( ret < 0 ) return ret;
        if ( (ret = client_send(cli)) < 0 ) return ret;
    }
    property_map_t *head = NULL;
    arrow_linked_list_for_each(head, req->header, property_map_t) {
        ringbuf_clear(buf);
        ret = snprintf((char*)tmpbuffer,
                           ringbuf_capacity(cli->queue),
                           "%s: %s\r\n", P_VALUE(head->key), P_VALUE(head->value));
    	if ( ret < 0 ) return ret;
        if ( ringbuf_push(cli->queue, tmpbuffer, ret) < 0 ) return -1;
        if ( (ret = client_send(cli)) < 0 ) return ret;
    }
    return client_send_direct(cli, "\r\n", 2);
}

static int send_payload(http_client_t *cli, http_request_t *req) {
    if ( !IS_EMPTY(req->payload) && property_size(&req->payload)) {
        if ( req->is_chunked ) {
            char *data = P_VALUE(req->payload);
            int len = (int)property_size(&req->payload);
            int trData = 0;
            while ( len >= 0 ) {
                char buf[6];
                int chunk = len > CHUNK_SIZE ? CHUNK_SIZE : len;
                int ret = sprintf(buf, "%02X\r\n", chunk);
                client_send_direct(cli, buf, ret);
                if ( chunk ) client_send_direct(cli, data + trData, chunk);
                trData += chunk;
                len -= chunk;
                client_send_direct(cli, "\r\n", 2);
                if ( !chunk ) break;
            }
            return 0;
        } else {
          int ret = client_send_direct(cli, P_VALUE(req->payload), property_size(&req->payload));
          return ret;
        }
    }
    return -1;
}

static uint8_t *wait_line(http_client_t *cli, uint8_t *buf, size_t maxsize) {
  int size = 0;
  while ( size < 2 || strncmp((char*)buf + size - 2, "\r\n", 2) != 0 ) {
      // pop from buffer by one symbol
      if ( ringbuf_pop(cli->queue, buf + size, 1) == 0 ) {
          size ++;
          buf[size] = 0x0;
          if ( (size_t)size == maxsize ) return NULL;
      } else {
          // empty buffer
          if( ringbuf_capacity(cli->queue) > LINE_CHUNK ) {
              int ret = client_recv( cli, LINE_CHUNK );
              if ( ret < 0 ) return NULL;
          } else {
              return NULL;
          }
      }
  }
  return buf + size - 2;
}

static int receive_response(http_client_t *cli, http_response_t *res) {
    ringbuf_clear(cli->queue);
    CREATE_CHUNK(tmp, RINGBUFFER_SIZE);
    uint8_t *crlf = NULL;
    do {
        crlf = wait_line(cli, (uint8_t*)tmp, RINGBUFFER_SIZE);
        if ( !crlf ) {
            DBG("couldn't wait end of a line");
            FREE_CHUNK(tmp);
            return -1;
        }
        if( (crlf - (uint8_t *)tmp) < 10 ) {
            // too short line: sizeof(HTTP/1.1)
            *crlf = 0x0;
            crlf = NULL;
        }
    } while ( !crlf );
    *crlf = 0x0;
    DBG("resp: {%s}", tmp);
    char *p = strstr(tmp, "HTTP/1.1 ");
    if ( !p ) return -1;
    p = copy_till_to_int(p + 9, " OK", (int*)&res->m_httpResponseCode);
    if( !p ) {
        DBG("Not a correct HTTP answer : %s", tmp);
        FREE_CHUNK(tmp);
        return -1;
    }

    DBG("Response code %d", res->m_httpResponseCode);
    cli->response_code = res->m_httpResponseCode;
    FREE_CHUNK(tmp);
    return 0;
}

static int receive_headers(http_client_t *cli, http_response_t *res) {
    uint8_t *crlf = NULL;
    CREATE_CHUNK(tmp, RINGBUFFER_SIZE);
    CREATE_CHUNK(key, CHUNK_SIZE>>2);
    CREATE_CHUNK(value, CHUNK_SIZE);
    int ret = 0;
    while( ( crlf = wait_line(cli, (uint8_t*)tmp, RINGBUFFER_SIZE) ) ) {
        if ( crlf == (uint8_t*)tmp ) {
            HTTP_DBG("Headers read done");
            ret = 0;
            break;
        }

        char *p = copy_till(tmp, ": ", key);
        if ( !p ) {
            DBG("Could not parse key");
            ret = -1;
            goto recv_header_end;
        }
        p = copy_till(p, "\r\n", value);
        if ( !p ) {
            DBG("Could not parse value");
            ret = -1;
            goto recv_header_end;
        }

            HTTP_DBG("Read header : %s: %s", key, value);
            if( !strcmp(key, "Content-Length") ) {
                res->recvContentLength = atoi(value);
            } else if( !strcmp(key, "Transfer-Encoding") ) {
                if( !strcmp(value, "Chunked") || !strcmp(value, "chunked") )
                    res->is_chunked = 1;
            }
#if defined(HTTP_PARSE_HEADER)
            else if( !strcmp(key, "Content-Type") ) {
                http_response_set_content_type(res, p_stack(value));
            } else {
                http_response_add_header(res,
                                         p_stack(key),
                                         p_stack(value));
            }
#endif
    }
recv_header_end:
    FREE_CHUNK(tmp);
    FREE_CHUNK(key);
    FREE_CHUNK(value);
    return ret;
}

static int get_chunked_payload_size(http_client_t *cli, http_response_t *res) {
    // find the \r\n in the payload
    // next string shoud start at HEX chunk size
    SSP_PARAMETER_NOT_USED(res);
    CREATE_CHUNK(tmp, RINGBUFFER_SIZE);
    int chunk_len = 0;
    uint8_t *crlf = wait_line(cli, (uint8_t*)tmp, RINGBUFFER_SIZE);
    if ( !crlf ) return -1; // no \r\n - wrong string
    char *p = copy_till_hex_to_int((char*)tmp, "\r\n", &chunk_len);
    if ( !p ) {
        // couldn't read a chunk size - fail
        chunk_len = 0;
        FREE_CHUNK(tmp);
        return -1;
    }
    HTTP_DBG("detect chunk %d", chunk_len);
    FREE_CHUNK(tmp);
    return chunk_len;
}

static int receive_payload(http_client_t *cli, http_response_t *res) {
    int chunk_len = 0;
    int no_data_error = 0;
    do {
        if ( res->is_chunked ) {
            chunk_len = get_chunked_payload_size(cli, res);
        } else {
            chunk_len = res->recvContentLength;
            DBG("Con-Len %lu", res->recvContentLength);
        }
        if ( !chunk_len || chunk_len < 0 ) break;
        while ( chunk_len ) {
            uint32_t need_to_read = ARROW_MIN(chunk_len, CHUNK_SIZE);
            HTTP_DBG("need to read %d", need_to_read);
            while ( (int)ringbuf_size(cli->queue) < (int)need_to_read ) {
                HTTP_DBG("get chunk add %d", need_to_read-ringbuf_size(cli->queue));
                int ret = client_recv(cli, need_to_read-ringbuf_size(cli->queue));
                if ( ret < 0 ) {
                    // ret < 0 - error
                    DBG("No data");
                    if ( no_data_error ++ > 2) return -1;
                }
            }
            HTTP_DBG("add payload{%d:s}", need_to_read);//, buf);
            if ( ringbuf_pop(cli->queue, tmpbuffer, need_to_read) < 0 ) return -1;
            if ( http_response_add_payload(res,
                                           p_stack_raw(tmpbuffer, need_to_read) ) < 0 ) {
                ringbuf_clear(cli->queue);
                DBG("Payload is failed");
                return -1;
            }
            chunk_len -= need_to_read;
            HTTP_DBG("%d %d", chunk_len, need_to_read);
        }
        if ( !res->is_chunked ) break;
        else {
          uint8_t *crlf = wait_line(cli, tmpbuffer, CHUNK_SIZE);
          if ( !crlf ) {
              DBG("No new line");
          }
        }
    } while(1);

    HTTP_DBG("body{%s}", P_VALUE(res->payload));
    return 0;
}

int default_http_client_do(http_client_t *cli, http_response_t *res);
int __attribute_weak__ http_client_do(http_client_t *cli, http_response_t *res) {
    return default_http_client_do(cli, res);
}

int default_http_client_do(http_client_t *cli, http_response_t *res) {
    int ret;
    http_request_t *req = cli->request;
    if ( !req ) return -1;
    http_response_init(res, &req->_response_payload_meth);

    if ( send_start(cli, req, cli->queue) < 0 ) {
        DBG("send start fail");
        return -1;
    }

    if ( send_header(cli, req, cli->queue) < 0 ) {
        DBG("send header fail");
        return -1;
    }

    if ( !IS_EMPTY(req->payload) ) {
        if ( send_payload(cli, req) < 0 ) {
            DBG("send payload fail");
            return -1;
        }
    }

    HTTP_DBG("Receiving response");

    ringbuf_clear(cli->queue);

    ret = receive_response(cli, res);
    if ( ret < 0 ) {
        DBG("Receiving error (%d)", ret);
        return -1;
    }

    HTTP_DBG("Reading headers");
    res->header = NULL;
    memset(&res->content_type, 0x0, sizeof(property_map_t));
    memset(&res->payload, 0x0, sizeof(http_payload_t));
    res->is_chunked = 0;
    res->processed_payload_chunk = 0;

    //Now let's get a headers
    ret = receive_headers(cli, res);
    if ( ret < 0 ) {
        DBG("Receiving headers error (%d)", ret);
        return -1;
    }

    ret = receive_payload(cli, res);
    if ( ret < 0 ) {
        DBG("Receiving payload error (%d)", ret);
        return -1;
    }
    return 0;
}
