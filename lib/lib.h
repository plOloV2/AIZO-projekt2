#include"graph.h"
#include"result_struct.h"

struct graph** create_graph(uint16_t size);

void free_graph(struct graph** graph);

void free_result(struct result* result);
