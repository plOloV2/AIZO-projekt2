#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<omp.h>
#include"graph.h"


uint16_t gcd(uint16_t a, uint16_t b){
    uint16_t temp;
    while (b != 0){
        temp = a % b;

        a = b;
        b = temp;
    }
    return a;
}


struct graph* create_graph(uint16_t size, uint8_t conf){

    struct graph* result = (struct graph*)malloc(sizeof(struct graph));
    if(result == NULL)
        return NULL;


    result->matrix = (uint8_t**)malloc(sizeof(uint8_t*) * size);
    if(result->matrix == NULL)
        return NULL;

    for(uint16_t i = 0; i < size; i++){
        result->matrix[i] = (uint8_t*)malloc(sizeof(uint8_t) * size);
        if(result->matrix[i] == NULL)
        return NULL;
    }


    // result->matrix_opt = (uint8_t**)malloc(sizeof(uint8_t*) * (size - 1));
    // if(result->matrix_opt == NULL)
    //     return NULL;

    // for(uint16_t i = 0; i < (size - 1); i++){
    //     result->matrix_opt[i] = (uint8_t*)malloc(sizeof(uint8_t) * (i + 1));
    //     if(result->matrix_opt[i] == NULL)
    //     return NULL;
    // }


    // result->list = (struct connect*)malloc(sizeof(struct connect) * size);
    // if(result->list == NULL)
    //     return NULL;


    unsigned int seed;
    FILE *f = fopen("/dev/urandom", "rb");
    if(f != NULL){
        if (fread(&seed, sizeof(seed), 1, f) != 1) {
            //Read failed
            seed = (unsigned int)time(NULL) + omp_get_thread_num();
        }
        fclose(f);
    } else {
        //File open failed
        seed = (unsigned int)time(NULL) + omp_get_thread_num();
    }

    uint8_t step;
    do{
        step = (rand_r(&seed) % (size - 1)) + 1;
    }while(gcd(step, size) != 1);

    uint16_t p = 0, pn = step;

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        result->matrix[p][pn] = (rand_r(&seed) % 254) + 1;
        result->matrix[pn][p] = result->matrix[p][pn];

        p += step;
        pn += step;

    }

    return result;

}
