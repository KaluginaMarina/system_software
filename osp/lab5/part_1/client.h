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

/**
 * Функция для получения требуемой информации и обмена информацией между клиентом и сервером
 * при помощи разделяемого сегмента памяти
 * @param mem_id -- значение сегмента памяти
 */
void get_param_shared_memory(int mem_id);

/**
 * Функция для получения информации от сервера с использованием файла, отображенного mmap
 * @param filename -- имя файла
 */
void get_param_mmap_file(char* filename);

/**
 * Функция для получения информации от сервера с использованием message queue
 * @param mem_id -- значение очереди
 */
void get_param_message_queue_param(int mem_id);
#endif //PART_1_CLIENT_H
