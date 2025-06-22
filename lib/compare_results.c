#include"result_struct.h"
#include<stdlib.h>

// funkcja porónująca dwie listy wyników do siebie w zależnośći od parametru mode
uint8_t compare_results(uint8_t mode, struct result *a, struct result *b){

    // porównanie algorytmu Forda-Fulkersona
    if(mode == 2){

        // sortuje krawędzie po wierzchołkach początkowych i końcowych
        a = sort_result_list(a);
        b = sort_result_list(b);

        // wyznacza przepływ wyjściowy z początkowego wierzchołka
        uint64_t flow_a = 0, flow_b = 0;

        while(a->start == 0){

            flow_a += a->weight;
            a = a->next;

        }

        while(b->start == 0){

            flow_b += b->weight;
            b = b->next;
            
        }

        // jeżeli są takie same zwraca prawde
        return flow_a == flow_b;

    }

    while(a != NULL && b != NULL){

        // porównanie dla algorytmów Prima i Kruskala
        if(mode){

            // listy wynikowe muszą być identyczne
            if(a->start != b->start || a->end != b->end || a->weight != b->weight)
            return 0;

        // porównanie dla algorytmów Dijkstry i Forda-Bellmana        
        } else{

            // tylko odległości muszą być równe
            if(a->weight != b->weight)
            return 0;

        }

        a = a->next;
        b = b->next;

    }

    return (a == NULL && b == NULL) ? 1 : 0;

}
