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
 * File Name    : http_client_own.h
 * Description  : HTTP client library owner header file.
 **********************************************************************************************************************/

#ifndef HTTP_CLIENT_OWN_H_
#define HTTP_CLIENT_OWN_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "tx_api.h"

#include "http_client.h"


/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** HTTP client internal buffer max size */
#define HTTP_CLIENT_BUF_SIZE                    2048


/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Public functions
 **********************************************************************************************************************/

//void http_client_build( http_client* this_ );
//bool http_client_destroy( http_client* this_ );


#endif /* HTTP_CLIENT_OWN_H_ */
