#ifndef MATRIX_H
#define MATRIX_H

#include"graph.h"

int16_t dir_get_edge_matrix(struct graph* graph, uint16_t start, uint16_t end);

int16_t undir_get_edge_matrix(struct graph* graph, uint16_t start, uint16_t end);

#endif
