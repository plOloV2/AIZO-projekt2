#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct result* Ford_Bellman(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    struct result** node_array = malloc(sizeof(struct result*) * graph->size);
    if(!node_array){

        fprintf(stderr, "NODE ARRAY alloc failed (Ford-Bellman)\n");
        return NULL;
    }

    for(uint16_t i = 0; i < graph->size; i++){

        struct result* node = malloc(sizeof(struct result));
        if(!node){

            fprintf(stderr, "NODE alloc failed (Ford-Bellman)\n");
            free_result(head);
            free(node_array);
            return NULL;

        }

        node->weight = INT32_MAX;       // Infinite distance
        node->start = graph->size;      // Invalid predecessor
        node->end = i;
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
    
    for(uint16_t pass = 1; pass < graph->size; pass++){

        uint8_t change = 0;

        if(mode == 0){

            for(uint32_t e = 0; e < graph->dir_edges; e++){

                int16_t w = 0;
                uint16_t u = graph->size, v = graph->size;

                for(uint16_t i = 0; i < graph->size; i++){

                    int16_t val = graph->dir_matrix[i][e];

                    if(val > 0){

                        u = i; 
                        w = val;

                    }else if(val < 0)
                        v = i;
                
                }

                if(u < graph->size && v < graph->size && node_array[u]->weight != INT32_MAX){

                    int32_t nd = node_array[u]->weight + w;

                    if(nd < node_array[v]->weight){

                        node_array[v]->weight = nd;
                        node_array[v]->start  = u;
                        change = 1;

                    }

                }

            }

        }else {

            for (uint16_t u = 0; u < graph->size; u++){

                struct edge* e = graph->dir_list[u];

                while(e){

                    uint16_t v = e->target;

                    if(node_array[u]->weight != INT32_MAX){

                        int32_t nd = node_array[u]->weight + e->weight;

                        if(nd < node_array[v]->weight){

                            node_array[v]->weight = nd;
                            node_array[v]->start  = u;
                            change = 1;

                        }

                    }

                    e = e->next;

                }

            }

        }

        if(!change)
            break;

    }

    free(node_array);
    return head;

}
