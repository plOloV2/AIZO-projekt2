#include<stdint.h>

struct edge {
    uint16_t target;
    int16_t weight;
    struct edge* next;
};

struct graph {
    uint16_t size;
    uint16_t edges;
    int16_t** matrix;
    struct edge** undir_list;
    struct edge** suc_list;
};

