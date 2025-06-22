#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// struktura trzymająca pomocne informacje o krawędziach
// wykorzystywana lokalnie
struct edge_info {
    uint16_t u;
    uint16_t v;
    int16_t weight;
};

// funkcja porównania dwóch krawędzi, wykorzystywana do sortownia po ich wadze
static int compare_edges(const void* a, const void* b){

    const struct edge_info* edge1 = (const struct edge_info*)a;
    const struct edge_info* edge2 = (const struct edge_info*)b;

    if(edge1->weight < edge2->weight) 
        return -1;

    if(edge1->weight > edge2->weight) 
        return 1;

    return 0;

}

// funkcja sprawdzająca kolor wierzchołka, zwraca jego kolor
uint16_t union_find(uint16_t* union_tab, uint16_t u){

    uint16_t root = u;

    while (union_tab[root] != root) 
        root = union_tab[root];
    
    // kompresja ścieżki
    while(u != root){

        uint16_t next = union_tab[u];
        union_tab[u] = root;
        u = next;

    }
    
    return root;

}

// funkcja "malująca" wierzchołek u i jego poddrzewo na kolor wierzchołka v
void union_connect(uint16_t* parent, uint16_t u, uint16_t v){

    uint16_t ru = union_find(parent, u);
    uint16_t rv = union_find(parent, v);
    parent[ru] = rv;

}

// funkcja implementująca algorytm Kruskala
struct result* Kruskal(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    // alokacja pamięci pod tablicę wszystkich krawędzi
    struct edge_info* edges_sorted = malloc(sizeof(struct edge_info) * graph->undir_edges);
    if(!edges_sorted){

        fprintf(stderr, "Blad alokacji tablicy krawedzi (Kruskal)\n");
        return NULL;

    }

    uint32_t id = 0;

    // wydobycie krawędzi z macierzy/listy
    for(uint16_t u = 0; u < graph->size; u++){
        for(uint16_t v = u + 1; v < graph->size; v++){

            int16_t w = 0;

            if(mode == 0){

                // przeszukiwanie macierzy
                for(uint32_t e_i = 0; e_i < graph->undir_edges; e_i++){

                    if(graph->undir_matrix[u][e_i] > 0 && graph->undir_matrix[v][e_i] > 0){

                        w = graph->undir_matrix[u][e_i];
                        break;

                    }

                }

            }else {

                // przeszukiwanie listy
                struct edge* e = graph->undir_list[u];
                while(e){

                    if(e->target == v){

                        w = e->weight;
                        break;

                    }

                    e = e->next;

                }

            }

            if(w == 0)
                continue;

            if(id >= graph->undir_edges){

                fprintf(stderr, "Liczba krawędzi się nie zgadza (Kruskal)\n");
                free(edges_sorted);
                return NULL;

            }

            // dodanie znalezionej krawędzi do listy
            edges_sorted[id++] = (struct edge_info){.u = u, .v = v, .weight = w};

        }

    }

    // posortowanie listy krawędzi po wadze 
    qsort(edges_sorted, id, sizeof(struct edge_info), compare_edges);

    // tablica trzymająca kolory wierzchołków
    uint16_t union_tab[graph->size];

    // inicjacja tablicy z osobnym kolorem dla każdego wierzchołka
    for(uint16_t i = 0; i < graph->size; i++)
        union_tab[i] = i;

    // liczy ile krawędzi użyliśmy
    uint16_t used = 0;

    // główna pętla algorytmu
    for(uint32_t i = 0; i < id && used < graph->size - 1; i++){

        // w tej pętli wybieramy po kolei krawędzie z posortowanej listy
        // jeżeli krawędź łączy wierzchołki o różnych kolorach dodajemy ją do rozwiązania
        // kolorujemy wierzchołki na ten sam kolor

        uint16_t u = edges_sorted[i].u;
        uint16_t v = edges_sorted[i].v;

        if(union_find(union_tab, u) != union_find(union_tab, v)){
            
            union_connect(union_tab, u, v);

            struct result* node = malloc(sizeof(*node));
            if(!node){

                fprintf(stderr, "Alokacja wezelka spadla z rowerka (Kruskal)");
                free(edges_sorted);
                free_result(head);
                return NULL;

            }

            node->start = u;
            node->end = v;
            node->weight = edges_sorted[i].weight;
            node->next = NULL;

            if(!head)
                head = tail = node;
            else{

                tail->next = node;
                tail = node;

            }

            used++;

        }

    }

    // jeżeli nie użyliśmy dość dużo krawędzi graf jest niespójny
    if(used != graph->size - 1){

        fprintf(stderr, "Graf niespojny (Kruskal)\n");
        free_result(head);
        head = NULL;

    }

    free(edges_sorted);
    return head;

}
