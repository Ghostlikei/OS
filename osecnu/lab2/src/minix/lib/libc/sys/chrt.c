#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int chrt(long ddl){
    struct timespec time;
    message m;

    memset(&m, 0, sizeof(m));
    alarm((unsigned int)ddl);
    if (ddl < 0) {
        return 0;
    } else if (ddl > 0) {
        clock_gettime(CLOCK_REALTIME, &time);
        ddl = time.tv_sec + ddl;
    }
    m.m2_l1 = ddl;

    return _syscall(PM_PROC_NR, PM_CHRT, &m);
}