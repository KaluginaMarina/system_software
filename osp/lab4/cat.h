#ifndef LAB4_CAT_H
#define LAB4_CAT_H

#define NO_FLAGS 0
#define FLAG_NUMBERS 1
#define FLAG_TAIL 2
#define FLAG_HEAD 4
#define FLAG_HELP 8

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


/**
 * Функция для получения ключей из командной строки
 * @param args - аргументы командной строки
 * @param argv - аргументы командной строки
 * @return - маску флагов типа int
 */
unsigned int parse_flags(int args, char *argv[]);

#endif //LAB4_CAT_H
