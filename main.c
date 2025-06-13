#include<stdio.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

/*
TO DO:
-Dijkstra
-Ford-Bellman
-Ford-Fulkerson
-args handle (max = 257)
-time mesurment
-save to .csv
-check EVERYTHING
*/

void display_graphs(struct graph* graf);

int main(int argc, char** argv){

    if(argc < 2){
        fprintf(stderr, "Too few arguments");
        return 1;
    }

    struct graph** new = create_graph(11);

    if(new == NULL)
        return 2;

    printf("graf1: \n");
    display_graphs(new[0]);
    printf("\ngraf2: \n");
    display_graphs(new[1]);
    printf("\ngraf3: \n");
    display_graphs(new[2]);

    free_graph(new);

    return 0;
}
