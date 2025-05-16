#include"graph.h"

void set_value_matrix(struct graph* graph, uint16_t ver1, uint16_t ver2, uint16_t val){

    if(ver1 == ver2)
        return;

    if(ver2 > ver1){

        uint16_t temp = ver1;
        ver1 = ver2;
        ver2 = temp;

    }

    graph->matrix[ver1 - 1][ver2] = val;

}


uint16_t get_value_matrix(struct graph* graph, uint16_t ver1, uint16_t ver2){

    if(ver1 == ver2)
        return 0;

    if(ver2 > ver1){

        uint16_t temp = ver1;
        ver1 = ver2;
        ver2 = temp;

    }

    return graph->matrix[ver1 - 1][ver2];

}
