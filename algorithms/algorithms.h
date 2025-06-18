#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include"../lib/result_struct.h"

struct result* Prim(struct graph* graph, int16_t (*find_edge)(struct graph*, uint16_t, uint16_t));

struct result* Kruskal(struct graph* graph, int16_t (*find_edge)(struct graph*, uint16_t, uint16_t));

struct result* Dijkstra(struct graph* graph, int16_t (*find_edge)(struct graph*, uint16_t, uint16_t));

#endif
