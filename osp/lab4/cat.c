#include <getopt.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "cat.h"
int N = 1;

unsigned int parse_flags(int argc, char *argv[]){
    unsigned int flags = 0, opt = 0;
    while ((opt = getopt(argc, argv, "nhtdh")) != -1) {
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
                    exit(EXIT_FAILURE);
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
                    exit(EXIT_FAILURE);
                }
                break;
            case 'h':
                flags |= FLAG_HELP;
                break;
            default:
                fprintf(stderr, "Неверное значение ключей:\n Cat [-n] [-t|d COUNT] [-h] [FILE]\n");
                exit(EXIT_FAILURE);
        }
    }
    if (optind == argc) {
        flags |= FLAG_STDIN;
    }
    return flags;
}