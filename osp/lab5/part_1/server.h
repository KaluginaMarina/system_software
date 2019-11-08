#ifndef PART_1_SERVER_SERVER_H
#define PART_1_SERVER_SERVER_H

#include <bits/types/time_t.h>

/**
 * Флаги обмена клиента с сервером
 * SHARED_MEMORY -- с использованием разделяемого сегмента памяти
 * MESSAGE_QUEUE -- с использованием System V message queue
 * MMAP_FILE -- с использованием файла, отображённого в память mmap'ом
 */
#define SHARED_MEMORY 1
#define MESSAGE_QUEUE 2
#define MMAP_FILE 4

/**
 * права для обращения к файлам и сегментам памяти
 */
#define PERM 0644

/**
 * флаг
 */
unsigned int flag;

/**
 * хранение параметров сервера
 */
struct server_param {
    time_t start_time; // начала работы сервера
    time_t work_time; // время работы системы в секундах
    long int pid; // id процесса
    long int uid; // id пользователя
    long int gid;  // id группы процесса
    double loadavg[3]; // загрузка системы за последние 1, 5, 15 минут
};

struct msgbuf {
    long mtype;
    char mtext[sizeof(struct server_param)];
};

/**
 * Функция для запуска сервера
 * Высчитывание значений процесса
 * @param args - аргументы командной строки
 * @param argv - аргументы командной строки
 */
void start_server(int argc, char *argv[]);

/**
 * Функция установки средней загрузки системы за последние 1, 5, 15 минут
 * и количества секунд, прошедших с момента запуска процесса
 */
void set_param(struct server_param *server_param);

/*
 * Функция для получения ключей из командной строки
 * @param argc - аргументы командной строки
 * @param argv - аргументы командной строки
 * @param filename - указатель на запись строки с ключом -f
 * @return -- флаги ключей
 */
unsigned int parse_flag(int argc, char *argv[], char* filename);

/**
 * Функция для хранения требуемой информации и обмена информацией между клиентом и сервером
 * при помощи разделяемого сегмента памяти
 * @return указатель на структуру
 */
struct server_param* shared_memory_param();

/**
 * Функция для хранения требуемой информации и обмена информацией между клиентом и сервером
 * при помощи System V message queue
 * @return указатель на структуру
 */
struct server_param* message_queue_param();

/**
 * Функция для обмена информации при помощи файла, отображённого в память с использованием mmap
 * @param filename -- имя файла
 * @return указатель на структуру
 */
struct server_param* mmap_file(char* filename);

/**
 * Функция устанавливает параметры pid, gid, uid и start_time
 * @param server_param -- указатель на структуру server_param
 */
void set_ids(struct server_param *server_param);
#endif //PART_1_SERVER_SERVER_H
