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

unsigned int parse_flags(int argc, char *argv[]) {
    unsigned int flags = 0, opt = 0;
    while ((opt = getopt(argc, argv, "nhtd")) != -1) {
        switch (opt) {
            case 'n':
                flags |= FLAG_NUMBERS;
                break;
            case 't':
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
                    fprintf(stderr, "Ключи d и t могут использоваться только по отдельности.\n");
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
            default:
                fprintf(stderr, "Неверное значение ключей:\n Cat [-n] [-t|d COUNT] [-h] [FILE]\n");
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
        scanf("%s", str);
        if (flags & FLAG_NUMBERS) {
            printf("%d ", i);
        }
        printf("%s\n", str);
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

    char *buffer = NULL;
    unsigned int sz = read_file(filename, buffer);
    write_console(buffer, sz, flags);

}

void print_help() {

}

unsigned int read_file(char *filename, char *buffer){
    int file;
    errno = 0;
    file = open(filename, O_RDONLY);
    if (errno == EACCES) {
        fprintf(stderr, "Недостаточно прав для открытия файла.\n");
        exit(EXIT_FAILURE_FILE);
    } else if (errno == ENOENT) {
        fprintf(stderr, "Файл не существует.");
        exit(EXIT_FAILURE_FILE);
    } else if (errno) {
        fprintf(stderr, "Невозможно открыть файл.");
        exit(EXIT_FAILURE_FILE);
    }

    unsigned int sz = size(file);
    buffer = (char *) malloc(sz * sizeof(char));

    if (read(file, buffer, sz) != sz) {
        fprintf(stderr, "Ошибка чтения файла.\n");
        exit(EXIT_FAILURE_FILE);
    }
    return sz;
}

void write_console(char *buffer, unsigned int sz, unsigned int flags){
    
}

unsigned int size(int file){
    unsigned int sz = 0;
    struct stat buf;
    fstat(file, &buf);
    sz = buf.st_size;
    return sz;
}