#ifndef GRAPH_H
#define GRAPH_H

#include<stdint.h>

struct edge {
    uint16_t target;            // end vertex of edge
    int16_t weight;             // weight of edge
    struct edge* next;          // pointer to next edge
};

struct graph {
    uint16_t size;              // number of vertexes
    uint32_t edges;             // number of edges
    int16_t** dir_matrix;       // directed incident matrix
    int16_t** undir_matrix;     // undirected incident matrix
    struct edge** undir_list;   // undirected list
    struct edge** suc_list;     // directed list
};

#endif
