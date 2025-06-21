#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<omp.h>
#include<string.h>
#include"graph.h"


uint16_t find_step(uint16_t size,  unsigned int* seed);


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
        result[i]->dir_edges = ((uint32_t)size * (size - 1) * dens_pct[i] + 99) / 100;
        result[i]->undir_edges = ((uint32_t)size * (size - 1) / 2 * dens_pct[i] + 99) / 100;

        // allocates rest of matrix
        for(uint16_t r = 0; r < size; r++){

            result[i]->dir_matrix[r] = calloc(result[i]->dir_edges, sizeof(int16_t));
            if(!result[i]->dir_matrix[r]){

                fprintf(stderr, "Failed to allocate dir_matrix[%d][%d] (create_graph)\n", i, r);
                return NULL;

            }

            result[i]->undir_matrix[r] = calloc(result[i]->undir_edges, sizeof(int16_t));
            if(!result[i]->undir_matrix[r]){

                fprintf(stderr, "Failed to allocate undir_matrix[%d][%d] (create_graph)\n", i, r);
                return NULL;

            }

        }


        // Optimized directed graph creation with bitset
        size_t dir_bitset_bytes = ((size_t)size * size + 7) / 8;
        uint8_t *dir_exists = calloc(1, dir_bitset_bytes);
        if(!dir_exists){

            fprintf(stderr, "Failed to allocate dir_exists bitset (create_graph)\n");
            return NULL;

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

            uint32_t index = (uint32_t)p * size + pn;
            dir_exists[index / 8] |= (1 << (index % 8));
            
            p += step;
            pn += step;

        }

        // fills rest of matrix with random edges
        for(uint16_t j = size - 1; j < result[i]->dir_edges; j++){

            uint16_t x = rand_r(&seed) % size;
            uint16_t y = rand_r(&seed) % size;

            while(x == y)
                y = rand_r(&seed) % size;

            uint32_t index = (uint32_t)x * size + y;

            if(dir_exists[index / 8] & (1 << (index % 8))){

                j--;

            }else {

                int16_t val = (rand_r(&seed) % (INT16_MAX - 1)) + 1;

                result[i]->dir_matrix[x][j] = val;
                result[i]->dir_matrix[y][j] = -val;

                dir_exists[index / 8] |= (1 << (index % 8));

            }
            
        }

        free(dir_exists);

        // Optimized undirected graph creation with bitset
        size_t undir_bitset_bytes = ((size_t)size * size + 7) / 8;
        uint8_t *undir_exists = calloc(1, undir_bitset_bytes);
        if(!undir_exists){

            fprintf(stderr, "Failed to allocate undir_exists bitset (create_graph)\n");
            return NULL;

        }

        // repeated code for creating undirected graph
        step = find_step(size, &seed);
        p = 0; 
        pn = step;

        for(uint16_t j = 0; j < size - 1; j++){

            pn %= size;
            p %= size;

            int16_t val = (rand_r(&seed) % (INT16_MAX - 1)) + 1;

            result[i]->undir_matrix[p][j] = val;
            result[i]->undir_matrix[pn][j] = val;

            uint16_t u1 = p < pn ? p : pn;
            uint16_t v1 = p < pn ? pn : p;
            uint32_t index = (uint32_t)u1 * size + v1;
            undir_exists[index / 8] |= (1 << (index % 8));
            
            p += step;
            pn += step;

        }

        // fills rest of matrix with random edges
        for(uint16_t j = size - 1; j < result[i]->undir_edges; j++){

            uint16_t x = rand_r(&seed) % size;
            uint16_t y = rand_r(&seed) % size;

            while(x == y)
                y = rand_r(&seed) % size;

            uint16_t u1 = x < y ? x : y;
            uint16_t v1 = x < y ? y : x;
            uint32_t index = (uint32_t)u1 * size + v1;

            if(undir_exists[index / 8] & (1 << (index % 8))){

                j--;

            }else {

                int16_t val = (rand_r(&seed) % (INT16_MAX - 1)) + 1;

                result[i]->undir_matrix[x][j] = val;
                result[i]->undir_matrix[y][j] = val;

                undir_exists[index / 8] |= (1 << (index % 8));

            }
            
        }

        free(undir_exists);

        // code that takes matrix representation and makes list representation
        result[i]->dir_list = malloc(sizeof(struct edge*) * size);
        result[i]->undir_list = malloc(sizeof(struct edge*) * size);

        for(uint16_t v = 0; v < size; v++){

            result[i]->dir_list[v] = NULL;
            result[i]->undir_list[v] = NULL;

        }

        // Build directed adjacency list from directed incident matrix
        for(uint32_t j = 0; j < result[i]->dir_edges; j++){

            uint16_t start = size, end = size;
            int16_t weight = 0;

            for(uint16_t u = 0; u < size; u++){

                if(result[i]->dir_matrix[u][j] > 0){

                    start = u;
                    weight = result[i]->dir_matrix[u][j];

                }else if(result[i]->dir_matrix[u][j] < 0)
                    end = u;

            }

            if(start < size && end < size){

                struct edge* new_edge = malloc(sizeof(struct edge));
                if(!new_edge){

                    fprintf(stderr, "Failed to allocate edge (directed)\n");
                    return NULL;

                }

                new_edge->target = end;
                new_edge->weight = weight;
                new_edge->next = result[i]->dir_list[start];
                result[i]->dir_list[start] = new_edge;

            }

        }

        // Build undirected adjacency list from undirected incident matrix
        for(uint32_t j = 0; j < result[i]->undir_edges; j++){

            uint16_t u1 = size, u2 = size;
            int16_t weight = 0;
            int count = 0;

            for(uint16_t u = 0; u < size; u++){

                if(result[i]->undir_matrix[u][j] != 0){
                    if(count == 0){

                        u1 = u;
                        weight = result[i]->undir_matrix[u][j];
                        count++;

                    }else if(count == 1){

                        u2 = u;
                        count++;
                        break;

                    }

                }

            }

            if(count == 2){

                // Edge u1 -> u2
                struct edge* new_edge1 = malloc(sizeof(struct edge));
                if(!new_edge1){

                    fprintf(stderr, "Failed to allocate edge (undirected)\n");
                    return NULL;

                }

                new_edge1->target = u2;
                new_edge1->weight = weight;
                new_edge1->next = result[i]->undir_list[u1];
                result[i]->undir_list[u1] = new_edge1;

                // Edge u2 -> u1
                struct edge* new_edge2 = malloc(sizeof(struct edge));
                if(!new_edge2){

                    fprintf(stderr, "Failed to allocate edge (undirected)\n");
                    free(new_edge1);
                    return NULL;

                }

                new_edge2->target = u1;
                new_edge2->weight = weight;
                new_edge2->next = result[i]->undir_list[u2];
                result[i]->undir_list[u2] = new_edge2;
                
            }

        }

    }

    return result;

}
