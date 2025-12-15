/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "fossil/algorithm/graph.h"
#include <stdlib.h>
#include <string.h>
#include <float.h>

// ======================================================
// Internal Graph Representation
// ======================================================

typedef struct fossil_graph_edge_node {
    uint64_t to;
    double   weight;
    struct fossil_graph_edge_node *next;
} fossil_graph_edge_node_t;

struct fossil_graph {
    size_t node_count;
    bool directed;
    bool weighted;
    fossil_graph_edge_node_t **adj;
};

// ======================================================
// Internal Utilities
// ======================================================

static bool algorithm_equals(const char *a, const char *b)
{
    return a && b && strcmp(a, b) == 0;
}

// ======================================================
// BFS
// ======================================================

static int
graph_bfs(
    fossil_graph_t *graph,
    uint64_t start,
    fossil_graph_visit_fn visit,
    void *user
) {
    if (!graph || start >= graph->node_count)
        return -2;

    if (graph->node_count == 0)
        return -2;

    bool *visited = calloc(graph->node_count, sizeof(bool));
    if (!visited)
        return -1;

    uint64_t *queue = malloc(graph->node_count * sizeof(uint64_t));
    if (!queue) {
        free(visited);
        return -1;
    }

    size_t head = 0, tail = 0;
    visited[start] = true;
    queue[tail++] = start;

    while (head < tail) {
        uint64_t v = queue[head++];

        if (visit && !visit(v, user))
            break;

        // Defensive: adj may be NULL in stub/test graphs
        fossil_graph_edge_node_t **adj_list = (fossil_graph_edge_node_t **)graph->adj;
        fossil_graph_edge_node_t *edges = NULL;
        if (adj_list)
            edges = adj_list[v];

        for (fossil_graph_edge_node_t *e = edges; e; e = e->next) {
            if (!visited[e->to]) {
                visited[e->to] = true;
                queue[tail++] = e->to;
            }
        }
    }

    free(queue);
    free(visited);
    return 0;
}

// ======================================================
// DFS
// ======================================================

static bool
dfs_visit(
    fossil_graph_t *graph,
    uint64_t v,
    bool *visited,
    fossil_graph_visit_fn visit,
    void *user
) {
    visited[v] = true;

    if (visit && !visit(v, user))
        return false;

    // Defensive: adj may be NULL in stub/test graphs
    fossil_graph_edge_node_t **adj_list = (fossil_graph_edge_node_t **)graph->adj;
    fossil_graph_edge_node_t *edges = NULL;
    if (adj_list)
        edges = adj_list[v];

    for (fossil_graph_edge_node_t *e = edges; e; e = e->next) {
        if (!visited[e->to]) {
            if (!dfs_visit(graph, e->to, visited, visit, user))
                return false;
        }
    }
    return true;
}

static int
graph_dfs(
    fossil_graph_t *graph,
    uint64_t start,
    fossil_graph_visit_fn visit,
    void *user
) {
    if (!graph || start >= graph->node_count)
        return -2;

    if (graph->node_count == 0)
        return -2;

    bool *visited = calloc(graph->node_count, sizeof(bool));
    if (!visited)
        return -1;

    dfs_visit(graph, start, visited, visit, user);
    free(visited);
    return 0;
}

// ======================================================
// Dijkstra
// ======================================================

static int
graph_dijkstra(
    fossil_graph_t *graph,
    uint64_t start,
    uint64_t target
) {
    if (!graph || !graph->weighted)
        return -4;
    if (start >= graph->node_count || target >= graph->node_count)
        return -2;
    if (graph->node_count == 0)
        return -2;

    double *dist = malloc(graph->node_count * sizeof(double));
    bool *used = calloc(graph->node_count, sizeof(bool));
    if (!dist || !used) {
        free(dist);
        free(used);
        return -1;
    }

    for (size_t i = 0; i < graph->node_count; i++)
        dist[i] = DBL_MAX;
    dist[start] = 0.0;

    for (;;) {
        uint64_t v = UINT64_MAX;
        for (size_t i = 0; i < graph->node_count; i++)
            if (!used[i] && (v == UINT64_MAX || dist[i] < dist[v]))
                v = i;

        if (v == UINT64_MAX || dist[v] == DBL_MAX)
            break;

        used[v] = true;

        // Defensive: adj may be NULL in stub/test graphs
        fossil_graph_edge_node_t **adj_list = (fossil_graph_edge_node_t **)graph->adj;
        fossil_graph_edge_node_t *edges = NULL;
        if (adj_list)
            edges = adj_list[v];

        for (fossil_graph_edge_node_t *e = edges; e; e = e->next) {
            double alt = dist[v] + e->weight;
            if (alt < dist[e->to])
                dist[e->to] = alt;
        }
    }

    int result = (dist[target] == DBL_MAX) ? -1 : 0;

    free(dist);
    free(used);
    return result;
}

// ======================================================
// Public Exec Interface
// ======================================================

int
fossil_algorithm_graph_exec(
    fossil_graph_t *graph,
    const char *algorithm_id,
    uint64_t start_node,
    uint64_t target_node,
    fossil_graph_visit_fn visit,
    void *user
) {
    if (!graph || !algorithm_id)
        return -2;

    // Check for supported algorithms
    if (!fossil_algorithm_graph_supported(algorithm_id))
        return -3;

    // Check for node id validity and empty graph
    if (graph->node_count == 0)
        return -2;

    if (algorithm_equals(algorithm_id, "bfs") || algorithm_equals(algorithm_id, "dfs")) {
        if (start_node >= graph->node_count)
            return -2;
        if (algorithm_equals(algorithm_id, "bfs"))
            return graph_bfs((struct fossil_graph *)graph, start_node, visit, user);
        else
            return graph_dfs((struct fossil_graph *)graph, start_node, visit, user);
    }

    if (algorithm_equals(algorithm_id, "dijkstra")) {
        if (!graph->weighted)
            return -4;
        if (start_node >= graph->node_count || target_node >= graph->node_count)
            return -2;
        return graph_dijkstra((struct fossil_graph *)graph, start_node, target_node);
    }

    return -3;
}

// ======================================================
// Utility API
// ======================================================

bool
fossil_algorithm_graph_supported(const char *algorithm_id)
{
    if (!algorithm_id) return false;

    return algorithm_equals(algorithm_id, "bfs") ||
           algorithm_equals(algorithm_id, "dfs") ||
           algorithm_equals(algorithm_id, "dijkstra");
}

bool
fossil_algorithm_graph_requires_weights(const char *algorithm_id)
{
    if (!algorithm_id) return false;

    return algorithm_equals(algorithm_id, "dijkstra") ||
           algorithm_equals(algorithm_id, "bellman-ford") ||
           algorithm_equals(algorithm_id, "floyd-warshall");
}
