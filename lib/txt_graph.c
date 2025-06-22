#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "graph.h"

/*
    ---   POPRAWNY FORMAT PLIKU Z GRAFEM   ---

rozmiar = V
skierowane_krawedzie = Es
v0 u0 w0
... 
vEs uEs wEs
nieskierowane_krawedzie = En
v0 u0 w0
... 
vEn uEn wEn

    ---   PRZYKŁAD   ---

rozmiar = 5
skierowane_krawedzie = 11
0 1 10
0 2 5
1 2 2
1 3 1
2 1 3
2 3 9
2 4 2
3 2 8
4 0 3
4 3 6
3 4 7
nieskierowane_krawedzie = 7
0 1 1
0 2 3
1 2 4
1 3 2
2 3 5
2 4 6
3 4 4

*/

// funcja odczytująca graf z pliku
struct graph* read_graph(const char* filename){

    FILE* file = fopen(filename, "r");
    if(!file){

        fprintf(stderr, "Blad podczas otwierania pliku");
        return NULL;

    }

    uint16_t size;
    uint32_t dir_edges, undir_edges;

    // wczytanie rozmiaru grafu
    if(fscanf(file, "rozmiar = %hu\n", &size) != 1){

        fprintf(stderr, "Niepoprawny format rozmiaru grafu\n");
        fclose(file);
        return NULL;

    }

    // wczytanie ilości skierowanych krawędzi
    if(fscanf(file, "skierowane_krawedzie = %u\n", &dir_edges) != 1){

        fprintf(stderr, "Niepoprawny format ilosci skierowanych krawedzi\n");
        fclose(file);
        return NULL;

    }

    // alokacja pamięci
    struct graph* g = calloc(1, sizeof(struct graph));
    if(!g){

        fprintf(stderr, "Blad alokacji grafu\n");
        fclose(file);
        return NULL;

    }

    g->size = size;
    g->dir_edges = dir_edges;

    uint16_t* dir_sources = malloc(dir_edges * sizeof(uint16_t));
    uint16_t* dir_targets = malloc(dir_edges * sizeof(uint16_t));
    int16_t* dir_weights = malloc(dir_edges * sizeof(int16_t));

    if(!dir_sources || !dir_targets || !dir_weights){

        fprintf(stderr, "Blad alokacji tablic dla grafu skierowanego\n");
        free(dir_sources);
        free(dir_targets);
        free(dir_weights);
        free(g);
        fclose(file);
        return NULL;

    }

    // odczytanie wartosci z pliku
    for(uint32_t i = 0; i < dir_edges; i++){

        if(fscanf(file, "%hu %hu %hd\n", &dir_sources[i], &dir_targets[i], &dir_weights[i]) != 3){
            
            fprintf(stderr, "Blad odczytu skierowanej krawedzi %u\n", i);
            free(dir_sources);
            free(dir_targets);
            free(dir_weights);
            free(g);
            fclose(file);
            return NULL;

        }

    }

    // wczytanie ilości skierowanych krawędzi
    if(fscanf(file, "nieskierowane_krawedzie = %u\n", &undir_edges) != 1){

        fprintf(stderr, "Niepoprawny format ilosci nieskierowanych krawedzi\n");
        free(dir_sources);
        free(dir_targets);
        free(dir_weights);
        free(g);
        fclose(file);
        return NULL;

    }

    g->undir_edges = undir_edges;

    uint16_t* undir_u = malloc(undir_edges * sizeof(uint16_t));
    uint16_t* undir_v = malloc(undir_edges * sizeof(uint16_t));
    int16_t* undir_weights = malloc(undir_edges * sizeof(int16_t));

    if(!undir_u || !undir_v || !undir_weights){

        fprintf(stderr, "Blad alokacji tablic dla grafu nieskierowanego\n");
        free(dir_sources);
        free(dir_targets);
        free(dir_weights);
        free(undir_u);
        free(undir_v);
        free(undir_weights);
        free(g);
        fclose(file);
        return NULL;

    }

    // odczytanie wartosci z pliku
    for(uint32_t i = 0; i < undir_edges; i++){

        if(fscanf(file, "%hu %hu %hd\n", &undir_u[i], &undir_v[i], &undir_weights[i]) != 3){

            fprintf(stderr, "Blad odczytu skierowanej krawedzi %u\n", i);
            free(dir_sources);
            free(dir_targets);
            free(dir_weights);
            free(undir_u);
            free(undir_v);
            free(undir_weights);
            free(g);
            fclose(file);
            return NULL;

        }

    }

    fclose(file);

    // inicjjalizacja macieży skierowanej
    g->dir_matrix = malloc(size * sizeof(int16_t*));
    if(!g->dir_matrix){

        fprintf(stderr, "Blad alokacji dir_matrix\n");
        free(dir_sources);
        free(dir_targets);
        free(dir_weights);
        free(undir_u);
        free(undir_v);
        free(undir_weights);
        free(g);
        return NULL;

    }

    for(uint16_t i = 0; i < size; i++){

        g->dir_matrix[i] = calloc(dir_edges, sizeof(int16_t));
        if(!g->dir_matrix[i]){

            fprintf(stderr, "Blad alokacji dir_matrix[%u]\n", i);

            for(uint16_t j = 0; j < i; j++) 
                free(g->dir_matrix[j]);

            free(g->dir_matrix);
            free(dir_sources);
            free(dir_targets);
            free(dir_weights);
            free(undir_u);
            free(undir_v); 
            free(undir_weights);
            free(g);
            return NULL;

        }

    }

    // zapełnienie tejże macieży
    for(uint32_t j = 0; j < dir_edges; j++){

        g->dir_matrix[dir_sources[j]][j] = dir_weights[j];
        g->dir_matrix[dir_targets[j]][j] = -dir_weights[j];

    }

    // inicjalizacja macieży nieskierowanej
    g->undir_matrix = malloc(size * sizeof(int16_t*));
    if(!g->undir_matrix){

        fprintf(stderr, "Blad alokacji undir_matrix\n");

        for(uint16_t i = 0; i < size; i++)
            free(g->dir_matrix[i]);

        free(g->dir_matrix);
        free(dir_sources);
        free(dir_targets);
        free(dir_weights);
        free(undir_u);
        free(undir_v);
        free(undir_weights);
        free(g);
        return NULL;

    }

    for(uint16_t i = 0; i < size; i++){

        g->undir_matrix[i] = calloc(undir_edges, sizeof(int16_t));

        if(!g->undir_matrix[i]){
            fprintf(stderr, "Blad alokacji undir_matrix[%u]\n", i);

            for (uint16_t j = 0; j < size; j++)
                free(g->dir_matrix[j]);

            free(g->dir_matrix);

            for (uint16_t j = 0; j < i; j++)
                free(g->undir_matrix[j]);
                
            free(g->undir_matrix);
            free(dir_sources);
            free(dir_targets);
            free(dir_weights);
            free(undir_u);
            free(undir_v);
            free(undir_weights);
            free(g);
            return NULL;

        }

    }

    // zapełnienie tejże macieży
    for(uint32_t j = 0; j < undir_edges; j++){

        g->undir_matrix[undir_u[j]][j] = undir_weights[j];
        g->undir_matrix[undir_v[j]][j] = undir_weights[j];

    }

    // inicjalizacja list
    g->dir_list = calloc(size, sizeof(struct edge*));
    g->undir_list = calloc(size, sizeof(struct edge*));
    if(!g->dir_list || !g->undir_list){

        fprintf(stderr, "Blad alokacji listy sasiedztwa\n");

        for(uint16_t i = 0; i < size; i++){

            free(g->dir_matrix[i]);
            free(g->undir_matrix[i]);
        }

        free(g->dir_matrix);
        free(g->undir_matrix);
        free(g->dir_list);
        free(g->undir_list);
        free(g);
        return NULL;

    }

    // budowa skierowanej listy
    for(uint32_t j = 0; j < dir_edges; j++){

        uint16_t src = dir_sources[j];
        struct edge* new_edge = malloc(sizeof(struct edge));

        if(!new_edge){

            fprintf(stderr, "Blad alokacji skierowanej krawedzi\n");
            return NULL;

        }
        new_edge->target = dir_targets[j];
        new_edge->weight = dir_weights[j];
        new_edge->next = g->dir_list[src];
        g->dir_list[src] = new_edge;
    }

    // budowa nieskierowanej listy
    for(uint32_t j = 0; j < undir_edges; j++){
        uint16_t u = undir_u[j];
        uint16_t v = undir_v[j];
        int16_t weight = undir_weights[j];

        struct edge* edge1 = malloc(sizeof(struct edge));
        if(!edge1){

            fprintf(stderr, "Blad alokacji nieskierowanej krawedzi\n");
            return NULL;

        }

        edge1->target = v;
        edge1->weight = weight;
        edge1->next = g->undir_list[u];
        g->undir_list[u] = edge1;

        struct edge* edge2 = malloc(sizeof(struct edge));
        if(!edge2){

            free(edge1);
            fprintf(stderr, "Blad alokacji nieskierowanej krawedzi\n");
            return NULL;

        }

        edge2->target = u;
        edge2->weight = weight;
        edge2->next = g->undir_list[v];
        g->undir_list[v] = edge2;

    }

    // czyszczenie pamięci
    free(dir_sources);
    free(dir_targets);
    free(dir_weights);
    free(undir_u);
    free(undir_v);
    free(undir_weights);

    return g;
}
