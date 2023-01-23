#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct vertex {
    bool val;
    struct list adj;
} VERTEX;

int traverseGraph(VERTEX* wire);

#endif