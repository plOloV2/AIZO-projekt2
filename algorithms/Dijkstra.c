#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// funkcja implementująca algorytm Dijkstry
struct result* Dijkstra(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    // alokacja tablicy wyników dla szybszego dostępu do nich
    struct result** node_array = malloc(sizeof(struct result*) * graph->size);
    if(!node_array){

        fprintf(stderr, "Alokacja tablicy wynikow nie wyszla (Dijkstra)\n");
        return NULL;
    }

    // inicjalizacja struktur wynikowych
    for(uint16_t i = 0; i < graph->size; i++){

        struct result* node = calloc(1, sizeof(struct result));
        if(!node){

            fprintf(stderr, "Alokacja wezelka spadla z rowerka (Dijkstra)\n");
            free_result(head);
            free(node_array);
            return NULL;

        }

        // domyślne wartości: nieskończona odległość, brak poprzednika
        node->weight = INT32_MAX;       // niepoprawna odległość
        node->end = graph->size;        // niepoprawny poprzednik

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
    for(uint16_t i = 0; i < graph->size; i++){

        int32_t best_w = INT32_MAX;         // najlepsza (najmniejsza) znana odległość
        uint16_t min_index = graph->size;   // indeks wierzchołka z najlepszą odległością
        
        // znajduje nieodwiedzony wierzchołek o najmniejszej odległości
        for(uint16_t j = 0; j < graph->size; j++){
            
            if(!node_array[j]->start && node_array[j]->weight < best_w){

                best_w = node_array[j]->weight;
                min_index = j;

            }

        }
        
        // przerwij jeśli nie ma więcej osiągalnych wierzchołków
        if(min_index == graph->size)
            break;
        
        node_array[min_index]->start = 1;   // oznacz wierzchołek jako odwiedzony
        
        // aktualizacja odległości do sąsiadów
        if(mode == 0){

            // macierz: przeszukuje wiersz odpowiadający min_index w macierzy
            for(uint32_t e = 0; e < graph->dir_edges; e++){

                int16_t w = graph->dir_matrix[min_index][e];
                if(w <= 0)
                    continue;

                for(uint16_t v = 0; v < graph->size; v++){

                    if(v == min_index) 
                        continue;

                    // krawędź istnieje jeśli wartość w kolumnie e jest ujemna
                    if(graph->dir_matrix[v][e] < 0){

                        if(node_array[v]->start)
                            continue;

                        // wyznacza nową odległość
                        int32_t nd = node_array[min_index]->weight + w;

                        // relaksacja krawędzi
                        if(nd < node_array[v]->weight){

                            node_array[v]->weight = nd;
                            node_array[v]->end = min_index;

                        }

                        break;

                    }

                }

            }

        }else {

            // lista: przeglądamy listę sąsiadów min_index
            for(struct edge* e = graph->dir_list[min_index]; e; e = e->next){

                uint16_t v = e->target;

                if(node_array[v]->start)
                    continue;

                // wyznacza nową odległość
                int32_t nd = node_array[min_index]->weight + e->weight;

                // relaksacja krawędzi
                if(nd < node_array[v]->weight){

                    node_array[v]->weight = nd;
                    node_array[v]->end    = min_index;

                }

            }

        }

    }

    free(node_array);

    return head;

}
