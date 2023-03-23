/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name mytop.h
 * @date 2023.3.9
 * @brief Provide mytop command utils
*/
#pragma once

/**
 * @note Like minix "top" command, We have to provide these infomation below:
 * 1. main memory (pagesize * total) / 1K
 * 2. free memory (pagesize * total) / 1K
 * 3. cache size  (pagesize * cached) / 1K
 * 4. cpu used (1 - sum(free ticks) / sum(ticks))
 * 
 * PID USERNAME PRI NICE    SIZE STATE   TIME     CPU COMMAND
*/

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pwd.h>
#include <sys/stat.h>
#include <signal.h>
#include <termios.h>
#include <assert.h>
#include <ctype.h>

#include "def.h"

#undef __aarch64__ // used for writing code

typedef struct imeminfo {
    unsigned long pagesize;
    unsigned long total;
    unsigned long free;
    unsigned long largest;
    unsigned long cached;
} imeminfo;

typedef struct iproc {
    int pid;
    char username[kMaxTokenLen];
    unsigned long uid;
    int priority;
    int nice;
    unsigned long memory;
    char state;
    unsigned long ticks;
    double cpu;
    char name[kMaxTokenLen];
} iproc;

/**
 * @note mytop function: like top in minix
 * @return -1: opening file error
 * @return 0: success
*/
int pmytop();