#include "../include/node.h"

cnode* init() {
    cnode* head = (cnode*)malloc(sizeof(cnode));
    head->cmdline.argc = 0;
    head->next = NULL;
    return head;
}

void insert(cnode* head, pcmd cmdline) {
    cnode* node = (cnode*)malloc(sizeof(cnode));
    node->cmdline = cmdline;
    node->next = NULL;
    cnode* p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = node;
    head->cmdline.argc = head->cmdline.argc + 1;

}

pcmd get(cnode* head, int n) {
    cnode* p = head->next;
    int i = 1;
    while (p != NULL && i < n) {
        p = p->next;
        i++;
    }
    return p->cmdline;
}

void delete_list(cnode* head) {
    cnode* p = head->next;
    while (p != NULL) {
        cnode* q = p->next;
        free(p);
        p = q;
    }
    head->next = NULL;
}

void print_list(cnode* head) {
    cnode* p = head->next;
    printf("---Printing the linked list---\n");
    printf("List header info: Type: %d\n", head->cmdline.type);
    while (p != NULL) {
        printf("argc: %d, argv[0]: %s, type: %d\n", p->cmdline.argc, p->cmdline.argv[0], p->cmdline.type);
        p = p->next;
    }
}
