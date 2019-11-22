#ifndef PART_3_SERVER_H
#define PART_3_SERVER_H

#include <time.h>

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

/**
 * Вывод информации на экран
 */
void print_param();

void *print_hup();
void *print_term();
void *print_int();
void *print_usr1();
void *print_usr2();
#endif //PART_3_SERVER_H
