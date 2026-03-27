#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")

#define MAX_WORDS 10000

typedef struct {
    char word[50];
    int count;
} Word;

Word words[MAX_WORDS];
int word_count = 0;

// thêm word vào bảng
void add_word(char *w) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i].word, w) == 0) {
            words[i].count++;
            return;
        }
    }
    strcpy(words[word_count].word, w);
    words[word_count].count = 1;
    word_count++;
}

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 9001;

    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("[WORKER] Running on port %d\n", port);

    while (1) {
        SOCKET client = accept(server_fd, NULL, NULL);

        char *data;
        recv_msg(client, &data);

        word_count = 0;
        int words_total = 0;

        char *copy_words = _strdup(data);
        char *token = strtok(copy_words, " \r\n\t");

        while (token) {
            add_word(token);
            words_total++;
            token = strtok(NULL, " \r\n\t");
        }

        free(copy_words);

        int lines = 0;
        char *copy_lines = _strdup(data);
        char *line = strtok(copy_lines, "\n");

        while (line) {
            lines++;
            line = strtok(NULL, "\n");
        }

        free(copy_lines);

        char result[200000] = "";
        sprintf(result, "%d %d ", lines, words_total);

        for (int i = 0; i < word_count; i++) {
            char temp[100];
            sprintf(temp, "%s:%d ", words[i].word, words[i].count);
            strcat(result, temp);
        }

        send_msg(client, result, strlen(result));

        free(data);
        closesocket(client);
    }

    WSACleanup();
    return 0;
}
