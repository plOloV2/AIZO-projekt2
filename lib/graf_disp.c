#include<stdint.h>
#include<stdio.h>
#include"graph.h"

void display_graphs(struct graph* graf){

    for(uint16_t i = 0; i < graf->size; i++){

        for(uint16_t j = 0; j < graf->edges; j++)
            printf("%i;", graf->matrix[i][j]);

        printf("\n");

    }

}