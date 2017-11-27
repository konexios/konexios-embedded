/*
  * Copyright (c) 2015 Qualcomm Atheros, Inc..
  * All Rights Reserved.
  * Qualcomm Atheros Confidential and Proprietary.
  */

#include "qcom_common.h"
#include "qcom/socket_api.h"
#include "qcom/select_api.h"
#include "qcom/qcom_internal.h"

/*****************************************************************************************************************************/
/*                                                                                                                           */
/*       OTA_FTP                                                                                                             */
/*                                                                                                                           */
/*****************************************************************************************************************************/
#define OTA_FTP_DATA_PORT                        1068
#define OTA_FTP_BUF_SIZE                         1024
#define OTA_FTP_INCOMING_CONNECT_TIMEOUT         20
#define SSL_OTA_INBUF_SIZE           20000
#define SSL_OUTBUF_SIZE              3500

/*
 * OTA FTP Status codes
 */
typedef enum {
        QCOM_OTA_ERR_FTP_CONNECT_FAIL = 2000,
        QCOM_OTA_ERR_FTP_BIND_FAIL,
        QCOM_OTA_ERR_FTP_PEER_CLOSED,
        QCOM_OTA_ERR_FTP_SEND_COMMAND,
        QCOM_OTA_ERR_FTP_LISTEN_DATA_PORT,
        QCOM_OTA_ERR_FTP_DATA_CONNECTION_TIMEOUT,
        QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT,
        QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE,
        QCOM_OTA_ERR_FTP_SYST,
        QCOM_OTA_ERR_FTP_LOGIN_INCORRECT,
        QCOM_OTA_ERR_FTP_SET_TYPE,
        QCOM_OTA_ERR_FTP_SET_PORT,
        QCOM_OTA_ERR_FTP_FILE_NOT_FOUND,
       
} QCOM_OTA_FTP_STATUS_CODE_t;


//#define DEBUG_OTA_FTP_PRINTF 
#if defined(DEBUG_OTA_FTP_PRINTF)
#define OTA_FTP_D_PRINTF(args...) printf(args)
#else
#define OTA_FTP_D_PRINTF(args...)
#endif

           
typedef struct {
    A_UINT8  device_id;
    A_UINT32 data_sock;                /* data socket.*/
    A_UINT32 peer_sock; 
    A_UINT32 control_sock;             /* control socket.*/    
    A_UINT32 remote_ip_address;
    A_UINT32 local_ip_address;  
    A_UINT16 cmd_port;
    A_UINT16 data_port;
    A_UINT32 flags;
    A_UINT32 partition_index;
     A_UINT32 total_write;
} QCOM_OTA_FTP_SESSION_INFO;

A_INT32 qcom_ota_ftp_upgrade(A_UINT8 device_id, A_UINT32 ip_addr, A_UINT16 port, char *user_name, char *password, char *filename, A_UINT32 flags, A_UINT32 partition_index, A_UINT32 *size);
A_INT32 ota_ftp_init(A_UINT8 device_id);
A_INT32 ota_ftp_finit();
A_INT32 ota_ftp_open_control_sock(A_UINT32 ip_address, A_UINT16 port);
A_INT32 ota_ftp_close_control_sock();
A_INT32 ota_ftp_open_data_sock(A_UINT32 ip_address, A_UINT16 port);
A_INT32 ota_ftp_close_data_sock();
A_INT32 ota_ftp_send_cmd(char *cmd);
A_INT32 ota_ftp_recv_cmd(struct timeval tmo, int *resp_code);
A_INT32 ota_ftp_send_cmd_resp(char *cmd, int *resp_code);
A_INT32 ota_ftp_program_ota(A_UINT32 *size);

/* OTA FTP Global */
QCOM_OTA_FTP_SESSION_INFO *ota_ftp_sess = NULL;
static A_UINT16 ota_data_port = OTA_FTP_DATA_PORT;

A_UINT16 ota_get_data_port()
{
    ota_data_port++;
    if( ota_data_port >= 0x7fff )
		ota_data_port = OTA_FTP_DATA_PORT;	
    	
    return ota_data_port;
}

A_INT32 ota_ftp_init(A_UINT8 device_id)
{
	A_UINT32 addr = 0, mask = 0, gw = 0;

	OTA_FTP_D_PRINTF("FTP Init....\n");

	ota_ftp_sess = qcom_mem_alloc(sizeof(QCOM_OTA_FTP_SESSION_INFO));
	if (!ota_ftp_sess) {
		return A_ERROR;
	}
	memset(ota_ftp_sess, '\0', sizeof(QCOM_OTA_FTP_SESSION_INFO));

	ota_ftp_sess->device_id = device_id;
	ota_ftp_sess->control_sock = A_ERROR;
	ota_ftp_sess->peer_sock = A_ERROR;
	ota_ftp_sess->data_sock = A_ERROR;

	//get local ip address
	qcom_ipconfig(device_id, IP_CONFIG_QUERY, &addr, &mask, &gw);

	ota_ftp_sess->local_ip_address = addr;
	ota_ftp_sess->data_port = ota_get_data_port();

  if((int)(ota_ftp_sess->control_sock = qcom_socket(AF_INET, SOCK_STREAM, 0)) == A_ERROR)
	{
		ota_ftp_sess->control_sock = A_ERROR;
		printf("ERROR: Unable to create ftp control socket\n");
		goto error_1;
	}

  if((int)(ota_ftp_sess->data_sock = qcom_socket(AF_INET, SOCK_STREAM, 0)) == A_ERROR)
	{
		ota_ftp_sess->data_sock = A_ERROR;
		printf("ERROR: Unable to create ftp data socket\n");
		goto error_1;
	}

	return QCOM_OTA_OK;
error_1:
	ota_ftp_finit();
	return A_ERROR; 
}
A_INT32 ota_ftp_finit()
{
    ota_ftp_close_data_sock();
    ota_ftp_close_control_sock();

    /* Clean up and free all resources */
    if(ota_ftp_sess != NULL) {
        qcom_mem_free(ota_ftp_sess);
		ota_ftp_sess = NULL;
    }

    return QCOM_OTA_OK;  
}

A_INT32 ota_ftp_close_data_sock()
{
    if( (int)ota_ftp_sess->data_sock != A_ERROR ) {
        qcom_socket_close(ota_ftp_sess->data_sock);
        ota_ftp_sess->data_sock = A_ERROR;
    }
    return QCOM_OTA_OK;
}

A_INT32 ota_ftp_open_data_sock(A_UINT32 ip_addr, A_UINT16 port)
{
    struct sockaddr_in local_addr;
    A_INT32 rtn = QCOM_OTA_OK;

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_addr.s_addr =  htonl(ip_addr);
    local_addr.sin_port =  htons(port);
    local_addr.sin_family = AF_INET;

    OTA_FTP_D_PRINTF("connect ftp data port ip:%xh port:%d\n", ip_addr, port);
    
    /* bind to local port.*/
    if(qcom_bind( ota_ftp_sess->data_sock, (struct sockaddr*)(&local_addr), sizeof(local_addr)) == A_ERROR)
    {
		//printf("socket bind failed.\n");
		rtn = QCOM_OTA_ERR_FTP_BIND_FAIL;
    }
    
    return rtn;     
}

A_INT32 ota_ftp_close_control_sock()
{
    OTA_FTP_D_PRINTF("close ftp control connection\n");
  
    if( (int)ota_ftp_sess->control_sock != A_ERROR ) {
        qcom_socket_close(ota_ftp_sess->control_sock);
        ota_ftp_sess->control_sock = A_ERROR;
    }
    return QCOM_OTA_OK;
}

A_INT32 ota_ftp_open_control_sock(A_UINT32 ip_addr, A_UINT16 port)
{
    struct sockaddr_in foreign_addr;
    A_INT32 rtn = QCOM_OTA_OK;

    OTA_FTP_D_PRINTF("FTP Connecting ip:%xh port:%d\n", ip_addr, port);
    
    ota_ftp_sess->remote_ip_address = ip_addr;
    ota_ftp_sess->cmd_port = port;

    memset(&foreign_addr, 0, sizeof(foreign_addr));
    foreign_addr.sin_addr.s_addr = htonl(ip_addr);
    foreign_addr.sin_port = htons(port);
    foreign_addr.sin_family = AF_INET;
			
    /* Connect to the server.*/
    if(qcom_connect( ota_ftp_sess->control_sock, (struct sockaddr*)(&foreign_addr), sizeof(foreign_addr)) == A_ERROR)
    {
		printf("Connection failed.\n");
		rtn = QCOM_OTA_ERR_FTP_CONNECT_FAIL;
    }
    
    return rtn;    
}

/* return: send length */
int ota_ftp_send_cmd(char *cmd)
{
    OTA_FTP_D_PRINTF("send: %s", cmd);
    if ( (int)ota_ftp_sess->control_sock != A_ERROR)
        return (qcom_send(ota_ftp_sess->control_sock, cmd, strlen(cmd),0));  
    return A_ERROR;
}

A_INT32 ota_ftp_recv_cmd(struct timeval to, int *resp_code)
{
#define  FTP_CMD_BUF_LEN 256
    struct timeval tmo;
    q_fd_set sockSet,master;	
    A_INT32 conn_sock, received = 0;
    A_INT32 rtn = QCOM_OTA_OK;
    char buffer[FTP_CMD_BUF_LEN];
	
    *resp_code = 0;

    FD_ZERO(&master);
    FD_SET(ota_ftp_sess->control_sock, &master);
    if (FD_ISSET(ota_ftp_sess->control_sock, &master) == 0)
        return QCOM_OTA_ERR_FTP_PEER_CLOSED;

    tmo.tv_sec = to.tv_sec;
    tmo.tv_usec =to.tv_usec;

    while(1) /* Receiving data.*/
    {
        sockSet = master;        /* Wait for Input */
        conn_sock = qcom_select(ota_ftp_sess->control_sock,&sockSet, NULL, NULL, &tmo);
        if(conn_sock !=0)
        {
            if(FD_ISSET( ota_ftp_sess->control_sock,&sockSet) )
            {
                /*Packet is available, receive it*/
                received = qcom_recv( ota_ftp_sess->control_sock, (char*)(&buffer[0]), FTP_CMD_BUF_LEN, 0);
                if( received <= 0 )
                {
                    /* Test ended, peer closed connection*/
                    rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                }
                break;
            } else {
	            /* Test ended, peer closed connection*/
	            rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
	            break;                 
            }
        } else {
             /* Test ended, peer closed connection*/
            rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
            break;
	     		
        }
    }

    /*Valid packet received*/
    if (rtn == QCOM_OTA_OK)
    {
        buffer[received] = '\0';
        OTA_FTP_D_PRINTF("recv: %s", buffer);
    
        buffer[3] = '\0';
        *resp_code = atoi(buffer);
    }
    
    return rtn; 
}

A_INT32 ota_ftp_send_cmd_resp(char *cmd, int *resp_code)
{
    struct timeval tmo;
    tmo.tv_sec = 5;
    tmo.tv_usec = 0;   	
    *resp_code = 0;
    if( ota_ftp_send_cmd(cmd) <= 0 )
    	return QCOM_OTA_ERR_FTP_SEND_COMMAND;
    
    return(ota_ftp_recv_cmd(tmo, resp_code));
}


A_INT32 ota_ftp_program_ota(A_UINT32 *size)
{
    struct sockaddr_in foreign_addr;
    struct timeval tmo, to;
    q_fd_set sockSet,master;	
    A_INT32 conn_sock, received = 0;
    A_INT32 addr_len;
    A_UINT32 count, is_first, offset, img_offset, packet_received, len;
    A_UINT32 ret_size;
    A_INT32 rtn = QCOM_OTA_OK;
    int resp;
    A_UINT32 cmd_recv_complete = 0;
    A_INT32 cmd_rtn = QCOM_OTA_OK;
    char *buffer;
  
    *size = 0;
    /*Allocate buffer*/
    if((buffer = qcom_mem_alloc(OTA_FTP_BUF_SIZE)) == NULL)
    {
        OTA_FTP_D_PRINTF("Out of memory error\n");
        rtn = A_ERROR;
        goto error_1;
    }    

    FD_ZERO(&master);
    FD_SET(ota_ftp_sess->data_sock, &master);

    /* Listen. */
    OTA_FTP_D_PRINTF("Listening data socket\n");   
    if(qcom_listen( ota_ftp_sess->data_sock, 10) < 0)  
    {
       //chnage to non-blocking
       //printf("ERROR: Socket listen error.\r\n");
       // rtn = QCOM_OTA_ERR_FTP_LISTEN_DATA_PORT; 
       // goto error_1;
    }

    tmo.tv_sec = 10;
    tmo.tv_usec = 0;  
	/* block for 10sec or until a packet is received */
	sockSet = master;        /* Wait for Input */
	conn_sock = qcom_select(ota_ftp_sess->data_sock+1,&sockSet, NULL, NULL, &tmo);

    if( conn_sock <= 0 ) {	
        OTA_FTP_D_PRINTF("FTP incoming data connection timeout\n");   
        rtn = QCOM_OTA_ERR_FTP_DATA_CONNECTION_TIMEOUT;
        goto error_1;
    }

    /*Accept incoming connection*/
    OTA_FTP_D_PRINTF("Accept incoming data connection\n");   
    addr_len = sizeof(foreign_addr);
    if( (int)(ota_ftp_sess->peer_sock = qcom_accept( ota_ftp_sess->data_sock, (struct sockaddr*)&foreign_addr, (int*)&addr_len)) <= 0 )
    {
        rtn = QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT;
        ota_ftp_sess->peer_sock = A_ERROR;
        goto error_2;
    }
    
    /* -----------------------------------------------------*/
    /* process FTP data                                   */
    /* -----------------------------------------------------*/
    printf("FTP Transfer File\n");	
	
    count = 0;    
    is_first = 1;
    img_offset = 0;
    
    ota_ftp_sess->total_write = 0;

    tmo.tv_sec = 8;
    tmo.tv_usec = 0;  

    FD_ZERO(&master);
    FD_SET(ota_ftp_sess->peer_sock, &master);

	if(FD_ISSET(ota_ftp_sess->peer_sock, &master)== 0)
	{
	    rtn = QCOM_OTA_ERR_FTP_ACCEPT_DATA_CONNECT;
	    goto error_2;
	}

    FD_SET(ota_ftp_sess->control_sock, &master);

	if(FD_ISSET(ota_ftp_sess->control_sock, &master)== 0)
	{
	    rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
	    goto error_2;
	}

    do{
        /*-----------------------------------------------------------*/
        /* Receiving data from FTP server.*/
        /* Copy the master list everytime before calling select() because select
         * modifies sockSet */
        sockSet = master;
        packet_received = 0;
        conn_sock = qcom_select(ota_ftp_sess->peer_sock+1, &sockSet, NULL, NULL, &tmo);
        if(conn_sock != 0)
        {
            if (FD_ISSET(ota_ftp_sess->peer_sock, &sockSet)) {
           
                /*Packet is available, receive it*/
                packet_received = 1;
                received = qcom_recv( ota_ftp_sess->peer_sock, buffer, OTA_FTP_BUF_SIZE, 0);
                //OTA_FTP_D_PRINTF("recv %dB\n", received);   

                if( received <=  0 )
                {
                    /* peer closed connection*/
                    break;
                }

                /* handle data */
                offset = 0;
                len = received;
        
                if(is_first == 1) 
                {
                    //need parse image hdr
                    is_first = 0;   
                    if( (rtn = qcom_ota_parse_image_hdr((A_UINT8 *)buffer,&offset)) != QCOM_OTA_OK ) 
                    {
                        OTA_FTP_D_PRINTF("OTA Parse Image Hdr Error\n");
                        goto error_2;  
                    }
                    OTA_FTP_D_PRINTF("OTA Parse Image Hdr: %d\n", offset);
                
                    len -= offset;
					received -= offset;
                }

				if ((rtn = qcom_ota_partition_erase_sectors(ota_ftp_sess->total_write + received)) != QCOM_OTA_OK )
				{
					goto error_2;
				}
				
                while (len > 0)
                {
                	A_UINT32 write_size;
	                if (len > MAX_OTA_AREA_READ_SIZE) {
	                        write_size = MAX_OTA_AREA_READ_SIZE;
	                } else {
	                        write_size = len;
	                }
					
					if((rtn = qcom_ota_partition_write_data(img_offset, (A_UINT8 *)&buffer[offset],write_size, &ret_size)) != QCOM_OTA_OK )
					{
						goto error_2;                    
					}

					printf(".");

					offset += ret_size;
					img_offset += ret_size;
					len -= ret_size;
                }
                ota_ftp_sess->total_write += received;
            }
            if (FD_ISSET(ota_ftp_sess->control_sock, &sockSet)) {
                packet_received = 1;
                cmd_recv_complete = 1;
                /* peer closed connection*/
                to.tv_sec = 1;
                to.tv_usec = 0;    
                ota_ftp_recv_cmd(to, &resp);

                if( resp != 0 ) {
                    //we have received the whole file
                    if( resp == 226 )     //recv 226 Transfer complete
                    {
                        cmd_rtn = QCOM_OTA_OK;
                        OTA_FTP_D_PRINTF("Transfer Complete received\n");
                    } else {
                        //get error
                        OTA_FTP_D_PRINTF("Transfer not Complete:%d\n", resp);
                        cmd_rtn = QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE;
                    }
                } else {
                   cmd_rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                }

            }
            /*none of data and ctrl socket receive packet */
            if(packet_received == 0) {
            
                /* peer closed connection*/
                rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
                goto error_2;
            }
        } else{
        /*select timeout*/
            break;
        }
    }while(1); /* while */

    if(cmd_recv_complete){
        rtn = cmd_rtn;
    }
    else{
        to.tv_sec = 0;
        to.tv_usec = 510000;    

        ota_ftp_recv_cmd(to, &resp);
        if( resp != 0 ) {
            //we have received the whole file
            if( resp == 226 )     //recv 226 Transfer complete
            {
                rtn = QCOM_OTA_OK;
                OTA_FTP_D_PRINTF("Transfer Complete received\n");
                
            } else {
                //get error
                OTA_FTP_D_PRINTF("Transfer not Complete:%d\n", resp);
                rtn = QCOM_OTA_ERR_FTP_FILE_NOT_COMPLETE;
            }
        }
        else
        {
            // error
        	rtn = QCOM_OTA_ERR_FTP_PEER_CLOSED;
        } 
    }
error_2:
    printf("\nTotal write: %d, rtn:%d\n", ota_ftp_sess->total_write, rtn);  
    if( (int)ota_ftp_sess->peer_sock != A_ERROR ) {
        qcom_socket_close(ota_ftp_sess->peer_sock);
        ota_ftp_sess->peer_sock = A_ERROR;
    }  
    ota_ftp_close_data_sock();   
    *size = ota_ftp_sess->total_write;
error_1:
    if(buffer)
    	qcom_mem_free(buffer);
    return rtn; 
}

A_INT32 qcom_ota_ftp_upgrade(A_UINT8 device_id, A_UINT32 ip_addr, A_UINT16 port, char *user_name, char *password, char *filename, A_UINT32 flags, A_UINT32 partition_index, A_UINT32 *size)
{
    struct timeval tmo;
    A_INT32 rtn = A_OK;
    int resp;
    A_UINT32 partition_size, good_image = 0;
    char buf[256];
    
    OTA_FTP_D_PRINTF("Enter qcom_ota_ftp_upgrade\n");
    OTA_FTP_D_PRINTF("ota_ftp_update: ip:%xH, port:%d\n", ip_addr, port);
    OTA_FTP_D_PRINTF("                User:%s, Password:%s\n", user_name, password);
    OTA_FTP_D_PRINTF("                file:%s\n", filename);
    OTA_FTP_D_PRINTF("                flags:%d, partition_index:%d \n", flags, partition_index);   

    *size = 0;
    tmo.tv_sec = 5;
    tmo.tv_usec = 0;
	
    if( (flags & QCOM_OTA_TARGET_FIRMWARE_UPGRADE) == 0)
    {
		A_PRINTF("OTA FTP supports Target Firmware Upgrade only, check flags setting\n");
		return QCOM_OTA_ERR_UNSUPPORT_PROTOCOL; 
    }
    if( (rtn = qcom_ota_session_start(flags, partition_index)) != QCOM_OTA_OK )
    {
		OTA_FTP_D_PRINTF("OTA Session Start Fail\n");
		return rtn;
    }
	#if 0
    if( (rtn = qcom_ota_partition_erase()) != QCOM_OTA_OK )
    {
		OTA_FTP_D_PRINTF("OTA Partition Erase Fail\n");
		return rtn;
    }
	#endif

    /* check partition size */
    partition_size = qcom_ota_partition_get_size();
    OTA_FTP_D_PRINTF("OTA Partition Get Size: %d\n",partition_size);
    if( partition_size == 0) {
        rtn = A_ERROR;
        return rtn;
    }	

    if( ota_ftp_init(device_id) != QCOM_OTA_OK )
    {
        OTA_FTP_D_PRINTF("Error : ota_ftp_init error\n");
		goto error_1;
    }

    printf("\nConnect to FTP Server...\n");
     //open control sock
    if( (rtn = ota_ftp_open_control_sock(ip_addr, port)) != QCOM_OTA_OK )
    	goto error_1;
    
    //recev welcome
    if( (rtn = ota_ftp_recv_cmd(tmo, &resp)) != QCOM_OTA_OK )
    {
    	goto error_1;
    }
    if( resp != 220 )
    {
		printf("FTP Welcome error: %d\n",resp);
		goto error_1;
    }
	
	{
		// To receive all welcome message
	    tmo.tv_sec = 0;
	    tmo.tv_usec = 200000; 
	    while (ota_ftp_recv_cmd(tmo, &resp) == QCOM_OTA_OK);
    }
	
    //send user
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "USER %s\r\n", user_name);
    if( (rtn = ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
        goto error_1;
    }
    if( resp != 331 )
    {
        rtn = QCOM_OTA_ERR_FTP_LOGIN_INCORRECT;      
        printf("FTP USER error: %d",resp);
        goto error_1;
    }

    //send password
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "PASS %s\r\n", password);
    if( (rtn = ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
        goto error_1;
    }
    if( resp != 230 )
    {
		rtn = QCOM_OTA_ERR_FTP_LOGIN_INCORRECT;
		printf("FTP PASS error: %d\n",resp);
		goto error_1;
    }

    printf("FTP Login ...\n");

    //send syst
    if( (rtn = ota_ftp_send_cmd_resp("SYST\r\n",&resp)) != QCOM_OTA_OK )
    {
    	goto error_1;
    }
    if( resp != 215 )
    {
		rtn = QCOM_OTA_ERR_FTP_SYST;
		printf("FTP SYST error: %d\n",resp);
		goto error_1;
    }
    
    //setup BINARY mode
    if( (rtn = ota_ftp_send_cmd_resp("TYPE I\r\n",&resp)) != QCOM_OTA_OK )
    {
    	goto error_1;
    }
    if( resp != 200 )
    {
		rtn = QCOM_OTA_ERR_FTP_SET_TYPE;
		printf("FTP TYPE error: %d\n",resp);
		goto error_1;
    }
    
    //open data socket
    if( (rtn = ota_ftp_open_data_sock(ota_ftp_sess->local_ip_address, ota_ftp_sess->data_port)) != QCOM_OTA_OK ) 
   		goto error_1;    

    //send port
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "PORT %d,%d,%d,%d,%d,%d\r\n", 
                                               (ota_ftp_sess->local_ip_address)>>24&0xff,
                                               (ota_ftp_sess->local_ip_address)>>16 &0xff,
                                               (ota_ftp_sess->local_ip_address)>>8 & 0xff, 
		                                 		(ota_ftp_sess->local_ip_address)&0xff, 
                                               ota_ftp_sess->data_port/256,
                                               ota_ftp_sess->data_port%256);
    if( (rtn = ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
    	goto error_1;
    }
    if( resp != 200 )
    {
		rtn = QCOM_OTA_ERR_FTP_SET_PORT;
		printf("FTP PORT error: %d\n",resp);
		goto error_1;
    }

    //send get file
    A_MEMSET(buf, 0, sizeof(buf));
    qcom_sprintf(buf, "RETR %s\r\n", filename);
    if( (rtn = ota_ftp_send_cmd_resp(buf,&resp)) != QCOM_OTA_OK )
    {
    	goto error_1;
    }
    if( resp != 150 )
    {
		rtn = QCOM_OTA_ERR_FTP_FILE_NOT_FOUND;
		printf("FTP RETR error: %d\n",resp);
		goto error_1;
    }

	
    //get file and write ota
    rtn = ota_ftp_program_ota(size);
    if( rtn == QCOM_OTA_OK ) 
    {
        //we are done
        if( (rtn = qcom_ota_partition_verify_checksum()) == QCOM_OTA_OK )  
        {
            good_image = 1;        
            OTA_FTP_D_PRINTF("OTA Partition Verify Checksum is correct\n");
        } 
        else
        {
            OTA_FTP_D_PRINTF("OTA Partition Verify Checksum is NOT correct\n");
        }
    }
 
error_1:
    //send quit and recv goodbye
    ota_ftp_send_cmd_resp("QUIT\r\n", &resp);
    qcom_ota_session_end(good_image);
    ota_ftp_finit(); 
    OTA_FTP_D_PRINTF("Leave qcom_ota_ftp_upgrade %d\n", rtn);
    return rtn;
}

/*_____________________ OTA upgrade through HTTP(s) _________________________*/

#define OTA_HTTPS_MAX_FILE_LEN          (256)
#define OTA_HTTPS_MAX_SERVER_LEN        (64)
#define OTA_HTTPS_REQ_MAX	        (600)           /* 192(field of header) + length of file name + length of server name */
#define OTA_HTTPS_RCV_MAX	        (350)
#define OTA_HTTPS_TIMEOUT               (3000)
#define OTA_HTTPS_PORT                  (443)
#define OTA_HTTPS_ALT_PORT              (46030)

#define DEBUG_OTA_HTTPS
#if defined(DEBUG_OTA_HTTPS)
#define ota_hprintf(args...) A_PRINTF(args)
#else
#define ota_hprintf(args...)
#endif

typedef struct _QCOM_OTA_HTTPS_REQ {
        char    *server;
        A_INT32 ip_addr;
        A_UINT16 port;	
        char    *file;	
        char    *message;	
        A_INT32 size;	
        struct  sockaddr_in ip;
} QCOM_OTA_HTTPS_REQ;

typedef struct _QCOM_OTA_HTTPS_RES {
        char    *buf;		
        A_INT32 buf_length;	

        A_INT32 response_code;
        A_INT32 content_length;
        A_INT32 header_length; 
		A_BOOL  parsed_ota_header;

        A_INT32 read_length;
        A_INT32 partition_size;
} QCOM_OTA_HTTPS_RES;


typedef struct _QCOM_OTA_HTTP {
        A_INT32                 sock_fd;
        A_INT16                 port;

        QCOM_OTA_HTTPS_REQ      *req;
        QCOM_OTA_HTTPS_RES      *res;

        SSL_CTX                 *ssl_ctx; 
        SSL                     *hssl;	
} QCOM_OTA_HTTPS;

typedef A_INT32 (* OTA_HTTPS_RESP_CB)(
                char* buf, 		
                A_INT32 len, 	
                A_INT32 tot, 
                A_UINT32 *img_offset
                );

static QCOM_OTA_HTTPS *_qcom_ota_https_open(char *server, A_INT32 ip_addr, A_INT32 port, char *file, A_INT32 param);
static void _qcom_ota_https_close(QCOM_OTA_HTTPS *ota_https);
static A_INT32 _qcom_ota_https_request(QCOM_OTA_HTTPS *ota_https, OTA_HTTPS_RESP_CB func, A_UINT32 timeout, A_UINT32 *size);
static A_INT32 _qcom_ota_https_read_packet(QCOM_OTA_HTTPS *ota_https, A_UINT32 timeout);
static A_INT32 _qcom_ota_https_check_header(QCOM_OTA_HTTPS *ota_https);
static A_INT32 _qcom_ota_https_connect(QCOM_OTA_HTTPS *ota_https, A_UINT32 timeout);
static A_INT32 _qcom_ota_https_query(QCOM_OTA_HTTPS *ota_https);
static A_INT32 _qcom_ota_https_program(char *buf, A_INT32 len, A_INT32 tot, A_UINT32 *img_offset);

A_INT32 qcom_ota_https_upgrade(
                A_UINT8         device_id, 
                char            *server,
                A_UINT32        ip_addr, 
                A_UINT16        port, 
                char            *filename, 
                A_UINT32        flags, 
                A_UINT32        partition_index, 
                A_UINT32        *size)
{
  (void)(device_id);
        QCOM_OTA_HTTPS *ota_https = 0;
        A_INT32 ret = QCOM_OTA_OK, good_image = 0;
        A_UINT32 partition_size;

        A_PRINTF("Enter qcom_ota_https upgrade\n");
        A_PRINTF("ota_https_upgrade: ip:%xH, port:%d\n", ip_addr, port);
        A_PRINTF("                file:%s\n", filename);
        A_PRINTF("                flags:%d, partition_index:%d \n", flags, partition_index);   

        if ((flags & QCOM_OTA_TARGET_FIRMWARE_UPGRADE) == 0) {
                A_PRINTF("OTA HTTPS supports Target Firmware Upgrade only, check flags setting\n");
                return QCOM_OTA_ERR_UNSUPPORT_PROTOCOL; 
        }

        if ((ret = qcom_ota_session_start(flags, partition_index)) != QCOM_OTA_OK) {
                A_PRINTF("OTA Session Start Fail\n");
                return ret;
        }

#if 0
        if ((ret = qcom_ota_partition_erase()) != QCOM_OTA_OK) {
                A_PRINTF("OTA Partition Erase Fail\n");
                return ret;
        }
#endif

        if (!(partition_size = qcom_ota_partition_get_size())) {
                A_PRINTF("No partition to upgrade\n");
                return QCOM_OTA_ERR_INVALID_PARTITION_INDEX;
        }

        ota_hprintf("OTA Partition Get Size: %d\n", partition_size);

        if (!(ota_https = _qcom_ota_https_open(server, ip_addr, port, filename, flags))) {
                ret = QCOM_OTA_ERR_INSUFFICIENT_MEMORY;
                goto _error;
        }

        ota_https->res->partition_size = partition_size;

        ret = _qcom_ota_https_request(ota_https, _qcom_ota_https_program, OTA_HTTPS_TIMEOUT, size);
        A_PRINTF("qcom size %d", *size);
        if (ret == QCOM_OTA_OK && *size > 0) {
                if ((ret = qcom_ota_partition_verify_checksum()) == QCOM_OTA_OK)  {
                        good_image = 1;        
                        A_PRINTF("OTA Partition Verify Checksum is correct\n");
                } else {
                        A_PRINTF("OTA Partition Verify Checksum is NOT correct (%d)\n", ret);
                }
        }

_error:
        qcom_ota_session_end(good_image);
        _qcom_ota_https_close(ota_https);

        A_PRINTF("Leave qcom_ota_https upgrade\n");

        return ret;
}

static QCOM_OTA_HTTPS *_qcom_ota_https_open(char *server, A_INT32 ip_addr, A_INT32 port, char *file, A_INT32 param)
{
  (void)(param);
        QCOM_OTA_HTTPS *ota_https = 0;
        struct sockaddr_in loc;
        A_UINT32 file_len, server_len;

        if (!port) port = 80;

        ota_https = (QCOM_OTA_HTTPS *)qcom_mem_alloc(sizeof(QCOM_OTA_HTTPS));
        if (!ota_https) {
                A_PRINTF("Memory allocate failed\n");
                goto _alloc_error;
        }
        A_MEMZERO(ota_https, sizeof(QCOM_OTA_HTTPS));

        ota_https->req = (QCOM_OTA_HTTPS_REQ *)qcom_mem_alloc(sizeof(QCOM_OTA_HTTPS_REQ));
        if (!ota_https->req) {
                A_PRINTF("Memory allocate failed\n");
                goto _alloc_error;
        }
        A_MEMZERO(ota_https->req, sizeof(QCOM_OTA_HTTPS_REQ));

        ota_https->req->ip_addr                 = ip_addr;
        ota_https->req->port                    = port;
        ota_https->req->message                 = (char *)qcom_mem_alloc(OTA_HTTPS_REQ_MAX);
        if (!ota_https->req->message) {
                A_PRINTF("Memory allocate failed\n");
                goto _alloc_error;
        }
        ota_https->req->size                    = OTA_HTTPS_REQ_MAX;

        /* TODO, check strdup & mem free routine, address mis-aligned exception happens when mem_free invoked
        ota_https->req->file                    = strdup(file);
        ota_https->req->server                  = strdup(server);
        */
        if ((file_len = A_STRLEN(file) + 1) > OTA_HTTPS_MAX_FILE_LEN) {
                A_PRINTF("file length is too big\n");
                goto _alloc_error;
        }

        ota_https->req->file                    = qcom_mem_alloc(file_len);
        if (!ota_https->req->file) {
                A_PRINTF("Memory allocate failed\n");
                goto _alloc_error;
        }

        if ((server_len = A_STRLEN(server) + 1) > OTA_HTTPS_MAX_SERVER_LEN) {
                A_PRINTF("server length is too big\n");
                goto _alloc_error;
        }

        ota_https->req->server                  = qcom_mem_alloc(server_len);
        if (!ota_https->req->server) {
                A_PRINTF("Memory allocate failed\n");
                goto _alloc_error;
        }

        A_STRCPY(ota_https->req->file, file);
        A_STRCPY(ota_https->req->server, server);

        memset((char*)&ota_https->req->ip, 0, sizeof(struct sockaddr_in));
        ota_https->req->ip.sin_family           = AF_INET;
        ota_https->req->ip.sin_addr.s_addr      = htonl(ip_addr);
        ota_https->req->ip.sin_port             = htons(port);
        ota_https->sock_fd                      = qcom_socket(AF_INET, SOCK_STREAM, 0);
        if (ota_https->sock_fd < 0) {
                A_PRINTF("socket create error\n");
                goto _socket_error;
        }

        memset((char*)&loc, 0, sizeof(struct sockaddr_in));
        loc.sin_family          = AF_INET;
        loc.sin_addr.s_addr     = INADDR_ANY;
        loc.sin_port            = htons((A_INT16)0);

        if (qcom_bind(ota_https->sock_fd, (struct sockaddr*)&loc, sizeof(struct sockaddr_in)) < 0) {
                A_PRINTF("bind error\n");
                goto _socket_error;
        }

        ota_https->port	= loc.sin_port;
        ota_https->res = (QCOM_OTA_HTTPS_RES *)qcom_mem_alloc(sizeof(QCOM_OTA_HTTPS_RES));
        if (!ota_https->res) {
                A_PRINTF("Memory allocate failed\n");
                goto _alloc_error;
        }
        A_MEMZERO(ota_https->res, sizeof(QCOM_OTA_HTTPS_RES));

        ota_https->res->buf = (char *)qcom_mem_alloc(OTA_HTTPS_RCV_MAX);
        if (!ota_https->res->buf) {
                A_PRINTF("Memory allocate failed\n");
                goto _alloc_error;
        }

        if (port == OTA_HTTPS_PORT || port == OTA_HTTPS_ALT_PORT) {
                A_INT32 reuse = 1;
                A_INT32 yes = 1;
                struct linger lin;

                lin.l_onoff = FALSE;
                lin.l_linger = 5;

                qcom_socket_set_non_blocking(ota_https->sock_fd, 1);
                qcom_setsockopt(ota_https->sock_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse));
                qcom_setsockopt(ota_https->sock_fd, SOL_SOCKET, SO_LINGER, (char *)&lin, sizeof(lin));
                qcom_setsockopt(ota_https->sock_fd, SOL_SOCKET, TCP_NODELAY, (char *)&yes, sizeof(yes));
                qcom_setsockopt(ota_https->sock_fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&yes, sizeof(yes));

                if (!(ota_https->ssl_ctx = qcom_SSL_ctx_new(SSL_CLIENT, SSL_OTA_INBUF_SIZE, SSL_OUTBUF_SIZE, 0))) {
                        A_PRINTF("SSL ctx new error\n");
                        goto _alloc_error;
                }

                ota_https->hssl = qcom_SSL_new(ota_https->ssl_ctx);
                qcom_SSL_set_fd(ota_https->hssl, ota_https->sock_fd);
        } 

        return ota_https;

_socket_error:
_alloc_error:
        _qcom_ota_https_close(ota_https);
        return 0;
}

static void _qcom_ota_https_close(QCOM_OTA_HTTPS *ota_https)
{
        QCOM_OTA_HTTPS_REQ *req;
        QCOM_OTA_HTTPS_RES *res;

        if (!ota_https) return;

        req = ota_https->req;
        res = ota_https->res;

        if (req) {
                if (req->file) qcom_mem_free(req->file);
                if (req->server) qcom_mem_free(req->server);
                if (req->message) qcom_mem_free(req->message);

                req->file = 0;
                req->server = 0;
                req->message = 0;

                qcom_mem_free(req);
                ota_https->req = 0;
        }

        if (res) {
                if (res->buf) qcom_mem_free(res->buf);
                res->buf = 0;

                qcom_mem_free(res);
                ota_https->res = 0;
        }

        if (ota_https->ssl_ctx) {
                qcom_SSL_ctx_free(ota_https->ssl_ctx);
                ota_https->ssl_ctx = 0;
        }
        if (ota_https->hssl){
            qcom_SSL_shutdown(ota_https->hssl);
            ota_https->hssl = 0; 
        }
		
		if (ota_https->sock_fd > 0) {
			qcom_socket_close(ota_https->sock_fd);
		}
		
        qcom_mem_free(ota_https);

        return;
}

static A_INT32 _qcom_ota_https_request(
                QCOM_OTA_HTTPS          *ota_https,
                OTA_HTTPS_RESP_CB       func,
                A_UINT32                timeout,
                A_UINT32 *size)
{
        QCOM_OTA_HTTPS_RES *res;
        A_INT32	ret = QCOM_OTA_OK;
        A_UINT32 offset = 0, img_offset = 0;

        res = ota_https->res;
        res->read_length = 0;
		*size = 0;

        qcom_socket_set_non_blocking(ota_https->sock_fd, 1);
        if ((ret = _qcom_ota_https_connect(ota_https, timeout)) < 0)
                return ret;

        if ((ret = _qcom_ota_https_query(ota_https)) < 0) {
                return ret;
        }

        do {
                ret = _qcom_ota_https_read_packet(ota_https, timeout);
                if (ret < 0) {
                        A_PRINTF("https read error\n");
                        return ret;
                }

                if (!res->response_code) {
                        ret = _qcom_ota_https_check_header(ota_https);
                        if (ret == 0) continue;	/* not finished header receive */
                        else if (ret < 0) return ret; /* error */
                        ota_hprintf("response: %d, content length: %d, header length: %d\n", 
                                        res->response_code, res->content_length, res->header_length);

                        if (res->content_length > res->partition_size || res->content_length < 0) {
                                ret = QCOM_OTA_ERR_IMAGE_DOWNLOAD_FAIL;
                                return ret;
                        }
						
						if (!res->buf_length) {
							continue; // Wait for next packet
						}
                }

				res->read_length += res->buf_length;
				if (!res->parsed_ota_header) {
                        if ((ret = qcom_ota_parse_image_hdr((A_UINT8 *)res->buf, &offset)) != 0) {
                                A_PRINTF("OTA parse error(%d)\n", ret);
                                return ret;
                        }

                        res->buf_length -= offset;
                        ota_hprintf("OTA parse offset: %d, buf_length: %d\n", offset, res->buf_length);
						res->parsed_ota_header = TRUE;
                } else {
                        offset = 0;
                }

                if (!func)
                        continue;

                if (res->buf_length) {
                        if ((ret = func(&res->buf[offset], res->buf_length, res->read_length, &img_offset)) < 0) {
                                A_PRINTF("OTA write error\n");
                                return ret;
                        }
                }

                res->buf_length = 0;
                if (res->read_length >= res->content_length) {
                        ota_hprintf("read length: %d, img_offset: %d\n", res->read_length, img_offset);
                        break;
                }
        } while (1);

        *size = res->read_length;

		return QCOM_OTA_OK;
}

static A_INT32 _qcom_ota_https_query(QCOM_OTA_HTTPS *ota_https)
{
        QCOM_OTA_HTTPS_REQ *req = ota_https->req;
        A_INT32 ret = QCOM_OTA_OK;
        char *cp = req->message;

        cp += sprintf(cp, "GET /%s HTTP/1.1\r\n", req->file);
        cp += sprintf(cp, "Host: %s\r\n", req->server);
        cp += sprintf(cp, "Accept: text/html, */*\r\n");
        cp += sprintf(cp, "User-Agent: IOE Client\r\n");
        cp += sprintf(cp, "Connection: keep-alive\r\n");
        cp += sprintf(cp, "Cache-control: no-cache\r\n");
        cp += sprintf(cp, "\r\n");

        req->size = A_STRLEN(req->message);
        ota_hprintf("%s\nHTTP(S) query message size: %d\n\n", req->message, req->size);

        if (req->port == OTA_HTTPS_PORT || req->port == OTA_HTTPS_ALT_PORT) {
                if ((ret = qcom_SSL_write(ota_https->hssl, req->message, req->size)) < 0) {
                        A_PRINTF("ssl write error(%d)\n", ret);
                }
        } else {
		if ((ret = qcom_send(ota_https->sock_fd, req->message, req->size, 0)) < 0) {
                        A_PRINTF("qcom_send error(%d)\n", ret);
                }
        }

        return ret;
}

static A_INT32 _qcom_ota_https_parse_header_response_code(char *buf)
{
        A_INT32 response_code;
        A_INT32 ver1, ver2, i;

        i = sscanf(buf, "HTTP/%d.%d %d ", &ver1, &ver2, &response_code);
        if (i != 3) return (-1);

        return response_code;
}

static A_INT32 _qcom_ota_https_parse_header_content_length(char *buf)
{
#define _MAX_CONTENT_DIGIT      (20)
        char *str;
        char temp[_MAX_CONTENT_DIGIT];
        A_INT32 i = 0;

        str = A_STRSTR(buf, "Content-Length: ");
        if (str) {
                str += 16;

                while (*str) {
                        if (i >= _MAX_CONTENT_DIGIT)
                                return (-1);

                        if (*str == '\r' && *(str+1) == '\n') {
                                break;
                        } else {
                                temp[i] = *str;
                        }

                        i++; str++;
                }

                temp[i] = 0;
                return atoi(temp);
        }

        return (-1);
}

static A_INT32 _qcom_ota_https_check_header(QCOM_OTA_HTTPS *ota_https)
{
        QCOM_OTA_HTTPS_RES *res = ota_https->res;
        char *hend;
        A_INT32 len;

        if (res->response_code > 0) return res->response_code;
        if (!res->buf_length) return 0;

        hend = A_STRSTR(res->buf, "\r\n\r\n");
        if (!hend) return 0;

        *hend = 0;

        res->response_code = _qcom_ota_https_parse_header_response_code(res->buf);
        res->content_length = _qcom_ota_https_parse_header_content_length(res->buf);

        if (res->response_code >= 300) return -(res->response_code);

        len = res->buf_length - (A_INT32)((hend + 4) - res->buf);
        res->header_length = res->buf_length - len;

        A_MEMMOVE(res->buf, hend + 4, len);
        res->buf_length = len;

        return res->response_code;
}

static A_INT32 _qcom_ota_https_read_packet(QCOM_OTA_HTTPS *ota_https, A_UINT32 timeout)
{
        QCOM_OTA_HTTPS_RES *res = ota_https->res;
        struct timeval tv;
        q_fd_set rset;
        A_INT32 len, ret;

        FD_ZERO(&rset);
        tv.tv_sec = (timeout/1000);
        tv.tv_usec = 0;

        FD_SET(ota_https->sock_fd, &rset);
        if ((ret = qcom_select(ota_https->sock_fd + 1, &rset, 0, 0, &tv)) <= 0) {
                if (ret == 0) return (-QCOM_OTA_ERR_SERVER_RSP_TIMEOUT);
                else return (ret);
        }

        if (ota_https->req->port == OTA_HTTPS_PORT || ota_https->req->port == OTA_HTTPS_ALT_PORT) {
                len = qcom_SSL_read(ota_https->hssl, &res->buf[0], OTA_HTTPS_RCV_MAX);
                if (len > 0) {
                        res->buf_length = len;
                } else {
                        A_PRINTF("SSL read error(%d)\n", len);
                        return (-1);
                }
        } else {
                len = qcom_recv(ota_https->sock_fd, &res->buf[0], OTA_HTTPS_RCV_MAX, 0);
                if (len > 0) {
                        res->buf_length = len;
                } else {
                        A_PRINTF("qcom_recv error(%d)\n", len);
                        return (-1);
                }
        }

        return len;
}

static A_INT32 _qcom_ota_https_connect(QCOM_OTA_HTTPS *ota_https, A_UINT32 timeout)
{
        struct timeval tv;
        q_fd_set wset;
        A_INT32 ret;

        FD_ZERO(&wset);
        tv.tv_sec = (timeout/1000);
        tv.tv_usec = 0;

        FD_SET(ota_https->sock_fd, &wset);
        if ((ret = qcom_connect(ota_https->sock_fd, (struct sockaddr*)&ota_https->req->ip, sizeof(ota_https->req->ip))) < 0) {
			printf("Connection failed.\n");
			return ret;
		}

        if ((ret = qcom_select(ota_https->sock_fd + 1, 0, &wset, 0, &tv)) <= 0) {
                if (ret == 0) return (-QCOM_OTA_ERR_SERVER_RSP_TIMEOUT);
                else return (ret);
        }

        if (ota_https->req->port == OTA_HTTPS_PORT || ota_https->req->port == OTA_HTTPS_ALT_PORT) {
                if ((ret = qcom_SSL_connect(ota_https->hssl)) < 0) {
                        A_PRINTF("ssl connect error(%d)\n", ret);
                        return ret;
                }
        } 

        return 0;
}

static A_INT32 _qcom_ota_https_program(char *buf, A_INT32 len, A_INT32 tot, A_UINT32 *img_offset)
{
  (void)(tot);
        A_UINT32 offset = 0, ret_size = 0, write_size = 0;
        A_INT32 ret = QCOM_OTA_OK;

        while (len > 0) {
                if (len > MAX_OTA_AREA_READ_SIZE) {
                        write_size = MAX_OTA_AREA_READ_SIZE;
                } else {
                        write_size = len;
                }

                len -= write_size;

                A_PRINTF("erase %d   ", *img_offset + write_size);
                if ((ret = qcom_ota_partition_erase_sectors(*img_offset + write_size)) != QCOM_OTA_OK) {
                        A_PRINTF("OTA Partition Erase Fail\n");
                        return ret;
                }

                A_PRINTF("write %d %d   ", *img_offset, write_size);
                if ((ret = qcom_ota_partition_write_data(*img_offset, 
                                       (A_UINT8 *)&buf[offset], write_size, &ret_size)) != QCOM_OTA_OK) {
                        A_PRINTF("OTA write error(%d), write_size(%d), ret_size(%d)\n", ret, write_size, ret_size);
                        return ret;
                } 

                A_PRINTF(".%d ", ret_size);

                offset += ret_size;
                *img_offset += ret_size;
        }
        A_PRINTF("\r\n");

        return QCOM_OTA_OK;
}
