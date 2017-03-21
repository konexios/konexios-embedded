/***********************************************************************************************************************
 * Copyright [2016] RELOC s.r.l. - All rights strictly reserved
 * This Software is provided for evaluation purposes; any other use — including reproduction, modification, use for
 * a commercial product, distribution, or republication — without the prior written permission of the Copyright holder
 * is strictly prohibited. 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * File Name        : http_client.c
 * Description      : Minimal HTTP client library main file.
 *********************************************************************************************************************/

#define MODULE_NAME "HTTP_CLIENT"

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// synergy
#include "hal_data.h"

// TODO manage include paths
#include "../common/reloc_instance_integrity.h"
#include "../common/reloc_macro.h"
#include "TRACE_USE.h"

// Wi-Fi driver
#include "sf_winc1500_api.h"

#include "http_header.h"
#include "http_client_own.h"
#include "http_client_use.h"


/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private function prototypes
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/

static uint8_t buf[HTTP_CLIENT_BUF_SIZE];

static int32_t g_sock_id;
static const sf_wifi_socket_cfg_t g_sock_cfg = { SF_WINC1500_SOCKET_PROTOCOL_FAMILY_AF_INET,
                                                 SF_WIFI_SOCKET_TYPE_STREAM, 0 };
static sf_wifi_socket_address_t socket_address = { 80, 2, 0x81245A40 };


/**********************************************************************************************************************
 * Functions
 *********************************************************************************************************************/

/**
 * http_client_req
 */
http_err_t http_client_req( http_request_t *request, http_response_t *response )
{
    http_err_t ret = HTTP_ERR_INTERNAL;
    uint8_t *pbuf = buf;
    uint16_t m = HTTP_CLIENT_BUF_SIZE;
    uint16_t s;
    uint8_t i;
    uint8_t local[50];
    uint32_t u32tmp;


    if (request->host_address && request->port)
    {
        socket_address.address = request->host_address;
        socket_address.port = (int16_t)request->port;
    }
    else if (request->p_wifi->p_api->hostGetByName( request->p_wifi->p_ctrl,
                                                   request->host,
                                                   &socket_address.address ) == SSP_SUCCESS)
    {
        // go ahead..
        if (request->port)
        {
            socket_address.port = (int16_t)request->port;
        }
        else
        {
            // use standard HTTP port (80)
            socket_address.port = 80;
        }
    }
    else
    {
        return HTTP_ERR_DNS;
    }


    // build HTTP request

    // add HTTP method
    switch(request->method)
    {
        case HTTP_METHOD_GET:
            s = (uint16_t)strlen("GET ");
            strncpy( (char*)pbuf, "GET ", m );
            break;
        case HTTP_METHOD_POST:
            s = (uint16_t)strlen("POST ");
            strncpy( (char*)pbuf, "POST ", m );
            break;
        case HTTP_METHOD_DELETE:
            s = (uint16_t)strlen("DELETE ");
            strncpy( (char*)pbuf, "DELETE ", m );
            break;
        case HTTP_METHOD_PUT:
            s = (uint16_t)strlen("PUT ");
            strncpy( (char*)pbuf, "PUT ", m );
            break;
        case HTTP_METHOD_OPTIONS:
            s = (uint16_t)strlen("OPTION ");
            strncpy( (char*)pbuf, "OPTION ", m );
            break;
        case HTTP_METHOD_HEAD:
            s = (uint16_t)strlen("HEAD ");
            strncpy( (char*)pbuf, "HEAD ", m );
            break;
        default:
            return HTTP_ERR_DNS;
            break;
    }
    pbuf += s;
    m = (uint16_t) (m - s);

    // add URI
    s = (uint16_t)strlen((char*)request->uri);
    strncpy( (char*)pbuf, (char*)request->uri, m );
    pbuf += s;
    m = (uint16_t) (m - s);

    // add HTTP/1.1 label
    s = (uint16_t)strlen(" HTTP/1.1\r\n");
    strncpy( (char*)pbuf, (char*)" HTTP/1.1\r\n", m );
    pbuf += s;
    m = (uint16_t) (m - s);

    // add Headers
    // start w/ host :)
    s = (uint16_t)strlen((char*)HTTP_HEADER_HOST);
    strncpy( (char*)pbuf, (char*)HTTP_HEADER_HOST, m );
    pbuf += s;
    m = (uint16_t) (m - s);
    s = (uint16_t)strlen((char*)request->host);
    strncpy( (char*)pbuf, (char*)request->host, m );
    pbuf += s;
    m = (uint16_t) (m - s - 2);
    *pbuf++ = (uint8_t)'\r';
    *pbuf++ = (uint8_t)'\n';
    // go ahead with remaining headers
    for (i=0; i<request->headers_count; i++)
    {
        s = (uint16_t)strlen((char*)request->headers[i].key);
        strncpy( (char*)pbuf, (char*)request->headers[i].key, m );
        pbuf += s;
        m = (uint16_t) (m - s - 1);

        s = (uint16_t)strlen((char*)request->headers[i].value);
        strncpy( (char*)pbuf, (char*)request->headers[i].value, m );
        pbuf += s;
        m = (uint16_t) (m - s - 2);
        *pbuf++ = (uint8_t)'\r';
        *pbuf++ = (uint8_t)'\n';
    }

    // add Content-Length
    snprintf( (char*)local, 50, "Content-Length: %d\r\n\r\n", strlen((char*)request->body));
    strncpy( (char*)pbuf, (char*)local, m );
    s = (uint16_t)strlen((char*)local);
    pbuf += s;
    m = (uint16_t) (m - s);

    // add body where it applies
    if ( (request->method == HTTP_METHOD_POST) ||
         (request->method == HTTP_METHOD_PUT) )
    {
        s = (uint16_t) strlen((char*)request->body);
        if (s)
        {
            strncpy( (char*)pbuf, (char*)request->body, m );
            pbuf += s;
        }
    }

    *pbuf++ = 0;


    // HTTP request
    if (request->p_wifi->p_api->socketOpen( request->p_wifi->p_ctrl,
                                            &g_sock_id,
                                            (sf_wifi_socket_cfg_t * const) &g_sock_cfg ) == SSP_SUCCESS)
    {
        if (request->p_wifi->p_api->socketConnect( request->p_wifi->p_ctrl,
                                                   g_sock_id,
                                                   &socket_address ) == SSP_SUCCESS)
        {
            if (request->p_wifi->p_api->socketSend( request->p_wifi->p_ctrl,
                                                    g_sock_id,
                                                    buf, strlen((char*)buf), NULL ) == SSP_SUCCESS)
            {
                if (request->p_wifi->p_api->socketReceive( request->p_wifi->p_ctrl,
                                                           g_sock_id,
                                                           buf, HTTP_CLIENT_BUF_SIZE, &u32tmp ) == SSP_SUCCESS)
                {
                    ret = HTTP_ERR_OK;
                }
                else
                {
                    ret = HTTP_ERR_SOCKET_RECEIVE;
                }
            }
            else
            {
                ret = HTTP_ERR_SOCKET_SEND;
            }
        }
        else
        {
            ret = HTTP_ERR_SOCKET_CONNECT;
        }
    }
    else
    {
        ret = HTTP_ERR_SOCKET_OPEN;
    }

    if (g_sock_id >= 0)
    {
        if (request->p_wifi->p_api->socketClose( request->p_wifi->p_ctrl,
                                             g_sock_id ) != SSP_SUCCESS)
        {
            g_sock_id = -1;
        }
        else {
            ret = HTTP_ERR_SOCKET_CLOSE;
        }
    }

    // parse response content
    if (ret == HTTP_ERR_OK)
    {
        if (strstr((char*)buf, "HTTP/1.1") )
        {
            // TODO
            //.. go ahead searching for status code, parsing length etc.
        }
        else
        {
            ret = HTTP_ERR_RESPONSE_PARSE;
        }
        buf[u32tmp] = 0;
        response->content = buf;
    }

    return ret;
}

