#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *probel = " ";

void xargs(char *command){
    char *arg = (char*)malloc(256 * sizeof(char));
    while (scanf("%s", arg) != EOF) {
        char *res = (char *)malloc(512 * sizeof(char));
        strcpy(res, command);
        strcat(res, probel);
        strcat(res, arg);
        system(res);
    }
}

int main(int argc, char *argv[]) {
    char *command = (char*)malloc(256 * sizeof(char));
    for (int i = 1; i < argc; ++i){
        strcat(command, probel);
        strcat(command, argv[i]);
    }
    xargs(command);
    return 0;
}