#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<omp.h>
#include<string.h>
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

    uint16_t r;

    do{
        r = rand_r(seed) % (size - 1) + 1;
    } while(gcd(r, size) != 1);
        
    return r;

}

void set_value_matrix(struct graph* graph, uint16_t start, uint16_t end, int16_t val, uint16_t edge_n){

    graph->matrix[start][edge_n] = val;
    graph->matrix[end][edge_n] = -val;

}

struct graph** create_graph(uint16_t size){

    struct graph** result = (struct graph**)malloc(sizeof(struct graph*) * 3);
    if(result == NULL)
        return NULL;

    for(uint8_t i = 0; i < 3; i++){

        result[i] = (struct graph*)malloc(sizeof(struct graph));
        if(result[i] == NULL)
            return NULL;


        result[i]->matrix = (int16_t**)malloc(sizeof(int16_t*) * size);
        if(result[i]->matrix == NULL)
            return NULL;


        result[i]->undir_list = (struct edge**)malloc(sizeof(struct edge*) * size);
        if(result[i]->undir_list == NULL)
            return NULL;


        result[i]->suc_list = (struct edge**)malloc(sizeof(struct edge*) * size); 
        if(result[i]->suc_list == NULL)
            return NULL;

    }

    unsigned int seed;
    FILE *f = fopen("/dev/urandom", "rb");
    if(f != NULL){
        if(fread(&seed, sizeof(seed), 1, f) != 1)   //Read failed
            seed = (unsigned int)time(NULL) + omp_get_thread_num();
        
        fclose(f);
    }else   //File open failed
        seed = (unsigned int)time(NULL) + omp_get_thread_num();
    

    uint16_t step = find_step(size, &seed);

    uint16_t p = 0, pn = step;

    result[0]->size = (uint16_t)((size * (size - 1) + 3) / 4);

    for(uint16_t i = 0; i < size; i++){

        result[0]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[0]->size);
        if(result[0]->matrix[i] == NULL)
            return NULL;

        memset(result[0]->matrix[i], 0, sizeof(int16_t) * result[0]->size);

    }

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        set_value_matrix(result[0], p, pn, (rand_r(&seed) % 32,766) + 1, i);

        p += step;
        pn += step;

    }


    step = find_step(size, &seed);

    p = 0, pn = step;

    result[1]->size = (uint16_t)((size * (size - 1) + 1) / 2);

    for(uint16_t i = 0; i < size; i++){

        result[1]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[1]->size);
        if(result[1]->matrix[i] == NULL)
            return NULL;

        memset(result[1]->matrix[i], 0, sizeof(int16_t) * result[1]->size);

    }

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        set_value_matrix(result[1], p, pn, (rand_r(&seed) % 32,766) + 1, i);

        p += step;
        pn += step;

    }

    
    result[2]->size = (uint16_t)((size * (size - 1)) * 99 + ((size * (size - 1)) * 99 + 99) / 100);

    for(uint16_t i = 0; i < size; i++){

        result[2]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[2]->size);
        if(result[2]->matrix[i] == NULL)
            return NULL;

        
        memset(result[2]->matrix[i], 0, sizeof(int16_t) * result[2]->size);

    }



    return result;

}
