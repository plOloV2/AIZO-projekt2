#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

struct graph* read_graph(const char* filename);

void display_graphs(struct graph* graf);

void print_results_Pr(struct result *r1, struct result *r2){

    printf("\nPrim macierz:\n");

    struct result *cur = r1;
    
    while(cur){

        printf("Krawedz z %u do %u, waga: %d\n", cur->start, cur->end, cur->weight);

        cur = cur->next;

    }

    printf("\nPrim lista:\n");

    cur = r2;
    
    while(cur){

        printf("Krawedz z %u do %u, waga: %d\n", cur->start, cur->end, cur->weight);

        cur = cur->next;

    }

    free_result(r1);
    free_result(r2);
    
}

void print_results_Kr(struct result *r1, struct result *r2){

    printf("\nKruskal macierz:\n");

    struct result *cur = r1;
    
    while(cur){

        printf("Krawedz z %u do %u, waga: %d\n", cur->start, cur->end, cur->weight);

        cur = cur->next;

    }

    printf("\nKruskal lista:\n");

    cur = r2;
    
    while(cur){

        printf("Krawedz z %u do %u, waga: %d\n", cur->start, cur->end, cur->weight);

        cur = cur->next;

    }

    free_result(r1);
    free_result(r2);
    
}

void print_results_Di(struct result *r1, struct result *r2){

    printf("\nDijkstra macierz:\n");

    struct result *cur = r1;

    uint8_t it = 0;
    
    while(cur){

        printf("Dystans z 0 do %u to: %d. Poprzedni wierzcholek: %u\n", it++, cur->weight, cur->end);

        cur = cur->next;

    }

    printf("\nDijkstra lista:\n");

    cur = r2;
    it = 0;
    
    while(cur){

        printf("Dystans z 0 do %u to: %d. Poprzedni wierzcholek: %u\n", it++, cur->weight, cur->end);

        cur = cur->next;

    }

    free_result(r1);
    free_result(r2);
    
}

void print_results_FB(struct result *r1, struct result *r2){

    printf("\nFord-Bellman macierz:\n");

    struct result *cur = r1;

    uint8_t it = 0;
    
    while(cur){

        printf("Dystans z 0 do %u to: %d. Poprzedni wierzcholek: %u\n", it++, cur->weight, cur->start);

        cur = cur->next;

    }

    printf("\nFord-Bellman lista:\n");

    cur = r2;
    it = 0;
    
    while(cur){

        printf("Dystans z 0 do %u to: %d. Poprzedni wierzcholek: %u\n", it++, cur->weight, cur->start);

        cur = cur->next;

    }

    free_result(r1);
    free_result(r2);
    
}

void print_results_FF(struct result *r1, struct result *r2){

    printf("\nFord-Fulkerson macierz:\n");

    struct result *cur = r1;

    cur = sort_result_list(cur);
    
    while(cur){

        printf("Przepływ z %u do %u to: %d\n", cur->start, cur->end, cur->weight);

        cur = cur->next;

    }

    printf("\nFord-Fulkerson lista:\n");

    cur = r2;

    cur = sort_result_list(cur);
    
    while(cur){

        printf("Przepływ z %u do %u to: %d\n", cur->start, cur->end, cur->weight);

        cur = cur->next;

    }

    free_result(r1);
    free_result(r2);
    
}

int main(){

    struct graph* graf = read_graph("test.txt");
    display_graphs(graf);

    print_results_Pr(Prim(graf, 0), Prim(graf, 1));

    print_results_Kr(Kruskal(graf, 0), Kruskal(graf, 1));

    print_results_Di(Dijkstra(graf, 0), Dijkstra(graf, 1));

    print_results_FB(Ford_Bellman(graf, 0), Ford_Bellman(graf, 1));

    print_results_FF(Ford_Fulkerson(graf, 0), Ford_Fulkerson(graf, 1));

    return 0;

}
