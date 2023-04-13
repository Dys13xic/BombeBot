#include "graph.h"
#include "list.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>

// Allocate memory and append node to end of list.
int append(LIST* target, void* data) {
    NODE *newLink = malloc(sizeof(NODE));
    newLink->data = data;
    newLink->next = NULL;

    if(target->tail == NULL) {
        target->head = newLink;
        target->tail = newLink;
    }
    else {
        target->tail->next = newLink;
        target->tail = newLink;
    }

    return S_SUCCESS;
}

// De-allocate memory for adjacency list nodes determined through double-ended scrambler.
// Passing true into the retainListHead parameter will prevent the freeing of list's head node.
int clean(LIST* target, bool retainListHead) {

    if(target->head) {
        NODE* current = target->head;
        NODE* temp = NULL;

        if(retainListHead) {
            target->head->next = NULL;
            // Skip over initial node.
            current = current->next;
        }
        else {
            target->head = NULL;
        }
        target->tail = target->head;

        while(current) {
            temp = current;
            current = current->next;
            free(temp);
        }
    }
    return S_SUCCESS;
}