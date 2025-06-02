#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<omp.h>
#include<string.h>
#include"graph.h"

uint16_t find_step(uint16_t size,  unsigned int* seed);

void add_edge_matrix(struct graph* graph, uint16_t start, uint16_t end, int16_t val, uint16_t edge_n);

int16_t get_edge_matrix(struct graph* graph, uint16_t start, uint16_t end);

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


        result[i]->undir_list = (struct edge**)malloc(sizeof(struct edge*));
        if(result[i]->undir_list == NULL)
            return NULL;


        result[i]->suc_list = (struct edge**)malloc(sizeof(struct edge*)); 
        if(result[i]->suc_list == NULL)
            return NULL;

        result[i]->size = size;

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

    result[0]->edges = (uint16_t)((size * (size - 1) + 3) / 4);

    for(uint16_t i = 0; i < size; i++){

        result[0]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[0]->edges);
        if(result[0]->matrix[i] == NULL)
            return NULL;

        memset(result[0]->matrix[i], 0, sizeof(int16_t) * result[0]->edges);

    }

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        add_edge_matrix(result[0], p, pn, (rand_r(&seed) % 32766) + 1, i);

        p += step;
        pn += step;

    }

    for(uint16_t i = (size - 1); i < result[0]->edges; i++){

        uint16_t x = rand_r(&seed) % size;
        uint16_t y = rand_r(&seed) % size;

        while(x == y)
            y = rand_r(&seed) % size;

        if(get_edge_matrix(result[0], x, y) == 0)
            add_edge_matrix(result[0], x, y, (rand_r(&seed) % 32766) + 1, i);
        
        else 
            i--;

    }


    step = find_step(size, &seed);

    p = 0, pn = step;

    result[1]->edges = (uint16_t)((size * (size - 1) + 1) / 2);

    for(uint16_t i = 0; i < size; i++){

        result[1]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[1]->edges);
        if(result[1]->matrix[i] == NULL)
            return NULL;

        memset(result[1]->matrix[i], 0, sizeof(int16_t) * result[1]->edges);

    }

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        add_edge_matrix(result[1], p, pn, (rand_r(&seed) % 32766) + 1, i);

        p += step;
        pn += step;

    }

    for(uint16_t i = (size - 1); i < result[1]->edges; i++){

        uint16_t x = rand_r(&seed) % size;
        uint16_t y = rand_r(&seed) % size;

        while(x == y)
            y = rand_r(&seed) % size;

        if(get_edge_matrix(result[1], x, y) == 0)
            add_edge_matrix(result[1], x, y, (rand_r(&seed) % 32766) + 1, i);
        
        else 
            i--;

    }

    
    result[2]->edges = (uint16_t)((size * (size-1) * 99 + 99) / 100);

    for(uint16_t i = 0; i < size; i++){

        result[2]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[2]->edges);
        if(result[2]->matrix[i] == NULL)
            return NULL;

        
        memset(result[2]->matrix[i], 0, sizeof(int16_t) * result[2]->edges);

    }

    step = size * (size-1) - result[2]->edges;
    uint16_t nx[step], ny[step];

    for(uint16_t i = 0; i < step; i++){

        nx[i] = rand_r(&seed) % size;
        ny[i] = rand_r(&seed) % size;

        while(nx[i] == ny[i])
            ny[i] = rand_r(&seed) % size;

        
        for(uint16_t j = 0; j < i; j++){

            if(nx[j] == nx[i] && ny[j] == ny[i]){

                nx[i] = rand_r(&seed) % size;
                ny[i] = rand_r(&seed) % size;

                while(nx[i] == ny[i])
                    ny[i] = rand_r(&seed) % size;

                j = 0;

            }

        }

    }

    for(uint16_t i = 0; i < result[2]->edges; i++){

        for(uint16_t j = 0; j < size; j++){

            for(uint16_t k = 0; k < size; k++){

                p = 0;

                for(uint16_t l = 0; l < step; l++){
            
                    if(j == nx[l] && k == ny[l]){
                        p = 1;
                        break;
                    }

                }
                
                if(j == k || p != 0)
                    continue;

                add_edge_matrix(result[2], j, k, (rand_r(&seed) % 32766) + 1, i);

            }

        }

    }


    return result;

}
