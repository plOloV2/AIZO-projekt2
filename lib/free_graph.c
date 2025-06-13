#include"graph.h"
#include<stdlib.h>

void free_graph(struct graph** graphs){

    if(graphs == NULL) 
        return;

    for(int i = 0; i < 3; i++){
        if (graphs[i] == NULL) 
            continue;

        // Free matrix
        if(graphs[i]->matrix != NULL){

            for(uint16_t row = 0; row < graphs[i]->size; row++)
                free(graphs[i]->matrix[row]);
            
            free(graphs[i]->matrix);

        }

        // Free undirected adjacency lists
        if(graphs[i]->undir_list != NULL){
            for(uint16_t j = 0; j < graphs[i]->size; j++){

                struct edge* current = graphs[i]->undir_list[j];

                while(current != NULL){

                    struct edge* next = current->next;
                    free(current);
                    current = next;

                }

            }

            free(graphs[i]->undir_list);

        }

        // Free successor adjacency lists
        if(graphs[i]->suc_list != NULL){

            for(uint16_t j = 0; j < graphs[i]->size; j++){

                struct edge* current = graphs[i]->suc_list[j];

                while(current != NULL){

                    struct edge* next = current->next;
                    free(current);
                    current = next;

                }

            }

            free(graphs[i]->suc_list);

        }

        // Free graph structure
        free(graphs[i]);

    }

    // Free the graph array
    free(graphs);

}
