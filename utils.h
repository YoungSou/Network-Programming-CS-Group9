#ifndef UTILS_H
#define UTILS_H

#include <winsock2.h>
#include <stdint.h>

int send_msg(SOCKET sock, const char *data, uint32_t len);
int recv_msg(SOCKET sock, char **out_data);

#endif