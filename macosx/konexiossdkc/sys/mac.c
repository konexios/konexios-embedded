#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <string.h>
#include <unistd.h>
#include <net/if_dl.h>
#include <ifaddrs.h>

int get_mac_address(char* mac_addr)
{
    struct ifaddrs *iflist, *cur;
    if (getifaddrs(&iflist) == 0) {
        for (cur = iflist; cur; cur = cur->ifa_next) {
            if ((cur->ifa_addr->sa_family == AF_LINK) &&
                    (strcmp(cur->ifa_name, "en0") == 0) &&
                    cur->ifa_addr) {
                struct sockaddr_dl* sdl = (struct sockaddr_dl*)cur->ifa_addr;
                memcpy(mac_addr, LLADDR(sdl), sdl->sdl_alen);
                break;
            }
        }

        freeifaddrs(iflist);
    }
    return 1;
}
