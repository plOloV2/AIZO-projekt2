#include<stdint.h>
#include<stdio.h>
#include"graph.h"

void display_graphs(struct graph* graf){

    printf("\nMaciez:\n");

    for(uint16_t i = 0; i < graf->size; i++){

        for(uint16_t j = 0; j < graf->edges; j++)
            printf("%i;", graf->matrix[i][j]);

        printf("\n");

    }

    printf("\nLista nieskierowana:\n");

    for(uint16_t i = 0; i < graf->size; i++){

        struct edge* dis = graf->undir_list[i];

        while(dis != NULL){

            printf(" %i - %i; ", dis->target, dis->weight);
            
            dis = dis->next;

        }

        printf("\n");

    }

    printf("\nLista skierowana:\n");

    for(uint16_t i = 0; i < graf->size; i++){
        
        struct edge* dis = graf->suc_list[i];
        
        while(dis != NULL){

            printf(" %i - %i; ", dis->target, dis->weight);
            
            dis = dis->next;

        }
        
        printf("\n");

    }

}
