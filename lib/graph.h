#include<stdint.h>

struct edge {
    uint16_t target;
    uint16_t weight;
    struct edge* next;
};

struct graph {
    uint16_t size;
    int16_t** matrix;
    struct edge* undir_list;
    struct edfe* suc_list;
};

