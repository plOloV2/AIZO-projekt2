#ifndef RESULT_STRUCT_H
#define RESULT_STRUCT_H

#include<stdint.h>

// KAŻDY ALGORYTM WYKORZYTUJE TĄ STRUKTURĘ NA SWÓJ SPOSÓB
// PONIŻSZY OPIS ZMIENNYCH NIE ZAWSZE MA ZASTOSOWANIE DO WYNIKÓW ZWRACANYCH PRZEZ ALGORYTMY 

struct result{
    uint16_t start;             // początkowy wierzchołek krawędzi
    uint16_t end;               // końcowy wierzchołek krawędzi
    int32_t weight;             // waga krawędzi
    struct result* next;        // wskaźnik do następnej struktury
};

// definicja funkcji operujących na strukturze result
void free_result(struct result* result);

uint8_t compare_results(uint8_t mode, struct result *a, struct result *b);

struct result* sort_result_list(struct result* head);

#endif
