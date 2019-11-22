#include <bits/types/time_t.h>
#include <errno.h>
#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include "server.h"

struct server_param *server_param;

void check_errno(char *strerr) {
    if (errno) {
        fprintf(stderr, "%s. Ошибка: %s\n", strerr, strerror(errno));
        exit(1);
    }
}

void set_ids(struct server_param *server_param) {
    time(&server_param->start_time);
    server_param->pid = getpid();
    server_param->uid = getuid();
    server_param->gid = getgid();
}

void start_server(int argc, char *argv[]) {
    server_param = malloc(sizeof(struct server_param));
    set_ids(server_param);
    server_signal();
    printf("Сервер запущен.\npid = %ld, uid = %ld, gid = %ld\n", server_param->pid, server_param->uid,
           server_param->gid);
    while (true) {
        sleep(1);
        set_param(server_param);
    }
}

void set_param(struct server_param *server_param) {
    time_t now;
    time(&now);
    server_param->work_time = now - server_param->start_time;
    getloadavg(server_param->loadavg, 3);
}

void set_signal(int sig, void *func) {
    struct sigaction action;
    sigset_t mask;
    action.sa_handler = func;
    action.sa_flags = 0;
    action.sa_mask  = mask;
    sigemptyset(&mask);
    sigaction(sig, &action, NULL);
    check_errno("Не удалось создать segaction");
}


void server_signal(){
    errno = 0;
    set_signal(SIGHUP, print_hup);
    set_signal(SIGTERM, print_term);
    set_signal(SIGINT, print_int);
    set_signal(SIGUSR1, print_usr1);
    set_signal(SIGUSR2, print_usr2);
}

void *print_hup() {
    printf("Сервер был уничтожен HUP'ом");
    print_param();
    exit(0);
}
void *print_term() {
    printf("Сервер был уничтожен TERM'ом");
    print_param();
    exit(0);
}
void *print_int() {
    printf("Сервер был уничтожен INT'ом");
    print_param();
    exit(0);
}
void *print_usr1() {
    printf("Сервер был уничтожен USR1'ом");
    print_param();
    exit(0);
}

void *print_usr2() {
    printf("Сервер был уничтожен USR2'ом");
    print_param();
    exit(0);
}

void print_param(){
    printf("\nwork_time = %ld, 1min = %.2f, 5min = %.2f, 15min = %.2f\n", server_param->work_time,
           server_param->loadavg[0],
           server_param->loadavg[1], server_param->loadavg[2]);
    exit(0);
}