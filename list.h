#ifndef LIST_H
#define LIST_H

typedef struct node {
    void* data;
    struct node* next;
} NODE;

typedef struct list {
    NODE* head;
    NODE* tail;
} LIST;

int append(LIST* target, void* data);
int clean(LIST* target);

#endif