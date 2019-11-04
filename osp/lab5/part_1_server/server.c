#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <errno.h>
#include "server.h"

void start_server(){
    errno = 0;
    int mem_id = shmget(IPC_PRIVATE, sizeof(struct server_param), IPC_CREAT | 0644);
    if(mem_id < 0){
        fprintf(stderr, "Error: shmget.\n");
        exit(1);
    }

    struct server_param *server_param = (struct server_param*)shmat(mem_id, NULL, 0);
    if (server_param == NULL) {
        fprintf(stderr, "Error: shmat.\n");
        exit(1);
    }

    if (EACCES == errno) {
        fprintf(stderr, "Permission denied\n");
        exit(1);
    }

    time(&server_param->start_time);
    server_param->pid = getpid();
    server_param->uid = getuid();
    server_param->gid = getgid();
    while (true){
        sleep(1);
        set_param(server_param);
        printf("%d %f %f %f \n", server_param->work_time, server_param->loadavg[0], server_param->loadavg[1], server_param->loadavg[2]);
    }
}

void set_param(struct server_param *server_param){
    time_t now;
    time(&now);
    server_param->work_time = now - server_param->start_time;
    getloadavg(server_param->loadavg, 3);
}