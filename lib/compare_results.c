#include"result_struct.h"
#include<stdlib.h>

uint8_t compare_results(struct result *a, struct result *b){

    while(a != NULL && b != NULL){

        if(a->start != b->start || a->end != b->end || a->weight != b->weight)
            return 0;

        a = a->next;
        b = b->next;

    }

    return (a == NULL && b == NULL) ? 1 : 0;

}
