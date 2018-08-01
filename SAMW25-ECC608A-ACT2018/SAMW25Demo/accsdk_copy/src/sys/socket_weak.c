#include <http/http_client.h>

int __attribute_weak__ socket_connect_done(int sock) {
    SSP_PARAMETER_NOT_USED(sock);
    return 0;
}
