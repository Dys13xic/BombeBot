#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "list.h"

typedef struct vertex {
    bool val;
    LIST adj;
} VERTEX;

int traverseGraph(VERTEX* wire);

#endif