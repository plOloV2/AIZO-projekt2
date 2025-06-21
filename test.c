#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

struct graph* read_graph(const char* filename);
void display_graphs(struct graph* graf);
void print_results(struct result *head);

int main(){

    struct graph* graf = read_graph("test.txt");
    display_graphs(graf);

    printf("\nDijkstra macierz\n");

    print_results(Dijkstra(graf, 0));

    printf("\nDijkstra lista\n");

    print_results(Dijkstra(graf, 1));

    return 0;

}
