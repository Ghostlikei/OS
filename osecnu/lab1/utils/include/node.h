/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name node.h
 * @date 2023.3.21
 * @brief Set pipeline, make each command into node
*/

#pragma once

#include "def.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

typedef struct pcmd {
    int argc;
    char argv[kMaxArgs][kMaxTokenLen];
    CmdType type;
} pcmd;

typedef struct cnode {
    pcmd cmdline;
    struct cnode* next;
} cnode;

/**
 * @note allocate and return the dummy head of list
 * @remark argc will save the number of cmdlines
*/
cnode* init();

/**
 * @note append a node at the end of linked list
 * @param cnode* head: the head of a command line linked list
 * @param pcmd cmdline: you need to package a struct pcmd 
 * And insert into the end of linked list
*/
void insert(cnode* head, pcmd cmdline);

/**
 * @note get the nth node cmdline
 * @param cnode* head: the head of a command line linked list
 * @param int n: the nth note that input before
 * @return pcmd: the result of return node
*/
pcmd get(cnode* head, int n);

/**
 * @note delete all the linked list
 * @param cnode* head: the header of the linked list
*/
void delete_list(cnode* head);

#ifdef SHDEBUG
/**
 * @note print the all list, with argc and argv[0]
 * @param cnode* head: the header of the linked list
*/
void print_list(cnode* head);
#endif