#ifndef PART_1_CLIENT_H
#define PART_1_CLIENT_H

unsigned int flag;

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
unsigned int parse_flag_cl(int argc, char* argv[], int* mem_id, char* filename);
#endif //PART_1_CLIENT_H
