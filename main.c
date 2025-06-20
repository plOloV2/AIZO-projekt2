#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

/*
TO DO:
-Ford-Bellman
-Ford-Fulkerson
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

    uint8_t err = 0;


    for(uint8_t i = 1; i < argc; i++){

        uint32_t ammount = atoi(argv[i]);

        if(ammount > 65866){

            fprintf(stderr, "Argument too big. Max size: 65866 .\nContinueing to next arg...");
            continue;

        }

        if(ammount < 15){

            fprintf(stderr, "Argument too smll. Min size: 15 .\nContinueing to next arg...");
            continue;

        }

        // #pragma omp parallel for
        // for(uint8_t i = 0; i < 100; i++){

            struct graph** new = create_graph(ammount);

            if(new == NULL)
                err = 2;

            printf("graf1: \n");
            display_graphs(new[0]);
            printf("\ngraf2: \n");
            display_graphs(new[1]);
            printf("\ngraf3: \n");
            display_graphs(new[2]);

            struct result* prime = Prim(new[0], 0);

            struct result* kruskal = Kruskal(new[0], 0);

            struct result* dijkstra = Dijkstra(new[0], 0);

            free_result(prime);
            free_result(kruskal);
            free_result(dijkstra);

            free_graph(new);

        // }       

    }

    if(err != 0)
        return 2;

    return 0;
}
