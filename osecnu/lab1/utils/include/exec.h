/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name exec.h
 * @date 2023.3.9
 * @brief Provide execute command utils
*/
#pragma once

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "node.h"
#include "def.h"
#include "builtin.h"

/**
 * @note getcmd into char 
 * @param char* buf: parse commands into buffer 
 * @param int len :the size of buf, in order to avoid buffer overflow 
 * @return 1 :get success 
 * @return 0 :get failed 
*/
int pgetline(char* buf, int len);

/**
 * @note Read buffer and set it into linked list,
 * Where each node saves a command with type.
 * This method is inspired from "zsh".
 * @param char* buf: buf that readed from getcmd()
 * @return cnode* header: header of linked list, head->cmdline.argc records the number of commands
*/
cnode* parsecmd(char* buf);

/**
 * @note run cmd with argc, argv
 * @param int argc: number of args
 * @param char** argv: args
*/
void runcmd(int argc, char* argv[]) __attribute__((noreturn));

/**
 * @note run the pipeline
 * @param cnode* head: the header of linked list
 * @param int number: number of programs to be run
*/
void run_pipeline(cnode* head, int number);

/**
 * @note parse the read cmd and try to execute it, like other `eval()` function
 * @param char* cmdline: buffer readed from console
*/
void eval(char* cmdline);

/**
 * @note if command == "cd", we need to modify some
 * infomation of parent process. this must be done outside child process
 * @param char* buf: directory to be chdir
*/
void pchdir(char* buf);

/**
 * @note if command == "exit", we need to terminate the main process
*/
void pexit() __attribute__((noreturn));
