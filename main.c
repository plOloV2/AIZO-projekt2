#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

int main(int argc, char** argv){

    // wielkosc grafów jest podawana jako argumenty, jezeli nie podano zadnego, program oknczy sie
    if(argc < 2){

        fprintf(stderr, "Zbyt malo argumentow");
        return 1;

    }

    double total_time = omp_get_wtime();

    uint8_t err = 0;

    // głowna pętla programu, wykonuje sie dla kazdego argumentu
    for(uint8_t i = 1; i < argc; i++){

        uint8_t progres = 0;
        double pass_time = omp_get_wtime();

        // wypisanie postępu w procentach dla uzytkownika
        printf("\nPostep: %i%%", progres);
        fflush(stdout);

        // pobiera rozmiar grafu z argumentów
        uint16_t amount = atoi(argv[i]);

        if(amount < 15){

            fprintf(stderr, "Podany argument jest zbyt maly. Minimalny rozmiar to: 15 .\nPrzechodze do kolejnego argumentu...");
            continue;

        }

        // tablica times trzyma czasy wykonania sie algorytmow 
        double times[5][3][2][100];
        double data_time_avg = 0;

        // pętla w której dokonywane są obliczenia, każdaiteracja wykonywana jest przez inny wątek
        #pragma omp parallel for
        for(uint8_t i = 0; i < 100; i++){

            // pomiar czasu utworzenia danych
            double data_creation_time = omp_get_wtime();

            struct graph** new = create_graph(amount);

            data_creation_time = omp_get_wtime() - data_creation_time;

            #pragma omp atomic
                data_time_avg += data_creation_time;

            // jeżeli dane nie zostały utworzone, podnieś flagę i kontynuuj
            if(new == NULL){

                err = 2;
                continue;

            }
                
            // dla każdej gęstości grafu mierzy czas wykonywania i zapisuje go do tablicy times
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
            
            // czyszczenie pamięci
            free_graph(new);

            // po zakonczeniu obliczen zwiększ procent ukonczenia
            #pragma omp critical
            {
                    
                printf("\rPostep: %i%%", ++progres);
                fflush(stdout);

            }

        }

        // zapisuje wyniki do plików .csv
        save_times(times, amount);

        // jeżeli podczas wykonywania programu doszło do błędu alokacji danych kończy program
        if(err != 0)
            return 2;

        data_time_avg /= 100;

        pass_time = omp_get_wtime() - pass_time;

        uint8_t h = 0, m = 0;

        while(pass_time >= 3600){

            h++;
            pass_time -= 3600;

        }

        while(pass_time >= 60){

            m++;
            pass_time -= 60;

        }

        // wypisuje statystyki koncowe dla tego przejścia
        printf("\rUkonczono etap %i z %i.\nRozmiar problemu: %i.\nCzas rozwiazania problemu: %uh %um %fs.\nSredni czas utworzenia zestawu losowych danych: %f s\n", i, argc - 1, amount, h, m, pass_time, data_time_avg);

    }

    total_time = omp_get_wtime() - total_time;

    uint8_t h = 0, m = 0;

    while(total_time >= 3600){

        h++;
        total_time -= 3600;

    }

    while(total_time >= 60){

        m++;
        total_time -= 60;

    }

    // wypisuje czas wykonania całości obliczeń
    printf("\n\nKoniec obliczen!\nCalosc trwala: %uh %um %fs.\n", h, m, total_time);

    return 0;

}
