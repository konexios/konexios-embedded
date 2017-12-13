/* Copyright (c) 2017 Arrow Electronics, Inc.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Apache License 2.0
 * which accompanies this distribution, and is available at
 * http://apache.org/licenses/LICENSE-2.0
 * Contributors: Arrow Electronics, Inc.
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>

int get_mac_address(char *mac) {
    struct ifreq buffer;
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    memset(&buffer, 0x00, sizeof(buffer));
    strcpy(buffer.ifr_name, "wlan0");
    ioctl(sock, SIOCGIFHWADDR, &buffer);
    close(sock);
    int s;
    for( s  = 0; s < 6; s++ ) {
        mac[s] = buffer.ifr_hwaddr.sa_data[s];
    }
    return 0;
}
