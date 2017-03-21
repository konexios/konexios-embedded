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

#ifndef SF_WINC1500_PRIVATE_API_H
#define SF_WINC1500_PRIVATE_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "sf_winc1500_api.h"

/***********************************************************************************************************************
 * Private Instance API Functions. DO NOT USE! Use functions through Interface API structure instead.
 **********************************************************************************************************************/
ssp_err_t SF_WINC1500_Open(sf_winc1500_ctrl_t * const p_ctrl, sf_winc1500_cfg_t const * const p_cfg);
ssp_err_t SF_WINC1500_InfoGet(sf_winc1500_ctrl_t * const p_ctrl, sf_wifi_device_info_t * const p_device_info);
ssp_err_t SF_WINC1500_Reset(sf_winc1500_ctrl_t * const p_ctrl);
ssp_err_t SF_WINC1500_EventSignal(sf_winc1500_ctrl_t * const p_ctrl);
ssp_err_t SF_WINC1500_Connect(sf_winc1500_ctrl_t * const p_ctrl);
ssp_err_t SF_WINC1500_Close(sf_winc1500_ctrl_t * const p_ctrl);
ssp_err_t SF_WINC1500_Run(sf_winc1500_ctrl_t * const p_ctrl);
ssp_err_t SF_WINC1500_VersionGet(ssp_version_t * const p_version);
ssp_err_t SF_WINC1500_HostGetByName(sf_winc1500_ctrl_t * const p_ctrl, uint8_t * p_host_name, uint32_t * p_host_ip);
ssp_err_t SF_WINC1500_SocketOpen(sf_winc1500_ctrl_t * const p_ctrl, int32_t * p_sid, sf_wifi_socket_cfg_t * const p_cfg);
ssp_err_t SF_WINC1500_SocketClose(sf_winc1500_ctrl_t * const p_ctrl, int32_t sid);
ssp_err_t SF_WINC1500_SocketConnect(sf_winc1500_ctrl_t * const p_ctrl, int32_t sid, sf_wifi_socket_address_t * p_socket_address);
ssp_err_t SF_WINC1500_SocketSend(sf_winc1500_ctrl_t * const p_ctrl, int32_t sid, uint8_t * p_buf, uint32_t length, uint32_t * p_bytes_sent);
ssp_err_t SF_WINC1500_SocketReceive(sf_winc1500_ctrl_t * const p_ctrl, int32_t sid, uint8_t * p_buf, uint32_t length, uint32_t * p_bytes_received);


#endif /* SF_WINC1500_PRIVATE_API_H */
