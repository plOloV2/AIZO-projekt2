#include "result_struct.h"
#include <stddef.h>

// pomocnicza funkcja spajająca dwie posortowane listy
static struct result* merge(struct result* a, struct result* b){
    
    struct result dummy;
    struct result* tail = &dummy;
    dummy.next = NULL;

    while(a && b){
        // porównanie wartości start
        if(a->start < b->start){

            tail->next = a;
            a = a->next;

        }else if(a->start > b->start){

            tail->next = b;
            b = b->next;

        }else {
            // jeżeli wartości start są równe porównujemy end
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
    
    tail->next = a ? a : b;
    return dummy.next;

}

// funkcja sortująca liste wyników po watość start i end
struct result* sort_result_list(struct result* head){

    // lista pusta lub z jednym elementem
    if(head == NULL || head->next == NULL)
        return head;

    // znalezienie środka listy
    struct result* slow = head;
    struct result* fast = head->next;
    while(fast && fast->next){

        slow = slow->next;
        fast = fast->next->next;

    }

    // podział listy na dwie połowy
    struct result* mid = slow->next;
    slow->next = NULL;

    // rekursywne sortowanie połówek
    struct result* left = sort_result_list(head);
    struct result* right = sort_result_list(mid);

    // sklejenie połówek w całość
    return merge(left, right);
    
}
