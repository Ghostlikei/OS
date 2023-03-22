/**
 * @author Tongle Shen(10215501403@stu.ecnu.edu.cn)
 * @name node.h
 * @date 2023.3.21
 * @brief Set pipeline, make each command into node
*/

/**
 * @note This method is 
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
*/
void insert(cnode* head, pcmd cmdline);

/**
 * @note get the nth node cmdline
*/
pcmd get(cnode* head, int n);

/**
 * @note delete all the linked list
*/
void delete_list(cnode* head);

/**
 * @note print the all list
*/
void print_list(cnode* head);