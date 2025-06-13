#include "../lib/graph.h"
#include "../lib/result_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Temporary structure to hold edge information
struct edge_info {
    uint16_t u;
    uint16_t v;
    int16_t weight;
};

// Union-Find (Disjoint Set) functions
static uint16_t find(uint16_t x, uint16_t parent[]) {
    if (parent[x] != x) {
        parent[x] = find(parent[x], parent); // Path compression
    }
    return parent[x];
}

static void link(uint16_t root_x, uint16_t root_y, uint16_t parent[], uint16_t rank[]) {
    if (rank[root_x] < rank[root_y]) {
        parent[root_x] = root_y;
    } else if (rank[root_x] > rank[root_y]) {
        parent[root_y] = root_x;
    } else {
        parent[root_y] = root_x;
        rank[root_x]++;
    }
}

// Comparison function for sorting edges by weight
static int compare_edges(const void* a, const void* b) {
    const struct edge_info* edge1 = (const struct edge_info*)a;
    const struct edge_info* edge2 = (const struct edge_info*)b;
    if (edge1->weight < edge2->weight) return -1;
    if (edge1->weight > edge2->weight) return 1;
    return 0;
}

struct result* Kruskal(struct graph* graph, int16_t (*find_edge)(struct graph*, uint16_t, uint16_t)) {
    if (graph->size == 0) {
        return NULL;
    }

    // Calculate maximum possible edges
    uint32_t max_edges = (graph->size * (graph->size - 1)) / 2;
    struct edge_info* edge_array = malloc(max_edges * sizeof(struct edge_info));
    if (!edge_array) {
        fprintf(stderr, "Failed to allocate edge array (Kruskal)\n");
        return NULL;
    }

    // Collect all unique edges (u < v to avoid duplicates)
    uint32_t num_edges = 0;
    for (uint16_t u = 0; u < graph->size; u++) {
        for (uint16_t v = u + 1; v < graph->size; v++) {
            int16_t weight = find_edge(graph, u, v);
            if (weight > 0) {
                edge_array[num_edges].u = u;
                edge_array[num_edges].v = v;
                edge_array[num_edges].weight = weight;
                num_edges++;
            }
        }
    }

    // Sort edges by weight
    qsort(edge_array, num_edges, sizeof(struct edge_info), compare_edges);

    // Allocate memory for Union-Find
    uint16_t* parent = malloc(graph->size * sizeof(uint16_t));
    uint16_t* rank = malloc(graph->size * sizeof(uint16_t));
    if (!parent || !rank) {
        fprintf(stderr, "Failed to allocate Union-Find arrays (Kruskal)\n");
        free(edge_array);
        free(parent);
        free(rank);
        return NULL;
    }

    // Initialize Union-Find
    for (uint16_t i = 0; i < graph->size; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    // Build MST as linked list
    struct result* head = NULL;
    struct result* tail = NULL;
    uint16_t edge_count = 0;

    for (uint32_t i = 0; i < num_edges && edge_count < graph->size - 1; i++) {
        uint16_t u = edge_array[i].u;
        uint16_t v = edge_array[i].v;
        int16_t weight = edge_array[i].weight;

        uint16_t root_u = find(u, parent);
        uint16_t root_v = find(v, parent);

        if (root_u != root_v) {
            // Add edge to MST
            struct result* node = malloc(sizeof(struct result));
            if (!node) {
                fprintf(stderr, "Failed to allocate result node (Kruskal)\n");
                // Cleanup: free MST list, Union-Find, and edge array
                while (head) {
                    struct result* temp = head;
                    head = head->next;
                    free(temp);
                }
                free(edge_array);
                free(parent);
                free(rank);
                return NULL;
            }

            node->start = u;
            node->end = v;
            node->weight = weight;
            node->next = NULL;

            if (!head) {
                head = tail = node;
            } else {
                tail->next = node;
                tail = node;
            }

            link(root_u, root_v, parent, rank);
            edge_count++;
        }
    }

    // Check if MST is complete
    if (edge_count < graph->size - 1) {
        fprintf(stderr, "Disconnected graph (Kruskal)\n");
        while (head) {
            struct result* temp = head;
            head = head->next;
            free(temp);
        }
        head = NULL;
    }

    // Cleanup
    free(edge_array);
    free(parent);
    free(rank);

    return head;
}