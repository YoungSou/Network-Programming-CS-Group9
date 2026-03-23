#include "utils.h"
#include <stdlib.h>
#include <string.h>

int send_msg(SOCKET sock, const char *data, uint32_t len) {
    uint32_t net_len = htonl(len);

    if (send(sock, (char*)&net_len, 4, 0) != 4) return -1;
    if (send(sock, data, len, 0) != len) return -1;

    return 0;
}

int recv_msg(SOCKET sock, char **out_data) {
    uint32_t net_len;
    if (recv(sock, (char*)&net_len, 4, 0) != 4) return -1;

    uint32_t len = ntohl(net_len);

    *out_data = (char*)malloc(len + 1);

    int received = 0;
    while (received < len) {
        int r = recv(sock, *out_data + received, len - received, 0);
        if (r <= 0) return -1;
        received += r;
    }

    (*out_data)[len] = '\0';
    return len;
}