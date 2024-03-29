/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name history.h
 * @date 2023.3.9
 * @brief Provide history utils
*/
#pragma once

/**
 * @note Like zsh and bash, we create a ./history file to record cmdline
 * It will be created at lab1/.sh_history, can be modified at def.h
*/

#include <stdio.h>
#include "def.h"

/**
 * @param int n: list number of previous n command 
 * @return -1: error
 * @return 0: success
*/
int phistory(int n);