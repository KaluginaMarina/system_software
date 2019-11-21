#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <zconf.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "threads.h"

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

void print_array() {
    for (int i = 0; i < SIZE; ++i) {
        printf("%c ", array[i]);
    }
    printf("\n");
}

unsigned long time_threads = 1000000;
void parse_flag(int argc, char *argv[]) {
    unsigned int opt = 0;
    if (argc == 1) {
        fprintf(stderr,
                "Не указан номер подзадания.\nИспользуйте: \n\t./threads -1|-2|-3 [TIME]|-4.\n");
        exit(1);
    }
    while ((opt = getopt(argc, argv, "1234")) != -1) {
        switch (opt) {
            case '1':
                if (argc > 2) {
                    fprintf(stderr, "Встречено несколько флагов. Используйте: ./threads -1|-2|-3 [TIME]|-4\n");
                    exit(1);
                }
                printf("Первое подзадание.\n");
                first_task();
                break;
            case '2':
                if (argc > 2) {
                    fprintf(stderr, "Встречено несколько флагов. Используйте: ./threads -1|-2|-3 [TIME]|-4\n");
                    exit(1);
                }
                printf("Второе подзадание.\n");
                second_task();
                break;
            case '3':
                printf("Третье подзадание.\n");
                if (argc == optind) {
                    printf("Не указано количество микросекунд.\nЗадано дефолтное значение TIME = %ld\n", time_threads);
                } else {
                    char *p;
                    errno = 0;
                    time_threads = (unsigned long) strtol(argv[optind], &p, 10);
                    if (errno || *p != '\0' || time < 0) {
                        fprintf(stderr, "Встречен неверный формат. Используйте: ./threads -1|-2|-3 [TIME]|-4\n");
                        exit(1);
                    }
                }
                third_task();
                break;
            case '4':
                if (argc > 2) {
                    fprintf(stderr, "Встречено несколько флагов. Используйте: ./threads -1|-2|-3 [TIME]|-4\n");
                    exit(1);
                }
                printf("Четвертое подзадание:\n");
                break;
            default:
                fprintf(stderr, "Неверный ключ. Используйте: \n\t./threads -1|-2|-3 [TIME]|-4.\n");
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
    errno = 0;
    pthread_t thread1, thread2;
    sem_init(&sem, 0, 1);
    check_errno("Невозможно выполнить sem_init");

    pthread_create(&thread1, NULL, task1_thread1, NULL);
    pthread_create(&thread2, NULL, task1_thread2, NULL);
    check_errno("Не удалось создать поток");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
}

int sem_id;
struct sembuf *sembuf;

void second_task() {
    errno = 0;
    pthread_t thread1, thread2;
    sembuf = malloc(sizeof(struct sembuf));
    sembuf->sem_flg = 0;
    sembuf->sem_num = 0;
    sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | PERM);
    check_errno("Невозможно выполнить semget");
    semctl(sem_id, 0, SETVAL, 1);

    pthread_create(&thread1, NULL, task2_thread1, NULL);
    pthread_create(&thread2, NULL, task2_thread2, NULL);
    check_errno("Невозможно создать поток");

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    semctl(sem_id, 0, IPC_CREAT);
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void third_task(){
    errno = 0;
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);
    check_errno("Невозможно инициаллизироавть mutex");

    pthread_create(&thread1, NULL, task3_thread1, NULL);
    pthread_create(&thread2, NULL, task3_thread2, NULL);
    check_errno("невозможно создать поток");

    while (true) {
        pthread_mutex_lock(&mutex);
        check_errno("Невозможно выполнить pthread_mutex_lock");
        printf("что-то написано\n");
        usleep(time_threads);
        pthread_mutex_unlock(&mutex);
        check_errno("Невозможно выполнить pthread_mutex_lock");
        usleep(time_threads);
    }
}

void change_reg() {
    for (int i = 0; i < SIZE; ++i) {
        if (array[i] >= 'A' && array[i] <= 'Z') {
            array[i] = array[i] - 'A' + 'a';
        } else {
            array[i] = array[i] - 'a' + 'A';
        }
    }
    print_array();
}

void reverse() {
    for (int i = 0; i < SIZE / 2; ++i) {
        char tmp = array[i];
        array[i] = array[SIZE - i - 1];
        array[SIZE - i - 1] = tmp;
    }
    print_array();
}

void *task1_thread1() {
    while (true) {
        errno = 0;
        sem_wait(&sem);
        check_errno("Невозможно заблокировать ресурс (in sem_wait)");
        change_reg();
        sleep(1);
        sem_post(&sem);
        check_errno("Невозможно разблокировать ресурс (in sem_post)");
        sleep(1);
    }
}

void *task1_thread2() {
    while (true) {
        sem_wait(&sem);
        check_errno("Невозможно заблокировать ресурс (in sem_wait)");
        reverse();
        sleep(1);
        sem_post(&sem);
        check_errno("Невозможно заблокировать ресурс (in sem_post)");
        sleep(1);
    }
}

void *task2_thread1() {
    while (true) {
        errno = 0;
        sembuf->sem_op = -1;
        semop(sem_id, sembuf, 1);
        check_errno("Невозможно заблокировать ресурс (in sembuf)");
        change_reg();
        sleep(1);
        sembuf->sem_op = 1;
        semop(sem_id, sembuf, 1);
        check_errno("Невозможно разблокировать ресурс (in sembuf)");
    }
}

void *task2_thread2() {
    while (true) {
        errno = 0;
        sembuf->sem_op = -1;
        semop(sem_id, sembuf, 1);
        check_errno("Невозможно заблокировать ресурс (in semop)");
        reverse();
        sleep(1);
        sembuf->sem_op = 1;
        semop(sem_id, sembuf, 1);
        check_errno("Невозможно разблокировать ресурс (in semop)");
    }
}

void *task3_thread1() {
    while (true) {
        usleep(time_threads);
        pthread_mutex_lock(&mutex);
        check_errno("Невозможно заблокировать ресурс (в функции pthread_mutex_lock)");
        change_reg();
        usleep(time_threads);
        pthread_mutex_unlock(&mutex);
        check_errno("Невозможно разблокировать ресурс (в функции pthread_mutex_unlock)");
        usleep(time_threads);
    }
}

void *task3_thread2() {
    while (true) {
        usleep(time_threads);
        pthread_mutex_lock(&mutex);
        check_errno("Невозможно заблокировать ресурс (pthread_mutex_lock)");
        reverse();
        usleep(time_threads);
        pthread_mutex_unlock(&mutex);
        check_errno("Невозможно заблокировать ресурс (pthread_mutex_lock)");
        usleep(time_threads);
    }
}