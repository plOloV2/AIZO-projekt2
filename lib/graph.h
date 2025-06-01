#include<stdint.h>

struct connect {
    uint16_t target;
    uint16_t weight;
};

struct graph {
    struct connect** list;
    uint16_t** matrix;
};

uint16_t find_edge(uint16_t a, uint16_t b, void* graph, uint8_t type);

void display_graphs(struct graph*, uint16_t size);
