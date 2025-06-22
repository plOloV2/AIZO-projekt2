#include"result_struct.h"
#include<stdlib.h>
#include<stdio.h>

void free_result(struct result* head){

    struct result* current = head;

    while (current != NULL){
        
        struct result* next = current->next;  // Save next pointer before freeing
        free(current);                        // Free current node
        current = next;                       // Move to next node

    }

}
