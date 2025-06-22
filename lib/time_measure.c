#include<omp.h>
#include<stdio.h>
#include"graph.h"
#include"result_struct.h"

// funkcja dokonująca pomiaru czasu, sprawdzenia wyników oraz zapisania zmierzonych czasów do tablicy
uint8_t measure_time(struct graph* graph, uint8_t comp_mode, double* t1, double* t2, struct result* (*alg)(struct graph*, uint8_t)){

    uint8_t err = 0;

    double time1, time2;

    // pomiar wykonania algorytmu dla miecierzy
    time1 = omp_get_wtime();

    struct result* r1 = alg(graph, 0);

    time1 = omp_get_wtime() - time1;

    if(!r1)
        err += 1;


    // pomiar wykonania algorytmu dla listy
    time2 = omp_get_wtime();

    struct result* r2 = alg(graph, 1);

    time2 = omp_get_wtime() - time2;

    if(!r2)
        err += 2;


    // porównanie wyników obydwu algorytmów do siebie
    if(!compare_results(comp_mode, r1, r2) || err != 0){

        time1 = time2 = -1;
        fprintf(stderr, "Coś nie działa...\n");

    }
        

    // czyszczenie pamięci
    free_result(r1);
    free_result(r2);

    // zapisanie czasów w milisekundach
    *t1 = time1 * 1000;
    *t2 = time2 * 1000;

    return err;
    
}
