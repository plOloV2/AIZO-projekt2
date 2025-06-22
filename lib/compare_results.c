#include"result_struct.h"
#include<stdlib.h>

uint8_t compare_results(uint8_t mode, struct result *a, struct result *b){

    if(mode == 2){

        a = sort_result_list(a);
        b = sort_result_list(b);

        mode = 0;

        return 1;

    }

    while(a != NULL && b != NULL){

        if(mode){

            if(a->start != b->start || a->end != b->end || a->weight != b->weight)
            return 0;

        } else{

            if(a->weight != b->weight)
            return 0;

        }

        a = a->next;
        b = b->next;

    }

    return (a == NULL && b == NULL) ? 1 : 0;

}
