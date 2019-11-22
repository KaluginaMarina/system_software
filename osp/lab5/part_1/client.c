#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "server.h"
#include "client.h"

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

struct server_param* get_param_shared_memory(int mem_id) {
    printf("Клиент-серверное взаимодействие осуществляется при помощи разделяемых сегментов памяти.\n");
    printf("mem_id = %d\n", mem_id);
    errno = 0;
    struct server_param *serverparam = (struct server_param *) shmat(mem_id, NULL, SHM_RDONLY);
    check_errno("Невозможно получить доступ к указанной памяти");
    return serverparam;
}


struct server_param* get_param_message_queue_param(int mem_id) {
    errno = 0;
    struct msgbuff msgbuff;
    msgbuff.mtype = MSGTYPE_QUERY;
    msgsnd(mem_id, &msgbuff, 0, 0);
    check_errno("Невозможно отправить запрос для получения информации в очереди");
    msgrcv(mem_id, &msgbuff, sizeof(struct server_param), MSGTYPE_REPLY, 0);
    check_errno("Невозможно получить сообщение");

    struct server_param *server_param = (struct server_param*) malloc(sizeof(struct server_param));
    memcpy(server_param, msgbuff.mtext, sizeof(struct server_param));
    return server_param;
}



struct server_param* get_param_mmap_file(char *filename) {
    errno = 0;
    int file = open(filename, O_RDONLY, NULL);
    check_errno("Невозможно открыть файл");

    struct server_param *server_param = (struct server_param *) mmap(NULL, sizeof(struct server_param), PROT_READ,
                                                                     MAP_SHARED, file, 0);
    check_errno("Невозможно отобразить файл");
    close(file);
    return server_param;
}

void get_param(int argc, char *argv[]) {
    int mem_id;
    char *filename = malloc(256 * sizeof(char));
    struct server_param *server_param = malloc(sizeof(struct server_param));
    flag = parse_flag_cl(argc, argv, &mem_id, filename);

    if (flag & SHARED_MEMORY) {
        server_param = get_param_shared_memory(mem_id);
        printf("Клиент-серверное взаимодействие осуществляется при помощи разделяемых сегментов памяти.\n");
    } else if (flag & MESSAGE_QUEUE) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи System V message queue.\n");
        printf("mem_id = %d\n", mem_id);
        get_param_message_queue_param(mem_id);
        server_param = get_param_message_queue_param(mem_id);
    } else if (flag & MMAP_FILE) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи файла, отображённого в память с использованием mmap.\n");
        printf("filename = %s\n", filename);
        server_param = get_param_mmap_file(filename);
    }
    printf("work_time = %ld, loadavg: 1mim = %f, 5min = %f, 15min = %f\n", server_param->work_time,
           server_param->loadavg[0], server_param->loadavg[1], server_param->loadavg[2]);
}

unsigned int parse_flag_cl(int argc, char *argv[], int *mem_id, char *filename) {
    unsigned int flag = 0, opt = 0;
    char *p;
    while ((opt = getopt(argc, argv, "sqf")) != -1) {
        switch (opt) {
            case 's':
                flag |= SHARED_MEMORY;
                if (flag ^ SHARED_MEMORY) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                if (optind == argc) {
                    fprintf(stderr, "Не указан mem_id.\n");
                    exit(1);
                }
                errno = 0;
                *mem_id = (int) strtol(argv[optind], &p, 10);
                if (errno != 0 || *mem_id < 0) {
                    fprintf(stderr, "Указан неверный mem_id.\n");
                    exit(1);
                }
                break;
            case 'q':
                flag |= MESSAGE_QUEUE;
                if (flag ^ MESSAGE_QUEUE) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                if (optind == argc) {
                    fprintf(stderr, "Не указан mem_id.\n");
                    exit(1);
                }
                errno = 0;
                *mem_id = (int) strtol(argv[optind], &p, 10);
                if (errno != 0 || *mem_id < 0) {
                    fprintf(stderr, "Указан неверный mem_id.\n");
                    exit(1);
                }
                break;
            case 'f':
                flag |= MMAP_FILE;
                if (flag ^ MMAP_FILE) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                if (optind == argc) {
                    fprintf(stderr, "Не указан filename.\n");
                    exit(1);
                }
                strcpy(filename, argv[optind]);
                break;
            default:
                fprintf(stderr, "Используйте: \n\t./server -s mem_id|-q mem_id|-f filename.\n");
                exit(1);
        }
    }
    if (!flag) {
        fprintf(stderr,
                "Не указан способ передачи сообщения между клиентом и сервером.\nИспользуйте: \n\t./server -s mem_id|-q mem_id|-f filename.\n");
        exit(1);
    }
    return flag;
}
