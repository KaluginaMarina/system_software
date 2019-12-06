#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <zconf.h>
#include <netinet/in.h>


void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

unsigned int check_args(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Используйте: ./server port");
        exit(1);
    }
    char *p;
    unsigned int port = (int) strtol(argv[optind], &p, 10);
    if (errno != 0 || port < 0) {
        fprintf(stderr, "Указан неверный порт.\n");
        exit(1);
    }
    return port;
}

void start_server(unsigned int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    check_errno("Невозможно создать сокет");
    // ......sockaddr_in...........
    struct sockaddr_in addr;
    addr.sin_family = AF_INET; addr.sin_port = htons(port);
    struct in_addr sin_addr;
    sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_addr = sin_addr;
    //.............................
    bind(sockfd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    check_errno("Невозможно привязать к сокету локальный адрес");
    listen(sockfd, 10);
    check_errno("Невозможно начать прослушивать соединения");

    printf("Listening on %d\n", port);

    while(true) {
        int client = accept(sockfd, NULL, NULL);
        check_errno("Ошибка при соединении с клиентом");
        if (!fork()) {
            check_errno("Невозможно создать подпоток");
            printf("Клиент приконнектился: %d\n", client);


        }
    }
}

int main(int argc, char* argv[]) {
    start_server(check_args(argc, argv));
    return 0;
}