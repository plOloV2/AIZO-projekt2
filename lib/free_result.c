#include"result_struct.h"
#include<stdlib.h>
#include<stdio.h>

// funkcja czyszcząca listę wyników
void free_result(struct result* head){

    struct result* current = head;

    while (current != NULL){
        
        struct result* next = current->next;
        free(current);
        current = next;

    }

    head = NULL;

}
