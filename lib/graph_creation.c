#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>
#include<time.h>
#include<omp.h>
#include<string.h>
#include"graph.h"

// funckja wykorzystywana do utworzenia ścieżki Hamiltona w grafie
uint16_t find_step(uint16_t size,  unsigned int* seed);

// struktura trzymająca krawędź (wykorzystywana lokalnie)
struct edge_pair {

    uint16_t x;
    uint16_t y;

};

// główna funkcja tworząca losowe grafy o 3 gęstościach (25%, 50%, 99%)
struct graph** create_graph(uint16_t size){

    // allokacja pamięci pod struktury
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


    // gęstości grafów
    const uint8_t dens_pct[3] = {25, 50, 99};

    // wyznaczenie nasiona dla funkcji rand_r()
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

        // wyznaczenie liczby krawędzi w grafie
        result[i]->dir_edges = ((uint32_t)size * (size - 1) * dens_pct[i] + 99) / 100;
        result[i]->undir_edges = ((uint32_t)size * (size - 1) / 2 * dens_pct[i] + 99) / 100;

        // allokacja pamięci dla macierzy
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

        // --- FABRYKA GRAFU SKIEROWANEGO ---
        uint32_t total_dir_possible = (uint32_t)size * (size - 1);
        uint16_t step = find_step(size, &seed);
        uint16_t p = 0, pn = step;

        // alokacja pamięci pod ścieżkę Hamiltona
        struct edge_pair* dir_cycle_edges = malloc((size-1) * sizeof(struct edge_pair));
        if(!dir_cycle_edges){

            fprintf(stderr, "Failed to allocate dir_cycle_edges\n");
            return NULL;

        }

        // wyznaczenie tejże ścieżki za pomocą "kroku" (liczby względnie pierwszej do ilości wierzchołków)
        for(uint16_t j = 0; j < size - 1; j++){

            p %= size;
            pn %= size;
            dir_cycle_edges[j].x = p;
            dir_cycle_edges[j].y = pn;
            p += step;
            pn += step;

        }

        // alokacja pamięci pod tablice wszystkich możliwych skierowanych krawędzie
        struct edge_pair* all_dir_edges = malloc(total_dir_possible * sizeof(struct edge_pair));
        if(!all_dir_edges){

            fprintf(stderr, "Failed to allocate all_dir_edges\n");
            free(dir_cycle_edges);
            return NULL;

        }

        // wyznaczenie tych krawędzie
        uint32_t count = 0;
        for(uint16_t x = 0; x < size; x++){

            for(uint16_t y = 0; y < size; y++){

                if(x != y){

                    all_dir_edges[count].x = x;
                    all_dir_edges[count].y = y;
                    count++;

                }

            }

        }

        // alokacja pamięci pod zaznaczenie krawędzi wykorzystany w ścieżce Hamiltona
        size_t bitset_size = (size_t)size * size;
        uint8_t* dir_cycle_bitset = calloc(1, (bitset_size + 7) / 8);
        if(!dir_cycle_bitset){

            fprintf(stderr, "Failed to allocate dir_cycle_bitset\n");
            free(dir_cycle_edges);
            free(all_dir_edges);
            return NULL;
            
        }

        // zaznaczenie tych krawędzi
        for(uint16_t j = 0; j < size - 1; j++){

            uint16_t x = dir_cycle_edges[j].x;
            uint16_t y = dir_cycle_edges[j].y;
            uint32_t index = (uint32_t)x * size + y;
            dir_cycle_bitset[index / 8] |= (1 << (index % 8));

        }

        // alokacja pamięci pod wszystkie krawędzie - ścieżka Hamiltona
        uint32_t non_cycle_dir_count = total_dir_possible - (size - 1);
        struct edge_pair* non_cycle_dir_edges = malloc(non_cycle_dir_count * sizeof(struct edge_pair));
        if(!non_cycle_dir_edges){

            fprintf(stderr, "Failed to allocate non_cycle_dir_edges\n");
            free(dir_cycle_edges);
            free(all_dir_edges);
            free(dir_cycle_bitset);
            return NULL;

        }

        // wpisanie tych krawędzi
        uint32_t non_cycle_idx = 0;
        for(uint32_t j = 0; j < total_dir_possible; j++){

            uint16_t x = all_dir_edges[j].x;
            uint16_t y = all_dir_edges[j].y;
            uint32_t index = (uint32_t)x * size + y;

            if(!(dir_cycle_bitset[index / 8] & (1 << (index % 8)))) 
                non_cycle_dir_edges[non_cycle_idx++] = all_dir_edges[j];
            
        }

        // czyszczenie niepotrzebnych tablic
        free(all_dir_edges);
        free(dir_cycle_bitset);

        // tasowanie wyznaczonych krawędzi
        for(uint32_t j = non_cycle_dir_count - 1; j > 0; j--){

            uint32_t k = rand_r(&seed) % (j + 1);
            struct edge_pair tmp = non_cycle_dir_edges[j];
            non_cycle_dir_edges[j] = non_cycle_dir_edges[k];
            non_cycle_dir_edges[k] = tmp;

        }

        // przypisanie krawędzi zawartych w ścieżce do macierzy
        for(uint16_t j = 0; j < size - 1; j++){

            uint16_t x = dir_cycle_edges[j].x;
            uint16_t y = dir_cycle_edges[j].y;
            int16_t weight = (rand_r(&seed) % (INT16_MAX - 1)) + 1;
            result[i]->dir_matrix[x][j] = weight;
            result[i]->dir_matrix[y][j] = -weight;

        }

        // przypisanie pozostałych krawędzi do macierzy poprzez wybór z tablicy dostępnych krawędzi
        uint32_t non_cycle_needed = result[i]->dir_edges - (size - 1);
        for(uint32_t j = 0; j < non_cycle_needed; j++){

            uint16_t x = non_cycle_dir_edges[j].x;
            uint16_t y = non_cycle_dir_edges[j].y;
            int16_t weight = (rand_r(&seed) % (INT16_MAX - 1)) + 1;
            uint32_t col = size - 1 + j;
            result[i]->dir_matrix[x][col] = weight;
            result[i]->dir_matrix[y][col] = -weight;

        }

        // czyszczenie pozostałości
        free(dir_cycle_edges);
        free(non_cycle_dir_edges);


        // --- FABRYKA GRAFU NIESKIEROWANEGO ---
        // (praktycznie identyczna do fabryki grafy skierowanego)
        uint32_t total_undir_possible = (uint32_t)size * (size - 1) / 2;
        step = find_step(size, &seed);
        p = 0;
        pn = step;

        // alokacja pamięci pod ścieżkę Hamiltona
        struct edge_pair* undir_cycle_edges = malloc((size - 1) * sizeof(struct edge_pair));
        if(!undir_cycle_edges){

            fprintf(stderr, "Failed to allocate undir_cycle_edges\n");
            return NULL;

        }

        // wyznaczenie tejże ścieżki z pomocą liczby względnie pierwszej (step)
        for(uint16_t j = 0; j < size - 1; j++){

            p %= size;
            pn %= size;
            uint16_t u1 = p < pn ? p : pn;
            uint16_t v1 = p < pn ? pn : p;
            undir_cycle_edges[j].x = u1;
            undir_cycle_edges[j].y = v1;
            p += step;
            pn += step;

        }

        // alokacja pamięci od tablicę wszystkich możliwych nieskierowanych krawędzi
        struct edge_pair* all_undir_edges = malloc(total_undir_possible * sizeof(struct edge_pair));
        if(!all_undir_edges){

            fprintf(stderr, "Failed to allocate all_undir_edges\n");
            free(undir_cycle_edges);
            return NULL;

        }

        // zapełnienie tejże tablicy
        count = 0;
        for(uint16_t u = 0; u < size; u++){

            for(uint16_t v = u + 1; v < size; v++){

                all_undir_edges[count].x = u;
                all_undir_edges[count].y = v;
                count++;

            }

        }

        // alokacja pamięci pod zaznaczenie krawędzi wykorzystany w ścieżce Hamiltona
        bitset_size = (size_t)size * size;
        uint8_t* undir_cycle_bitset = calloc(1, (bitset_size + 7) / 8);
        if(!undir_cycle_bitset){

            fprintf(stderr, "Failed to allocate undir_cycle_bitset\n");
            free(undir_cycle_edges);
            free(all_undir_edges);
            return NULL;

        }

        // zaznaczenie krawędzi wykorzystanych w ścieżce
        for(uint16_t j = 0; j < size - 1; j++){

            uint16_t u = undir_cycle_edges[j].x;
            uint16_t v = undir_cycle_edges[j].y;
            uint32_t index = (uint32_t)u * size + v;
            undir_cycle_bitset[index / 8] |= (1 << (index % 8));

        }

        // alokacja pamięci pod tablicę możliwych krawędzi (wszyskie - ścieżka)
        uint32_t non_cycle_undir_count = total_undir_possible - (size - 1);
        struct edge_pair* non_cycle_undir_edges = malloc(non_cycle_undir_count * sizeof(struct edge_pair));
        if(!non_cycle_undir_edges){

            fprintf(stderr, "Failed to allocate non_cycle_undir_edges\n");
            free(undir_cycle_edges);
            free(all_undir_edges);
            free(undir_cycle_bitset);
            return NULL;

        }

        // zapełnienie tejże tablicy
        non_cycle_idx = 0;
        for(uint32_t j = 0; j < total_undir_possible; j++){

            uint16_t u = all_undir_edges[j].x;
            uint16_t v = all_undir_edges[j].y;
            uint32_t index = (uint32_t)u * size + v;
            if(!(undir_cycle_bitset[index / 8] & (1 << (index % 8))))
                non_cycle_undir_edges[non_cycle_idx++] = all_undir_edges[j];
            
        }

        // czyszczenie niepotrzebnych rzeczy
        free(all_undir_edges);
        free(undir_cycle_bitset);

        // tasowanie tablicy dostępnych krawędzi
        for(uint32_t j = non_cycle_undir_count - 1; j > 0; j--){

            uint32_t k = rand_r(&seed) % (j + 1);
            struct edge_pair tmp = non_cycle_undir_edges[j];
            non_cycle_undir_edges[j] = non_cycle_undir_edges[k];
            non_cycle_undir_edges[k] = tmp;

        }

        // przypisanie krawędzi zawartych w ścieżce do macierzy
        for(uint16_t j = 0; j < size - 1; j++){

            uint16_t u = undir_cycle_edges[j].x;
            uint16_t v = undir_cycle_edges[j].y;
            int16_t weight = (rand_r(&seed) % (INT16_MAX - 1)) + 1;
            result[i]->undir_matrix[u][j] = weight;
            result[i]->undir_matrix[v][j] = weight;

        }

        // przypisanie pozostałych krawędzi do macierzy poprzez wybór z tablicy dostępnych krawędzi
        uint32_t non_cycle_needed_undir = result[i]->undir_edges - (size - 1);
        for(uint32_t j = 0; j < non_cycle_needed_undir; j++){

            uint16_t u = non_cycle_undir_edges[j].x;
            uint16_t v = non_cycle_undir_edges[j].y;
            int16_t weight = (rand_r(&seed) % (INT16_MAX - 1)) + 1;
            uint32_t col = size - 1 + j;
            result[i]->undir_matrix[u][col] = weight;
            result[i]->undir_matrix[v][col] = weight;
            
        }

        // sprzątanko po skonczonym abrykowaniu
        free(undir_cycle_edges);
        free(non_cycle_undir_edges);

        // --- FABRYKA LIST Z MACIERZY ---
        result[i]->dir_list = malloc(sizeof(struct edge*) * size);
        result[i]->undir_list = malloc(sizeof(struct edge*) * size);

        for(uint16_t v = 0; v < size; v++){

            result[i]->dir_list[v] = NULL;
            result[i]->undir_list[v] = NULL;

        }

        // buduje skierowaną listę sąsiedztwa z skierowanej macieży incydencji
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

        // buduje nieskierowaną listę sąsiedztwa z nieskierowanej macieży incydencji
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

                // krawędź u1 -> u2
                struct edge* new_edge1 = malloc(sizeof(struct edge));
                if(!new_edge1){

                    fprintf(stderr, "Failed to allocate edge (undirected)\n");
                    return NULL;

                }

                new_edge1->target = u2;
                new_edge1->weight = weight;
                new_edge1->next = result[i]->undir_list[u1];
                result[i]->undir_list[u1] = new_edge1;

                // krawędź u2 -> u1
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
