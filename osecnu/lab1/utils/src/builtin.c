#include "../include/builtin.h"

int builtin_command(char* arg) {
    if (!strcmp(arg, "mytop")) {
        return 2;
    } else if (!strcmp(arg, "history")) {
        return 3;
    } else {
        return 0;
    }
}

ShError bexec(int op, char* argv[]) {
    switch (op) {
        case 2:
            if (pmytop() < 0) {
                return ExecError;
            }
            break;
        case 3:
            if (phistory(atoi(argv[1])) < 0) {
                return ExecError;
            }
            break;
        default:
            return InvalidArgs;
    }
    return Normal;
}