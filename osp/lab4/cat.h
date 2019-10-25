#ifndef LAB4_CAT_H
#define LAB4_CAT_H

/**
 * Флаги для обрабоки команд:
 * NO_FLAGS -- без флагов
 * FLAG_NUMBERS -- вывод номеров строк на каждой строке
 * FLAG_NUMBER_B -- вывод номеров строк, пропуская пустые строки
 * FLAG_TAIL -- вывод последних N строк (дефолтное значение N = 10)
 * FLAG_HEAD -- вывод первых N строк (дефолтное значение N = 10)
 * FLAG_HELP -- вывод справки по Cat
 * FLAG_STDIN -- исполнение без файла, работа с stdin
 * FLAG_END_OF_LINE -- вставлять символ $ в конце каждой строки
 */
#define FLAG_NUMBERS 1
#define FLAG_NUMBER_B 2
#define FLAG_TAIL 4
#define FLAG_HEAD 8
#define FLAG_HELP 16
#define FLAG_STDIN 32
#define FLAG_END_OF_LINE 64

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
 * @param sz - указатель на переменную, в которую записывается размер файла в байтах
 * @return указатель на буфер с результатом чтения с файла
 */
char *read_file(char *filename, int *sz);

/**
 * Функция вывода в stdin из буфера
 * @param buffer - буфер, из которого производится вывод текста
 * @param sz - размер буфера в байтах
 * @param flags - флаги ключей
 */
void write_console(char *buffer, unsigned int sz, unsigned int flags);
#endif //LAB4_CAT_H
