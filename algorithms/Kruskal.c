#include "../lib/graph.h"
#include "../lib/result_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct edge_info {
    uint16_t u;
    uint16_t v;
    int16_t weight;
};


void free_result(struct result* result);


static int compare_edges(const void* a, const void* b){

    const struct edge_info* edge1 = (const struct edge_info*)a;
    const struct edge_info* edge2 = (const struct edge_info*)b;

    if (edge1->weight < edge2->weight) return -1;

    if (edge1->weight > edge2->weight) return 1;

    return 0;

}


uint16_t union_find(uint16_t* union_tab, uint16_t u){

    uint16_t root = u;

    while (union_tab[root] != root) 
        root = union_tab[root];
    
    // Path compression
    while(u != root){

        uint16_t next = union_tab[u];
        union_tab[u] = root;
        u = next;

    }
    
    return root;
}


void union_connect(uint16_t* union_tab, uint16_t u, uint16_t v){

    union_tab[union_find(union_tab, u)] = union_find(union_tab, v);

}


struct result* Kruskal(struct graph* graph, int16_t (*find_edge)(struct graph*, uint16_t, uint16_t)){

    struct result* head = NULL;
    struct result* tail = NULL;

    struct edge_info* edges_sorted = malloc(sizeof(struct edge_info) * graph->edges);
    if(!edges_sorted){

        fprintf(stderr, "Failed to allocate edge array (Kruskal)\n");
        return NULL;

    }

    uint16_t id = 0;

    for(uint16_t u = 0; u < graph->size; u++){
        for(uint16_t v = u + 1; v < graph->size; v++){

            int16_t weigh = find_edge(graph, u, v);

            if(weigh == 0)
                continue;

            if(id >= graph->edges){

                fprintf(stderr, "Edge count mismatch (Kruskal)\n");
                free(edges_sorted);
                return NULL;

            }

            edges_sorted[id].u = u;
            edges_sorted[id].v = v;
            edges_sorted[id].weight = weigh;

            id++;

        }

    }

    qsort(edges_sorted, id, sizeof(struct edge_info), compare_edges);

    uint16_t union_tab[graph->size];

    for(uint16_t i = 0; i < graph->size; i++)
        union_tab[i] = i;

    uint16_t total_edges = id;
    id = 0;

    for(uint16_t i = 0; i < graph->size - 1; i++){

        while(id < total_edges && union_find(union_tab, edges_sorted[id].u) == union_find(union_tab, edges_sorted[id].v))
            id++;

        if(id == total_edges){

            fprintf(stderr, "Disconnected graph (Kruskal)\n");
            free_result(head);
            head = NULL;
            tail = NULL;
            free(edges_sorted);
            return NULL;

        }

        struct result* node = malloc(sizeof(struct result));
        if(!node){
            
            fprintf(stderr, "NODE alloc failed (Kruskal)\n");
            free_result(head);
            head = NULL;
            tail = NULL;
            free(edges_sorted);
            return NULL;

        }

        union_connect(union_tab, edges_sorted[id].u, edges_sorted[id].v);

        node->start = edges_sorted[id].u;
        node->end = edges_sorted[id].v;
        node->weight = edges_sorted[id].weight;
        node->next = NULL;

        if(!head){
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }

    }

    free(edges_sorted);
    return head;

}
