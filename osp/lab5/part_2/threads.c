#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <zconf.h>
#include <semaphore.h>
#include "threads.h"

void print_array() {
    for (int i = 0; i < SIZE; ++i) {
        printf("%c ", array[i]);
    }
    printf("\n");
}

void parse_flag(int argc, char *argv[]) {
    unsigned int opt = 0;
    if (argc > 2) {
        fprintf(stderr, "Встречено несколько флагов. Используйте: ./threads -1|-2|-3|-4\n");
        exit(1);
    }
    if (argc == 1) {
        fprintf(stderr,
                "Не указан номер подзадания.\nИспользуйте: \n\t./threads -1|-2|-3|-4.\n");
        exit(1);
    }
    while ((opt = getopt(argc, argv, "1234")) != -1) {
        switch (opt) {
            case '1':
                printf("Первое подзадание.\n");
                first_task();
                break;
            case '2':
                printf("Второе подзадание.\n");
                break;
            case '3':
                printf("Третье подзадание.\n");
                break;
            case '4':
                printf("Четвертое подзадание:\n");
                break;
            default:
                fprintf(stderr, "Неверный ключ. Используйте: \n\t./threads -1|-2|-3|-4.\n");
                exit(1);
        }
    }
}

void start(int argc, char *argv[]) {
    for (int i = 0; i < SIZE; ++i) {
        array[i] = i + 'a';
    }
    parse_flag(argc, argv);
}

sem_t sem;
void first_task() {
    pthread_t thread1;
    pthread_t thread2;
    errno = 0;
    sem_init(&sem, 0, 1);
    if (errno) {
        fprintf(stderr, "Невозможно сделать sem_init. Ошибка: %s\n", strerror(errno));
        exit(1);
    }
    pthread_create(&thread1, NULL, task1_thread1, NULL);
    pthread_create(&thread2, NULL, task1_thread2, NULL);
    if (errno) {
        fprintf(stderr, "Не удалось создать потоки. Ошибка: %s\n", strerror(errno));
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}

void change_reg(){
    for (int i = 0; i < SIZE; ++i) {
        if (array[i] >= 'A' && array[i] <= 'Z') {
            array[i] = array[i] - 'A' + 'a';
        } else {
            array[i] = array[i] - 'a' + 'A';
        }
    }
    print_array();
}

void reverse(){
    for (int i = 0; i < SIZE / 2; ++i) {
        char tmp = array[i];
        array[i] = array[SIZE - i - 1];
        array[SIZE - i - 1] = tmp;
    }
    print_array();
}

void *task1_thread1() {
    while (true) {
        usleep(500000);
        errno = 0;
        sem_wait(&sem);
        if (errno) {
            fprintf(stderr, "Невозможно выполнить sem_wait. Ошибка: %s", strerror(errno));
            exit(1);
        }
        usleep(500000);
        change_reg();
        usleep(500000);
        sem_post(&sem);
        if (errno) {
            fprintf(stderr, "Невозможно выполнить sem_post. Ошибка: %s", strerror(errno));
            exit(1);
        }
        usleep(500000);
    }
}

void *task1_thread2() {
    while (true) {
        usleep(500000);
        sem_wait(&sem);
        if (errno) {
            fprintf(stderr, "Невозможно выполнить sem_wait. Ошибка: %s", strerror(errno));
            exit(1);
        }
        usleep(500000);
        reverse();
        usleep(500000);
        sem_post(&sem);
        if (errno) {
            fprintf(stderr, "Невозможно выполнить sem_post. Ошибка: %s", strerror(errno));
            exit(1);
        }
        usleep(500000);
    }
}
