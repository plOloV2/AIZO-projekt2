#include<stdint.h>

struct result{
    uint16_t start;
    uint16_t end;
    int16_t weight;
    struct result* next;
};
