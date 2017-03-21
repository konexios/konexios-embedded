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
 * File Name        : sf_winc1500.c
 * Description      : WINC1500 Wi-Fi module Framework Layer API Driver.
 *********************************************************************************************************************/
#define MODULE_NAME                         "SF_WINC1500"

/**********************************************************************************************************************
 * Includes
 *********************************************************************************************************************/
#include <string.h>

#include "bsp_api.h"
#include "sf_winc1500.h"
#include "sf_winc1500_cfg.h"
#include "sf_winc1500_private_api.h"
//#include "r_spi_api.h"
#include "tx_api.h"

#include "bus_wrapper/include/nm_bus_wrapper.h"
#include "driver/include/m2m_types.h"
#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"
// debug only
#include "driver/source/nmasic.h"
#include "TRACE_USE.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/
/** Macro for error logger. */
#ifndef SF_WINC1500_ERROR_RETURN
#define SF_WINC1500_ERROR_RETURN(a, err) SSP_ERROR_RETURN((a), (err), g_module_name, &g_gpt_version)
#endif

/** Macro used to convert milliseconds to tick number */
#define CONV_MS_TO_TICK(a)                  ((a)*THREADX_TICK_RATE/1000)


#define WINC1500_INSTANCE_OPEN              (0x45648652UL)
#define WINC1500_INSTANCE_CLOSED            (0x00000000UL)

//... CONSTANTS

#define WINC1500_RESET_TIME_MS              (11)

#define WINC1500_READ_TIMEOUT_CYCLES        1000

#define SF_WINC1500_VER                     1

/**********************************************************************************************************************
 * Typedef definitions
 *********************************************************************************************************************/
/** Host resolution events */
typedef enum e_sf_wifi_host_events
{
    SF_WIFI_HOST_EVENT_RESOLVED             = (1 << 0),         ///< Host ip from name resolved
} e_sf_wifi_host_events_t;

/** Socket status */
typedef enum e_sf_wifi_socket_status
{
    SF_WIFI_SOCKET_STATUS_UNDEF             = (1 << 0),         ///< Socket undefined
    SF_WIFI_SOCKET_STATUS_CREATED           = (1 << 1),         ///< Socket created
    SF_WIFI_SOCKET_STATUS_CONNECTED         = (1 << 2),         ///< Socket connected
    SF_WIFI_SOCKET_STATUS_SENT              = (1 << 3),         ///< Socket data sent
#if 1
    SF_WIFI_SOCKET_STATUS_CREATING          = (1 << 4),         ///< Socket creation pending
#endif
} e_sf_wifi_socket_status_t;

/** Socket events */
typedef enum e_sf_wifi_socket_events
{
    SF_WIFI_SOCKET_EVENT_BIND               = (1 << 0),         ///< Bind socket event
    SF_WIFI_SOCKET_EVENT_LISTEN             = (1 << 1),         ///< Listen socket event
    SF_WIFI_SOCKET_EVENT_DNS_RESOLVE        = (1 << 2),         ///< DNS Resolution event
    SF_WIFI_SOCKET_EVENT_ACCEPT             = (1 << 3),         ///< Accept socket event
    SF_WIFI_SOCKET_EVENT_CONNECT            = (1 << 4),         ///< Connect socket event
    SF_WIFI_SOCKET_EVENT_RECV               = (1 << 5),         ///< Receive socket event
    SF_WIFI_SOCKET_EVENT_SEND               = (1 << 6),         ///< Send socket event
    SF_WIFI_SOCKET_EVENT_SENDTO             = (1 << 7),         ///< Sendto socket event
    SF_WIFI_SOCKET_EVENT_RECVFROM           = (1 << 8),         ///< Recvfrom socket event
#if 1
    SF_WIFI_SOCKET_EVENT_OPEN               = (1 << 9),         ///< Open socket event
    SF_WIFI_SOCKET_EVENT_CONNECTING         = (1 << 10),        ///< Socket connecting event
    SF_WIFI_SOCKET_EVENT_SENDING            = (1 << 11),        ///< Socket sending event
    SF_WIFI_SOCKET_EVENT_RECEIVING          = (1 << 12),        ///< Socket receiving event
    SF_WIFI_SOCKET_EVENT_CLOSE              = (1 << 13),        ///< Close socket event
#endif
} e_sf_wifi_socket_events_t;

typedef struct st_sf_wifi_socket_info
{
    TX_MUTEX                                mutex;              ///< Socket info mutex
    TX_EVENT_FLAGS_GROUP                    events;             ///< Socket event flags
    e_sf_wifi_socket_status_t               status;             ///< Socket status
    SOCKET                                  socket;             ///< Socket reference
    uint16_t                                tx_count;           ///< Socket received bytes count
    uint16_t                                rx_count;           ///< Socket received bytes count
} st_sf_wifi_socket_info_t;

typedef struct st_sf_wifi_socket_conv
{
    TX_MUTEX                                mutex;              ///< Socket info mutex
    SOCKET                                  socket[SF_WINC1500_CFG_PARAM_MAX_SOCKETS];          ///< Socket reference
} st_sf_wifi_socket_conv_t;

typedef struct st_sf_wifi_host_by_name
{
    TX_MUTEX                                mutex;              ///< Host resolution mutex
    TX_EVENT_FLAGS_GROUP                    events;             ///< Host resolution events
    bool                                    in_progress;        ///< Host resolution in progress (true: in progress, false: done)
    uint8_t *                               p_host_name;        ///< Host name
    uint32_t *                              p_host_ip;          ///< Host IP address
} st_sf_wifi_host_by_name_t;

typedef struct st_sf_wifi
{
    TX_THREAD                               thread;             ///< Wi-Fi module driver internal thread
    sf_winc1500_ctrl_t *                    p_ctrl;             ///< Wi-Fi module framework control structure
    st_sf_wifi_host_by_name_t               host;               ///< Support for ip from name host resolution
    st_sf_wifi_socket_info_t                socket_infos[SF_WINC1500_CFG_PARAM_MAX_SOCKETS];    ///< Socket info internal structure
    st_sf_wifi_socket_conv_t                socket_conv;        ///< Table for socket id conversion
} st_sf_wifi_t;

// socket open request
typedef struct st_sf_wifi_req_sock_open
{
    uint8_t                                 socket_info_id;
    sf_wifi_socket_cfg_t *                  p_cfg;
} st_sf_wifi_req_sock_open_t;
// socket close request
typedef struct st_sf_wifi_req_sock_close
{
    int8_t                                  sid;
    int8_t                                  socket;
} st_sf_wifi_req_sock_close_t;
// socket connect request
typedef struct st_sf_wifi_req_sock_connect
{
    int8_t                                  sid;
    int8_t                                  socket;
    sf_wifi_socket_address_t *              p_socket_address;
} st_sf_wifi_req_sock_connect_t;
// socket send request
typedef struct st_sf_wifi_req_sock_send
{
    int8_t                                  sid;
    int8_t                                  socket;
    uint8_t *                               p_buf;
    uint32_t                                length;
} st_sf_wifi_req_sock_send_t;
// socket receive request
typedef struct st_sf_wifi_req_sock_receive
{
    int8_t                                  sid;
    int8_t                                  socket;
    uint8_t *                               p_buf;
    uint32_t                                length;
} st_sf_wifi_req_sock_receive_t;
// socket request union
typedef union st_sf_wifi_req_all
{
    st_sf_wifi_req_sock_open_t              req_sock_open;
    st_sf_wifi_req_sock_close_t             req_sock_close;
    st_sf_wifi_req_sock_connect_t           req_sock_connect;
    st_sf_wifi_req_sock_send_t              req_sock_send;
    st_sf_wifi_req_sock_receive_t           req_sock_receive;
} st_sf_wifi_req_all_t;
// socket request structure
typedef struct st_sf_wifi_req
{
    uint8_t                                 type;
    st_sf_wifi_req_all_t                    request;
} st_sf_wifi_req_t;

/**********************************************************************************************************************
 * Private function prototypes
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
static const ssp_version_t g_version =
{
    .api_version_major  = SF_WINC1500_API_VERSION_MAJOR,
    .api_version_minor  = SF_WINC1500_API_VERSION_MINOR,
    .code_version_major = SF_WINC1500_CODE_VERSION_MAJOR,
    .code_version_minor = SF_WINC1500_CODE_VERSION_MINOR
};

static p_status_cb_t gp_status_cb = NULL;

/** Name of module used by error logger macro */
static const char g_module_name[] = "sf_winc1500";

static uint8_t wifi_internal_thread_stack[4096];

static st_sf_wifi_t g_wifi;
static TX_QUEUE st_sf_wifi_queue_req;
static uint8_t st_sf_wifi_queue_storage[300];

/**********************************************************************************************************************
 * Private global variables
 *********************************************************************************************************************/
const sf_winc1500_api_t g_sf_winc1500_api =
{
    .open               = SF_WINC1500_Open,
    .infoGet            = SF_WINC1500_InfoGet,
    .reset              = SF_WINC1500_Reset,
    .eventSignal        = SF_WINC1500_EventSignal,
    .connect            = SF_WINC1500_Connect,
    .close              = SF_WINC1500_Close,
    .run                = SF_WINC1500_Run,
    .versionGet         = SF_WINC1500_VersionGet,
    .hostGetByName      = SF_WINC1500_HostGetByName,
    .socketOpen         = SF_WINC1500_SocketOpen,
    .socketClose        = SF_WINC1500_SocketClose,
    .socketConnect      = SF_WINC1500_SocketConnect,
    .socketSend         = SF_WINC1500_SocketSend,
    .socketReceive      = SF_WINC1500_SocketReceive
};

/**********************************************************************************************************************
 * Functions
 *********************************************************************************************************************/
static ssp_err_t SF_WINC1500_eventManage(sf_winc1500_ctrl_t * const p_ctrl);

static void SF_WINC1500_wifiStatusCb ( uint8_t msg_type,
                                       void * p_msg );

static void SF_WINC1500_wifiTcpEventsCb ( SOCKET sock,
                                          uint8_t msg_type,
                                          void * p_msg );

static void SF_WINC1500_wifiDnsCb ( uint8_t * p_host_name,
                                    uint32_t server_ip );

static VOID SF_WINC1500_wifi_internal_thread (ULONG initial_input);

/**********************************************************************************************************************
 * @addtogroip SF_WINC1500
 * @{
 *********************************************************************************************************************/
 
/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::open.
 *
 * @retval SSP_SUCCESS          Lower level drivers opened successfully.
 * @retval SSP_ERR_ASSERTION    A pointer paramer was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_ABORTED      Test read of device identification register failed.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_Open ( sf_winc1500_ctrl_t      * const p_ctrl,
                             sf_winc1500_cfg_t const * const p_cfg)
{
    ssp_err_t err;
    uint8_t i;


#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_cfg);
    SSP_ASSERT(NULL != p_cfg->device);
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */
    
    /** Store user configurations in control block. */
    p_ctrl->device = p_cfg->device;
    p_ctrl->irq = p_cfg->irq;
    if (p_cfg->p_status_cb != (p_status_cb_t)NULL)
    {
        gp_status_cb = p_cfg->p_status_cb;
    }
    p_ctrl->p_device_info = p_cfg->p_device_info;
    p_ctrl->p_provisioning = p_cfg->p_provisioning;

    /** Save pointer into internal global Wi-Fi module structure */
    g_wifi.p_ctrl = p_ctrl;

    /** Initialize remaining control block parameters */
    p_ctrl->ip.s_addr = 0;
    p_ctrl->connected = false;
    memset((uint8_t *)&p_ctrl->param, 0, sizeof(tstrWifiInitParam));

    /** Open SPI Framework Device. */
    err = p_ctrl->device->p_api->open(  p_ctrl->device->p_ctrl,
                                        p_ctrl->device->p_cfg );
    SF_WINC1500_ERROR_RETURN( SSP_SUCCESS == err, err );
    
    /** Initialize the BSP */
    /** TODO add return check */
    nm_bsp_init( p_ctrl->irq );
    nm_bus_config( p_ctrl->device );

    p_ctrl->param.pfAppWifiCb = SF_WINC1500_wifiStatusCb;
    SSP_ASSERT( M2M_SUCCESS == m2m_wifi_init( &p_ctrl->param ) );

    // TODO move thread priority to module configuration!
#if 1
    SSP_ASSERT( tx_thread_create(&g_wifi.thread,
                                 (CHAR*)"wifi_internal_thread",
                                 SF_WINC1500_wifi_internal_thread,
                                 (ULONG) NULL,
                                 wifi_internal_thread_stack, 4096,
                                 3, 3, 1,
                                 TX_AUTO_START) == TX_SUCCESS );
#endif

    SSP_ASSERT( tx_mutex_create( &g_wifi.host.mutex,
                                 (CHAR*)"host_mutex",
                                 TX_NO_INHERIT ) == TX_SUCCESS );

    SSP_ASSERT( tx_event_flags_create( &g_wifi.host.events,
                                       (CHAR*)"host_events" ) == TX_SUCCESS );

    /** Initialize the socket layer */
    socketInit();
    /** Create incoming socket request parameters queue */
    SSP_ASSERT( tx_queue_create( &st_sf_wifi_queue_req,
                                 (CHAR*)"wifi_sock_req_queue",
                                 sizeof(st_sf_wifi_req_t),
                                 (VOID*)st_sf_wifi_queue_storage,
                                 300) == TX_SUCCESS );

    /** Register application socket callbacks */
    registerSocketCallback( SF_WINC1500_wifiTcpEventsCb, SF_WINC1500_wifiDnsCb );

    for (i=0; i<SF_WINC1500_CFG_PARAM_MAX_SOCKETS; i++)
    {
        g_wifi.socket_infos[i].status = SF_WIFI_SOCKET_STATUS_UNDEF;
        SSP_ASSERT( tx_mutex_create( &g_wifi.socket_infos[i].mutex,
                                     (CHAR*)"socket_mutex",
                                     TX_NO_INHERIT ) == TX_SUCCESS );
        SSP_ASSERT( tx_event_flags_create( &g_wifi.socket_infos[i].events,
                                           (CHAR*)"socket_events" ) == TX_SUCCESS );
    }

    SSP_ASSERT( tx_mutex_create( &g_wifi.socket_conv.mutex,
                                 (CHAR*)"socket_conv_mutex",
                                 TX_NO_INHERIT ) == TX_SUCCESS );
    for (i=0; i<SF_WINC1500_CFG_PARAM_MAX_SOCKETS; i++)
    {
        g_wifi.socket_conv.socket[i] = -1;
    }

    /** Setup device information structure */
    SSP_ASSERT( M2M_SUCCESS == m2m_wifi_get_mac_address(p_ctrl->p_device_info->mac) );

    /** Mark control block open so other tasks know it is valid. */
    p_ctrl->open = WINC1500_INSTANCE_OPEN;
    
    return SSP_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::infoGet.
 *
 * @param p_ctrl
 * @param p_device_info
 * @return
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_InfoGet(sf_winc1500_ctrl_t * const p_ctrl,
                              sf_wifi_device_info_t * const p_device_info)
{
    memcpy(p_device_info, p_ctrl->p_device_info, sizeof(sf_wifi_device_info_t));

    return SSP_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::reset.
 *
 * @retval SSP_SUCCESS          WINC1500 were reset successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_WINC1500_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_Reset (sf_winc1500_ctrl_t * const p_ctrl)
{
    //ssp_err_t err;
    

#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_WINC1500_ERROR_RETURN( WINC1500_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */
    
    /** TODO */

    return SSP_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::eventSignal.
 *
 * @retval SSP_SUCCESS          Incoming WINC1500 interrupt event successfully managed.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_WINC1500_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_EventSignal (sf_winc1500_ctrl_t * const p_ctrl)
{
#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT( NULL != p_ctrl );
    SF_WINC1500_ERROR_RETURN( WINC1500_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN );
#else
    SSP_PARAMETER_NOT_USED( p_ctrl );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */

    nm_bsp_wifi_chip_isr();

    return SSP_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::connect.
 *
 * @retval SSP_SUCCESS          Manage WINC1500 events.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_WINC1500_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_Connect (sf_winc1500_ctrl_t * const p_ctrl)
{
    uint8_t sec;

#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT( NULL != p_ctrl );
    SF_WINC1500_ERROR_RETURN( WINC1500_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN );
#else
    SSP_PARAMETER_NOT_USED( p_ctrl );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */

    if (p_ctrl->p_provisioning->mode == SF_WIFI_INTERFACE_MODE_CLIENT)
    {
        switch (p_ctrl->p_provisioning->sec)
        {
            case SF_WIFI_SECURITY_TYPE_OPEN:
                sec = M2M_WIFI_SEC_OPEN;
                break;
            case SF_WIFI_SECURITY_TYPE_WEP40:
            case SF_WIFI_SECURITY_TYPE_WEP128:
                sec = M2M_WIFI_SEC_WEP;
                break;
            case SF_WIFI_SECURITY_TYPE_WPA:
            case SF_WIFI_SECURITY_TYPE_WPA2:
                sec = M2M_WIFI_SEC_WPA_PSK;
                break;
            default:
                sec = M2M_WIFI_SEC_INVALID;
                break;
        }
        int8_t ret = m2m_wifi_connect( (char *)p_ctrl->p_provisioning->ssid,
                          (uint8)strlen((char *)p_ctrl->p_provisioning->ssid),
                          sec,
                          (char *)p_ctrl->p_provisioning->key,
                          M2M_WIFI_CH_ALL );
        if ( M2M_SUCCESS != ret ) {
            TRACE("m2m_wifi_connect fail\r\n");
        }

        return SSP_SUCCESS;
    }
    else if (p_ctrl->p_provisioning->mode == SF_WIFI_INTERFACE_MODE_AP)
    {
        // TODO Add AP mode management
    }

    return SSP_ERR_UNSUPPORTED;
}

/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::close.
 *
 * @retval SSP_SUCCESS          WINC1500 instance successfully closed and device resetted.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_WINC1500_Open.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_Close (sf_winc1500_ctrl_t * const p_ctrl)
{
    ssp_err_t err;
    uint8_t i;


#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SF_WINC1500_ERROR_RETURN( WINC1500_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */
    
    /** TODO ... */
    /** Close SPI Framework Device. */
    err = p_ctrl->device->p_api->close(p_ctrl->device->p_ctrl);
    SF_WINC1500_ERROR_RETURN( SSP_SUCCESS == err, err );

    for (i=0; i<SF_WINC1500_CFG_PARAM_MAX_SOCKETS; i++)
    {
        g_wifi.socket_infos[i].status = SF_WIFI_SOCKET_STATUS_UNDEF;
        SSP_ASSERT( tx_mutex_delete( &g_wifi.socket_infos[i].mutex ) == TX_SUCCESS );
        SSP_ASSERT( tx_event_flags_delete( &g_wifi.socket_infos[i].events ) == TX_SUCCESS );
    }

    /** Mark control block open so other tasks know it is NOT valid. */
    p_ctrl->open = WINC1500_INSTANCE_CLOSED;
    
    return SSP_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::versionGet.
 *
 * @retval SSP_SUCCESS          Version returned successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_version was NULL.
 * @return                      See @ref Common_Error_Codes.
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_VersionGet (ssp_version_t * const p_version)
{
#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_version);
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */
    
    *p_version = g_version;
    
    return SSP_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Implements sf_winc1500_api_t::hostGetByName
 *
 * @retval SSP_SUCCESS          Host resolved successfully.
 * @retval SSP_ERR_ASSERTION    Parameter p_version was NULL.
 * @return                      See @ref Common_Error_Codes.
 *********************************************************************************************************************/
ssp_err_t SF_WINC1500_HostGetByName (sf_winc1500_ctrl_t * const p_ctrl, uint8_t * p_host_name, uint32_t * p_host_ip)
{
    ssp_err_t ret;
    uint32_t host_ip;
    uint32_t tmp;


#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT(NULL != p_ctrl);
    SSP_ASSERT(NULL != p_host_name);
    SSP_ASSERT(NULL != p_host_ip);
    SF_WINC1500_ERROR_RETURN( WINC1500_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */

    ret =  SSP_ERR_INTERNAL;

    if (tx_mutex_get(&g_wifi.host.mutex, CONV_MS_TO_TICK(200)) == TX_SUCCESS)
    {
        //if (g_wifi.host.in_progress == false)
        //{
            g_wifi.host.in_progress = true;
            g_wifi.host.p_host_name = p_host_name;
            g_wifi.host.p_host_ip = &host_ip;
            ret = SSP_SUCCESS;
        //}
            SSP_ASSERT( tx_mutex_put(&g_wifi.host.mutex) == TX_SUCCESS );
    }

    if (ret == SSP_SUCCESS)
    {
        ret = SSP_ERR_INTERNAL;

        if (SOCK_ERR_NO_ERROR == gethostbyname(p_host_name))
        {
            if ( tx_event_flags_get( &g_wifi.host.events,
                                     SF_WIFI_HOST_EVENT_RESOLVED,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
            {
                if (host_ip > 0)
                {
                    *p_host_ip = host_ip;
                    ret = SSP_SUCCESS;
                }
            }
            else
            {
                SSP_ASSERT( 0 );
            }
        }
    }

    return ret;
}


/**
 *
 * @param socket
 * @param p_cfg
 * @return
 */
ssp_err_t SF_WINC1500_SocketOpen(sf_winc1500_ctrl_t * const p_ctrl,
                                 int32_t * p_sid,
                                 sf_wifi_socket_cfg_t * const p_cfg)
{
    ssp_err_t ret = SSP_ERR_INTERNAL;
    //SOCKET sid;
    uint8_t i;
    bool found = false;
    
    SSP_PARAMETER_NOT_USED( p_ctrl );
    
    *p_sid = -1;

#if ( SF_WINC1500_VER == 0 )
    // TODO INVERT placeholder search with socket actual creation
    sid = socket( (uint16_t)p_cfg->protocol_family, p_cfg->socket_type, 0);

    if (sid >= 0)
    {
        // TODO add retries if undefined socket(s) is(are) temporarily busy..
        for (i=0; i<SF_WINC1500_CFG_PARAM_MAX_SOCKETS; i++)
        {
            if (tx_mutex_get(&g_wifi.socket_infos[i].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
            {
                //if (g_wifi.socket_infos[i].status ==  SF_WIFI_SOCKET_STATUS_UNDEF)
                //{
                    g_wifi.socket_infos[i].status =  SF_WIFI_SOCKET_STATUS_CREATED;
                    g_wifi.socket_infos[i].socket = sid;
                    g_wifi.socket_conv.socket[i] = sid;
                    *p_sid = i;

                //}
                ASSERT( tx_mutex_put(&g_wifi.socket_infos[i].mutex) == TX_SUCCESS );
            }
            if (*p_sid >= 0)
                return SSP_SUCCESS;
        }

        // TODO close lower level socket on framework error
        close( sid );
    }
#else
    for (i=0; i<SF_WINC1500_CFG_PARAM_MAX_SOCKETS; i++)
    {
        //if (tx_mutex_get(&g_wifi.socket_infos[i].mutex, CONV_MS_TO_TICK(20)) == TX_SUCCESS)
        //if (tx_mutex_get(&g_wifi.socket_infos[i].mutex, TX_NO_WAIT) == TX_SUCCESS)
        SSP_ASSERT (tx_mutex_get(&g_wifi.socket_infos[i].mutex, TX_NO_WAIT) == TX_SUCCESS)
        {
            if (g_wifi.socket_infos[i].status == SF_WIFI_SOCKET_STATUS_UNDEF)
            {
                g_wifi.socket_infos[i].status = SF_WIFI_SOCKET_STATUS_CREATING;
                found = true;
            }
            SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[i].mutex) == TX_SUCCESS );
            if (found)
            {
                break;
            }
        }
    }
    if (i < SF_WINC1500_CFG_PARAM_MAX_SOCKETS)
    {
        st_sf_wifi_req_t req;
        uint32_t tmp;

        req.type = 0;   // open request
        req.request.req_sock_open.socket_info_id = i;
        req.request.req_sock_open.p_cfg = p_cfg;
        // free space found, append socket open request
        SSP_ASSERT( tx_queue_send( &st_sf_wifi_queue_req,
                               &req, TX_NO_WAIT ) == TX_SUCCESS );

        // start waiting for creation event
        if ( tx_event_flags_get( &g_wifi.socket_infos[i].events,
                                 SF_WIFI_SOCKET_EVENT_OPEN,
                                 TX_AND_CLEAR,
                                 &tmp,
                                 CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
                                 //TX_WAIT_FOREVER ) == TX_SUCCESS )
        {
            if (tx_mutex_get(&g_wifi.socket_infos[i].mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
            {
                // g_wifi.socket_infos[i].status is now SF_WIFI_SOCKET_STATUS_CREATED
                *p_sid = g_wifi.socket_infos[i].socket;
                SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[i].mutex) == TX_SUCCESS );

                if (*p_sid >= 0)
                    return SSP_SUCCESS;
            }
        }
    }

#endif
    return ret;
}

/**
 *
 * @param socket
 * @return
 */
ssp_err_t SF_WINC1500_SocketClose(sf_winc1500_ctrl_t * const p_ctrl, int32_t sid)
{
    ssp_err_t ret;
    int32_t socket;


#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT( sid < SF_WINC1500_CFG_PARAM_MAX_SOCKETS );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */

    SSP_PARAMETER_NOT_USED( p_ctrl );

    ret = SSP_ERR_INTERNAL;

    if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
    {
        socket = g_wifi.socket_infos[sid].socket;

        SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );

        ret = SSP_SUCCESS;
    }
    if (ret == SSP_SUCCESS)
    {
        ret = SSP_ERR_HW_LOCKED;
        {
            st_sf_wifi_req_t req;
            uint32_t tmp;

            req.type = 1;   // close request
            req.request.req_sock_close.sid = (int8_t)sid;
            req.request.req_sock_close.socket = (int8_t)socket;
            // append socket connect request
            SSP_ASSERT( tx_queue_send( &st_sf_wifi_queue_req,
                                       &req, TX_NO_WAIT ) == TX_SUCCESS );

            if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
            //ASSERT ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                     SF_WIFI_SOCKET_EVENT_CLOSE,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
                                     //TX_WAIT_FOREVER ) == TX_SUCCESS )
            {
                if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(5000)) == TX_SUCCESS)
                {
                    g_wifi.socket_infos[sid].status = SF_WIFI_SOCKET_STATUS_UNDEF;
                    g_wifi.socket_conv.socket[sid] = -1;
                    SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
                }
                //°°°
                /*
                if (tx_mutex_get(&g_wifi.socket_conv.mutex, CONV_MS_TO_TICK(5000)) == TX_SUCCESS)
                {
                    tx_mutex_put(&g_wifi.socket_conv.mutex);
                    ret = SSP_SUCCESS;
                }
                */
            }
        }
    }

    return ret;
}

/**
 *
 * @param socket
 * @param p_socket_address
 * @return
 */
ssp_err_t SF_WINC1500_SocketConnect(sf_winc1500_ctrl_t * const p_ctrl, int32_t sid, sf_wifi_socket_address_t * p_socket_address)
{
    ssp_err_t ret;
    int32_t socket;
    //struct sockaddr_in strAddr;


#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT( sid < SF_WINC1500_CFG_PARAM_MAX_SOCKETS );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */

    SSP_PARAMETER_NOT_USED( p_ctrl );

    ret = SSP_ERR_INTERNAL;


    if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
    //ASSERT (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
    {
        socket = g_wifi.socket_infos[sid].socket;
        if ( g_wifi.socket_infos[sid].status == SF_WIFI_SOCKET_STATUS_CREATED )
        {
            ret = SSP_SUCCESS;
        }
        SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
    }

    if (ret == SSP_SUCCESS)
    {
        ret = SSP_ERR_HW_LOCKED;

#if ( SF_WINC1500_VER == 0 )
        strAddr.sin_family = (uint16_t)p_socket_address->family;
        strAddr.sin_port = _htons( p_socket_address->port );
        strAddr.sin_addr.s_addr = p_socket_address->address;

        if (SOCK_ERR_NO_ERROR == connect( (SOCKET)socket, (struct sockaddr*)&strAddr, sizeof(struct sockaddr_in) ))
        {
            uint32_t tmp;

            // wait for low-level event..
            if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                     SF_WIFI_SOCKET_EVENT_CONNECT,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
            {
                if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
                {
                    g_wifi.socket_infos[sid].status =  SF_WIFI_SOCKET_STATUS_CONNECTED;
                    ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
                    ret = SSP_SUCCESS;
                }
                else
                {
                    ret = SSP_ERR_INTERNAL;
                }
            }
        }
#else
        {
            st_sf_wifi_req_t req;
            uint32_t tmp;

            req.type = 2;   // open request
            req.request.req_sock_connect.sid = (int8_t)sid;
            req.request.req_sock_connect.socket = (int8_t)socket;
            req.request.req_sock_connect.p_socket_address = p_socket_address;
            // append socket connect request
            SSP_ASSERT( tx_queue_send( &st_sf_wifi_queue_req,
                                       &req, TX_NO_WAIT ) == TX_SUCCESS );

            if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                     SF_WIFI_SOCKET_EVENT_CONNECTING,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
                                     //TX_WAIT_FOREVER ) == TX_SUCCESS )
            {
                ret = SSP_ERR_INTERNAL;

            #if 1
                if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                         SF_WIFI_SOCKET_EVENT_CONNECT,
                                         TX_AND_CLEAR,
                                         &tmp,
                                         CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
            #else
                ASSERT ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                         SF_WIFI_SOCKET_EVENT_CONNECT,
                                         TX_AND_CLEAR,
                                         &tmp,
                                         CONV_MS_TO_TICK(20000) ) == TX_SUCCESS )
            #endif
                {
                    if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(5000)) == TX_SUCCESS)
                    {
                        g_wifi.socket_infos[sid].status =  SF_WIFI_SOCKET_STATUS_CONNECTED;
                        SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
                        ret = SSP_SUCCESS;
                    }
                }
            }
        }
#endif
    }

    return ret;
}

/**
 *
 * @param p_ctrl
 * @param sid
 * @param p_buf
 * @param length
 * @param p_bytes_sent
 * @return
 */
ssp_err_t SF_WINC1500_SocketSend(sf_winc1500_ctrl_t * const p_ctrl, int32_t sid, uint8_t * p_buf, uint32_t length, uint32_t * p_bytes_sent)
{
    ssp_err_t ret;
    int32_t socket;


#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT( sid < SF_WINC1500_CFG_PARAM_MAX_SOCKETS );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */

    SSP_PARAMETER_NOT_USED( p_ctrl );

    ret = SSP_ERR_INTERNAL;

    if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
    //ASSERT (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
    {
        socket = g_wifi.socket_infos[sid].socket;
        if ( (g_wifi.socket_infos[sid].status == SF_WIFI_SOCKET_STATUS_CONNECTED) ||
             (g_wifi.socket_infos[sid].status == SF_WIFI_SOCKET_STATUS_SENT) )
        //ASSERT ( (g_wifi.socket_infos[sid].status == SF_WIFI_SOCKET_STATUS_CONNECTED) ||
        //     (g_wifi.socket_infos[sid].status == SF_WIFI_SOCKET_STATUS_SENT) )
        {
            ret = SSP_SUCCESS;
        }
        SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
    }

    if (ret == SSP_SUCCESS)
    {
        ret = SSP_ERR_HW_LOCKED;

#if ( SF_WINC1500_VER == 0 )

        if ( SOCK_ERR_NO_ERROR == send((SOCKET)socket, p_buf, (uint16_t)length, 0) )
        {
            uint32_t tmp;

            // wait for low-level event..
            if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                     SF_WIFI_SOCKET_EVENT_SEND,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
            {
                if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
                {
                    *p_bytes_sent = g_wifi.socket_infos[sid].tx_count;
                    g_wifi.socket_infos[sid].status =  SF_WIFI_SOCKET_STATUS_SENT;
                    ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
                    ret = SSP_SUCCESS;
                }
            }
        }
#else
        {
            st_sf_wifi_req_t req;
            uint32_t tmp;

            req.type = 3;   // send request
            req.request.req_sock_send.sid = (int8_t)sid;
            req.request.req_sock_send.socket = (int8_t)socket;
            req.request.req_sock_send.p_buf = p_buf;
            req.request.req_sock_send.length = length;
            // append socket send request
            SSP_ASSERT( tx_queue_send( &st_sf_wifi_queue_req,
                                       &req, TX_NO_WAIT ) == TX_SUCCESS );

            if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                     SF_WIFI_SOCKET_EVENT_SENDING,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
                                     //TX_WAIT_FOREVER ) == TX_SUCCESS )
            {
                ret = SSP_ERR_INTERNAL;

            #if 1
                if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                                     SF_WIFI_SOCKET_EVENT_SEND,
                                                     TX_AND_CLEAR,
                                                     &tmp,
                                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
            #else
                ASSERT ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                                     SF_WIFI_SOCKET_EVENT_SEND,
                                                     TX_AND_CLEAR,
                                                     &tmp,
                                                     CONV_MS_TO_TICK(20000) ) == TX_SUCCESS )
            #endif
                {
                    if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(5000)) == TX_SUCCESS)
                    //ASSERT (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(5000)) == TX_SUCCESS)
                    {
                        *p_bytes_sent = g_wifi.socket_infos[sid].tx_count;
                        g_wifi.socket_infos[sid].status =  SF_WIFI_SOCKET_STATUS_SENT;
                        SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
                        ret = SSP_SUCCESS;
                    }
                }
            }
        }
#endif
    }

    return ret;
}

/**
 *
 * @param p_ctrl
 * @param sid
 * @param p_buf
 * @param length
 * @param p_bytes_received
 * @return
 */
ssp_err_t SF_WINC1500_SocketReceive( sf_winc1500_ctrl_t * const p_ctrl,
                                     int32_t sid,
                                     uint8_t * p_buf,
                                     uint32_t length,
                                     uint32_t * p_bytes_received )
{
    ssp_err_t ret;
    int32_t socket;


#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT( sid < SF_WINC1500_CFG_PARAM_MAX_SOCKETS );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */

    SSP_PARAMETER_NOT_USED( p_ctrl );

    ret = SSP_ERR_INTERNAL;

    if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
    {
        socket = g_wifi.socket_infos[sid].socket;
        if ( (g_wifi.socket_infos[sid].status == SF_WIFI_SOCKET_STATUS_CONNECTED) ||
             (g_wifi.socket_infos[sid].status == SF_WIFI_SOCKET_STATUS_SENT) )
        {
            ret = SSP_SUCCESS;
        }
        SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
    }

    if (ret == SSP_SUCCESS)
    {
        ret = SSP_ERR_HW_LOCKED;

#if ( SF_WINC1500_VER == 0 )
        if ( SOCK_ERR_NO_ERROR == recv((SOCKET)socket, p_buf, (uint16_t)length, 0) )
        {
            uint32_t tmp;

            // wait for low-level event..
            if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                     SF_WIFI_SOCKET_EVENT_RECV,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
            {
                if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
                {
                    *p_bytes_received = g_wifi.socket_infos[sid].rx_count;
                    g_wifi.socket_infos[sid].status =  SF_WIFI_SOCKET_STATUS_CONNECTED;
                    ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
                    ret = SSP_SUCCESS;
                }
            }
        }
#else
        {

            st_sf_wifi_req_t req;
            uint32_t tmp;

            req.type = 4;   // send request
            req.request.req_sock_receive.sid = (int8_t)sid;
            req.request.req_sock_receive.socket = (int8_t)socket;
            req.request.req_sock_receive.p_buf = p_buf;
            req.request.req_sock_receive.length = length;
            // append socket send request
            SSP_ASSERT( tx_queue_send( &st_sf_wifi_queue_req,
                                       &req, TX_NO_WAIT ) == TX_SUCCESS );

            if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                     SF_WIFI_SOCKET_EVENT_RECEIVING,
                                     TX_AND_CLEAR,
                                     &tmp,
                                     CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
                                     //TX_WAIT_FOREVER ) == TX_SUCCESS )
            {
                ret = SSP_ERR_INTERNAL;

            #if 1
                if ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                         SF_WIFI_SOCKET_EVENT_RECV,
                                         TX_AND_CLEAR,
                                         &tmp,
                                         CONV_MS_TO_TICK(10000) ) == TX_SUCCESS )
            #else
                ASSERT ( tx_event_flags_get( &g_wifi.socket_infos[sid].events,
                                         SF_WIFI_SOCKET_EVENT_RECV,
                                         TX_AND_CLEAR,
                                         &tmp,
                                         CONV_MS_TO_TICK(20000) ) == TX_SUCCESS )
            #endif
                {
                    if (tx_mutex_get(&g_wifi.socket_infos[sid].mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
                    {
                        *p_bytes_received = g_wifi.socket_infos[sid].rx_count;
                        g_wifi.socket_infos[sid].status =  SF_WIFI_SOCKET_STATUS_CONNECTED;
                        SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[sid].mutex) == TX_SUCCESS );
                        ret = SSP_SUCCESS;
                    }
                }
            }
        }
#endif
    }

    return ret;
}



/**********************************************************************************************************************
 * @brief Implements SF_WINC1500_eventManage
 *
 * @retval SSP_SUCCESS          Manage WINC1500 events.
 * @retval SSP_ERR_ASSERTION    Parameter p_ctrl was NULL, or a lower level driver reported this error.
 * @retval SSP_ERR_NOT_OPEN     Driver is not configured. Call SF_WINC1500_Open.
 * @retval SSP_ERR_INTERNAL     Error during event management.
 * @return                      See @ref Common_Error_Codes or lower level drivers for other possible return codes.
 * @note This function is reentrant for any panel.
 *********************************************************************************************************************/
static ssp_err_t SF_WINC1500_eventManage (sf_winc1500_ctrl_t * const p_ctrl)
{
#if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE
    SSP_ASSERT( NULL != p_ctrl );
    SF_WINC1500_ERROR_RETURN( WINC1500_INSTANCE_OPEN == p_ctrl->open, SSP_ERR_NOT_OPEN );
#else
    SSP_PARAMETER_NOT_USED( p_ctrl );
#endif /* if SF_WINC1500_CFG_PARAM_CHECKING_ENABLE */


// §§§
#if 0
    while (m2m_wifi_handle_events(NULL) != M2M_SUCCESS)
    { ; }

    /*
    if (m2m_wifi_handle_events(NULL) == M2M_SUCCESS)
    {
        return SSP_SUCCESS;
    }
    else
    {
        return SSP_ERR_INTERNAL;
    }
    */
    return SSP_SUCCESS;
#else
    if (m2m_wifi_handle_events(NULL) == M2M_SUCCESS)
    {
        return SSP_SUCCESS;
    }
    else
    {
        return SSP_ERR_INTERNAL;
    }
#endif
}

/**********************************************************************************************************************
 * @brief Implements static SF_WINC1500_wifiStatusCb
 *********************************************************************************************************************/
static void SF_WINC1500_wifiStatusCb ( uint8_t msg_type,
                                       void * p_msg )
{
    switch (msg_type)
    {
        case M2M_WIFI_RESP_CON_STATE_CHANGED:
            {
                tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)p_msg;
                sf_winc1500_status_callback_args_t args = { SF_WINC1500_EVENT_UNDEF, NULL };

                if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED)
                {
                    args.event = SF_WINC1500_EVENT_CONNECT;

                    m2m_wifi_request_dhcp_client();
                }
                else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED)
                {
                    args.event = SF_WINC1500_EVENT_DISCONNECT;
                }


                if (gp_status_cb != (p_status_cb_t)NULL)
                {
                    if (args.event != SF_WINC1500_EVENT_UNDEF)
                    {
                        (*gp_status_cb)( &args );
                    }
                }
            }
            break;

        case M2M_WIFI_REQ_DHCP_CONF:
            break;

        default:
            break;
    }
}

/**********************************************************************************************************************
 * @brief Implements static SF_WINC1500_wifiTcpEventsCb
 *********************************************************************************************************************/
static void SF_WINC1500_wifiTcpEventsCb ( SOCKET sock,
                                          uint8_t msg_type,
                                          void * p_msg )
{
    uint8_t i;
    int32_t sid = -1;
    UINT ret;


    SSP_PARAMETER_NOT_USED( sock );
    SSP_PARAMETER_NOT_USED( msg_type );
    SSP_PARAMETER_NOT_USED( p_msg );

    // search for low-level socket
    if (tx_mutex_get(&g_wifi.socket_conv.mutex, CONV_MS_TO_TICK(50)) == TX_SUCCESS)
    {
        for (i=0; i<SF_WINC1500_CFG_PARAM_MAX_SOCKETS; i++)
        {
            if (g_wifi.socket_conv.socket[i] == sock)
            {
                sid = sock;
                break;
            }
        }
        tx_mutex_put(&g_wifi.socket_conv.mutex);
    }

    SSP_ASSERT( i < SF_WINC1500_CFG_PARAM_MAX_SOCKETS );

    if (sid >= 0)
    {
        // i is the socket info structure index
        if (msg_type == SOCKET_MSG_CONNECT)
        {
            // connect Event Handler
            tstrSocketConnectMsg *pstrConnect = (tstrSocketConnectMsg*)p_msg;
            if (pstrConnect->s8Error == 0)
            {
                ret = tx_event_flags_set( &g_wifi.socket_infos[i].events, SF_WIFI_SOCKET_EVENT_CONNECT, TX_OR );
                SSP_ASSERT( ret == TX_SUCCESS );
            }
        }
        else if (msg_type == SOCKET_MSG_SEND)
        {
            if (*(int16_t*)p_msg)
            {
                g_wifi.socket_infos[i].tx_count = *(uint16_t*)p_msg;

                ret = tx_event_flags_set( &g_wifi.socket_infos[i].events, SF_WIFI_SOCKET_EVENT_SEND, TX_OR );
                SSP_ASSERT( ret == TX_SUCCESS );
            }
        }
        else if (msg_type == SOCKET_MSG_RECV)
        {
            tstrSocketRecvMsg *pstrRecvMsg = (tstrSocketRecvMsg*)p_msg;
            if ((pstrRecvMsg->pu8Buffer != NULL) && (pstrRecvMsg->s16BufferSize > 0))
            {
                g_wifi.socket_infos[i].rx_count = (uint16_t)pstrRecvMsg->s16BufferSize;

                ret = tx_event_flags_set( &g_wifi.socket_infos[i].events, SF_WIFI_SOCKET_EVENT_RECV, TX_OR );
                SSP_ASSERT( ret == TX_SUCCESS );
            }
        }
    }
}

/**********************************************************************************************************************
 * @brief Implements static SF_WINC1500_wifiDnsCb
 *********************************************************************************************************************/
static void SF_WINC1500_wifiDnsCb ( uint8_t * p_host_name,
                                    uint32_t server_ip )
{
    SSP_PARAMETER_NOT_USED( p_host_name );
    SSP_PARAMETER_NOT_USED( server_ip );

    if (tx_mutex_get(&g_wifi.host.mutex, CONV_MS_TO_TICK(500)) == TX_SUCCESS)
    {
        //if (g_wifi.host.in_progress == true)
        //{
            // free pending request and signal to resolve procedure regardless actual host name match
            g_wifi.host.in_progress = false;

            if (0 == strcmp((char*)g_wifi.host.p_host_name, (char*)p_host_name))
            {
                *g_wifi.host.p_host_ip = server_ip;
            }
            else
            {
                *g_wifi.host.p_host_ip = 0;
            }

            tx_event_flags_set( &g_wifi.host.events, SF_WIFI_HOST_EVENT_RESOLVED, TX_OR );
        //}
        //ASSERT( tx_mutex_put(&g_wifi.host.mutex) == TX_SUCCESS );
        tx_mutex_put(&g_wifi.host.mutex);
    }

}

ssp_err_t SF_WINC1500_Run (sf_winc1500_ctrl_t * const p_ctrl)
{
    st_sf_wifi_req_t wifi_sock_req;


    SF_WINC1500_eventManage( p_ctrl );

#if ( SF_WINC1500_VER == 1 )
    if ( tx_queue_receive( &st_sf_wifi_queue_req,
                           &wifi_sock_req,
                           TX_NO_WAIT ) == TX_SUCCESS )
    {
        // open request
        if (wifi_sock_req.type == 0)
        {
            SOCKET sid;
            uint8_t i = wifi_sock_req.request.req_sock_open.socket_info_id;

            sid = socket( (uint16_t)wifi_sock_req.request.req_sock_open.p_cfg->protocol_family,
                          (uint16_t)wifi_sock_req.request.req_sock_open.p_cfg->socket_type, 0);
            if ( tx_mutex_get(&g_wifi.socket_infos[i].mutex, TX_WAIT_FOREVER) == TX_SUCCESS )
            {
                if (sid >= 0)
                {
                    g_wifi.socket_infos[i].status = SF_WIFI_SOCKET_STATUS_CREATED;
                    g_wifi.socket_infos[i].socket = sid;
                    g_wifi.socket_conv.socket[i] = sid;
                }
                else
                {
                    g_wifi.socket_infos[i].status = SF_WIFI_SOCKET_STATUS_UNDEF;
                    g_wifi.socket_infos[i].socket = -1;
                    g_wifi.socket_conv.socket[i] = -1;
                }
                SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[i].mutex) == TX_SUCCESS );
            }
            // signal open event
            SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                            SF_WIFI_SOCKET_EVENT_OPEN, TX_OR ) == TX_SUCCESS );
        }
        // close request
        else if (wifi_sock_req.type == 1)
        {
            uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_close.sid;

            close( (SOCKET)wifi_sock_req.request.req_sock_close.socket );
            // signal open event
            SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                            SF_WIFI_SOCKET_EVENT_CLOSE, TX_OR ) == TX_SUCCESS );
        }
        // connect request
        else if (wifi_sock_req.type == 2)
        {
            uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_connect.sid;
            struct sockaddr_in strAddr;

            strAddr.sin_family = (uint16_t)wifi_sock_req.request.req_sock_connect.p_socket_address->family;
            strAddr.sin_port = _htons( wifi_sock_req.request.req_sock_connect.p_socket_address->port );
            strAddr.sin_addr.s_addr = wifi_sock_req.request.req_sock_connect.p_socket_address->address;

            //if (SOCK_ERR_NO_ERROR == connect( (SOCKET)wifi_sock_req.request.req_sock_connect.socket,
            //                                  (struct sockaddr*)&strAddr, sizeof(struct sockaddr_in) ))
            SSP_ASSERT (SOCK_ERR_NO_ERROR == connect( (SOCKET)wifi_sock_req.request.req_sock_connect.socket,
                                              (struct sockaddr*)&strAddr, sizeof(struct sockaddr_in) ))
            {
                // signal connecting event
                SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                SF_WIFI_SOCKET_EVENT_CONNECTING, TX_OR ) == TX_SUCCESS );
            }
        }
        // send request
        else if (wifi_sock_req.type == 3)
        {
            uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_send.sid;

            /*
            if ( SOCK_ERR_NO_ERROR == send((SOCKET)wifi_sock_req.request.req_sock_send.socket,
                                           wifi_sock_req.request.req_sock_send.p_buf,
                                           (uint16_t)wifi_sock_req.request.req_sock_send.length, 0) )
            */
            SSP_ASSERT ( SOCK_ERR_NO_ERROR == send((SOCKET)wifi_sock_req.request.req_sock_send.socket,
                                           wifi_sock_req.request.req_sock_send.p_buf,
                                           (uint16_t)wifi_sock_req.request.req_sock_send.length, 0) )
            {
                // signal connecting event
                SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                SF_WIFI_SOCKET_EVENT_SENDING, TX_OR ) == TX_SUCCESS );
            }
        }
        // receive request
        else if (wifi_sock_req.type == 4)
        {
            uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_receive.sid;

            /*
            if ( SOCK_ERR_NO_ERROR == recv((SOCKET)wifi_sock_req.request.req_sock_receive.socket,
                                           wifi_sock_req.request.req_sock_receive.p_buf,
                                           (uint16_t)wifi_sock_req.request.req_sock_receive.length, 0) )
            */
            SSP_ASSERT ( SOCK_ERR_NO_ERROR == recv((SOCKET)wifi_sock_req.request.req_sock_receive.socket,
                                           wifi_sock_req.request.req_sock_receive.p_buf,
                                           (uint16_t)wifi_sock_req.request.req_sock_receive.length, 0) )
            {
                // signal connecting event
                SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                SF_WIFI_SOCKET_EVENT_RECEIVING, TX_OR ) == TX_SUCCESS );
            }
        }
        else
        {
            // request not managed !P
        }
    }
#endif

    return SSP_SUCCESS;
}

/**********************************************************************************************************************
 * @brief Wi-Fi module internal thread
 *********************************************************************************************************************/
static VOID SF_WINC1500_wifi_internal_thread (ULONG initial_input)
{
    st_sf_wifi_req_t wifi_sock_req;


    SSP_PARAMETER_NOT_USED( initial_input );

    tx_thread_sleep( CONV_MS_TO_TICK(2000) );

    while(1)
    {
        SF_WINC1500_eventManage( g_wifi.p_ctrl );

#if ( SF_WINC1500_VER == 1 )
        if ( tx_queue_receive( &st_sf_wifi_queue_req,
                               &wifi_sock_req,
                               TX_NO_WAIT ) == TX_SUCCESS )
        {
            // open request
            if (wifi_sock_req.type == 0)
            {
                SOCKET sid;
                uint8_t i = wifi_sock_req.request.req_sock_open.socket_info_id;

                sid = socket( (uint16_t)wifi_sock_req.request.req_sock_open.p_cfg->protocol_family,
                              (uint16_t)wifi_sock_req.request.req_sock_open.p_cfg->socket_type, 0);
                if ( tx_mutex_get(&g_wifi.socket_infos[i].mutex, TX_WAIT_FOREVER) == TX_SUCCESS )
                {
                    if (sid >= 0)
                    {
                        g_wifi.socket_infos[i].status = SF_WIFI_SOCKET_STATUS_CREATED;
                        g_wifi.socket_infos[i].socket = sid;
                        g_wifi.socket_conv.socket[i] = sid;
                    }
                    else
                    {
                        g_wifi.socket_infos[i].status = SF_WIFI_SOCKET_STATUS_UNDEF;
                        g_wifi.socket_infos[i].socket = -1;
                        g_wifi.socket_conv.socket[i] = -1;
                    }
                    SSP_ASSERT( tx_mutex_put(&g_wifi.socket_infos[i].mutex) == TX_SUCCESS );
                }
                // signal open event
                SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                SF_WIFI_SOCKET_EVENT_OPEN, TX_OR ) == TX_SUCCESS );
            }
            // close request
            else if (wifi_sock_req.type == 1)
            {
                uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_close.sid;

                close( (SOCKET)wifi_sock_req.request.req_sock_close.socket );
                // signal open event
                SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                SF_WIFI_SOCKET_EVENT_CLOSE, TX_OR ) == TX_SUCCESS );
            }
            // connect request
            else if (wifi_sock_req.type == 2)
            {
                uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_connect.sid;
                struct sockaddr_in strAddr;

                strAddr.sin_family = (uint16_t)wifi_sock_req.request.req_sock_connect.p_socket_address->family;
                strAddr.sin_port = _htons( wifi_sock_req.request.req_sock_connect.p_socket_address->port );
                strAddr.sin_addr.s_addr = wifi_sock_req.request.req_sock_connect.p_socket_address->address;

                //if (SOCK_ERR_NO_ERROR == connect( (SOCKET)wifi_sock_req.request.req_sock_connect.socket,
                //                                  (struct sockaddr*)&strAddr, sizeof(struct sockaddr_in) ))
                SSP_ASSERT (SOCK_ERR_NO_ERROR == connect( (SOCKET)wifi_sock_req.request.req_sock_connect.socket,
                                                  (struct sockaddr*)&strAddr, sizeof(struct sockaddr_in) ))
                {
                    // signal connecting event
                    SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                    SF_WIFI_SOCKET_EVENT_CONNECTING, TX_OR ) == TX_SUCCESS );
                }
            }
            // send request
            else if (wifi_sock_req.type == 3)
            {
                uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_send.sid;

                /*
                if ( SOCK_ERR_NO_ERROR == send((SOCKET)wifi_sock_req.request.req_sock_send.socket,
                                               wifi_sock_req.request.req_sock_send.p_buf,
                                               (uint16_t)wifi_sock_req.request.req_sock_send.length, 0) )
                */
                SSP_ASSERT ( SOCK_ERR_NO_ERROR == send((SOCKET)wifi_sock_req.request.req_sock_send.socket,
                                               wifi_sock_req.request.req_sock_send.p_buf,
                                               (uint16_t)wifi_sock_req.request.req_sock_send.length, 0) )
                {
                    // signal connecting event
                    SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                    SF_WIFI_SOCKET_EVENT_SENDING, TX_OR ) == TX_SUCCESS );
                }
            }
            // receive request
            else if (wifi_sock_req.type == 4)
            {
                uint8_t i = (uint8_t)wifi_sock_req.request.req_sock_receive.sid;

                /*
                if ( SOCK_ERR_NO_ERROR == recv((SOCKET)wifi_sock_req.request.req_sock_receive.socket,
                                               wifi_sock_req.request.req_sock_receive.p_buf,
                                               (uint16_t)wifi_sock_req.request.req_sock_receive.length, 0) )
                */
                SSP_ASSERT ( SOCK_ERR_NO_ERROR == recv((SOCKET)wifi_sock_req.request.req_sock_receive.socket,
                                               wifi_sock_req.request.req_sock_receive.p_buf,
                                               (uint16_t)wifi_sock_req.request.req_sock_receive.length, 0) )
                {
                    // signal connecting event
                    SSP_ASSERT( tx_event_flags_set( &g_wifi.socket_infos[i].events,
                                                    SF_WIFI_SOCKET_EVENT_RECEIVING, TX_OR ) == TX_SUCCESS );
                }
            }
            else
            {
                // request not managed !P
            }
        }

        tx_thread_sleep( CONV_MS_TO_TICK(100) );
#endif
    }
}
/**********************************************************************************************************************
 * @}
 *********************************************************************************************************************/
