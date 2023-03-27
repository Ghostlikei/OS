/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name builtin.h
 * @date 2023.3.20
 * @brief some builtin utils to help shell recognize my builtin commands
*/
#pragma once

#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "def.h"
#include "mytop.h"
#include "history.h"

/**
 * @note judge whether cmd matchs "history", "mytop", "exit"
 * @param char* indicates the command
 * @return 0: not a builtin_command
 * @return 1: exit (deprecated?)
 * @return 2: mytop
 * @return 3: history
*/
int builtin_command(char* arg);

/**
 * @note execute builtin command
 * @param int op: builtin command number map as before
 * @param char* argv[]: cmd argv
 * @return ShError: Normal/ExecError
*/
ShError bexec(int op, char* argv[]);