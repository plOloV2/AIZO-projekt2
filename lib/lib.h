#ifndef LIB_H
#define LIB_H

#include"graph.h"
#include"result_struct.h"

// definicje najważniejszych funkcji
struct graph** create_graph(uint16_t size);

void free_graph(struct graph** graph);

uint8_t measure_time(struct graph* graph, uint8_t comp_mode, double* t1, double* t2, struct result* (*alg)(struct graph*, uint8_t));

void save_times(double times[5][3][2][100], uint16_t ammount);

#endif
