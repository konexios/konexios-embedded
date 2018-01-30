/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>

int uart_file = -1;

int uart_init() {
    uart_file = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_file == -1) {
        printf("Error: Unable to open uart\r\n");
        return -1;
    }
    struct termios options;
    tcgetattr(uart_file, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_file, TCIFLUSH);
    tcsetattr(uart_file, TCSANOW, &options);
    return 0;
}

int uart_send(const char *buf, int size) {
//    printf(buf);
    int count = write(uart_file, buf, size);
    if (count < 0) {
        printf("UART TX error\n");
        return -1;
    }
    return count;
}

int uart_recv(char *buf, int size) {
    int rx_length = read(uart_file, buf, size);
    if (rx_length <= 0) {
        return -1;
    } else {
        buf[rx_length] = '\0';
//        printf(buf);
    }
    return rx_length;
}

int uart_recv_timeout(char *buf, int size, int timeout_ms) {
    fd_set set;
    struct timeval to;
    int rv;
    FD_ZERO(&set);
    FD_SET(uart_file, &set);
    to.tv_sec = timeout_ms / 1000;
    to.tv_usec = (timeout_ms % 1000) * 1000;
    rv = select(uart_file + 1, &set, NULL, NULL, &to);
    if ( rv < 0 ) {
        return -2;
    } else if ( rv == 0 ) {
        return 0;
    } else {
        return uart_recv(buf, size);
    }
    return -1;
}
