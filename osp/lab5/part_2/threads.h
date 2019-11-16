#ifndef PART_2_THREADS_H
#define PART_2_THREADS_H

#define SIZE 26

/**
 * Данный массив
 */
char array[SIZE];

/**
 * вывод массива в stdin
 */
void print_array();

/**
 * Функция для разбора ключей
 * @param argc - аргументы командной строки
 * @param argv - аргументы командной строки
 * @return
 */
void parse_flag(int argc, char *argv[]);

/**
 * Функция для запуска: заполняет массив, вызывает функцию для начала работы
 * @param argc - аргументы командной строки
 * @param argv - аргументы командной строки
 */
void start(int argc, char *argv[]);

#endif //PART_2_THREADS_H
