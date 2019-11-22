#ifndef PART_3_SERVER_H
#define PART_3_SERVER_H

#include <time.h>

/**
 * флаги заданий
 * FLAG_SOCKET -- клиент-серверное взаимодействие при помощи сокетов
 * FLAG_SIGNAL -- клиент-серверное взаимодействие при помощи сигналов
 * FLAG_PIPE -- клиент-серверное приложение при помощи неименованных каналов
 */
#define FLAG_SOCKET 1
#define FLAG_SIGNAL 2
#define FLAG_PIPE 4

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

/**
 * Функция устанавливает параметры pid, gid, uid и start_time
 * @param server_param -- указатель на структуру server_param
 */
void set_ids(struct server_param *server_param);

/**
 * Функция для сервера, использующего  unix domain socket
 */
 void server_signal();

/**
 * Функция проверки errno
 * @param strerr - строка, выводящаяся в случае ошибки
 */
void check_errno(char* strerr);

/*
 * Функция для получения ключей из командной строки
 * @param argc - аргументы командной строки
 * @param argv - аргументы командной строки
 * @param filename - указатель на запись строки с ключом -f
 * @return -- флаги ключей
 */
unsigned int parse_flag(int argc, char *argv[]);

/**
 * actions
 */
void *print_pid();
void *print_uid();
void *print_gid();
void *print_work_time();
void *print_loadavg();
#endif //PART_3_SERVER_H
