#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <zconf.h>
#include "server.h"

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    struct server_param *server_param = (struct server_param *) malloc(sizeof(struct server_param));

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    check_errno("Невозможно создать сокет");

    struct sockaddr_un sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strncpy(sock_addr.sun_path, "/tmp/socket", sizeof(sock_addr.sun_path) - 1);
    check_errno("Невозможно создать адрес");

    connect(sock, (const struct sockaddr *) &sock_addr, sizeof(struct sockaddr_un));
    check_errno("Невозможно приконнектиться к серверу");

    read(sock, server_param, sizeof(struct server_param));
    check_errno("Невозможно прочитать");

    printf("work_time = %ld, 1min = %.2f, 5min = %.2f, 15min = %.2f\n", server_param->work_time,
           server_param->loadavg[0],
           server_param->loadavg[1], server_param->loadavg[2]);

    close(sock);
    return 0;
}