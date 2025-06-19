#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<omp.h>
#include<string.h>
#include"graph.h"


uint16_t find_step(uint16_t size,  unsigned int* seed);


int16_t get_edge_matrix(int16_t** matrix, uint16_t start, uint16_t end, uint32_t edges){
    
    for(uint32_t i = 0; i < edges; i++){

        if(matrix[start][i] == 0)
            continue;

        if(matrix[end][i] != 0)
            return matrix[start][i];

    }

    return 0;

}


struct graph** create_graph(uint16_t size){

    struct graph** result = malloc(sizeof(struct graph*) * 3);
    if(!result){

        fprintf(stderr, "Failed to initialize RESULT table (create_graph)\n");
        return NULL;

    }

    for(uint8_t i = 0; i < 3; i++){

        result[i] = calloc(1, sizeof(struct graph));
        if(!result[i]){

            fprintf(stderr, "Failed to initialize RESULT[%i] table (create_graph)\n", i);
            return NULL;

        }


        result[i]->undir_matrix = malloc(sizeof(int16_t*) * size);
        if(!result[i]->undir_matrix){

            fprintf(stderr, "Failed to initialize RESULT[%i]->undir_matrix** table (create_graph)\n", i);
            return NULL;

        }

        result[i]->dir_matrix = malloc(sizeof(int16_t*) * size);
        if(!result[i]->dir_matrix){

            fprintf(stderr, "Failed to initialize RESULT[%i]->dir_matrix** table (create_graph)\n", i);
            return NULL;

        }

        result[i]->size = size;

    }


    // densities of created graphs
    const uint8_t dens_pct[3] = {25, 50, 99};


    unsigned int seed;
    FILE* f = fopen("/dev/urandom", "rb");
    if(f && fread(&seed, sizeof(seed), 1, f) == 1){

        fclose(f);

    } else{

        if(f)
            fclose(f);
        
        seed = (unsigned int)time(NULL) + omp_get_thread_num();

    }


    for(uint8_t i = 0; i < 3; i++){

        // calculates number of edges
        result[i]->edges = ((size * (size - 1)) * dens_pct[i] + 99) / 100;

        // allocates rest of matrix
        for(uint16_t r = 0; r < size; r++){

            result[i]->dir_matrix[r] = calloc(result[i]->edges, sizeof(int16_t));
            if(!result[i]->dir_matrix[r]){

                fprintf(stderr, "Failed to allocate dir_matrix[%d][%d] (create_graph)\n", i, r);
                return NULL;

            }

            result[i]->undir_matrix[r] = calloc(result[i]->edges / 2, sizeof(int16_t));
            if(!result[i]->undir_matrix[r]){

                fprintf(stderr, "Failed to allocate undir_matrix[%d][%d] (create_graph)\n", i, r);
                return NULL;

            }

        }


        // step is used to create path, that connects every vertex together
        // prevents disconected graphs
        uint16_t step = find_step(size, &seed);
        uint16_t p = 0, pn = step;

        for(uint16_t j = 0; j < size - 1; j++){

            pn %= size;
            p %= size;

            int16_t val = (rand_r(&seed) % (INT16_MAX - 1)) + 1;

            result[i]->dir_matrix[p][j] = val;
            result[i]->dir_matrix[pn][j] = -val;
            
            p += step;
            pn += step;

        }

        // fills rest of matrix with random edges
        for(uint16_t j = size - 1; j < result[i]->edges; j++){

            uint16_t x = rand_r(&seed) % size;
            uint16_t y = rand_r(&seed) % size;

            while(x == y)
                y = rand_r(&seed) % size;

            if(get_edge_matrix(result[i]->dir_matrix, x, y, result[i]->edges) == 0){

                int16_t val = (rand_r(&seed) % (INT16_MAX - 1)) + 1;

                result[i]->dir_matrix[p][j] = val;
                result[i]->dir_matrix[pn][j] = -val;

            }else 
                j--;
            
        }

        // repeated code for creating undirected graph
        uint16_t step = find_step(size, &seed);
        uint16_t p = 0, pn = step;

        for(uint16_t j = 0; j < size - 1; j++){

            pn %= size;
            p %= size;

            int16_t val = (rand_r(&seed) % (INT16_MAX - 1)) + 1;

            result[i]->undir_matrix[p][j] = val;
            result[i]->undir_matrix[pn][j] = val;
            
            p += step;
            pn += step;

        }

        // fills rest of matrix with random edges
        for(uint16_t j = size - 1; j < result[i]->edges / 2; j++){

            uint16_t x = rand_r(&seed) % size;
            uint16_t y = rand_r(&seed) % size;

            while(x == y)
                y = rand_r(&seed) % size;

            if(get_edge_matrix(result[i]->undir_matrix, x, y, result[i]->edges / 2) == 0){

                int16_t val = (rand_r(&seed) % (INT16_MAX - 1)) + 1;

                result[i]->undir_matrix[p][j] = val;
                result[i]->undir_matrix[pn][j] = val;

            }else 
                j--;
            
        }

        // code that takes matrix representation and makes list representation

    }

    return result;

}
