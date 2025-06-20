#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include"../lib/result_struct.h"

struct result* Prim(struct graph* graph, uint8_t mode);

struct result* Kruskal(struct graph* graph, uint8_t mode);

struct result* Dijkstra(struct graph* graph, uint8_t mode);

struct result* Ford_Bellman(struct graph* graph, uint8_t mode);

struct result* Ford_Fulkerson(struct graph* graph, uint8_t mode);

enum algorithm{
    prim,
    kruskal,
    dijkstra,
    ford_bellman,
    ford_fulkerson
};

#endif
