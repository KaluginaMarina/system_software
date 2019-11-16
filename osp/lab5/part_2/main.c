#include <stdio.h>

#define SIZE 26

char array[SIZE];

void print_array() {
    for (int i = 0; i < SIZE; ++i) {
        printf("%c\n", array[i]);
    }
}

int main() {
    for (int i = 0; i < SIZE; ++i) {
        array[i] = i + 'a';
    }

    print_array();
    return 0;
}