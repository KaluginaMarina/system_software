#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "threads.h"

void print_array() {
    for (int i = 0; i < SIZE; ++i) {
        printf("%c ", array[i]);
    }
}

void parse_flag(int argc, char *argv[]) {
    unsigned int opt = 0;
    if (argc > 2) {
        fprintf(stderr, "Встречено несколько флагов. Используйте: ./threads -1|-2|-3|-4\n");
        exit(1);
    }
    if (argc == 1) {
        fprintf(stderr,
                "Не указан номер подзадания.\nИспользуйте: \n\t./threads -1|-2|-3|-4.\n");
        exit(1);
    }
    while ((opt = getopt(argc, argv, "1234")) != -1) {
        switch (opt) {
            case '1':
                printf("Первое подзадание.\n");
                break;
            case '2':
                printf("Второе подзадание.\n");
                break;
            case '3':
                printf("Третье подзадание.\n");
                break;
            case '4':
                printf("Четвертое подзадание:\n");
                break;
            default:
                fprintf(stderr, "Неверный ключ. Используйте: \n\t./threads -1|-2|-3|-4.\n");
                exit(1);
        }
    }
}

void start(int argc, char *argv[]) {
    for (int i = 0; i < SIZE; ++i) {
        array[i] = i + 'a';
    }
    parse_flag(argc, argv);
}