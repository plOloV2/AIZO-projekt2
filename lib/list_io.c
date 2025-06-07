#include"graph.h"
#include<stdio.h>

int16_t dir_get_edge_list(struct graph* graph, uint16_t start, uint16_t end){

    struct edge* now = graph->suc_list[start];
    
    while(now != NULL){

        if(now->target == end)
            return now->weight;

        now = now->next;

    }
    
    return 0;
}

int16_t undir_get_edge_list(struct graph* graph, uint16_t start, uint16_t end){

    struct edge* now = graph->undir_list[start];
    
    while(now != NULL){

        if(now->target == end)
            return now->weight;

        now = now->next;

    }
    
    return 0;
}
