#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// funkcja implementująca algorytm Forda-Bellmana
struct result* Ford_Bellman(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    // alokacja tablicy wyników dla szybszego dostępu do nich
    struct result** node_array = malloc(sizeof(struct result*) * graph->size);
    if(!node_array){

        fprintf(stderr, "Alokacja tablicy wynikow nie wyszla (Ford-Bellman)\n");
        return NULL;
    }

    // inicjalizacja struktur wynikowych
    for(uint16_t i = 0; i < graph->size; i++){

        struct result* node = malloc(sizeof(struct result));
        if(!node){

            fprintf(stderr, "Alokacja wezelka spadla z rowerka (Ford-Bellman)\n");
            free_result(head);
            free(node_array);
            return NULL;

        }

        // domyślne wartości: nieskończona odległość, brak poprzednika
        node->weight = INT32_MAX;       // niepoprawna odległość
        node->start = graph->size;      // niepoprawny poprzednik
        node->end = i;                  // ID wierzchołka
        node->next = NULL;

        if(!head){
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }

        node_array[i] = node;

    }

    // odległość węzła źródłowego (0) = 0
    node_array[0]->weight = 0;
    
    // główna pętla algorytmu
    for(uint16_t pass = 1; pass < graph->size; pass++){

        // zlaga zmian - sprawdza czy dokonano relaksacji
        // jeżeli nie dokonano konczymy algorytm wcześniej
        uint8_t change = 0;

        // macierz: przeglądanie macierzy incydencji kolumnami
        if(mode == 0){

            for(uint32_t e = 0; e < graph->dir_edges; e++){

                int16_t w = 0;
                uint16_t u = graph->size, v = graph->size;

                for(uint16_t i = 0; i < graph->size; i++){

                    int16_t val = graph->dir_matrix[i][e];

                    if(val > 0){

                        u = i; 
                        w = val;

                    }else if(val < 0)
                        v = i;
                
                }

                // sprawdza czy znaleziono poprawną krawędź i czy wierzchołek u ma ustaloną odległość
                if(u < graph->size && v < graph->size && node_array[u]->weight != INT32_MAX){

                    // nowa odległość
                    int32_t nd = node_array[u]->weight + w;

                    // relaksacja krawędzi
                    if(nd < node_array[v]->weight){

                        node_array[v]->weight = nd;
                        node_array[v]->start  = u;
                        change = 1;

                    }

                }

            }

        }else {

            // lista: przeglądanie list sąsiedztwa
            for(uint16_t u = 0; u < graph->size; u++){

                struct edge* e = graph->dir_list[u];

                // przejchodzi przez wszystkie krawędzie wychodzące
                while(e){

                    uint16_t v = e->target;

                    if(node_array[u]->weight != INT32_MAX){

                        // nowa odległość
                        int32_t nd = node_array[u]->weight + e->weight;

                        // relaksacja krawędzi
                        if(nd < node_array[v]->weight){

                            node_array[v]->weight = nd;
                            node_array[v]->start  = u;
                            change = 1;

                        }

                    }

                    e = e->next;

                }

            }

        }

        // kończy wcześniej jeśli nie było zmian w tej iteracji
        if(!change)
            break;

    }

    free(node_array);
    return head;

}
