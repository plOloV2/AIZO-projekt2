#include "result_struct.h"
#include <stddef.h>

// Helper function to merge two sorted lists
static struct result* merge(struct result* a, struct result* b){
    
    struct result dummy; // Dummy node to simplify head handling
    struct result* tail = &dummy;
    dummy.next = NULL;

    while(a && b){
        // Compare start values first
        if(a->start < b->start){

            tail->next = a;
            a = a->next;

        }else if(a->start > b->start){

            tail->next = b;
            b = b->next;

        }else {
            // If start is equal, compare end values
            if(a->end <= b->end){

                tail->next = a;
                a = a->next;

            }else {

                tail->next = b;
                b = b->next;

            }

        }

        tail = tail->next;

    }
    // Attach the remaining elements
    tail->next = a ? a : b;
    return dummy.next;

}

// Main sorting function
struct result* sort_result_list(struct result* head){

    if(head == NULL || head->next == NULL)
        return head; // Base case: list is empty or has one element

    // Find midpoint using slow/fast pointers
    struct result* slow = head;
    struct result* fast = head->next;
    while(fast && fast->next){

        slow = slow->next;
        fast = fast->next->next;

    }

    // Split the list into two halves
    struct result* mid = slow->next;
    slow->next = NULL;

    // Recursively sort both halves
    struct result* left = sort_result_list(head);
    struct result* right = sort_result_list(mid);

    // Merge the sorted halves
    return merge(left, right);
    
}
