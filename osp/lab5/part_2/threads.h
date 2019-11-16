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
 *
 * @param argc
 * @param argv
 * @return
 */
unsigned int parse_flag(int argc, char *argv[]);

#endif //PART_2_THREADS_H
