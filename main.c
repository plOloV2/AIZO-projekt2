#include<stdio.h>
#include<stdlib.h>
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


    for(uint8_t i = 1; i < argc; i++){

        uint16_t ammount = atoi(argv[i]);

        struct graph** new = create_graph(ammount);

        if(new == NULL)
            return 2;

        printf("graf1: \n");
        display_graphs(new[0]);
        printf("\ngraf2: \n");
        display_graphs(new[1]);
        printf("\ngraf3: \n");
        display_graphs(new[2]);

        struct result* prime = Prim(new[0], undir_get_edge_list);

        struct result* kruskal = Kruskal(new[0], undir_get_edge_list);

        free_result(prime);
        free_result(kruskal);

        free_graph(new);

    }

    

    return 0;
}
