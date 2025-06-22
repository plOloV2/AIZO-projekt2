#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

// funkcja wczytujaca graf z pliku .txt
struct graph* read_graph(const char* filename);

// funkcja wyswietlajaca zawartosc struktury graph
void display_graphs(struct graph* graf);

// funkcje wypisujace wyniki dzialania algorytmow
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

    struct graph* test_graph = NULL;
    struct graph** created_graphs = NULL;
    int choice;
    uint16_t size;
    char filename[256];
    int density_choice;


    // zapytanie uzytkownika o opcje i utworzenie zgodnie z nia grafu
    printf("1. Stworz nowy graf\n");
    printf("2. Zaladuj graf z pliku\n");
    printf("Podaj opcje: ");
    scanf("%d", &choice);

    if(choice == 1){

        printf("Podaj liczbe krawedzi: ");
        scanf("%hu", &size);
        
        created_graphs = create_graph(size);
        if(!created_graphs){

            fprintf(stderr, "Blad tworzenia grafu\n");
            return 1;

        }

        printf("\nUtworzono grafy o gestosciach: 25%%, 50%%, 99%%\n");
        printf("Wybierz gestosc (0-2): ");
        scanf("%d", &density_choice);
        
        if(density_choice < 0 || density_choice > 2){

            fprintf(stderr, "Nieprawidlowa wartosc\n");
            return 1;

        }

        test_graph = created_graphs[density_choice];
        printf("\nUzywam grafu o gestosci %d%%:\n", (density_choice == 0) ? 25 : (density_choice == 1) ? 50 : 99);
        display_graphs(test_graph);
        
    }else if(choice == 2){

        printf("Podaj nazwe pliku: ");
        scanf("%255s", filename);
        
        test_graph = read_graph(filename);
        if(!test_graph){

            fprintf(stderr, "Nie udalo sie zaladowac grafu\n");
            return 1;

        }
        
        printf("\nZaladowano graf z pliku '%s':\n", filename);
        display_graphs(test_graph);

    }else {

        fprintf(stderr, "Nieprawodlowy wybor\n");
        return 1;

    }

    // uruchamia algorytmu dla wybranego wczesniej grafu i wyswietla wyniki ich pracy
    print_results_Pr(Prim(test_graph, 0), Prim(test_graph, 1));

    print_results_Kr(Kruskal(test_graph, 0), Kruskal(test_graph, 1));

    print_results_Di(Dijkstra(test_graph, 0), Dijkstra(test_graph, 1));

    print_results_FB(Ford_Bellman(test_graph, 0), Ford_Bellman(test_graph, 1));

    print_results_FF(Ford_Fulkerson(test_graph, 0), Ford_Fulkerson(test_graph, 1));

    return 0;

}
