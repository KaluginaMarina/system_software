#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/stat.h>
#include "cat.h"

int N = -1;
int I = 0;

unsigned int parse_flags(int argc, char *argv[]) {
    unsigned int flags = 0, opt = 0;
    while ((opt = getopt(argc, argv, "nhldbe")) != -1) {
        switch (opt) {
            case 'n':
                flags |= FLAG_NUMBERS;
                if (flags & FLAG_NUMBER_B) {
                    fprintf(stderr, "Ключи n и b могут использоваться только по отдельности.\n");
                    exit(EXIT_FAILURE_KEYS);
                }
                break;
            case 'b':
                flags |= FLAG_NUMBER_B;
                if (flags & FLAG_NUMBERS) {
                    fprintf(stderr, "Ключи n и b могут использоваться только по отдельности.\n");
                    exit(EXIT_FAILURE_KEYS);
                }
                break;
            case 'l':
                flags |= FLAG_TAIL;
                char *p;
                if (optind == argc) {
                    N = 10;
                } else {
                    N = (int) strtol(argv[optind], &p, 10);
                    if (errno != 0 || *p != '\0' || N > INT_MAX) {
                        N = 10;
                    } else {
                        optind++;
                    }
                }
                if (flags & FLAG_HEAD) {
                    fprintf(stderr, "Ключи d и l могут использоваться только по отдельности.\n");
                    exit(EXIT_FAILURE_KEYS);
                }
                break;
            case 'd':
                flags |= FLAG_HEAD;
                if (optind == argc) {
                    N = 10;
                } else {
                    N = (int) strtol(argv[optind], &p, 10);
                    if (errno != 0 || *p != '\0' || N > INT_MAX) {
                        N = 10;
                    } else {
                        optind++;
                    }
                }
                if (flags & FLAG_TAIL) {
                    fprintf(stderr, "Ключи d и t могут использоваться только по отдельности.\n");
                    exit(EXIT_FAILURE_KEYS);
                }
                break;
            case 'h':
                flags |= FLAG_HELP;
                break;
            case 'e':
                flags |= FLAG_END_OF_LINE;
                break;
            default:
                fprintf(stderr, "Неверное значение ключей:\n Cat [-n|b] [-e] [-l|d COUNT] [-h] [FILE ...]\n");
                exit(EXIT_FAILURE_KEYS);
        }
    }
    if (optind == argc) {
        flags |= FLAG_STDIN;
    }
    return flags;
}

void cat_stdin(unsigned int flags) {
    int i = 0;
    while (i != N) {
        char str[1024];
        if (scanf("%s", str) == EOF) {
            exit(EXIT_SUCCESS);
        }
        if (flags & FLAG_NUMBERS || flags & FLAG_NUMBER_B) {
            printf("%d ", ++I);
        }
        printf("%s", str);
        printf("%s", flags & FLAG_END_OF_LINE ? "$\n" : "\n");
        i++;
    }
}

void cat(unsigned int flags, char *filename) {
    if (flags & FLAG_HELP) {
        print_help();
    }
    if (flags & FLAG_STDIN) {
        cat_stdin(flags);
        return;
    }

    int sz;
    char *buffer = read_file(filename, &sz);
    if (buffer != NULL) {
    	write_console(buffer, sz, flags);
    }

}

char *read_file(char *filename, int *sz) {
    int file;
    errno = 0;
    file = open(filename, O_RDONLY);
    if (errno == EACCES) {
        fprintf(stderr, "Недостаточно прав для открытия файла.\n");
    	return NULL;
    } else if (errno == ENOENT) {
        fprintf(stderr, "Файл не существует.\n");
    	return NULL;
    } else if (errno) {
        fprintf(stderr, "Невозможно открыть файл.\n");
	return NULL;
    }

    *sz = size(file);
    char *buffer = (char *) malloc(*sz * sizeof(char));

    if (read(file, buffer, *sz) != *sz) {
        fprintf(stderr, "Ошибка чтения файла.\n");
        exit(EXIT_FAILURE_FILE);
    }
    close(file);
    return buffer;
}

void write_console(char *buffer, unsigned int sz, unsigned int flags) {
    int i = 0;
    int i1 = 0;
    char *cur = buffer, *end = sz + buffer;
    if (flags & FLAG_TAIL) {
        cur = end;
        while (i1 != N && cur-- != buffer){
            if (*cur == '\n') {
                i1++;
            }
        }
    }
    if (flags & FLAG_NUMBERS || ((flags & FLAG_NUMBER_B) && *cur != '\n')) {
        printf("%d ", ++I);
        i++;
    }
    while (cur < end) {
        if (*cur == '\n' && (flags & FLAG_END_OF_LINE)) {
            printf("$");
        }
        printf("%c", *cur);
        if (flags & FLAG_HEAD && *cur == '\n'){
            i1++;
            if (i1 == N){
                break;
            }
        }
        if (*cur == '\n' && end >= ++cur && (((flags & FLAG_NUMBERS) && (*cur != '\0')) ||
                                             (((flags & FLAG_NUMBER_B) && *cur != '\n' && *cur != '\0') || !cur--))) {
            printf("%d ", ++I);
            i++;
        } else {
            cur++;
        }
    }
}

unsigned int size(int file) {
    unsigned int sz = 0;
    struct stat buf;
    fstat(file, &buf);
    sz = buf.st_size;
    return sz;
}

void print_help() {
    printf("=================Название=====================\n"
           "Cat - Выводит файлы в стандартный поток вывода\n"
           "=================Синтаксис====================\n"
           "Cat [-n|b] [-e] [-l|d COUNT] [-h] [FILE ...]\n"
           "=================Ключи========================\n"
           "-n -- выводит файлы с номерами всех строк\n"
           "-b -- выводит файлы с номерами строк только для непустых строк\n"
           "-e -- выводит на концах строк $\n"
           "-d [COUNT] -- выводит первые COUNT строк. Без указания COUNT будет выводить 10 строк\n"
           "-l [COUNT] -- выводит последние COUNT строк. Без указания COUNT будет выводить 10 строк\n"
           "-h -- справка\n"
           "==============================================\n"
           "Cat без указания файлов выводит поток из stdin.\n"
           "Ключи -n и -b не используются вместе\n"
           "Ключи -d и -l не используются вместе\n"
           "==============================================\n\n");
}

