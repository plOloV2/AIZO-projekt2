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

void print_results(struct result *head){

    struct result *cur = head;
    
    while(cur){

        printf("Edge from %u to %u, weight = %d\n", cur->start, cur->end, cur->weight);

        cur = cur->next;

    }

}
