#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<omp.h>
#include<string.h>
#include"graph.h"
#include"matrix_io.h"

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

struct graph** create_graph(uint16_t size){

    struct graph** result = (struct graph**)malloc(sizeof(struct graph*) * 3);
    if(result == NULL)
        return NULL;

    for(uint8_t i = 0; i < 3; i++){

        result[i] = (struct graph*)malloc(sizeof(struct graph));
        if(result[i] == NULL)
            return NULL;

        result[i]->matrix = (uint16_t**)malloc(sizeof(uint16_t*) * (size - 1));
        if(result[i]->matrix == NULL)
            return NULL;

        for(uint16_t j = 0; j < size - 1; j++){

            result[i]->matrix[j] = (uint16_t*)malloc(sizeof(uint16_t) * (j + 1));
            if(result[i]->matrix[j] == NULL)
            return NULL;

            memset(result[i]->matrix[j], 0, sizeof(uint16_t) * (j + 1));

        }

        result[i]->list = (struct connect**)malloc(sizeof(struct connect*) * size);
        if(result[i]->list == NULL)
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
    

    uint16_t step1 = find_step(size, &seed);
    uint16_t step2 = find_step(size, &seed);

    uint16_t p1 = 0, pn1 = step1;
    uint16_t p2 = 0, pn2 = step2;

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn1 > size)
            pn1 %= size;

        if(p1 > size)
            p1 %= size;

        set_value_matrix(result[0], p1, pn1, (rand_r(&seed) % 0xfffe) + 1);

        p1 += step1;
        pn1 += step1;


        if(pn2 > size)
            pn2 %= size;

        if(p2 > size)
            p2 %= size;

        set_value_matrix(result[1], p2, pn2, (rand_r(&seed) % 0xfffe) + 1);

        p2 += step2;
        pn2 += step2;

    }

    uint16_t fill_per = (uint16_t)(0.25 * (size * (size - 1) / 2) - (size - 1));

    do{

        uint16_t x = rand_r(&seed) % size;
        uint16_t y = rand_r(&seed) % size;
        
        while(y == x)
            y = rand_r(&seed) % size;

        if(get_value_matrix(result[0], x, y) == 0){

            set_value_matrix(result[0], x, y, (rand_r(&seed) % 0xfffe) + 1);
            fill_per--;

        }
        

    }while(fill_per > 0);

    fill_per = (uint16_t)(0.5 * (size * (size - 1) / 2) - (size - 1));

    do{

        uint16_t x = rand_r(&seed) % size;
        uint16_t y = rand_r(&seed) % size;
        
        while(y == x)
            y = rand_r(&seed) % size;

        if(get_value_matrix(result[1], x, y) == 0){

            set_value_matrix(result[1], x, y, (rand_r(&seed) % 0xfffe) + 1);
            fill_per--;

        }
        

    }while(fill_per > 0);

    for(uint16_t i = 0; i < size - 1; i++){

        for(uint16_t j = 0; j < i + 1; j++)
            set_value_matrix(result[2], i, j, (rand_r(&seed) % 0xfffe) + 1);

    }

    fill_per = (uint16_t)(0.01 * (size * (size - 1) / 2));

    for(uint16_t i = 0; i < fill_per; i++){

        uint16_t x = rand_r(&seed) % size;
        uint16_t y = rand_r(&seed) % size;
        
        while(y == x)
            y = rand_r(&seed) % size;

        if(get_value_matrix(result[1], x, y) != 0)
            set_value_matrix(result[2], x, y, 0);

    }



    return result;

}
