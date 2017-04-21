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
 * File Name    : sf_winc1500_api.h
 * Description  : WINC1500 Framework Layer Driver API.
 **********************************************************************************************************************/

#ifndef SF_WINC1500_API_H
#define SF_WINC1500_API_H

/*******************************************************************************************************************//**
 * @ingroup SF_Interface_Library
 * @defgroup SF_WINC1500_API Wi-Fi module WINC1500 Driver Interface
 * @brief Wi-Fi module WINC1500, Framework Layer, Driver Interface.
 *
 * @section SF_WINC1500_API_SUMMARY Summary
 * This module is Framework layer driver for the Wi-Fi module WINC1500.
 * This Interface is implemented by @ref SF_WINC1500.
 *
 * Interfaces used: @see SPI_API
 *
 * Related SSP architecture topics:
 *  - What is an SSP Interface? @ref ssp-interfaces
 *  - What is an SSP Layer? @ref ssp-predefined-layers
 *  - How to use SSP Interfaces and Modules? @ref using-ssp-modules
 *
 * WINC1500 Driver Interface user manual:
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "sf_spi_api.h"
#include "r_external_irq_api.h"

#include "driver/include/m2m_wifi.h"
#include "socket/include/socket.h"

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SF_WINC1500_API_VERSION_MAJOR   (0)
#define SF_WINC1500_API_VERSION_MINOR   (1)

/** Wi-Fi socket protocol family AF_INET */
#define SF_WINC1500_SOCKET_PROTOCOL_FAMILY_AF_INET              2

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/** Wi-Fi packet types */
typedef enum e_sf_winc1500_event
{
    SF_WINC1500_EVENT_CONNECT           = (1 << 0),             ///< Connected with AP
    SF_WINC1500_EVENT_DISCONNECT        = (1 << 1),             ///< Disconnected from AP
    SF_WINC1500_EVENT_UNDEF             = (1 << 31)             ///< Undefined event
} sf_winc1500_event_t;

/** Wi-Fi interface mode of operation: AP or client. */
typedef enum e_sf_wifi_interface_mode
{
    SF_WIFI_INTERFACE_MODE_AP           = 1,                    ///< Access Point mode
    SF_WIFI_INTERFACE_MODE_CLIENT,                              ///< Client mode
} sf_wifi_interface_mode_t;

/** Link Layer security type */
typedef enum e_sf_wifi_security_type_t
{
    SF_WIFI_SECURITY_TYPE_OPEN          = 1,                    ///< Open, no encryption used
    SF_WIFI_SECURITY_TYPE_WEP40,                                ///< 40-bit WEP
    SF_WIFI_SECURITY_TYPE_WEP128,                               ///< 128-bit WEP
    SF_WIFI_SECURITY_TYPE_WPA,                                  ///< Wi-Fi Protected Access
    SF_WIFI_SECURITY_TYPE_WPA2                                  ///< Wi-Fi Protected Access v2
} sf_wifi_security_type_t;

/// SOCKETS ///
typedef enum e_sf_wifi_socket_type
{
    SF_WIFI_SOCKET_TYPE_STREAM          = 1,
    SF_WIFI_SOCKET_TYPE_DGRAM           = 2
} sf_wifi_socket_type_t;
/// SOCKETS ///

/** Device info structure */
typedef struct sf_wifi_device_info
{
    uint8_t                             name[32];
    uint8_t                             chipset[32];
    uint8_t                             mac[6];
} sf_wifi_device_info_t;

/** Data structure defining parameters for provisioning the Wi-Fi network interface. */
typedef struct sf_wifi_provisioning
{
    sf_wifi_interface_mode_t            mode;
    uint8_t                             ssid[33];
    sf_wifi_security_type_t             sec;
    uint8_t                             key[33];
} sf_wifi_provisioning_t;

/** Wi-Fi status callback parameter definition */
typedef struct st_sf_winc1500_status_callback_arg
{
    sf_winc1500_event_t                 event;                  ///< Event code
    uint8_t *                           data;                   ///< General purpose data storage
} sf_winc1500_status_callback_args_t;

/** Wi-Fi status callback type definition */
typedef void (* p_status_cb_t)(sf_winc1500_status_callback_args_t * p_args);

/** Channel control block. DO NOT INITIALIZE.  Initialization occurs when SF_WINC1500_Open is called */
typedef struct st_sf_winc1500_ctrl
{
    uint32_t                            open;                   ///< Used by driver to check if control block is valid
    const sf_spi_instance_t *           device;                 ///< SPI Framework Device
    const external_irq_instance_t *     irq;                    ///< IRQ external interrupt

    sf_wifi_device_info_t *             p_device_info;          ///< Device information
    sf_wifi_provisioning_t *            p_provisioning;         ///< Provisioning data
    p_status_cb_t                       p_status_cb;            ///< Pointer to status callback function

    TX_TIMER                            timer_manager;          ///< Low-level driver manager timer
    tstrWifiInitParam                   param;
    in_addr                             ip;
    SOCKET                              clientSocketHdl;
    bool                                connected;
} sf_winc1500_ctrl_t;

/** Configuration for RTOS integrated WINC1500 Driver. */
typedef struct st_sf_winc1500_cfg
{
    const sf_spi_instance_t *           device;                 ///< SPI Framework Device
    const external_irq_instance_t *     irq;                    ///< IRQ external interrupt

    sf_wifi_device_info_t *             p_device_info;          ///< Wi-Fi device information
    sf_wifi_provisioning_t *            p_provisioning;         ///< Provisioning data
    p_status_cb_t                       p_status_cb;            ///< Pointer to callback function
} sf_winc1500_cfg_t;

/// SOCKETS ///
typedef struct sf_wifi_socket_cfg
{
    int32_t                             protocol_family;
    sf_wifi_socket_type_t               socket_type;
    int32_t                             protocol;
} sf_wifi_socket_cfg_t;

typedef struct sf_wifi_socket_address
{
    int16_t                             port;                   ///< Port number on which socket is bind
    int16_t                             family;                 ///< Socket address family
    uint32_t                            address;                ///< IP address on which socket is bind
} sf_wifi_socket_address_t;
/// SOCKETS ///

/** WINC1500 Driver API structure. */
typedef struct st_sf_winc1500_api
{
    /** Open SPI Framework Driver and initialize internal values.
     * @par Implemented as
     *  - SF_WINC1500_Open()
     *
     * @param[in,out] p_ctrl   Pointer to a structure allocated by user. This control structure is initialized in
     *                         this function.
     * @param[in]     p_cfg    Pointer to configuration structure. All elements of the structure must be set by user.
     */
    ssp_err_t (* open)(sf_winc1500_ctrl_t           * const p_ctrl,
                       sf_winc1500_cfg_t const      * const p_cfg);

    /** Get device information.
     * @par Implemented as
     *  - SF_WINC1500_InfoGet()
     *
     * @param[in]   p_ctrl          Handle set in winc1500_api_t::open.
     * @param[out]  p_device_info   Pointer to device information structure.
     */
    ssp_err_t (* infoGet)(sf_winc1500_ctrl_t        * const p_ctrl,
                          sf_wifi_device_info_t     * const p_device_info);

    /** Reset WINC1500 device.
     * @par Implemented as
     *  - SF_WINC1500_Reset()
     *
     * @param[in]   p_ctrl      Handle set in winc1500_api_t::open.
     */
    ssp_err_t (* reset)(sf_winc1500_ctrl_t          * const p_ctrl);
    
    /** @brief Signal event from WINC1500 device irq
     * @par Implemented as
     *  - SF_WINC1500_EventSignal()
     *
     * @param[in]   p_ctrl       Handle set in winc1500_api_t::open.
     */
    ssp_err_t (* eventSignal)(sf_winc1500_ctrl_t    * const p_ctrl);

    /** @brief Connect WINC1500 device to LAN
     * @par Implemented as
     *  - SF_WINC1500_Connect()
     *
     * @param[in]   p_ctrl       Handle set in winc1500_api_t::open.
     */
    ssp_err_t (* connect)(sf_winc1500_ctrl_t        * const p_ctrl);

    /** @brief Close SPI channel at Framework layer.
     * @par Implemented as
     *  - SF_WINC1500_Close()
     *
     * @param[in]   p_ctrl       Handle set in winc1500_api_t::open.
     */
    ssp_err_t (* close)(sf_winc1500_ctrl_t          * const p_ctrl);

    /** @brief Run WINC1500 event manager.
     * @par Implemented as
     *  - SF_WINC1500_Run()
     *
     * @param[in]   p_ctrl       Handle set in winc1500_api_t::open.
     */
    ssp_err_t (* run)(sf_winc1500_ctrl_t          * const p_ctrl);

    /** @brief Gets version and stores it in provided pointer p_version.
     * @par Implemented as
     *  - SF_WINC1500_VersionGet()
     *
     * @param[out]  p_version  Code and API version used stored here.
     */
    ssp_err_t (* versionGet)(ssp_version_t          * const p_version);
    
    /** @brief Read Firmware version and Serial Numbers, stores them in provided pointers.
     * @par Implemented as
     *  - SF_WINC1500_ReadFw()
     *
     * @param[out]  p_version  Code and API version used stored here.
     */
    ssp_err_t (* readFirmwareVer)(sf_winc1500_ctrl_t * const p_ctrl,
                                  uint8_t* fw_ver,
                                  uint32_t* sn_a,
                                  uint32_t* sn_b);

    /** @brief Get host ip address by name */
    ssp_err_t (* hostGetByName)(sf_winc1500_ctrl_t * const p_ctrl,
                                uint8_t * p_host_name,
                                uint32_t * p_host_ip );

    /** @brief Open a socket */
    ssp_err_t (* socketOpen)(sf_winc1500_ctrl_t * const p_ctrl,
                             int32_t* sid,
                             sf_wifi_socket_cfg_t * const p_cfg);

    /** @brief Close a socket */
    ssp_err_t (* socketClose)(sf_winc1500_ctrl_t * const p_ctrl,
                              int32_t sid);

    /** @brief Connect to a remote server */
    ssp_err_t (* socketConnect)(sf_winc1500_ctrl_t * const p_ctrl,
                                int32_t sid,
                                sf_wifi_socket_address_t * const p_socket_address);

    /** @brief Send data on a socket */
    ssp_err_t (* socketSend)(sf_winc1500_ctrl_t * const p_ctrl,
                             int32_t sid,
                             uint8_t * p_buf,
                             uint32_t length,
                             uint32_t * p_bytes_sent);

    /** @brief Receive data from a socket */
    ssp_err_t (* socketReceive)(sf_winc1500_ctrl_t * const p_ctrl,
                             int32_t sid,
                             uint8_t * p_buf,
                             uint32_t length,
                             uint32_t * p_bytes_received);
} sf_winc1500_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_sf_winc1500_instance
{
    sf_winc1500_ctrl_t      * p_ctrl;    ///< Pointer to the control structure for this instance
    sf_winc1500_cfg_t const * p_cfg;     ///< Pointer to the configuration structure for this instance
    sf_winc1500_api_t const * p_api;     ///< Pointer to the API structure for this instance
} sf_winc1500_instance_t;


/*******************************************************************************************************************//**
 * @} (end defgroup SF_WINC1500_API)
 **********************************************************************************************************************/
#endif /* SF_WINC1500_API_H */
