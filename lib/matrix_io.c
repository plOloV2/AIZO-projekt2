#include<stdint.h>
#include"graph.h"

void add_edge_matrix(struct graph* graph, uint16_t start, uint16_t end, int16_t val, uint16_t edge_n){

    graph->matrix[start][edge_n] = val;
    graph->matrix[end][edge_n] = -val;

}

int16_t get_edge_matrix(struct graph* graph, uint16_t start, uint16_t end){
    
    for(uint16_t i = 0; i < graph->edges; i++){

        if(graph->matrix[start][i] == 0)
            continue;

        if(graph->matrix[end][i] != 0)
            return graph->matrix[start][i];

    }

    return 0;

}
