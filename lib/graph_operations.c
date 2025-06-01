#include<stdio.h>
#include<stdint.h>
#include"graph.h"
#include"matrix_io.h"


void display_graphs(struct graph* graf, uint16_t size){
    for(uint16_t i = 0; i < size; i++){

        for(uint16_t j = 0; j < size; j++)
            printf(" [%i] ", get_value_matrix(graf, i, j));

        printf("\n");
    }
}