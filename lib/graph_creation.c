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
    if(result == NULL){

        fprintf(stderr, "Failed to initialize RESULT table (create_graph)\n");
        return NULL;

    }

    for(uint8_t i = 0; i < 3; i++){

        result[i] = (struct graph*)malloc(sizeof(struct graph));
        if(result[i] == NULL){

            fprintf(stderr, "Failed to initialize RESULT[%i] table (create_graph)\n", i);
            return NULL;

        }


        result[i]->matrix = (int16_t**)malloc(sizeof(int16_t*) * size);
        if(result[i]->matrix == NULL){

            fprintf(stderr, "Failed to initialize RESULT[%i]->MATRIX** table (create_graph)\n", i);
            return NULL;

        }

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
        if(result[0]->matrix[i] == NULL){

            fprintf(stderr, "Failed to initialize RESULT[0]->MATRIX[%i]* table (create_graph)\n", i);
            return NULL;

        }

        memset(result[0]->matrix[i], 0, sizeof(int16_t) * result[0]->edges);

    }

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        add_edge_matrix(result[0], p, pn, (rand_r(&seed) % (INT16_MAX - 1)) + 1, i);

        p += step;
        pn += step;

    }

    for(uint16_t i = (size - 1); i < result[0]->edges; i++){

        uint16_t x = rand_r(&seed) % size;
        uint16_t y = rand_r(&seed) % size;

        while(x == y)
            y = rand_r(&seed) % size;

        if(get_edge_matrix(result[0], x, y) == 0)
            add_edge_matrix(result[0], x, y, (rand_r(&seed) % (INT16_MAX - 1)) + 1, i);
        
        else 
            i--;

    }


    step = find_step(size, &seed);

    p = 0, pn = step;

    result[1]->edges = (uint16_t)((size * (size - 1) + 1) / 2);

    for(uint16_t i = 0; i < size; i++){

        result[1]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[1]->edges);
        if(result[1]->matrix[i] == NULL){

            fprintf(stderr, "Failed to initialize RESULT[1]->MATRIX[%i]* table (create_graph)\n", i);
            return NULL;

        }

        memset(result[1]->matrix[i], 0, sizeof(int16_t) * result[1]->edges);

    }

    for(uint16_t i = 0; i < (size - 1); i++){

        if(pn > size)
            pn %= size;

        if(p > size)
            p %= size;

        add_edge_matrix(result[1], p, pn, (rand_r(&seed) % (INT16_MAX - 1)) + 1, i);

        p += step;
        pn += step;

    }

    for(uint16_t i = (size - 1); i < result[1]->edges; i++){

        uint16_t x = rand_r(&seed) % size;
        uint16_t y = rand_r(&seed) % size;

        while(x == y)
            y = rand_r(&seed) % size;

        if(get_edge_matrix(result[1], x, y) == 0)
            add_edge_matrix(result[1], x, y, (rand_r(&seed) % (INT16_MAX - 1)) + 1, i);
        
        else 
            i--;

    }

    
    result[2]->edges = (uint16_t)((size * (size-1) * 99 + 99) / 100);

    for(uint16_t i = 0; i < size; i++){

        result[2]->matrix[i] = (int16_t*)malloc(sizeof(int16_t) * result[2]->edges);
        if(result[2]->matrix[i] == NULL){

            fprintf(stderr, "Failed to initialize RESULT[2]->MATRIX[%i]* table (create_graph)\n", i);
            return NULL;

        }

        
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

    pn = 0;
    while(pn < result[2]->edges){

        for(uint16_t i = 0; i < size; i++){

            for(uint16_t j = 0; j < size; j++){

                p = 0;

                for(uint16_t k = 0; k < step; k++){
            
                    if(i == nx[k] && j == ny[k]){
                        p = 1;
                        break;
                    }

                }
                
                if(i == j || p != 0)
                    continue;

                add_edge_matrix(result[2], i, j, (rand_r(&seed) % (INT16_MAX - 1)) + 1, pn++);

            }

        }

    }

    for(uint8_t i = 0; i < 3; i++){

        result[i]->undir_list = (struct edge**)malloc(sizeof(struct edge*) * result[i]->size);
        if(result[i]->undir_list == NULL){

            fprintf(stderr, "Failed to initialize RESULT[%i]->UNDIR_LIST** table (create_graph)\n", i);
            return NULL;

        }

        result[i]->suc_list = (struct edge**)malloc(sizeof(struct edge*) * result[i]->size); 
        if(result[i]->suc_list == NULL){

            fprintf(stderr, "Failed to initialize RESULT[%i]->SUC_LIST** table (create_graph)\n", i);
            return NULL;

        }

        for(uint16_t j = 0; j < result[i]->size; j++){

            result[i]->undir_list[j] = NULL;
            result[i]->suc_list[j] = NULL;
            
        }
        
        for(uint16_t j = 0; j < result[i]->edges; j++){

            int16_t plus = 0;
            int16_t plus_i = 0, minus_i = 0;

            for(uint16_t k = 0; k < result[i]->size; k++){

                if(result[i]->matrix[k][j] > 0){
                    plus = result[i]->matrix[k][j];
                    plus_i = k;
                }

                if(result[i]->matrix[k][j] < 0)
                    minus_i = k;
                
            }
            

            struct edge* beg = result[i]->suc_list[plus_i];
            if(beg == NULL){

                beg = (struct edge*)malloc(sizeof(struct edge));

                if(beg == NULL){

                    fprintf(stderr, "Failed to inicialize RESULT[%i]->SUC_LIST[%i]* table (create_graph)\n", i, plus_i);
                    return NULL;

                }

                result[i]->suc_list[plus_i] = beg;
            }
            else {

                while(beg->next != NULL) 
                    beg = beg->next;
                
                beg->next = (struct edge*)malloc(sizeof(struct edge));

                if(beg->next == NULL){

                    fprintf(stderr, "Failed to initialize RESULT[%i]->SUC_LIST[%i]* table (create_graph)\n", i, plus_i);
                    return NULL;

                }

                beg = beg->next;

            }


            struct edge* beg_u = result[i]->undir_list[plus_i];
            if(beg_u == NULL){

                beg_u = (struct edge*)malloc(sizeof(struct edge));

                if(beg_u == NULL){

                    fprintf(stderr, "Failed to initialize RESULT[%i]->UNDIR_LIST[%i]* begin table (create_graph)\n", i, plus_i);
                    return NULL;

                }

                result[i]->undir_list[plus_i] = beg_u;
            }
            else {

                while(beg_u->next != NULL) 
                    beg_u = beg_u->next;
                
                beg_u->next = (struct edge*)malloc(sizeof(struct edge));

                if(beg_u->next == NULL){

                    fprintf(stderr, "Failed to initialize RESULT[%i]->UNDIR_LIST[%i]* begin table (create_graph)\n", i, plus_i);
                    return NULL;

                }

                beg_u = beg_u->next;
                
            }

            struct edge* end_u = result[i]->undir_list[minus_i];
            if(end_u == NULL){

                end_u = (struct edge*)malloc(sizeof(struct edge));

                if (end_u == NULL){

                    fprintf(stderr, "Failed to initialize RESULT[%i]->UNDIR_LIST[%i]* end table (create_graph)\n", i, plus_i);
                    return NULL;

                }

                result[i]->undir_list[minus_i] = end_u;
            }
            else {

                while(end_u->next != NULL) 
                    end_u = end_u->next;
                
                end_u->next = (struct edge*)malloc(sizeof(struct edge));

                if(end_u->next == NULL){

                    fprintf(stderr, "Failed to initialize RESULT[%i]->UNDIR_LIST[%i]* end table (create_graph)\n", i, plus_i);
                    return NULL;

                }

                end_u = end_u->next;
                
            }

            beg->target = minus_i;
            beg_u->target = minus_i;
            end_u->target = plus_i;

            beg->weight = plus;
            beg_u->weight = plus;
            end_u->weight = plus;

            beg->next = NULL;
            beg_u->next = NULL;
            end_u->next = NULL;

        }

    }

    return result;

}
