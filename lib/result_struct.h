#ifndef RESULT_STRUCT_H
#define RESULT_STRUCT_H

#include<stdint.h>

struct result{
    uint16_t start;             // start vertex of edge
    uint16_t end;               // end vertex of edge
    int32_t weight;             // weight of edge
    struct result* next;        // pointer to next edge
};

void free_result(struct result* result);

uint8_t compare_results(uint8_t mode, struct result *a, struct result *b);

struct result* sort_result_list(struct result* head);

#endif
