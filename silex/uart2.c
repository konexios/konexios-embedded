/*
 * Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include "uart2.h"
#include <debug.h>

#define UART2 "UART2"
static A_INT32 uart2_fd = -1;
static A_INT32 uart0_fd = -1;

static A_INT32 uart_act = -1;

int uart2_init() {
    A_INT32 uart1_fd;

    qcom_single_uart_init((A_CHAR *)"UART0");
    qcom_single_uart_init((A_CHAR *)"UART1");
    qcom_uart_set_buffer_size(UART2, 512, 512);
    qcom_single_uart_init(UART2);
    uart0_fd = qcom_uart_open((A_CHAR *)"UART0");
    if (uart0_fd == -1) {
        A_PRINTF("qcom_uart_open uart0 failed...\n");
        return;
    }
    uart1_fd = qcom_uart_open((A_CHAR *)"UART1");
    if (uart1_fd == -1) {
        A_PRINTF("qcom_uart_open uart1 failed...\n");
        return;
    }
    uart2_fd = qcom_uart_open(UART2);
    if (uart2_fd == -1) {
        A_PRINTF("qcom_uart_open uart2 failed...\n");
        return;
    }

    A_PRINTF("Init UART\r\n");
    qcom_uart_para com_uart_cfg;
    com_uart_cfg.BaudRate=     115200;
    com_uart_cfg.number=       8;
    com_uart_cfg.StopBits =    1;
    com_uart_cfg.parity =      0;
    com_uart_cfg.FlowControl = 0;
}

char get_char(int *status, int timeout_s) {
    q_fd_set fd;
    A_UINT32 uart_length;
    struct timeval tmo = {timeout_s, 0};
    int ret = -1;
    char uart_buf[2] = {0};

    FD_ZERO(&fd);
    FD_SET(uart0_fd, &fd);
//    FD_SET(uart1_fd, &fd);
    FD_SET(uart2_fd, &fd);
    tmo.tv_sec = timeout_s;
    tmo.tv_usec = 0;
    if (status) *status = 0;

    ret = qcom_select(uart2_fd + 1, &fd, NULL, NULL, &tmo);
    if (ret == 0)  {
//        A_PRINTF("UART receive timeout\n");
        if (status) *status = -1;
        return 0;
    } else {
        if (FD_ISSET(uart2_fd, &fd)) {
            uart_length = 1;
            qcom_uart_read(uart2_fd, uart_buf, &uart_length);
            uart_act = uart2_fd;
            //tx_thread_sleep(100);
            if (uart_length) {
              return uart_buf[0];
            }
        } else if (FD_ISSET(uart0_fd, &fd)) {
            uart_length = 1;
            qcom_uart_read(uart0_fd, uart_buf, &uart_length);
            uart_act = uart0_fd;
            //tx_thread_sleep(100);
            if (uart_length) {
              A_PRINTF(uart_buf);
              return uart_buf[0];
            }
        }
        return 0;
    }
}

int uart2_write(const char *msg, int size) {
    A_UINT32 uart_length = size;
    A_INT32 rw = 0;
    rw = qcom_uart_write(uart_act, (A_CHAR *)msg, &uart_length);
    if ( rw > 0) return size;
    return -1;
}
