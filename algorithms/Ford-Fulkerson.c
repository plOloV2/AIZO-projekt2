#include "../lib/graph.h"
#include "../lib/result_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


// struktury pomocnicze, wykorzystywane lokalnie

struct r_edge {
    uint16_t to;            // wierzchołek docelowy
    int16_t cap;            // przepustowość krawędzi
    struct r_edge *rev;     // wskaźnik do krawędzi przeciwnej
    struct r_edge *next;    // następna krawędź na liście
};

struct extracted_edge {
    uint16_t u;         // wierzchołek źródłowy
    uint16_t v;         // wierzchołek docelowy
    int16_t cap;        // przepustowość krawędzi
};


// funkcja implementująca algorytm Forda-Fulkersona
struct result* Ford_Fulkerson(struct graph* graph, uint8_t mode) {

    // alokacja pamięci dla wyłuskanych krawędzi
    struct extracted_edge *edges = malloc(graph->dir_edges * sizeof(struct extracted_edge));
    if(!edges){

        fprintf(stderr, "Alokacja tablicy krawędzi nie wyszla (Ford_Fulkerson)\n");
        return NULL;

    }

    // łuskanie krawędzi z grafu
    if(mode == 0){

        for(uint32_t e = 0; e < graph->dir_edges; e++){

            uint16_t u = graph->size;
            uint16_t v = graph->size;

            for(uint16_t i = 0; i < graph->size && (u == graph->size || v == graph->size); i++){

                int16_t val = graph->dir_matrix[i][e];

                if(val > 0){

                    // wierzchołek źródłowy (wartość dodatnia)
                    u = i;

                }else if(val < 0){

                    // wierzchołek docelowy (wartość ujemna)
                    v = i;

                }

            }

            // walidacja krawędzi
            if(u == graph->size || v == graph->size){

                fprintf(stderr, "Krawedzi %u nie ma w macierzy\n", e);
                free(edges);
                return NULL;

            }

            if(graph->dir_matrix[u][e] <= 0){

                fprintf(stderr, "Nieprawidlowa pojemnosc krawedzi %u->%u\n", u, v);
                free(edges);
                return NULL;

            }

            // zapisanie informacje o krawędzi
            edges[e].u = u;
            edges[e].v = v;
            edges[e].cap = graph->dir_matrix[u][e];

        }

    }else {

        uint32_t count = 0;
        for(uint16_t u = 0; u < graph->size; u++){

            for(struct edge* e = graph->dir_list[u]; e != NULL; e = e->next){

                if(count >= graph->dir_edges){

                    fprintf(stderr, "Za duzo krawedzi (Ford_Fulkerson)\n");
                    free(edges);
                    return NULL;

                }

                // zapisanie informacje o krawędzi
                edges[count].u = u;
                edges[count].v = e->target;
                edges[count].cap = e->weight;
                count++;

            }

        }

        // sprawdź spójność liczby krawędzi
        if(count != graph->dir_edges){

            fprintf(stderr, "Liczba krawedzi mi nie pasuje: powinno byc %u, a jest %u. (Ford_Fulkerson)\n", graph->dir_edges, count);
            free(edges);
            return NULL;

        }

    }

    // alokacja grafu pozostałej przepustowości 
    struct r_edge **res_graph = calloc(graph->size, sizeof(struct r_edge*));
    if(!res_graph){

        fprintf(stderr, "Alokacja grafu pozostalej przepustowosci sie nie udala (Ford_Fulkerson)\n");
        free(edges);
        return NULL;

    }

    // tablica do śledzenia krawędzi w przod
    struct r_edge **forward_edges = malloc(graph->dir_edges * sizeof(struct r_edge*));
    if(!forward_edges){

        fprintf(stderr, "Alokacja tablicy krawedzi w przod sie nie udala (Ford_Fulkerson)\n");
        free(edges);
        free(res_graph);
        return NULL;

    }

    // tworzenie krawędzi w grafie pozostałej
    for(uint32_t i = 0; i < graph->dir_edges; i++){

        uint16_t u = edges[i].u;
        uint16_t v = edges[i].v;
        int16_t cap = edges[i].cap;

        // tworzenie krawędzi do przodu (u -> v)
        struct r_edge *fwd = malloc(sizeof(struct r_edge));
        if(!fwd){

            fprintf(stderr, "Alokacja krawedzi w przod nie (Ford_Fulkerson)\n");
            goto cleanup_edges;

        }

        fwd->to = v;
        fwd->cap = cap;
        fwd->next = res_graph[u];
        res_graph[u] = fwd;

        // tworzenie krawędzi wstecznej (v -> u)
        struct r_edge *bck = malloc(sizeof(struct r_edge));
        if(!bck){

            free(fwd);
            fprintf(stderr, "Alokacja krawedzi w tyl nie (Ford_Fulkerson)\n");
            goto cleanup_edges;

        }

        bck->to = u;
        bck->cap = 0;
        bck->next = res_graph[v];
        res_graph[v] = bck;

        // powiązanie krawędzi przeciwstawnych
        fwd->rev = bck;
        bck->rev = fwd;
        forward_edges[i] = fwd;

    }

    // dane dla BFS
    uint16_t *parent = NULL;
    struct r_edge **parent_edge = NULL;
    uint8_t *visited = NULL;
    uint16_t *queue = NULL;

    // alokacja pamięci dla struktur BFS
    parent = malloc(graph->size * sizeof(uint16_t));
    parent_edge = calloc(graph->size, sizeof(struct r_edge*));
    visited = calloc(graph->size, sizeof(uint8_t));
    queue = malloc(graph->size * sizeof(uint16_t));

    if(!parent || !parent_edge || !visited || !queue){

        fprintf(stderr, "alokacja pamięci dla struktur BFS nie wyszla (Ford_Fulkerson)\n");
        free(parent);
        free(parent_edge);
        free(visited);
        free(queue);
        goto cleanup_edges;

    }

    // ustalenie źródła (0) i ujścia (ostatni wierzchołek)
    uint16_t source = 0;
    uint16_t sink = graph->size - 1;

    // główna pętla algorytmu Edmondsa-Karpa (BFS)
    while(1){

        // inicjalizacja danych BFS
        for(uint16_t i = 0; i < graph->size; i++){

            visited[i] = 0;
            parent[i] = graph->size;

        }

        uint16_t front = 0, rear = 0;
        queue[rear++] = source;         // rozpocznij od źródła
        visited[source] = 1;
        parent[source] = source;        // źródło jest swoim własnym rodzicem

        // flaga znalezienia ścieżki do ujścia
        int found_path = 0;

        // przeszukiwanie BFS
        while(front < rear){

            uint16_t u = queue[front++];

            // jeśli dotarliśmy do ujścia, przerwij
            if(u == sink){

                found_path = 1;
                break;

            }

            // przejrzyj wszystkie sąsiednie krawędzie
            for(struct r_edge *e = res_graph[u]; e != NULL; e = e->next){

                // jeśli krawędź ma dodatnią przepustowość i wierzchołek nie był odwiedzony
                if(e->cap > 0 && !visited[e->to]){

                    visited[e->to] = 1;         // zaznacz odwiedzenie
                    parent[e->to] = u;          // zapisz rodzica
                    parent_edge[e->to] = e;     // zapisz krawędź
                    queue[rear++] = e->to;      // dodaj do kolejki

                }

            }

        }

        // jeśli nie znaleziono ścieżki powiększającej, zakończ
        if(!found_path)
            break;

        // znajdź minimalną przepustowość na ścieżce
        int16_t min_cap = INT16_MAX;
        for(uint16_t v = sink; v != source; v = parent[v]){

            struct r_edge *e = parent_edge[v];
            if(e->cap < min_cap)
                min_cap = e->cap;

        }

        // aktualizuj przepustowości wzdłuż ścieżki
        for(uint16_t v = sink; v != source; v = parent[v]){

            struct r_edge *e = parent_edge[v];
            e->cap -= min_cap;          // zmniejsz przepustowość w kierunku przepływu
            e->rev->cap += min_cap;     // zwiększ przepustowość w kierunku przeciwnym

        }

    }

    // budowa listy wynikowej
    struct result *head = NULL, *tail = NULL;
    for(uint32_t i = 0; i < graph->dir_edges; i++){

        struct result *node = malloc(sizeof(struct result));

        if(!node){

            fprintf(stderr, "Alokacja wezelka spadla z rowerka (Ford_Fulkerson)\n");
            free_result(head);
            head = NULL;
            goto cleanup_bfs;

        }

        // oblicz rzeczywisty przepływ: oryginalna przepustowość - pozostała przepustowość
        node->start = edges[i].u;                               // wierzchołek źródłowy
        node->end = edges[i].v;                                 // wierzchołek docelowy
        node->weight = edges[i].cap - forward_edges[i]->cap;    // rzeczywisty przepływ
        node->next = NULL;

        // dodawanie do listy wyników
        if(!head){
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }

    }

// sekcje czyszczenia pamięci
cleanup_bfs:
    free(parent);
    free(parent_edge);
    free(visited);
    free(queue);

cleanup_edges:
    // zwolnienie pamięci grafu rezydualnego
    for(uint16_t i = 0; i < graph->size; i++){

        struct r_edge *e = res_graph[i];

        while(e){

            struct r_edge *next = e->next;
            free(e);
            e = next;

        }

    }

    free(res_graph);
    free(forward_edges);
    free(edges);

    return head;

}
