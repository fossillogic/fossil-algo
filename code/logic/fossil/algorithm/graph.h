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
#ifndef FOSSIL_ALGORITHM_GRAPH_H
#define FOSSIL_ALGORITHM_GRAPH_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm Graph — Core Types
// ======================================================

/**
 * @brief Opaque graph handle.
 *
 * The concrete representation (adjacency list, matrix, CSR, etc.)
 * is implementation-defined and hidden from the algorithm layer.
 */
typedef struct fossil_graph fossil_graph_t;

/**
 * @brief Graph edge descriptor (optional utility).
 */
typedef struct fossil_graph_edge {
    uint64_t from;
    uint64_t to;
    double   weight;
} fossil_graph_edge_t;

/**
 * @brief Visitor callback for traversal algorithms.
 *
 * @param node_id Current node being visited.
 * @param user User-provided context pointer.
 * @return true to continue traversal, false to stop early.
 */
typedef bool (*fossil_graph_visit_fn)(uint64_t node_id, void *user);

// ======================================================
// Fossil Algorithm Graph — Exec Interface
// ======================================================

/**
 * @brief Executes a graph algorithm using string-identified options.
 *
 * Supported algorithm identifiers (implementation-defined, typical set):
 *   - Traversal: "bfs", "dfs"
 *   - Shortest path: "dijkstra", "bellman-ford", "floyd-warshall"
 *   - Connectivity: "connected", "components"
 *   - Spanning tree: "mst-prim", "mst-kruskal"
 *   - Ordering: "toposort"
 *
 * Supported graph properties:
 *   - Directed / undirected
 *   - Weighted / unweighted
 *
 * Notes:
 * - Not all algorithms require all parameters.
 * - Algorithms that require weights assume non-null edge weights.
 * - No runtime validation of graph correctness is guaranteed.
 *
 * Return values:
 *   >= 0 : algorithm-specific success code
 *   -1   : algorithm failed or target not reachable
 *   -2   : invalid input (null pointers, invalid node ids)
 *   -3   : unknown or unsupported algorithm
 *   -4   : unsupported graph properties for algorithm
 *
 * Example:
 * @code
 * fossil_algorithm_graph_exec(
 *     graph,
 *     "bfs",
 *     start_node,
 *     0,
 *     visitor,
 *     NULL
 * );
 * @endcode
 *
 * @param graph Graph handle.
 * @param algorithm_id Algorithm identifier string.
 * @param start_node Start node (if applicable).
 * @param target_node Target node (if applicable, else ignored).
 * @param visit Optional visitor callback.
 * @param user User context passed to visitor.
 * @return int Status or algorithm-specific result.
 */
int fossil_algorithm_graph_exec(
    fossil_graph_t *graph,
    const char *algorithm_id,
    uint64_t start_node,
    uint64_t target_node,
    fossil_graph_visit_fn visit,
    void *user
);

// ======================================================
// Extended Utility API
// ======================================================

/**
 * @brief Checks whether a graph algorithm is supported.
 *
 * @param algorithm_id Algorithm identifier string.
 * @return true if supported, false otherwise.
 */
bool fossil_algorithm_graph_supported(const char *algorithm_id);

/**
 * @brief Returns whether the algorithm requires weighted edges.
 *
 * @param algorithm_id Algorithm identifier string.
 * @return true if weights are required.
 */
bool fossil_algorithm_graph_requires_weights(const char *algorithm_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {
    namespace algorithm {
    
    /**
     * @brief RAII-friendly C++ wrapper for Fossil graph algorithms.
     */
    class Graph
    {
    public:
        /**
         * @brief Execute a graph algorithm.
         *
         * @param graph Graph handle.
         * @param algorithm_id Algorithm identifier.
         * @param start_node Start node id.
         * @param target_node Target node id (optional).
         * @param visit Visitor callback (optional).
         * @param user User context pointer.
         * @return int Status or algorithm-specific result.
         */
        static int exec(
            fossil_graph_t *graph,
            const std::string &algorithm_id,
            uint64_t start_node = 0,
            uint64_t target_node = 0,
            fossil_graph_visit_fn visit = nullptr,
            void *user = nullptr
        ) {
            return fossil_algorithm_graph_exec(
                graph,
                algorithm_id.c_str(),
                start_node,
                target_node,
                visit,
                user
            );
        }
    
        /**
         * @brief Checks whether an algorithm is supported.
         */
        static bool supported(const std::string &algorithm_id) {
            return fossil_algorithm_graph_supported(algorithm_id.c_str());
        }
    
        /**
         * @brief Checks whether an algorithm requires weighted edges.
         */
        static bool requires_weights(const std::string &algorithm_id) {
            return fossil_algorithm_graph_requires_weights(algorithm_id.c_str());
        }
    };
    
    } // namespace algorithm
} // namespace fossil

#endif /* __cplusplus */

#endif /* FOSSIL_ALGORITHM_GRAPH_H */
