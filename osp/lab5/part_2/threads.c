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

// [главный поток, смена регистра, реверс, счетчик]
unsigned long time_threads[4] = {1000000, 1000000, 1000000, 1000000};
const char *format = "./threads -1\n"
                     "\t./threads -2\n"
                     "\t./threads -3 [TIME | TIME_MAIN TIME1 TIME2]\n"
                     "\t./threads -4 [TIME | TIME_MAIN TIME1 TIME2 TIME3]";

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

void parse_flag(int argc, char *argv[]) {
    unsigned int opt = 0;
    if (argc == 1) {
        fprintf(stderr,
                "Не указан номер подзадания.\nИспользуйте: \n\t.%s.\n", format);
        exit(1);
    }
    while ((opt = getopt(argc, argv, "1234")) != -1) {
        switch (opt) {
            case '1':
                if (argc > 2) {
                    fprintf(stderr, "Встречено несколько флагов. Используйте: \n\t%s\n", format);
                    exit(1);
                }
                printf("Первое подзадание.\n");
                first_task();
                break;
            case '2':
                if (argc > 2) {
                    fprintf(stderr, "Встречено несколько флагов. Используйте: \n\t%s\n", format);
                    exit(1);
                }
                printf("Второе подзадание.\n");
                second_task();
                break;
            case '3':
                errno = 0;
                printf("Третье подзадание.\n");
                if (argc == optind) {
                    printf("Не указано количество микросекунд.\nЗадано дефолтное значение TIME = %ld\n",
                           time_threads[0]);
                } else if (optind + 1 == argc) {
                    char *p;
                    u_long tmp = (unsigned long) strtol(argv[optind], &p, 10);
                    if (errno || *p != '\0' || time < 0) {
                        fprintf(stderr, "Встречен неверный формат. Используйте: \n\t%s\n", format);
                        exit(1);
                    }
                    for (int i = 0; i < 3; ++i) {
                        time_threads[i] = tmp;
                    }
                } else if (optind + 3 == argc) {
                    char *p;
                    for (int i = 0; i < 3; ++i) {
                        time_threads[i] = (unsigned long) strtol(argv[optind++], &p, 10);
                        if (errno || *p != '\0' || time < 0) {
                            fprintf(stderr,
                                    "Встречен неверный формат. Используйте: \n\t%s\n", format);
                            exit(1);
                        }
                    }
                } else {
                    fprintf(stderr,
                            "Встречен неверный формат. Используйте: \n\t%s\n", format);
                    exit(1);
                }
                third_task();
                break;
            case '4':
                printf("Четвертое подзадание:\n");
                if (argc == optind) {
                    printf("Не указано количество микросекунд.\nЗадано дефолтное значение TIME = %ld\n",
                           time_threads[0]);
                } else if (optind + 1 == argc) {
                    char *p;
                    u_long tmp = (unsigned long) strtol(argv[optind], &p, 10);
                    if (errno || *p != '\0' || time < 0) {
                        fprintf(stderr, "Встречен неверный формат. Используйте: \n\t%s\n", format);
                        exit(1);
                    }
                    for (int i = 0; i < 4; ++i) {
                        time_threads[i] = tmp;
                    }
                } else if (optind + 4 == argc) {
                    char *p;
                    for (int i = 0; i < 4; ++i) {
                        time_threads[i] = (unsigned long) strtol(argv[optind++], &p, 10);
                        if (errno || *p != '\0' || time < 0) {
                            fprintf(stderr,
                                    "Встречен неверный формат. Используйте: \n\t%s\n", format);
                            exit(1);
                        }
                    }
                } else {
                    fprintf(stderr,
                            "Встречен неверный формат. Используйте: \n\t.%s\n", format);
                    exit(1);
                }
                forth_task();
                break;
            default:
                fprintf(stderr, "Неверный ключ. Используйте: \n\t%s\n", format);
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

void third_task() {
    errno = 0;
    pthread_t thread1, thread2;
    pthread_mutex_init(&mutex, NULL);
    check_errno("Невозможно инициаллизироавть mutex");

    pthread_create(&thread1, NULL, task3_thread1, NULL);
    pthread_create(&thread2, NULL, task3_thread2, NULL);
    check_errno("Невозможно создать поток");

    while (true) {
        pthread_mutex_lock(&mutex);
        check_errno("Невозможно выполнить pthread_mutex_lock");
        print_array();
        pthread_mutex_unlock(&mutex);
        check_errno("Невозможно выполнить pthread_mutex_lock");
        usleep(time_threads[0]);
    }
}

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void forth_task() {
    errno = 0;
    pthread_t thread1, thread2, thread3;

    pthread_create(&thread1, NULL, task4_thread1, NULL);
    pthread_create(&thread2, NULL, task4_thread2, NULL);
    pthread_create(&thread3, NULL, task4_thread3, NULL);
    check_errno("Невозможно создать поток");

    while (true) {
        errno = 0;
        usleep(time_threads[0]);
        pthread_rwlock_rdlock(&rwlock);
        check_errno("Невозможно заблокировать ресурс главным потоком");
        print_array();
        pthread_rwlock_unlock(&rwlock);
        check_errno("Невозможно разблокировать ресурс в главном потоке");
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
}

void reverse() {
    for (int i = 0; i < SIZE / 2; ++i) {
        char tmp = array[i];
        array[i] = array[SIZE - i - 1];
        array[SIZE - i - 1] = tmp;
    }
}

u_short count_letters() {
    int res = 0;
    for (int i = 0; i < SIZE; ++i) {
        if (array[i] >= 'A' && array[i] <= 'Z') {
            res++;
        }
    }
    return res;
}

void *task1_thread1() {
    while (true) {
        errno = 0;
        sem_wait(&sem);
        check_errno("Невозможно заблокировать ресурс (in sem_wait)");
        change_reg();
        print_array();
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
        print_array();
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
        print_array();
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
        print_array();
        sleep(1);
        sembuf->sem_op = 1;
        semop(sem_id, sembuf, 1);
        check_errno("Невозможно разблокировать ресурс (in semop)");
    }
}

void *task3_thread1() {
    while (true) {
        pthread_mutex_lock(&mutex);
        check_errno("Невозможно заблокировать ресурс (в функции pthread_mutex_lock)");
        change_reg();
        pthread_mutex_unlock(&mutex);
        check_errno("Невозможно разблокировать ресурс (в функции pthread_mutex_unlock)");
        usleep(time_threads[1]);
    }
}

void *task3_thread2() {
    while (true) {
        pthread_mutex_lock(&mutex);
        check_errno("Невозможно заблокировать ресурс (pthread_mutex_lock)");
        reverse();
        pthread_mutex_unlock(&mutex);
        check_errno("Невозможно заблокировать ресурс (pthread_mutex_lock)");
        usleep(time_threads[2]);
    }
}

void *task4_thread1() {
    while (true) {
        usleep(time_threads[1]);
        pthread_rwlock_rdlock(&rwlock);
        check_errno("Невозможно заблокировать 1-й поток");
        change_reg();
        pthread_rwlock_unlock(&rwlock);
        check_errno("Невозможно разблокировать 1-й поток");
    }
}

void *task4_thread2() {
    while (true) {
        usleep(time_threads[2]);
        pthread_rwlock_rdlock(&rwlock);
        check_errno("Невозможно заблокировать 2-й поток");
        reverse();
        pthread_rwlock_unlock(&rwlock);
        check_errno("Невозможно разблокировать 2-й поток");
    }
}

void *task4_thread3() {
    while (true) {
        usleep(time_threads[3]);
        pthread_rwlock_wrlock(&rwlock);
        check_errno("Невозможно заблокировать 3-й поток");
        printf("Количество заглавных символов: %d\n", count_letters());
        pthread_rwlock_unlock(&rwlock);
        check_errno("Невозможно разблокировать 3-й поток");
    }
}