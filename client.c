#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8784

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    FILE *f = fopen("bigfile.txt", "r");

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *data = (char*)malloc(size + 1);
    fread(data, 1, size, f);
    data[size] = '\0';

    send_msg(sock, data, size);

    char *result;
    recv_msg(sock, &result);

    printf("RESULT:\n%s\n", result);

    free(data);
    free(result);
    closesocket(sock);

    WSACleanup();
}