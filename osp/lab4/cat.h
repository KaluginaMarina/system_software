#ifndef LAB4_CAT_H
#define LAB4_CAT_H

/**
 * Флаги для обрабоки команд:
 * NO_FLAGS -- без флагов
 * FLAG_NUMBERS -- вывод номеров строк на каждой строке
 * FLAG_TAIL -- вывод последних N строк (дефолтное значение N = 10)
 * FLAG_HEAD -- вывод первых N строк (дефолтное значение N = 10)
 * FLAG_HELP -- вывод справки по Cat
 * FLAG_STDIN -- исполнение без файла, работа с stdin
 */
#define NO_FLAGS 0
#define FLAG_NUMBERS 1
#define FLAG_TAIL 2
#define FLAG_HEAD 4
#define FLAG_HELP 8
#define FLAG_STDIN 16

/**
 * Коды ошибок
 * EXIT_SUCCESS -- успешное завершение программы
 * EXIT_FAILURE_KEYS -- ошибка ввода ключа
 * EXIT_FAILURE_FILE -- ошибка работы с файлом
 */
#define EXIT_SUCCESS 0
#define EXIT_FAILURE_KEYS 1
#define EXIT_FAILURE_FILE 2


/**
 * Функция для получения ключей из командной строки
 * @param args - аргументы командной строки
 * @param argv - аргументы командной строки
 * @return - маску флагов типа int
 */
unsigned int parse_flags(int args, char *argv[]);

/**
 * Функция, осуществляющая утилиту cat
 * @param flags - флаги ключей
 * @param filename - имя файла
 */
void cat(unsigned int flags, char *filename);

/**
 * Функция cat без введенного имени файла
 * получает входной поток из stdin
 * @param flags - флаги ключей
 */
void cat_stdin(unsigned int flags);

/**
 * Функция, которая овыводит описание работы утилиты
 */
void print_help();

/**
 * Функция для определения размера файла
 * @param file - дескриптор файла handle
 * @return размер файла в байтах
 */
unsigned int size(int file);

/**
 * Функция, которая производит чтение с файла
 * @param filename - имя файла
 * @param buffer - буфер для записи результата чтения
 * @return количество прочитанных байт
 */
unsigned int read_file(char *filename, char *buffer);

/**
 * Функция вывода в stdin из буфера
 * @param buffer - буфер, из которого производится вывод текста
 * @param sz - размер буфера в байтах
 * @param flags - флаги ключей
 */
void write_console(char *buffer, unsigned int sz, unsigned int flags);
#endif //LAB4_CAT_H
