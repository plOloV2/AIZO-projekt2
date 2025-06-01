#include<stdio.h>
#include"lib/lib.h"
#include"algorithms/algorithms.h"

int main(int argc, char** argv){

    if(argc < 2){
        perror("Too few arguments");
        return 1;
    }

    struct graph** new = create_graph(10);

    if(new == NULL)
        return 2;

    

    return 0;
}
