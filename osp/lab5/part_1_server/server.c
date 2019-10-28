#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "server.h"

void start_server(){
    time_t  start;
    time(&start);
    pid = getpid();
    uid = getuid();
    gid = getgid();
    while (true){
        sleep(1);
        set_param();
    }
}

void set_param(){
    time_t now;
    time(&now);
    work_time = now - start_time;
    getloadavg(loadavg, 3);
    printf("%ld -- %f %f %f\n", work_time, loadavg[0], loadavg[1], loadavg[2]);
}