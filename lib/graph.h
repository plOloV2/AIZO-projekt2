#include<stdint.h>

struct connect {
    uint16_t target;
    uint16_t weight;
};

struct graph {
    struct connect** list;
    uint16_t** matrix;
};
