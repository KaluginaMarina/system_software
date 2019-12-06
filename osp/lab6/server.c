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
        dprintf(fd, "Невозможно открыть \"%s\". Ошибка %s\n\n", dirname, strerror(errno));
        errno = 0;
        return;
    }

    dprintf(fd, "DIR \"%s\":\n", dirname);

    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL){
        dprintf(fd, "%s\n", ent->d_name);
        printf("%s\n", ent->d_name);
    }
    dprintf(fd, "\n\n");
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
    char *buf = (char*)malloc(2048 * sizeof(char));
    int bytes_read;
    char *request = NULL;
    while ((bytes_read = read(client, buf, 2048)) > 0) {
        request = realloc(request, len + bytes_read);
        memcpy(request + len, buf, bytes_read);
        len += bytes_read;
        printf("request: \"%s\"\n", request);
        if (*(request + len - 1) == '\0'){
            break;
        }
    }

    char *dirs = strtok(request, "\n");
    while (dirs != NULL) {
        print_dir(client, dirs);
        dirs = strtok(NULL, "\n");
    }
    write(client, "\0", 1);
    free (request);
    close(client);
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