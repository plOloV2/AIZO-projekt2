#ifndef LIB_H
#define LIB_H

#include"graph.h"
#include"result_struct.h"
#include"list.h"
#include"matrix.h"

struct graph** create_graph(uint16_t size);

void free_graph(struct graph** graph);

void free_result(struct result* result);

#endif
