#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void free_result(struct result* result);

struct result* Dijkstra(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    struct result** node_array = malloc(sizeof(struct result*) * graph->size);
    if(!node_array){

        fprintf(stderr, "NODE ARRAY alloc failed (Dijkstra)\n");
        return NULL;
    }

    for(uint16_t i = 0; i < graph->size; i++){

        struct result* node = calloc(1, sizeof(struct result));
        if(!node){

            fprintf(stderr, "NODE alloc failed (Dijkstra)\n");
            free_result(head);
            free(node_array);
            return NULL;

        }

        node->weight = INT32_MAX;       // Infinite distance
        node->end = graph->size;        // Invalid predecessor

        if(!head){
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }

        node_array[i] = node;

    }

    node_array[0]->weight = 0;

    // Main algorithm loop
    for(uint16_t i = 0; i < graph->size; i++){

        int32_t best_w = INT32_MAX;
        uint16_t min_index = graph->size; // Invalid index
        
        // Find unvisited node with smallest distance
        for(uint16_t j = 0; j < graph->size; j++){
            
            if(!node_array[j]->start && node_array[j]->weight < best_w){

                best_w = node_array[j]->weight;
                min_index = j;

            }

        }
        
            // Stop if no reachable nodes left
        if(min_index == graph->size)
            break;
        
        node_array[min_index]->start = 1;   // Mark visited
        
            // Update neighbors
        if(mode == 0){

            // MATRIX MODE: scan row u of undir_matrix[u][v]
            for(uint32_t e = 0; e < graph->undir_edges; e++){

                int16_t w = graph->undir_matrix[min_index][e];
                if(w <= 0)
                    continue;  // no edge

                for(uint16_t v = 0; v < graph->size; v++){

                    if(v == min_index) 
                        continue;

                    if(graph->undir_matrix[v][e] <= 0) 
                        continue;

                    // relax (min_index -> v) with weight w
                    if(node_array[v]->start) 
                        continue;

                    int32_t nd = node_array[min_index]->weight + w;

                    if(nd < node_array[v]->weight){

                        node_array[v]->weight = nd;
                        node_array[v]->end = min_index;

                    }

                }

            }

        }else {
            // LIST MODE: walk the adjacency list at undir_list[u]
            for(struct edge* e = graph->undir_list[min_index]; e; e = e->next){

                uint16_t v = e->target;

                if(node_array[v]->start)
                    continue;
                int32_t nd = node_array[min_index]->weight + e->weight;

                if(nd < node_array[v]->weight){

                    node_array[v]->weight = nd;
                    node_array[v]->end    = min_index;

                }

            }

        }

    }

    free(node_array);

    return head;

}
