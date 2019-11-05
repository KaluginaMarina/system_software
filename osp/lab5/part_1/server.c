#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include "server.h"

void set_ids(struct server_param *server_param){
    time(&server_param->start_time);
    server_param->pid = getpid();
    server_param->uid = getuid();
    server_param->gid = getgid();
}

struct server_param* shared_memory_param(){
    errno = 0;
    int mem_id = shmget(IPC_PRIVATE, sizeof(struct server_param), IPC_CREAT | 0644);
    if (mem_id < 0) {
        fprintf(stderr, "Error: shmget.\n");
        exit(1);
    }

    struct server_param *server_param = (struct server_param *) shmat(mem_id, NULL, 0);
    if (server_param == NULL) {
        fprintf(stderr, "Error: shmat.\n");
        exit(1);
    }

    if (EACCES == errno) {
        fprintf(stderr, "Permission denied\n");
        exit(1);
    }

    set_ids(server_param);

    printf("Сервер запущен.\npid = %ld, uid = %ld, gid = %ld\n", server_param->pid, server_param->uid,
           server_param->gid);
    printf("Используются разделяемый сегмент, mem_id = %d\n", mem_id);
    return server_param;
}

struct server_param* message_queue_param(int *mem_id) {
    errno = 0;
    *mem_id = msgget(IPC_PRIVATE, IPC_CREAT | 0644);
    if (errno) {
        fprintf(stderr, "Невозможно создать очередь сообщений.\n");
        exit(1);
    }

    struct server_param* server_param = malloc(sizeof(struct server_param));

    set_ids(server_param);

    printf("Сервер запущен.\npid = %ld, uid = %ld, gid = %ld\n", server_param->pid, server_param->uid,
           server_param->gid);
    printf("Используются очередь сообщений, mem_id = %d\n", *mem_id);
    return server_param;
}

void start_server(int argc, char *argv[]) {
    flag = parse_flag(argc, argv);
    int mem_id = 0;
    struct server_param *server_param;
    if (flag & SHARED_MEMORY) {
        server_param = shared_memory_param();
    } else if (flag & MESSAGE_QUEUE) {
      server_param = message_queue_param(&mem_id);
    } else if (flag & MMAP_FILE) {
        //MMAP_FILE
    }
    while (true) {
        sleep(1);
        set_param(server_param);
        if (flag & MESSAGE_QUEUE) {
            struct msgbuf msg = {

            };
            errno = 0;
            msgrcv(mem_id, &msg, 0, 1, 0); // mstype == 1 => type query
            if (errno) {
                fprintf(stderr, "Невозможно создать сообщение.\n");
                exit(1);
            }
            msg.mtype = 2; // reply
            memcpy(msg.mtext, server_param, sizeof(struct server_param));
            msgsnd(mem_id, &msg, sizeof(struct server_param), 0);
        }
        printf("work_time = %ld, 1min = %.2f, 5min = %.2f, 15min = %.2f\n", server_param->work_time, server_param->loadavg[0],
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
    while ((opt = getopt(argc, argv, "sqf")) != -1) {
        switch (opt) {
            case 's':
                flag |= SHARED_MEMORY;
                if (flag ^ SHARED_MEMORY) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                printf("Клиент-серверное взаимодействие осуществляется при помощи разделяемых сегментов памяти.\n");
                break;
            case 'q':
                flag |= MESSAGE_QUEUE;
                if (flag ^ MESSAGE_QUEUE) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                printf("Клиент-серверное взаимодействие осуществляется при помощи System V message queue.\n");
                break;
            case 'f':
                flag |= MMAP_FILE;
                if (flag ^ MMAP_FILE) {
                    fprintf(stderr, "Встречено несколько флагов.\n");
                    exit(1);
                }
                printf("Клиент-серверное взаимодействие осуществляется при помощи файла, отображённого в память с использованием mmap.\n");
                break;
            default:
                fprintf(stderr, "Используйте: \n\t./server -s|-q|-f.\n");
                exit(1);
        }
    }
    if (!flag) {
        fprintf(stderr,
                "Не указан способ передачи сообщения между клиентом и сервером.\nИспользуйте: \n\t./server -s|-q|-f.\n");
        exit(1);
    }
    return flag;
}