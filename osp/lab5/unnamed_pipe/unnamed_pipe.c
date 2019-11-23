#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <zconf.h>
#include <sys/stat.h>

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

unsigned int size(int file) {
    unsigned int sz = 0;
    struct stat buf;
    fstat(file, &buf);
    sz = buf.st_size;
    return sz;
}

void read_form_file(int file, int pipefd){
    unsigned int sz = size(file);
    char *buffer = (char *) malloc(sz * sizeof(char));
    char *buffer_res = (char *) malloc(sz/2 * sizeof(char));
    if (read(file, buffer, sz) != sz) {
        fprintf(stderr, "Ошибка чтения файла.\n");
        exit(1);
    }
    char *cur_res = buffer_res;
    for (char *cur = buffer + 1; cur < buffer + sz; cur += 2) {
        *cur_res = *cur;
        cur_res++;
    }
    *(++cur_res) = '\0';
    write(pipefd, buffer_res, sz/2);
    check_errno("Невозможно записать в pipe");
    close(pipefd);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Не указано имя файла");
        exit(1);
    }
    int file = open(argv[1], O_RDONLY);
    check_errno("Невозможно открыть файл");

    int pipefd[2];
    pipe(pipefd);
    check_errno("Невозможно создать pipe");
    int pipefd_rd = pipefd[0], pipefd_wr = pipefd[1];

    int frk = fork();
    check_errno("Невозможно создать подпоток");

    if (!frk){
        close(pipefd_wr);
        dup2(pipefd_rd, STDIN_FILENO);
        execlp("/usr/bin/wc", "wc", "-c", NULL);
        check_errno("Невозможно выполнить wc");
        printf("\n");
    } else {
        close(pipefd_rd);
        read_form_file(file, pipefd_wr);
    }

    return 0;
}

