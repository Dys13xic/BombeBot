#ifndef LIST_H
#define LIST_H

typedef struct list {
    struct node* head;
    struct node* tail;
} LIST;

typedef struct node {
    struct vertex* data;
    struct node* next;
} NODE;

int append(LIST* target, struct vertex* data);
int clean(LIST* target);

#endif