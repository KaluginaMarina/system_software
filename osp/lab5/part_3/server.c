#include <bits/types/time_t.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "server.h"

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

void set_ids(struct server_param *server_param) {
    time(&server_param->start_time);
    server_param->pid = getpid();
    server_param->uid = getuid();
    server_param->gid = getgid();
}


void start_server(int argc, char *argv[]) {
    struct server_param *server_param = malloc(sizeof(struct server_param));
    //unsigned int flag =
    parse_flag(argc, argv);

    set_ids(server_param);

    while (true) {
        sleep(1);
        set_param(server_param);
        printf("work_time = %ld, 1min = %.2f, 5min = %.2f, 15min = %.2f\n", server_param->work_time,
               server_param->loadavg[0],
               server_param->loadavg[1], server_param->loadavg[2]);
    }
}

void set_param(struct server_param *server_param) {
    time_t now;
    time(&now);
    server_param->work_time = now - server_param->start_time;
    getloadavg(server_param->loadavg, 3);
}

unsigned int parse_flag(int argc, char *argv[]) {
    unsigned int flag = 0, opt = 0;
    while ((opt = getopt(argc, argv, "sip")) != -1) {
        switch (opt) {
            case 's':
                flag |= FLAG_SOCKET;
                if (flag ^ FLAG_SOCKET) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                printf("Клиент-серверное взаимодействие осуществляется при помощи unix domain socket.\n");
                break;
            case 'i':
                flag |= FLAG_SIGNAL;
                if (flag ^ FLAG_SIGNAL) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                printf("Клиент-серверное взаимодействие осуществляется при помощи обработки сигналов.\n");
                break;
            case 'p':
                flag |= FLAG_PIPE;
                if (flag ^ FLAG_PIPE) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                printf("Клиент-серверное взаимодействие осуществляется при помощи неименованных каналов.\n");
                break;
            default:
                fprintf(stderr, "Используйте: \n\t./server -s|-i|-p filename.\n");
                exit(1);
        }
    }
    if (!flag) {
        fprintf(stderr,
                "Не указан способ передачи сообщения между клиентом и сервером.\nИспользуйте: \n\t./server -s|-i|-p filename.\n");
        exit(1);
    }
    return flag;
}
