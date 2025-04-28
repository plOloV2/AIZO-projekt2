#include<stdint.h>

struct connect {
    uint16_t target;
    uint8_t weight;
};

struct graph {
    struct connect** list;
    uint8_t** matrix;
    uint8_t** matrix_opt;
};
