#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void free_result(struct result* result);

struct result* Dijkstra(struct graph* graph, int16_t (*find_edge)(struct graph*, uint16_t, uint16_t)){

    struct result* head = NULL;
    struct result* tail = NULL;
    struct result** node_array = malloc(sizeof(struct result*) * graph->size);
    
    if (!node_array) {
        fprintf(stderr, "NODE ARRAY alloc failed (Dijkstra)\n");
        return NULL;
    }

    for(uint16_t i = 0; i < graph->size; i++){

        struct result* node = malloc(sizeof(struct result));

        if(!node){

            fprintf(stderr, "NODE alloc failed (Dijkstra)\n");
            free_result(head);
            free(node_array);
            head = NULL;
            tail = NULL;
            return NULL;

        }

        node->start = 0;                // 0 - unvisited node, 1 - visited
        node->weight = INT32_MAX;       // Infinite distance
        node->end = graph->size;        // Invalid predecessor
        node->next = NULL;

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

        int32_t min_dist = INT32_MAX;
        uint16_t min_index = graph->size; // Invalid index
        
            // Find unvisited node with smallest distance
        for(uint16_t j = 0; j < graph->size; j++){
            
            if(!node_array[j]->start && node_array[j]->weight < min_dist){

                min_dist = node_array[j]->weight;
                min_index = j;

            }

        }
        
            // Stop if no reachable nodes left
        if(min_index == graph->size)
            break;
        
        node_array[min_index]->start = 1;   // Mark visited
        
            // Update neighbors
        for(uint16_t v = 0; v < graph->size; v++){

            if(node_array[v]->start)
                continue;                   // Skip visited
            
            int16_t w = find_edge(graph, min_index, v);
            if(w == 0)
                continue;                   // No edge exists
            
                // Calculate new distance
            int32_t new_dist = node_array[min_index]->weight + w;
            
                // Update if shorter path found
            if(new_dist < node_array[v]->weight){

                node_array[v]->weight = new_dist;
                node_array[v]->end = min_index; // Set predecessor

            }

        }

    }

    free(node_array);

    return head;

}
