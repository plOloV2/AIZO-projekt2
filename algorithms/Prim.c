#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void free_result(struct result* result);


struct result* Prim(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    uint8_t* visited = calloc(graph->size, sizeof(uint8_t));
    if (visited == NULL){

        fprintf(stderr, "Failed to inicialize VISITED table (Prim's)\n");
        return NULL;

    }

    visited[0] = 1;

    for(uint16_t i = 1; i < graph->size; i++){
        
        int16_t min_weight = INT16_MAX;
        uint16_t best_u = 0, best_v = 0;
        uint8_t found_edge = 0;

        for(uint16_t u = 0; u < graph->size; u++){

            if(!visited[u])
                continue;
            
            for(uint16_t v = 0; v < graph->size; v++){

                if(visited[v])
                    continue;

                int16_t w = 0;

                if(mode == 0){
                    // Use undirected incident matrix
                    for(uint32_t e_idx = 0; e_idx < graph->undir_edges; e_idx++){

                        if(graph->undir_matrix[u][e_idx] > 0 && graph->undir_matrix[v][e_idx] > 0){

                            w = graph->undir_matrix[u][e_idx];
                            break;

                        }

                    }

                }else{
                    // Use undirected adjacency list
                    struct edge* e = graph->undir_list[u];
                    while(e){

                        if(e->target == v){

                            w = e->weight;
                            break;

                        }

                        e = e->next;

                    }

                }

                if(w == 0)
                    continue;

                found_edge = 1;

                if(w < min_weight){

                    min_weight = w;
                    best_u = u;
                    best_v = v;

                }

            }

        }


        if(!found_edge){

            fprintf(stderr, "Disconnected graph (Prim's)\n");
            break;

        }
        
    
        struct result* node = malloc(sizeof(struct result));
        if(!node){
            
            fprintf(stderr, "NODE alloc failed (Prim's)\n");
            free_result(head);
            head = NULL;
            tail = NULL;
            free(visited);
            return NULL;

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

