#include<omp.h>
#include<stdio.h>
#include"graph.h"
#include"result_struct.h"

uint8_t measure_time(struct graph* graph, uint8_t comp_mode, double* t1, double* t2, struct result* (*alg)(struct graph*, uint8_t)){

    double time1, time2;


    time1 = omp_get_wtime();

    struct result* r1 = alg(graph, 0);

    time1 = omp_get_wtime() - time1;

    if(!r1)
        return 1;


    time2 = omp_get_wtime();

    struct result* r2 = alg(graph, 1);

    time2 = omp_get_wtime() - time2;

    if(!r2)
        return 2;


    if(!compare_results(comp_mode, r1, r2)){

        time1 = time2 = 0;
        fprintf(stderr, "Something failed...\n");

    }
        

    free_result(r1);
    free_result(r2);

    *t1 = time1 * 1000;
    *t2 = time2 * 1000;

    return 0;
    
}