#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "graph.h"

struct graph* read_graph(const char* filename){

    FILE* file = fopen(filename, "r");
    if(!file){

        fprintf(stderr, "Failed to open file");
        return NULL;

    }

    uint16_t size;
    uint32_t dir_edges, undir_edges;

    // Read graph size
    if(fscanf(file, "size = %hu\n", &size) != 1){

        fprintf(stderr, "Invalid size format\n");
        fclose(file);
        return NULL;

    }

    // Read directed edges count
    if(fscanf(file, "directed_edges = %u\n", &dir_edges) != 1){

        fprintf(stderr, "Invalid directed edges format\n");
        fclose(file);
        return NULL;

    }

    // Allocate graph structure
    struct graph* g = calloc(1, sizeof(struct graph));
    if(!g){

        fprintf(stderr, "Failed to allocate graph\n");
        fclose(file);
        return NULL;

    }

    g->size = size;
    g->dir_edges = dir_edges;

    // Allocate directed edge arrays
    uint16_t* dir_sources = malloc(dir_edges * sizeof(uint16_t));
    uint16_t* dir_targets = malloc(dir_edges * sizeof(uint16_t));
    int16_t* dir_weights = malloc(dir_edges * sizeof(int16_t));

    if(!dir_sources || !dir_targets || !dir_weights){

        fprintf(stderr, "Failed to allocate directed edge arrays\n");
        free(dir_sources); free(dir_targets); free(dir_weights);
        free(g);
        fclose(file);
        return NULL;

    }

    // Read directed edges
    for(uint32_t i = 0; i < dir_edges; i++){

        if(fscanf(file, "%hu %hu %hd\n", &dir_sources[i], &dir_targets[i], &dir_weights[i]) != 3){
            
            fprintf(stderr, "Error reading directed edge %u\n", i);
            free(dir_sources); free(dir_targets); free(dir_weights);
            free(g);
            fclose(file);
            return NULL;

        }

    }

    // Read undirected edges count
    if(fscanf(file, "undirected_edges = %u\n", &undir_edges) != 1){

        fprintf(stderr, "Invalid undirected edges format\n");
        free(dir_sources); free(dir_targets); free(dir_weights);
        free(g);
        fclose(file);
        return NULL;

    }

    g->undir_edges = undir_edges;

    // Allocate undirected edge arrays
    uint16_t* undir_u = malloc(undir_edges * sizeof(uint16_t));
    uint16_t* undir_v = malloc(undir_edges * sizeof(uint16_t));
    int16_t* undir_weights = malloc(undir_edges * sizeof(int16_t));

    if(!undir_u || !undir_v || !undir_weights){

        fprintf(stderr, "Failed to allocate undirected edge arrays\n");
        free(dir_sources); free(dir_targets); free(dir_weights);
        free(undir_u); free(undir_v); free(undir_weights);
        free(g);
        fclose(file);
        return NULL;

    }

    // Read undirected edges
    for(uint32_t i = 0; i < undir_edges; i++){

        if(fscanf(file, "%hu %hu %hd\n", &undir_u[i], &undir_v[i], &undir_weights[i]) != 3){

            fprintf(stderr, "Error reading undirected edge %u\n", i);
            free(dir_sources); free(dir_targets); free(dir_weights);
            free(undir_u); free(undir_v); free(undir_weights);
            free(g);
            fclose(file);
            return NULL;

        }

    }

    fclose(file);

    // Initialize directed matrix
    g->dir_matrix = malloc(size * sizeof(int16_t*));
    if(!g->dir_matrix){

        fprintf(stderr, "Failed to allocate dir_matrix\n");
        free(dir_sources); free(dir_targets); free(dir_weights);
        free(undir_u); free(undir_v); free(undir_weights);
        free(g);
        return NULL;

    }

    for(uint16_t i = 0; i < size; i++){

        g->dir_matrix[i] = calloc(dir_edges, sizeof(int16_t));
        if(!g->dir_matrix[i]){
            fprintf(stderr, "Failed to allocate dir_matrix[%u]\n", i);
            for (uint16_t j = 0; j < i; j++) free(g->dir_matrix[j]);
            free(g->dir_matrix);
            free(dir_sources); free(dir_targets); free(dir_weights);
            free(undir_u); free(undir_v); free(undir_weights);
            free(g);
            return NULL;

        }

    }

    // Fill directed matrix
    for(uint32_t j = 0; j < dir_edges; j++){

        g->dir_matrix[dir_sources[j]][j] = dir_weights[j];
        g->dir_matrix[dir_targets[j]][j] = -dir_weights[j];

    }

    // Initialize undirected matrix
    g->undir_matrix = malloc(size * sizeof(int16_t*));
    if(!g->undir_matrix){

        fprintf(stderr, "Failed to allocate undir_matrix\n");
        for (uint16_t i = 0; i < size; i++) free(g->dir_matrix[i]);
        free(g->dir_matrix);
        free(dir_sources); free(dir_targets); free(dir_weights);
        free(undir_u); free(undir_v); free(undir_weights);
        free(g);
        return NULL;

    }

    for(uint16_t i = 0; i < size; i++){

        g->undir_matrix[i] = calloc(undir_edges, sizeof(int16_t));

        if(!g->undir_matrix[i]){
            fprintf(stderr, "Failed to allocate undir_matrix[%u]\n", i);
            for (uint16_t j = 0; j < size; j++) free(g->dir_matrix[j]);
            free(g->dir_matrix);
            for (uint16_t j = 0; j < i; j++) free(g->undir_matrix[j]);
            free(g->undir_matrix);
            free(dir_sources); free(dir_targets); free(dir_weights);
            free(undir_u); free(undir_v); free(undir_weights);
            free(g);
            return NULL;

        }

    }

    // Fill undirected matrix
    for(uint32_t j = 0; j < undir_edges; j++){

        g->undir_matrix[undir_u[j]][j] = undir_weights[j];
        g->undir_matrix[undir_v[j]][j] = undir_weights[j];

    }

    // Initialize adjacency lists
    g->dir_list = calloc(size, sizeof(struct edge*));
    g->undir_list = calloc(size, sizeof(struct edge*));
    if(!g->dir_list || !g->undir_list){

        fprintf(stderr, "Failed to allocate adjacency lists\n");

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

    // Build directed adjacency list
    for(uint32_t j = 0; j < dir_edges; j++){

        uint16_t src = dir_sources[j];
        struct edge* new_edge = malloc(sizeof(struct edge));

        if(!new_edge){

            fprintf(stderr, "Failed to allocate directed edge\n");
            return NULL;

        }
        new_edge->target = dir_targets[j];
        new_edge->weight = dir_weights[j];
        new_edge->next = g->dir_list[src];
        g->dir_list[src] = new_edge;
    }

    // Build undirected adjacency list
    for(uint32_t j = 0; j < undir_edges; j++){
        uint16_t u = undir_u[j];
        uint16_t v = undir_v[j];
        int16_t weight = undir_weights[j];

        // Add edge u->v
        struct edge* edge1 = malloc(sizeof(struct edge));
        if(!edge1){

            fprintf(stderr, "Failed to allocate undirected edge\n");
            return NULL;

        }

        edge1->target = v;
        edge1->weight = weight;
        edge1->next = g->undir_list[u];
        g->undir_list[u] = edge1;

        // Add edge v->u
        struct edge* edge2 = malloc(sizeof(struct edge));
        if(!edge2){

            free(edge1);
            fprintf(stderr, "Failed to allocate undirected edge\n");
            return NULL;

        }

        edge2->target = u;
        edge2->weight = weight;
        edge2->next = g->undir_list[v];
        g->undir_list[v] = edge2;

    }

    // Free temporary edge arrays
    free(dir_sources);
    free(dir_targets);
    free(dir_weights);
    free(undir_u);
    free(undir_v);
    free(undir_weights);

    return g;
}
