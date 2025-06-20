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
    uint32_t dir_edges;         // number of edges in directed graph
    uint32_t undir_edges;       // number of edges in undirected graph
    int16_t** dir_matrix;       // directed incident matrix
    int16_t** undir_matrix;     // undirected incident matrix
    struct edge** undir_list;   // undirected list
    struct edge** dir_list;     // directed list
};

/*
    - in matrix representation and in lists firs dimention of array is of size of graph
    - both matrix representations second array dimention is respective number of edges
    - values represented inside matrixes are weights of edges
    - edge inside directed incident matrix is mark with positive value at start edge index and negative at end index
    - in undirected incident matrix both values are positive
    - lists hold same graphs as their matrix counterparts
*/

#endif
