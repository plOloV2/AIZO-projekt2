#include<stdio.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"


void display_graphs(struct graph* graf, uint16_t size);

int main(int argc, char** argv){

    if(argc < 2){
        perror("Too few arguments");
        return 1;
    }

    struct graph** new = create_graph(10);

    if(new == NULL)
        return 2;

    printf("graf1: \n");
    display_graphs(new[0], 10);
    printf("\ngraf2: \n");
    display_graphs(new[1], 10);
    printf("\ngraf3: \n");
    display_graphs(new[2], 10);

    return 0;
}
