#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <zconf.h>

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}


void getinfo(int argc, char* argv[]) {
    struct addrinfo addrinfo;
    addrinfo.ai_family = AF_INET;
    addrinfo.ai_socktype = SOCK_STREAM;
    addrinfo.ai_protocol = IPPROTO_TCP;
    struct addrinfo *addr;

    getaddrinfo(argv[1], argv[2], &addrinfo, &addr);
    check_errno("Невозможно получить getaddrinfo");

    int sockfd = 0;
    for (struct addrinfo* a = addr; a != NULL; a = a->ai_next) {
        if ((sockfd = socket(a->ai_family, a->ai_socktype, a->ai_protocol)) == -1)
            continue;
        if (connect(sockfd, a->ai_addr, a->ai_addrlen) == 0)
            break;
        sockfd = close(sockfd);
    }
    if (sockfd <= 0) {
        fprintf(stderr, "Невозможно приконнектиться к %s:%s\n", argv[1], argv[2]);
        exit(1);
    }
    freeaddrinfo(addr);

    for (int i = 3; i < argc; ++i) {
        write(sockfd, argv[i], strlen(argv[i]));
    }

    char *buf = (char*)malloc(2048 * sizeof(char));
    int bytes_read;
    while ((bytes_read = read(sockfd, buf, 2048)) > 0) {
        write(STDOUT_FILENO, buf, bytes_read);
    }

    close(sockfd);
}

int main(int argc, char* argv[]) {
    getinfo(argc, argv);
    return 0;
}