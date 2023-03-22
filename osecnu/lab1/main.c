/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name main.c
 * @date 2023.3.9
 * @brief (OS ECNU lab1) A simple shell program include some user mode utils
*/

#include "utils/utils.h"

void sigint_handler(int) __attribute__((noreturn));
static inline int is_exit(char*);

int main() {
    char buf[kMaxTokens];

    signal(SIGINT, sigint_handler);

    while(pgetline(buf, kMaxTokens)) {
        if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ') {
            pchdir(buf);
            continue;
        } else if (is_exit(buf)) {
            break;
        } else {
            // Try to parse args and run this CMD
            eval(buf);
        }
    }
}

void sigint_handler(int sig) {
#ifdef SHDEBUG
    printf("\nReceived signal %d from SIGINT, exiting shell...\n", sig);
#endif
    assert(sig);
    exit(0);
}

static inline int is_exit(char* buf) {
    if (buf[0] == 'e'
        && buf[1] == 'x'
        && buf[2] == 'i'
        && buf[3] == 't'
        && (buf[4] == '\n' || buf[4] == ' ')) {
        return 1;
    }
    return 0;
}