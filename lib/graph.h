#ifndef GRAPH_H
#define GRAPH_H

#include<stdint.h>

struct edge {
    uint16_t target;            // końcowy wierzchołek krawędzi
    int16_t weight;             // waga krawędzi
    struct edge* next;          // wskaźnik do następnej krawędzi
};

struct graph {
    uint16_t size;              // liczba wierzchołków
    uint32_t dir_edges;         // liczba krawędzi w skierowanym grafie
    uint32_t undir_edges;       // liczba krawędzi w nieskierowanym grafie
    int16_t** dir_matrix;       // skierowana mcież incydencji
    int16_t** undir_matrix;     // nieskierowana mcież incydencji
    struct edge** undir_list;   // nieskierowana lista sąsiedztwa
    struct edge** dir_list;     // skierowana lista sąsiedztwa
};

/*
    - w reprezentacji macierzowej i listowej, pierwszy wymiar tablicy odpowiada rozmiarowi grafu
    - w obu reprezentacjach macierzowych drugi wymiar tablicy odpowiada liczbie krawędzi
    - wartości wewnątrz macierzy reprezentują wagi krawędzi
    - krawędź w skierowanej macierzy incydencji jest oznaczona dodatnią wartością przy indeksie 
        wierzchołka początkowego i ujemną przy indeksie wierzchołka końcowego
    - w nieskierowanej macierzy incydencji obie wartości są dodatnie
    - listy przechowują te same grafy, co odpowiadające im macierze
*/

#endif
