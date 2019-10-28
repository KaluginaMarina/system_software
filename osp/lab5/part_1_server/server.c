#include <stdio.h>
#include <time.h>
#include <zconf.h>
#include <stdbool.h>
#include <stdlib.h>
#include "server.h"

void start_server(){
    time(&start_time);
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
}