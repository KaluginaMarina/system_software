#ifndef PART_1_SERVER_SERVER_H
#define PART_1_SERVER_SERVER_H

#include <bits/types/time_t.h>

time_t  start_time; // начала работы сервера
time_t work_time; // время работы системы в секундах
long int pid; // id процесса
long int uid; // id пользователя
long int gid;  // id группы процесса
double loadavg[3]; // загрузка системы за последние 1, 5, 15 минут

/**
 * Функция для запуска сервера
 * Высчитывание значений процесса
 */
void start_server();

/**
 * Функция установки средней загрузки системы за последние 1, 5, 15 минут
 * и количества секунд, прошедших с момента запуска процесса
 */
void set_param();
#endif //PART_1_SERVER_SERVER_H
