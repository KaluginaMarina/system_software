#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <zconf.h>
#include <netinet/in.h>
#include <dirent.h>


void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

void print_dir(int fd, const char *dirname) {
    errno = 0;
    DIR *dir = opendir(dirname);
    if (errno != 0) {
        dprintf(fd, "Failed to open %s: %s\n\n", dirname, strerror(errno));
        errno = 0;
        return;
    }

    dprintf(fd, "Contents of %s:\n", dirname);

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL)
        dprintf(fd, "%s\n", ent->d_name);

    dprintf(fd, "\n");
    closedir(dir);
}

unsigned int check_args(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Используйте: ./server port");
        exit(1);
    }
    char *p;
    unsigned int port = (int) strtol(argv[optind], &p, 10);
    if (errno != 0 || port < 0 || *p != '\0') {
        fprintf(stderr, "Указан неверный порт.\n");
        exit(1);
    }
    return port;
}

void read_client(int client) {
    int len = 0;
    char buf[2048];
    int bytes_read;
    char *request = NULL;
    while ((bytes_read = read(client, buf, 2048)) > 0) {
        request = realloc(request, len + bytes_read);
        memcpy(request + len, buf, bytes_read);
        len += bytes_read;

        if (len < 2) continue;
        if (request[len - 1] == '\n') {
            if (len == 1) {
                continue;
            }
            if (len == 2) {
                printf("Не получено ничего");
                write(client, "Ничего", 6);
                close(client);
                return;
            }
            if (len > 4 && request[len - 2] == '\n') {
                len -= 1;
                char *path_start = request;
                for (char *path_end = request; path_end < request + len; ++path_end) {
                    if (*path_end == '\n') {
                        *path_end = '\0';
                        print_dir(client, path_start);
                        path_end += 2;
                        path_start = path_end;
                    }
                }
                dprintf(client, "\n");
            }
        }
        printf("%s\n", request);
    }
}

void start_server(unsigned int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    check_errno("Невозможно создать сокет");
    // ......sockaddr_in...........
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    struct in_addr sin_addr;
    sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_addr = sin_addr;
    //.............................
    bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    check_errno("Невозможно привязать к сокету локальный адрес");
    listen(sockfd, 10);
    check_errno("Невозможно начать прослушивать соединения");

    printf("Listening on %d\n", port);

    while (true) {
        int client = accept(sockfd, NULL, NULL);
        check_errno("Ошибка при соединении с клиентом");
        if (!fork()) {
            check_errno("Невозможно создать подпоток");
            printf("Клиент приконнектился: %d\n", client);

            read_client(client);
        }
    }
}

int main(int argc, char *argv[]) {
    start_server(check_args(argc, argv));
    return 0;
}