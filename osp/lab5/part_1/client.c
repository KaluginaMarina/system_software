#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "server.h"
#include "client.h"


void get_param(int argc, char* argv[]) {
    int mem_id;
    char* filename = malloc(256 * sizeof(char));
    flag = parse_flag_cl(argc, argv, &mem_id, filename);

    if (flag & SHARED_MEMORY) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи разделяемых сегментов памяти.\n");
        printf("mem_id = %d\n", mem_id);
    } else if (flag & MESSAGE_QUEUE) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи System V message queue.\n");
        printf("mem_id = %d\n", mem_id);
    } else if (flag & MMAP_FILE) {
        printf("Клиент-серверное взаимодействие осуществляется при помощи файла, отображённого в память с использованием mmap.\n");
        printf("filename = %s", filename);
    }
}

unsigned int parse_flag_cl(int argc, char* argv[], int* mem_id, char* filename) {
    unsigned int flag = 0, opt = 0;
    char* p;
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
                if (errno != 0 || *mem_id <= 0) {
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
                if (errno != 0 || *mem_id <= 0) {
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