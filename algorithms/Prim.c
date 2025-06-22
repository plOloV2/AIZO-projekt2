#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// implementacja algorytmu Prima
struct result* Prim(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    // alokacja listy odwiedzonych krawędzi
    uint8_t* visited = calloc(graph->size, sizeof(uint8_t));
    if (visited == NULL){

        fprintf(stderr, "Failed to inicialize VISITED table (Prim's)\n");
        return NULL;

    }

    // oznacza pierwszą krawędź jako odwiedzoną
    visited[0] = 1;

    for(uint16_t i = 1; i < graph->size; i++){
        
        int16_t min_weight = INT16_MAX;     // nieprawidłowa waga, jeśli się nie zmieni krawędź nie została znaleziona
        uint16_t best_u = 0, best_v = 0;    // znalezione wierzhołki
        uint8_t found_edge = 0;             // flaga znalezienia krawędzi

        for(uint16_t u = 0; u < graph->size; u++){

            // pomijamy nieodwiedzone krawędzie
            if(!visited[u])
                continue;
            
            for(uint16_t v = 0; v < graph->size; v++){

                // pomijamy odwiedzone krawędzie
                if(visited[v])
                    continue;

                int16_t w = 0;

                if(mode == 0){

                    // przeszukuje macierz w poszukiwaniu krawędzi pomiędzy U i V
                    for(uint32_t e_idx = 0; e_idx < graph->undir_edges; e_idx++){

                        if(graph->undir_matrix[u][e_idx] > 0 && graph->undir_matrix[v][e_idx] > 0){

                            w = graph->undir_matrix[u][e_idx];
                            break;

                        }

                    }

                }else{

                    // przeszukuje listę w poszukiwaniu krawędzi pomiędzy U i V
                    struct edge* e = graph->undir_list[u];
                    while(e){

                        if(e->target == v){

                            w = e->weight;
                            break;

                        }

                        e = e->next;

                    }

                }

                // jeżeli w jest zerem to krawędź nie została znaleziona
                if(w == 0)
                    continue;

                found_edge = 1;

                // jeżeli w jest mniejsze od poprzedniego najlepszego, zapisz jego wartośći
                if(w < min_weight){

                    min_weight = w;
                    best_u = u;
                    best_v = v;

                }

            }

        }

        // brak krawiędzi to niespójny graf (błąd)
        if(!found_edge){

            fprintf(stderr, "Niespojny graf (Prim)\n");
            break;

        }
        
        // alokacja pamięcie dla nowego węzła listy wyników
        struct result* node = malloc(sizeof(struct result));
        if(!node){
            
            fprintf(stderr, "Alokacja wezelka spadla z rowerka (Prim)\n");
            free_result(head);
            head = NULL;
            tail = NULL;
            free(visited);
            return NULL;

        }

        // nadanie wartości temu węzłowi
        node->start = best_u;
        node->end = best_v;
        node->weight = min_weight;
        node->next = NULL;

        // podpięcie węzła pod listę
        if(!head){
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }

        visited[best_v] = 1;
        
    }

    // czyszczenie pamięci i zwrócenie wyniku
    free(visited);
    return head;

}

