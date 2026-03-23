#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")

#define PORT 8784
#define MAX_WORKERS 2
#define MAX_WORDS 50000

char *worker_ips[MAX_WORKERS] = {"127.0.0.1", "127.0.0.1"};
int worker_ports[MAX_WORKERS] = {9001, 9002};

typedef struct {
    char word[50];
    int count;
} Word;

Word global_words[MAX_WORDS];
int global_count = 0;

typedef struct {
    char *text;
    int index;
    char *result;
} Task;

void add_global(char *w, int c) {
    for (int i = 0; i < global_count; i++) {
        if (strcmp(global_words[i].word, w) == 0) {
            global_words[i].count += c;
            return;
        }
    }
    strcpy(global_words[global_count].word, w);
    global_words[global_count].count = c;
    global_count++;
}

DWORD WINAPI send_to_worker(LPVOID arg) {
    Task *task = (Task*)arg;

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(worker_ports[task->index]);
    addr.sin_addr.s_addr = inet_addr(worker_ips[task->index]);

    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    send_msg(sock, task->text, strlen(task->text));
    recv_msg(sock, &task->result);

    closesocket(sock);
    return 0;
}

int cmp(const void *a, const void *b) {
    return ((Word*)b)->count - ((Word*)a)->count;
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("[COORDINATOR] Ready...\n");

    while (1) {
        SOCKET client = accept(server_fd, NULL, NULL);

        char *data;
        recv_msg(client, &data);

        DWORD start = GetTickCount();

        int mid = strlen(data) / 2;

        Task tasks[MAX_WORKERS];
        HANDLE threads[MAX_WORKERS];

        tasks[0].text = _strdup(data);
        tasks[1].text = _strdup(data + mid);

        for (int i = 0; i < MAX_WORKERS; i++) {
            tasks[i].index = i;
            threads[i] = CreateThread(NULL, 0, send_to_worker, &tasks[i], 0, NULL);
        }

        WaitForMultipleObjects(MAX_WORKERS, threads, TRUE, INFINITE);

        int total_lines = 0, total_words = 0;
        global_count = 0;

        for (int i = 0; i < MAX_WORKERS; i++) {
            char *token = strtok(tasks[i].result, " ");

            int lines = atoi(token);
            token = strtok(NULL, " ");
            int words = atoi(token);

            total_lines += lines;
            total_words += words;

            token = strtok(NULL, " ");
            while (token) {
                char w[50];
                int c;
                sscanf(token, "%[^:]:%d", w, &c);
                add_global(w, c);
                token = strtok(NULL, " ");
            }
        }

        qsort(global_words, global_count, sizeof(Word), cmp);

        DWORD end = GetTickCount();

        char final[5000];

        sprintf(final,
            "=== Aggregated Results ===\n"
            "Total lines : %d\n"
            "Total words : %d\n"
            "Unique words: %d\n"
            "Top 5 words : ",
            total_lines, total_words, global_count
        );

        char temp[200];
        for (int i = 0; i < 5 && i < global_count; i++) {
            sprintf(temp, "%s(%d) ", global_words[i].word, global_words[i].count);
            strcat(final, temp);
        }

        sprintf(temp, "\nProcessing time: %lu ms\n", end - start);
        strcat(final, temp);

        send_msg(client, final, strlen(final));

        closesocket(client);
        free(data);
    }

    WSACleanup();
}