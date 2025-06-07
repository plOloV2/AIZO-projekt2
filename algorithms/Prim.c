#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct result* Prim(struct graph* graph, int16_t (*find_edge)(struct graph*, uint16_t, uint16_t)){

    struct result* head = NULL;
    struct result* tail = NULL;

    uint8_t* visited = (uint8_t*)malloc(sizeof(uint8_t) * graph->size);
    if (visited == NULL){

        fprintf(stderr, "Failed to inicialize VISITED table (Prim's)\n");
        return NULL;

    }
        

    memset(visited, 0, sizeof(uint8_t) * graph->size);

    visited[0] = 1;

    for(uint16_t i = 1; i < graph->size; i++){
        
        int16_t min_weight = INT16_MAX;
        uint16_t best_u = 0, best_v = 0;

        for (uint16_t u = 0; u < graph->size; u++){

            if (visited[u] == 0)
                continue;
            
            for (uint16_t v = 0; v < graph->size; v++){

                if (visited[v] == 1)
                    continue;

                int16_t w = find_edge(graph, u, v);

                if(w > 0 && w < min_weight){
                    min_weight = w;
                    best_u = u;
                    best_v = v;
                }

            }

        }


        if(min_weight == INT16_MAX){

            fprintf(stderr, "Disconnected graph (Prim's)\n");
            free(visited);
            return NULL;

        }
        
    
        struct result* node = malloc(sizeof(struct result));
        if(!node){
            
            fprintf(stderr, "NODE alloc failed (Prim's)\n");
            free(visited);
            return head;

        }

        node->start = best_u;
        node->end = best_v;
        node->weight = min_weight;
        node->next = NULL;

        if(!head){
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }

        visited[best_v] = 1;
        
    }


    free(visited);
    return head;

}
