#include "list.h"
#include "graph.h"
#include "error.h"

int traverseGraph(VERTEX* wire) {
    if (wire->val != true) {
        NODE* current = wire->adj.head;
        while(current) {
            traverseGraph(current->data);
            current = current->next;
        }
    }
    return S_SUCCESS;
}