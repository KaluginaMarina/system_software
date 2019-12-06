#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}


void getinfo(char* argv[]) {
    struct addrinfo addrinfo;
    addrinfo.ai_family = AF_INET;
    addrinfo.ai_socktype = SOCK_STREAM;
    addrinfo.ai_protocol = IPPROTO_TCP;
    struct addrinfo *addr;

    getaddrinfo(argv[1], argv[2], &addrinfo, &addr);
    check_errno("Невозможно ")

};

int main(int argc, char* argv[]) {
    getinfo();
    return 0;
}