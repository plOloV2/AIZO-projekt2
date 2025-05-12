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

uint16_t find_step(uint16_t size,  unsigned int* seed){

    uint16_t* candidats = (uint16_t*)malloc(sizeof(uint16_t) * size);
    uint16_t index = 0, result;

    for(uint16_t i = 1; i < size; i++){

        if(gcd(i, size) == 1){
            candidats[index] = i;
            index++;
        }

        if(index == size / 2)
            break;

    }

    result = candidats[rand_r(seed) % index];

    free(candidats);
    candidats = NULL;

    return result;

}

struct graph** create_graph(uint16_t size){

    struct graph** result = (struct graph**)malloc(sizeof(struct graph*));
    if(result == NULL)
        return NULL;

    for(uint8_t i = 0; i < 3; i++){

        result[i] = (struct graph*)malloc(sizeof(struct graph));
        if(result == NULL)
            return NULL;

        result[i]->matrix = (uint16_t**)malloc(sizeof(uint16_t*) * size - 1);
        if(result[i]->matrix == NULL)
            return NULL;

        for(uint16_t j = 0; j < size - 1; j++){

            result[i]->matrix[j] = (uint16_t*)malloc(sizeof(uint16_t) * (j + 1));
            if(result[i]->matrix[j] == NULL)
            return NULL;

            for(uint16_t k = 0; k < i + 1; k++)
                result[i]->matrix[j][k] = 0;
            
        }

        result[i]->list = (struct connect*)malloc(sizeof(struct connect) * size);
        if(result[i]->list == NULL)
            return NULL;

    }

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

    uint16_t step = find_step(size, &seed);

    uint16_t p = 0, pn = step;

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        result->matrix[p][pn] = (rand_r(&seed) % 0xfffe) + 1;
        result->matrix[pn][p] = result->matrix[p][pn];

        p += step;
        pn += step;

    }

    return result;

}
