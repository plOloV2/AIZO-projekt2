#include"graph.h"
#include<stdlib.h>

// funkcja czyszcząca tablice danych 
void free_graph(struct graph** graphs){

    if(graphs == NULL) 
        return;

    for(int i = 0; i < 3; i++){
        if (graphs[i] == NULL) 
            continue;

        // usuwa macierz skierowaną
        if(graphs[i]->dir_matrix != NULL){

            for(uint16_t row = 0; row < graphs[i]->size; row++)
                free(graphs[i]->dir_matrix[row]);
            
            free(graphs[i]->dir_matrix);

        }

        // usuwa macierz nieskierowaną
        if(graphs[i]->undir_matrix != NULL){

            for(uint16_t row = 0; row < graphs[i]->size; row++)
                free(graphs[i]->undir_matrix[row]);
            
            free(graphs[i]->undir_matrix);

        }

        // usuwa liste nieskierowaną
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

        // usuwa liste skierowaną
        if(graphs[i]->dir_list != NULL){

            for(uint16_t j = 0; j < graphs[i]->size; j++){

                struct edge* current = graphs[i]->dir_list[j];

                while(current != NULL){

                    struct edge* next = current->next;
                    free(current);
                    current = next;

                }

            }

            free(graphs[i]->dir_list);

        }

        // usuwa structure graph
        free(graphs[i]);

    }

    // usuwa całą tablicę
    free(graphs);

    graphs = NULL;

}
