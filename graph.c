#include "graph.h"
#include "list.h"
#include "error.h"

int traverseGraph(VERTEX* wire) {
    if (wire && wire->val != true) {
        wire->val = true;

        NODE* current = wire->adj.head;
        while(current) {
            traverseGraph(current->data);
            current = current->next;
        }
    }
    return S_SUCCESS;
}

int cycleCount(LIST* vertices) {
    int vertexCount = sizeof(vertices)/sizeof(LIST);
    // TODO continue implementation
    
}