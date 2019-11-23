#include <stdio.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/msg.h>
//#include <sys/loadavg.h>
#include <sys/ipc.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <signal.h>
#include <sys/file.h>
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

int mem_id;
struct server_param *shared_memory_param() {
    errno = 0;
    server_signal();
    mem_id = shmget(IPC_PRIVATE, sizeof(struct server_param), IPC_CREAT | PERM);
    if (mem_id < 0) {
        fprintf(stderr, "Error: shmget.\n");
        exit(1);
    }

    struct server_param *server_param = (struct server_param *) shmat(mem_id, NULL, 0);
    if (server_param == NULL) {
        fprintf(stderr, "Error: shmat.\n");
        exit(1);
    }

    check_errno("Невозможно создать разделяемый сегмент памяти");

    set_ids(server_param);

    printf("Сервер запущен.\npid = %ld, uid = %ld, gid = %ld\n", server_param->pid, server_param->uid,
           server_param->gid);
    printf("Используются разделяемый сегмент, mem_id = %d\n", mem_id);
    return server_param;
}

struct server_param *message_queue_param(int *mem_id_q) {
    errno = 0;
    *mem_id_q = msgget(IPC_PRIVATE, IPC_CREAT | PERM);

    check_errno("Невозможно создать очередь сообщений");

    struct server_param *server_param = malloc(sizeof(struct server_param));

    set_ids(server_param);

    printf("Сервер запущен.\npid = %ld, uid = %ld, gid = %ld\n", server_param->pid, server_param->uid,
           server_param->gid);
    printf("Используются очередь сообщений, mem_id = %d\n", *mem_id_q);
    return server_param;
}

int file;
struct server_param *mmap_file(char *filename) {
    errno = 0;
    file = open(filename, O_CREAT | O_RDWR, PERM);
    server_signal();
    check_errno("Невозможно создать/открыть файл");
    flock(file, LOCK_EX|LOCK_NB);
    ftruncate(file, sizeof(struct server_param));
    check_errno("Невозможно открыть файл");


    struct server_param *server_param = (struct server_param *) mmap(NULL, sizeof(struct server_param), PROT_WRITE,
                                                                     MAP_SHARED, file, 0);
    check_errno("Невозможно отобразить файл");

    set_ids(server_param);
    printf("Сервер запущен.\npid = %ld, uid = %ld, gid = %ld\n", server_param->pid, server_param->uid,
           server_param->gid);
    printf("Используются файл, filename = \"%s\"\n", filename);
    return server_param;
}

void start_server(int argc, char *argv[]) {
    char* filename = malloc(256 * sizeof(char));
    flag = parse_flag(argc, argv, filename);
    int mem_id = 0;
    struct server_param *server_param;
    if (flag & SHARED_MEMORY) {
        server_param = shared_memory_param();
    } else if (flag & MESSAGE_QUEUE) {
        server_param = message_queue_param(&mem_id);
    } else if (flag & MMAP_FILE) {
        server_param = mmap_file(filename);
    }
    while (true) {
        sleep(1);
        set_param(server_param);
        if (flag & MESSAGE_QUEUE) {
            struct msgbuff msg;
            errno = 0;
            msgrcv(mem_id, &msg, 0, MSGTYPE_QUERY, 0);
            check_errno("Невозможно создать сообщение");

            msg.mtype = MSGTYPE_REPLY;
            memcpy(msg.mtext, server_param, sizeof(struct server_param));
            msgsnd(mem_id, &msg, sizeof(struct server_param), 0);
            check_errno("Невозможно записать сообщение");
        }
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

unsigned int parse_flag(int argc, char *argv[], char* filename) {
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
                if (optind == argc) {
                    fprintf(stderr, "Не указано имя файла.\n");
                    exit(1);
                }
                strcpy(filename, argv[optind]);
                optind++;
                printf("Клиент-серверное взаимодействие осуществляется при помощи файла, отображённого в память с использованием mmap.\n");
                break;
            default:
                fprintf(stderr, "Используйте: \n\t./server -s|-q|-f filename.\n");
                exit(1);
        }
    }
    if (!flag) {
        fprintf(stderr,
                "Не указан способ передачи сообщения между клиентом и сервером.\nИспользуйте: \n\t./server -s|-q|-f filename.\n");
        exit(1);
    }
    return flag;
}


void *die() {
    printf("Уничтожено.\n");
    shmdt((const void *) mem_id);
    flock(file, LOCK_UN);
    exit(0);
}

void set_signal(int sig, void *func) {
    struct sigaction action;
    sigset_t *mask = (sigset_t*) malloc(sizeof(sigset_t));
    action.sa_handler = func;
    action.sa_flags = 0;
    action.sa_mask = *mask;
    sigemptyset(mask);
    sigaction(sig, &action, NULL);
    check_errno("Не удалось создать segaction");
}

void server_signal(){
    errno = 0;
    set_signal(SIGINT, die);
}
