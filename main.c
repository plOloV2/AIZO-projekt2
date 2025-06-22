#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

/*
TO DO:
-Ford-Fulkerson
-check EVERYTHING
*/

int main(int argc, char** argv){

    if(argc < 2){
        fprintf(stderr, "Zbyt malo argumentow");
        return 1;
    }

    uint8_t err = 0;


    for(uint8_t i = 1; i < argc; i++){

        uint8_t progres = 0;

        printf("\nPostep: %i%%", progres);
        fflush(stdout);

        uint16_t amount = atoi(argv[i]);

        if(amount < 15){

            fprintf(stderr, "Podany argument jest zbyt maly. Minimalny rozmiar to: 15 .\nPrzechodze do kolejnego argumentu...");
            continue;

        }

        double times[5][3][2][100];
        double data_time_avg = 0;

        #pragma omp parallel for
        for(uint8_t i = 0; i < 100; i++){

            double data_creation_time = omp_get_wtime();

            struct graph** new = create_graph(amount);

            data_creation_time = omp_get_wtime() - data_creation_time;

            #pragma omp atomic
                data_time_avg += data_creation_time;

            if(new == NULL)
                err = 2;

            for(uint8_t j = 0; j < 3; j++){

                uint8_t error;

                error = measure_time(new[j], 1, &times[prim][j][0][i], &times[prim][j][1][i], Prim);

                if(error)
                    fprintf(stderr, "Error %i in Prim's, i = %i, j = %i\n", error, i, j);
                

                error = measure_time(new[j], 1, &times[kruskal][j][0][i], &times[kruskal][j][1][i], Kruskal);
                
                if(error)
                    fprintf(stderr, "Error %i in Kruskal, i = %i, j = %i\n", error, i, j);


                error = measure_time(new[j], 0, &times[dijkstra][j][0][i], &times[dijkstra][j][1][i], Dijkstra);
                
                if(error)
                    fprintf(stderr, "Error %i in Dijkstra, i = %i, j = %i\n", error, i, j);


                error = measure_time(new[j], 0, &times[ford_bellman][j][0][i], &times[ford_bellman][j][1][i], Ford_Bellman);
                
                if(error)
                    fprintf(stderr, "Error %i in Ford-Bellman, i = %i, j = %i\n", error, i, j);


                error = measure_time(new[j], 2, &times[ford_fulkerson][j][0][i], &times[ford_fulkerson][j][1][i], Ford_Fulkerson);
                
                if(error)
                    fprintf(stderr, "Error %i in Ford-Fulkerson, i = %i, j = %i\n", error, i, j);

            }
            

            free_graph(new);

            #pragma omp critical
            {
                    
                printf("\rPostep: %i%%", ++progres);
                fflush(stdout);

            }

        }

        save_times(times, amount);

        data_time_avg /= 100;

        printf("\rUkonczono etap %i z %i.\nRozmiar problemu: %i.\nSredni czas utworzenia zestawu losowych danych: %f s\n", i, argc - 1, amount, data_time_avg);

    }

    if(err != 0)
        return 2;

    return 0;
}
