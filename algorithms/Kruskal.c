#include"../lib/graph.h"
#include"../lib/result_struct.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


struct edge_info {
    uint16_t u;
    uint16_t v;
    int16_t weight;
};


void free_result(struct result* result);


static int compare_edges(const void* a, const void* b){

    const struct edge_info* edge1 = (const struct edge_info*)a;
    const struct edge_info* edge2 = (const struct edge_info*)b;

    if(edge1->weight < edge2->weight) 
        return -1;

    if(edge1->weight > edge2->weight) 
        return 1;

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


void union_connect(uint16_t* parent, uint16_t u, uint16_t v){

    uint16_t ru = union_find(parent, u);
    uint16_t rv = union_find(parent, v);
    parent[ru] = rv;

}


struct result* Kruskal(struct graph* graph, uint8_t mode){

    struct result* head = NULL;
    struct result* tail = NULL;

    struct edge_info* edges_sorted = malloc(sizeof(struct edge_info) * graph->undir_edges);
    if(!edges_sorted){

        fprintf(stderr, "Failed to allocate edge array (Kruskal)\n");
        return NULL;

    }

    uint32_t id = 0;

    for(uint16_t u = 0; u < graph->size; u++){
        for(uint16_t v = u + 1; v < graph->size; v++){

            int16_t w = 0;

            if(mode == 0){
                // lookup in incidence matrix
                for(uint32_t e_i = 0; e_i < graph->undir_edges; e_i++){

                    if(graph->undir_matrix[u][e_i] > 0 && graph->undir_matrix[v][e_i] > 0){

                        w = graph->undir_matrix[u][e_i];
                        break;

                    }

                }

            }else {
                // lookup in adjacency list
                struct edge* e = graph->undir_list[u];
                while(e){

                    if(e->target == v){

                        w = e->weight;
                        break;

                    }

                    e = e->next;

                }

            }

            if(w == 0)
                continue;

            if(id >= graph->undir_edges){

                fprintf(stderr, "Edge count mismatch (Kruskal)\n");
                free(edges_sorted);
                return NULL;

            }

          
            edges_sorted[id++] = (struct edge_info){.u = u, .v = v, .weight = w};

        }

    }

    qsort(edges_sorted, id, sizeof(struct edge_info), compare_edges);

    uint16_t union_tab[graph->size];

    for(uint16_t i = 0; i < graph->size; i++)
        union_tab[i] = i;

    uint16_t used = 0;

    for(uint32_t i = 0; i < id && used < graph->size - 1; i++){

        uint16_t u = edges_sorted[i].u;
        uint16_t v = edges_sorted[i].v;

        if(union_find(union_tab, u) != union_find(union_tab, v)){
            
            union_connect(union_tab, u, v);

            struct result* node = malloc(sizeof(*node));
            if(!node){

                fprintf(stderr, "NODE alloc failed (Kruskal)");
                free(edges_sorted);
                free_result(head);
                return NULL;

            }

            node->start = u;
            node->end = v;
            node->weight = edges_sorted[i].weight;
            node->next = NULL;

            if(!head)
                head = tail = node;
            else{

                tail->next = node;
                tail = node;

            }

            used++;

        }

    }

    // If we didn't pick enough edges, graph was disconnected
    if(used != graph->size - 1){

        fprintf(stderr, "Disconnected graph (Kruskal)\n");
        free_result(head);
        head = NULL;

    }

    free(edges_sorted);
    return head;

}
