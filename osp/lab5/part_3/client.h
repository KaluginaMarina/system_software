#ifndef PART_3_CLIENT_H
#define PART_3_CLIENT_H

unsigned int flag;

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
 * Функция для получения параметров сервера
 * @param argc - аргументы комендной строки
 * @param argv - аргменты командной строки
 */
void get_param(int argc, char* argv[]);

/**
 * Функция для получения командной строки
 * @param argc - аргументы командной строки
 * @param argv - аргументы командной строки
 * @param id - для SHARED_MEMORY и MESSAGE_QUEUE записывает в mem_id значение участка памяти
 * @param filename - для MMAP_FILE записывает в filename имя файла
 * @return - флаги клчюей
 */
unsigned int parse_flag_cl(int argc, char* argv[]);

/**
 * Функция проверки errno
 * @param strerr - строка, выводящаяся в случае ошибки
 */
void check_errno(char *strerr);
#endif //PART_3_CLIENT_H
