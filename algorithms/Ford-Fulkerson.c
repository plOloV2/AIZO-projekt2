#include "../lib/graph.h"
#include "../lib/result_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct r_edge {
    uint16_t to;
    int16_t cap;
    struct r_edge *rev;
    struct r_edge *next;
};

struct extracted_edge {
    uint16_t u;
    uint16_t v;
    int16_t cap;
};

struct result* Ford_Fulkerson(struct graph* graph, uint8_t mode) {

    // Allocate memory for extracted edges
    struct extracted_edge *edges = malloc(graph->dir_edges * sizeof(struct extracted_edge));
    if(!edges){

        fprintf(stderr, "Edges array allocation failed (Ford_Fulkerson)\n");
        return NULL;

    }

    // Extract edges based on representation mode
    if(mode == 0){ // Incident matrix mode

        for(uint32_t e = 0; e < graph->dir_edges; e++){

            uint16_t u = graph->size; // Invalid index
            uint16_t v = graph->size;

            for(uint16_t i = 0; i < graph->size && (u == graph->size || v == graph->size); i++){

                int16_t val = graph->dir_matrix[i][e];

                if(val > 0){

                    u = i;

                }else if(val < 0){

                    v = i;

                }

            }

            // Validate edge
            if(u == graph->size || v == graph->size){

                fprintf(stderr, "Edge %u undefined in matrix\n", e);
                free(edges);
                return NULL;

            }

            if(graph->dir_matrix[u][e] <= 0){

                fprintf(stderr, "Invalid capacity for edge %u->%u\n", u, v);
                free(edges);
                return NULL;

            }

            edges[e].u = u;
            edges[e].v = v;
            edges[e].cap = graph->dir_matrix[u][e];

        }

    }else { // Adjacency list mode

        uint32_t count = 0;
        for(uint16_t u = 0; u < graph->size; u++){

            for(struct edge* e = graph->dir_list[u]; e != NULL; e = e->next){

                if(count >= graph->dir_edges){

                    fprintf(stderr, "Edge count exceeded (Ford_Fulkerson)\n");
                    free(edges);
                    return NULL;

                }

                if(e->weight < 0){

                    fprintf(stderr, "Negative capacity in edge %u->%u. (Ford_Fulkerson)\n", u, e->target);
                    free(edges);
                    return NULL;

                }

                edges[count].u = u;
                edges[count].v = e->target;
                edges[count].cap = e->weight;
                count++;

            }

        }

        if(count != graph->dir_edges){

            fprintf(stderr, "Edge count mismatch: expected %u, got %u. (Ford_Fulkerson)\n", graph->dir_edges, count);
            free(edges);
            return NULL;

        }

    }

    // Build residual graph
    struct r_edge **res_graph = calloc(graph->size, sizeof(struct r_edge*));
    if(!res_graph){

        fprintf(stderr, "Residual graph allocation failed (Ford_Fulkerson)\n");
        free(edges);
        return NULL;

    }

    struct r_edge **forward_edges = malloc(graph->dir_edges * sizeof(struct r_edge*));
    if(!forward_edges){

        fprintf(stderr, "Forward edges array allocation failed (Ford_Fulkerson)\n");
        free(edges);
        free(res_graph);
        return NULL;

    }

    // Create residual edges
    for(uint32_t i = 0; i < graph->dir_edges; i++){

        uint16_t u = edges[i].u;
        uint16_t v = edges[i].v;
        int16_t cap = edges[i].cap;

        // Create forward edge
        struct r_edge *fwd = malloc(sizeof(struct r_edge));
        if(!fwd){

            fprintf(stderr, "Forward edge allocation failed (Ford_Fulkerson)\n");
            goto cleanup_edges;

        }

        fwd->to = v;
        fwd->cap = cap;
        fwd->next = res_graph[u];
        res_graph[u] = fwd;

        // Create backward edge
        struct r_edge *bck = malloc(sizeof(struct r_edge));
        if(!bck){

            free(fwd);
            fprintf(stderr, "Backward edge allocation failed (Ford_Fulkerson)\n");
            goto cleanup_edges;

        }

        bck->to = u;
        bck->cap = 0;
        bck->next = res_graph[v];
        res_graph[v] = bck;

        // Link edges
        fwd->rev = bck;
        bck->rev = fwd;
        forward_edges[i] = fwd;

    }

    // BFS data structures
    uint16_t *parent = NULL;
    struct r_edge **parent_edge = NULL;
    uint8_t *visited = NULL;
    uint16_t *queue = NULL;

    parent = malloc(graph->size * sizeof(uint16_t));
    parent_edge = calloc(graph->size, sizeof(struct r_edge*));
    visited = calloc(graph->size, sizeof(uint8_t));
    queue = malloc(graph->size * sizeof(uint16_t));

    if(!parent || !parent_edge || !visited || !queue){

        fprintf(stderr, "BFS arrays allocation failed (Ford_Fulkerson)\n");
        free(parent);
        free(parent_edge);
        free(visited);
        free(queue);
        goto cleanup_edges;

    }

    uint16_t source = 0;
    uint16_t sink = graph->size - 1;
    int32_t total_flow = 0;

    // Edmonds-Karp algorithm
    while(1){

        // Initialize BFS data
        for(uint16_t i = 0; i < graph->size; i++){

            visited[i] = 0;
            parent[i] = graph->size; // Invalid index

        }

        uint16_t front = 0, rear = 0;
        queue[rear++] = source;
        visited[source] = 1;
        parent[source] = source;

        int found_path = 0;
        while(front < rear){

            uint16_t u = queue[front++];

            if(u == sink){

                found_path = 1;
                break;

            }

            for(struct r_edge *e = res_graph[u]; e != NULL; e = e->next){

                if(e->cap > 0 && !visited[e->to]){

                    visited[e->to] = 1;
                    parent[e->to] = u;
                    parent_edge[e->to] = e;
                    queue[rear++] = e->to;

                }

            }

        }

        if(!found_path)
            break;

        // Find minimum residual capacity on path
        int16_t min_cap = INT16_MAX;
        for(uint16_t v = sink; v != source; v = parent[v]){

            struct r_edge *e = parent_edge[v];
            if(e->cap < min_cap)
                min_cap = e->cap;

        }

        // Update residual capacities
        for(uint16_t v = sink; v != source; v = parent[v]){

            struct r_edge *e = parent_edge[v];
            e->cap -= min_cap;
            e->rev->cap += min_cap;

        }

        total_flow += min_cap;

    }

    // Build result linked list
    struct result *head = NULL, *tail = NULL;
    for(uint32_t i = 0; i < graph->dir_edges; i++){

        struct result *node = malloc(sizeof(struct result));

        if(!node){

            fprintf(stderr, "Result node allocation failed (Ford_Fulkerson)\n");
            free_result(head);
            head = NULL;
            goto cleanup_bfs;

        }

        // Calculate actual flow: original capacity - residual capacity
        node->start = edges[i].u;
        node->end = edges[i].v;
        node->weight = edges[i].cap - forward_edges[i]->cap;
        node->next = NULL;

        if(!head){
            head = tail = node;
        }else {
            tail->next = node;
            tail = node;
        }

    }

cleanup_bfs:
    free(parent);
    free(parent_edge);
    free(visited);
    free(queue);

cleanup_edges:
    // Free residual graph
    for(uint16_t i = 0; i < graph->size; i++){

        struct r_edge *e = res_graph[i];

        while(e){

            struct r_edge *next = e->next;
            free(e);
            e = next;

        }

    }

    free(res_graph);
    free(forward_edges);
    free(edges);

    return head;

}
