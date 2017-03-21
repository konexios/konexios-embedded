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
 * File Name    : http_client_use.h
 * Description  : Minimal HTTP client library header file.
 **********************************************************************************************************************/

#ifndef HTTP_CLIENT_USE_H_
#define HTTP_CLIENT_USE_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "http_client.h"


/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * HTTP client error codes
 */
typedef enum http_err_
{
    HTTP_ERR_OK                     = 0,    ///< Successful operation
    HTTP_ERR_INTERNAL,                      ///< Internal error
    HTTP_ERR_PARAMS,                        ///< Invalid parameters
    HTTP_ERR_DNS,                           ///< Error resolving name
    HTTP_ERR_SOCKET_OPEN,                   ///< Socket open error
    HTTP_ERR_SOCKET_CONNECT,                ///< Socket connect error
    HTTP_ERR_SOCKET_SEND,                   ///< Socket send error
    HTTP_ERR_SOCKET_RECEIVE,                ///< Socket receive error
    HTTP_ERR_SOCKET_CLOSE,                  ///< Socket close error
    HTTP_ERR_RESPONSE_PARSE                 ///< Response parse error
} http_err_t;

/**
 * HTTP method.
 */
typedef enum http_method_
{
    HTTP_METHOD_GET                 = 1,    ///< Method type of GET
    HTTP_METHOD_POST,                       ///< Method type of POST
    HTTP_METHOD_DELETE,                     ///< Method type of DELETE
    HTTP_METHOD_PUT,                        ///< Method type of PUT
    HTTP_METHOD_OPTIONS,                    ///< Method type of OPTIONS
    HTTP_METHOD_HEAD,                       ///< Method type of HEAD
} http_method_t;

/**
 * HTTP status codes
 */
typedef enum http_status_code_
{
    HTTP_STATUS_CODE_OK                     = 200,  ///< 200 OK
    HTTP_STATUS_CODE_CREATED                = 201,  ///< 201 Created
    HTTP_STATUS_CODE_ACCEPTED               = 202,  ///< 202 Accepted
    HTTP_STATUS_CODE_NO_CONTENT             = 204,  ///< 204 No Content
    HTTP_STATUS_CODE_BAD_REQUEST            = 400,  ///< 400 Bad Request
    HTTP_STATUS_CODE_UNAUTHORIZED           = 401,  ///< 401 Unauthorized
    HTTP_STATUS_CODE_FORBIDDEN              = 403,  ///< 403 Forbidden
    HTTP_STATUS_CODE_NOT_FOUND              = 404,  ///< 403 Not Found
    HTTP_STATUS_CODE_METHOD_NOT_ALLOWED     = 405,  ///< 405 Method Not Allowed
    HTTP_STATUS_CODE_REQUEST_TIMEOUT        = 408,  ///< 408 Request Timeout
    HTTP_STATUS_CODE_CONFLICT               = 409,  ///< 409 Conflict
    HTTP_STATUS_CODE_INTERNAL_SERVER_ERROR  = 500,  ///< 500 Internal Server Error
    HTTP_STATUS_CODE_NOT_IMPLEMENTED        = 501,  ///< 501 Not Implemented

} http_status_code_t;

/**
 * HTTP header key-value pairs
 */
typedef struct http_header_
{
    uint8_t *key;                           ///< key string
    uint8_t *value;                         ///< value string
} http_header_t;

/**
 * HTTP request structure
 */
typedef struct http_request_
{
    http_method_t method;                   ///< HTTP request method
    uint32_t host_address;                  ///< host IP address
    uint8_t *host;                          ///< host name
    uint16_t port;                          ///< host port
    uint8_t *uri;                           ///< HTTP request uri
    uint8_t headers_count;                  ///< headers count
    http_header_t *headers;                 ///< header items
    uint8_t *body;                          ///< body content
    sf_winc1500_instance_t * p_wifi;        ///< pointer to Wi-Fi driver instance
} http_request_t;

/**
 * HTTP response structure
 */
typedef struct http_response_
{
    http_status_code_t code;                ///< HTTP response status code
    uint8_t *content;                       ///< response content
} http_response_t;


/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public functions
 **********************************************************************************************************************/

http_err_t http_client_req(http_request_t *request, http_response_t *response);


#endif /* HTTP_CLIENT_USE_H_ */

