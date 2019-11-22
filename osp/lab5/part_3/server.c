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
    while (true) {
        sleep(1);
        set_param(server_param);
        //print_loadavg();
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
    set_signal(SIGHUP, print_pid);
    set_signal(SIGTERM, print_gid);
    set_signal(SIGUSR1, print_work_time);
    set_signal(SIGUSR2, print_uid);
    set_signal(SIGINT, print_loadavg);
}


void *print_pid() {
    printf("PID: %ld\n", server_param->pid);
    exit(0);
}
void *print_uid() {
    printf("UID: %ld\n", server_param->uid);
    exit(0);
}
void *print_gid() {
    printf("GID: %ld\n", server_param->gid);
    exit(0);
}
void *print_work_time() {
    printf("Work time: %ld\n", server_param->work_time);
    exit(0);
}
void *print_loadavg(){
    printf("work_time = %ld, 1min = %.2f, 5min = %.2f, 15min = %.2f\n", server_param->work_time,
           server_param->loadavg[0],
           server_param->loadavg[1], server_param->loadavg[2]);
    exit(0);
}