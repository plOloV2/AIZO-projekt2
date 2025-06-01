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

// uint16_t count_edges(uint16_t a, void* graph, uint8_t type);
