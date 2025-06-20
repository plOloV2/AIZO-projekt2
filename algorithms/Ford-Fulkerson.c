#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void free_result(struct result* result);


struct result* Ford_Fulkerson(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;
    struct result** node_array = malloc(sizeof(struct result*) * graph->size);
    
    if(!node_array){

        fprintf(stderr, "NODE ARRAY alloc failed (F-B)\n");
        return NULL;

    }

    for(uint16_t i = 0; i < graph->size; i++){

        struct result* node = malloc(sizeof(struct result));

        if(!node){

            fprintf(stderr, "NODE alloc failed (F-B)\n");
            free_result(head);
            free(node_array);
            head = NULL;
            tail = NULL;
            return NULL;

        }

        node->start = 0;
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


    if(mode)
        mode = 0;

    return head;

}
