#ifndef RESULT_STRUCT_H
#define RESULT_STRUCT_H

#include<stdint.h>

struct result{
    uint16_t start;             // start vertex of edge
    uint16_t end;               // end vertex of edge
    int32_t weight;             // weight of edge
    struct result* next;        // pointer to next edge
};

#endif
