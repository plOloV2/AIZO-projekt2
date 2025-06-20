#include<stdint.h>
#include<stdio.h>
#include"graph.h"

void display_graphs(struct graph* graf){

    printf("\nMaciez nieskierowana:\n");

    for(uint16_t i = 0; i < graf->size; i++){

        for(uint16_t j = 0; j < graf->undir_edges; j++)
            printf("%i;", graf->undir_matrix[i][j]);

        printf("\n");

    }

    printf("\nMaciez skierowana:\n");

    for(uint16_t i = 0; i < graf->size; i++){

        for(uint16_t j = 0; j < graf->dir_edges; j++)
            printf("%i;", graf->dir_matrix[i][j]);

        printf("\n");

    }

    printf("\nLista nieskierowana:\n");

    for(uint16_t i = 0; i < graf->size; i++){

        printf("Vertex %d: ", i);

        struct edge* dis = graf->undir_list[i];

        while(dis != NULL){

            printf(" %i - %i; ", dis->target, dis->weight);
            
            dis = dis->next;

        }

        printf("\n");

    }

    printf("\nLista skierowana:\n");

    for(uint16_t i = 0; i < graf->size; i++){

        printf("Vertex %d: ", i);
        
        struct edge* dis = graf->dir_list[i];
        
        while(dis != NULL){

            printf(" %i - %i; ", dis->target, dis->weight);
            
            dis = dis->next;

        }
        
        printf("\n");

    }

}
