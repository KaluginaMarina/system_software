#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "client.h"
#include "server.h"

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

void get_param(int argc, char *argv[]) {
    struct server_param *server_param = malloc(sizeof(struct server_param));
    flag = parse_flag_cl(argc, argv);

    if (flag & FLAG_SOCKET) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи unix domain socket.\n");
    } else if (flag & FLAG_SIGNAL) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи обработки сигналов.\n");
    } else if (flag & FLAG_PIPE) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи неименованных каналов.\n");
    }
    printf("work_time = %ld, loadavg: 1mim = %f, 5min = %f, 15min = %f\n", server_param->work_time,
           server_param->loadavg[0], server_param->loadavg[1], server_param->loadavg[2]);
}

unsigned int parse_flag_cl(int argc, char *argv[]) {
    unsigned int flag = 0, opt = 0;
    while ((opt = getopt(argc, argv, "sip")) != -1) {
        switch (opt) {
            case 's':
                flag |= FLAG_SOCKET;
                if (flag ^ FLAG_SOCKET) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                break;
            case 'i':
                flag |= FLAG_SIGNAL;
                if (flag ^ FLAG_SIGNAL) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                break;
            case 'p':
                flag |= FLAG_PIPE;
                if (flag ^ FLAG_PIPE) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
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
