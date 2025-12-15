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
#include <fossil/pizza/framework.h>

#include "fossil/algorithm/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_SUITE(c_algorithm_graph_fixture);

FOSSIL_SETUP(c_algorithm_graph_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(c_algorithm_graph_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

// Minimal stub graph for testing
typedef struct fossil_graph {
    size_t node_count;
    bool directed;
    bool weighted;
    void *adj;
} fossil_graph_t;

// Dummy visitor that records visited nodes
static bool test_visitor(uint64_t node_id, void *user) {
    (void)node_id; // Mark node_id as used to avoid unused parameter warning
    size_t *count = (size_t *)user;
    (*count)++;
    return true;
}

FOSSIL_TEST(c_test_graph_supported_algorithms) {
    ASSUME_ITS_TRUE(fossil_algorithm_graph_supported("bfs"));
    ASSUME_ITS_TRUE(fossil_algorithm_graph_supported("dfs"));
    ASSUME_ITS_TRUE(fossil_algorithm_graph_supported("dijkstra"));
    ASSUME_ITS_FALSE(fossil_algorithm_graph_supported("mst-kruskal"));
    ASSUME_ITS_FALSE(fossil_algorithm_graph_supported(NULL));
}

FOSSIL_TEST(c_test_graph_requires_weights) {
    ASSUME_ITS_TRUE(fossil_algorithm_graph_requires_weights("dijkstra"));
    ASSUME_ITS_TRUE(fossil_algorithm_graph_requires_weights("bellman-ford"));
    ASSUME_ITS_TRUE(fossil_algorithm_graph_requires_weights("floyd-warshall"));
    ASSUME_ITS_FALSE(fossil_algorithm_graph_requires_weights("bfs"));
    ASSUME_ITS_FALSE(fossil_algorithm_graph_requires_weights(NULL));
}

FOSSIL_TEST(c_test_graph_exec_invalid_inputs) {
    int rc = fossil_algorithm_graph_exec(NULL, "bfs", 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -2);

    fossil_graph_t dummy = {0};
    rc = fossil_algorithm_graph_exec(&dummy, NULL, 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -2);

    rc = fossil_algorithm_graph_exec(&dummy, "notalgo", 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -3);
}

FOSSIL_TEST(c_test_graph_exec_unsupported_algorithm) {
    fossil_graph_t dummy = {0};
    int rc = fossil_algorithm_graph_exec(&dummy, "mst-kruskal", 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -3);
}

FOSSIL_TEST(c_test_graph_exec_requires_weights_on_unweighted) {
    fossil_graph_t dummy = {.node_count = 2, .weighted = false};
    int rc = fossil_algorithm_graph_exec(&dummy, "dijkstra", 0, 1, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -4);
}

FOSSIL_TEST(c_test_graph_exec_invalid_node_ids) {
    fossil_graph_t dummy = {.node_count = 2, .weighted = true};
    int rc = fossil_algorithm_graph_exec(&dummy, "dijkstra", 5, 1, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -2);
    rc = fossil_algorithm_graph_exec(&dummy, "bfs", 3, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -2);
}

FOSSIL_TEST(c_test_graph_exec_bfs_and_dfs_empty_graph) {
    fossil_graph_t dummy = {.node_count = 0};
    int rc = fossil_algorithm_graph_exec(&dummy, "bfs", 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -2);
    rc = fossil_algorithm_graph_exec(&dummy, "dfs", 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, -2);
}

FOSSIL_TEST(c_test_graph_exec_bfs_and_dfs_null_visitor) {
    fossil_graph_t dummy = {.node_count = 1};
    int rc = fossil_algorithm_graph_exec(&dummy, "bfs", 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    rc = fossil_algorithm_graph_exec(&dummy, "dfs", 0, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_graph_exec_bfs_and_dfs_with_visitor) {
    fossil_graph_t dummy = {.node_count = 1};
    size_t count = 0;
    int rc = fossil_algorithm_graph_exec(&dummy, "bfs", 0, 0, test_visitor, &count);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_I32(count, 1);

    count = 0;
    rc = fossil_algorithm_graph_exec(&dummy, "dfs", 0, 0, test_visitor, &count);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_I32(count, 1);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_graph_tests) {
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_supported_algorithms);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_requires_weights);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_exec_invalid_inputs);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_exec_unsupported_algorithm);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_exec_requires_weights_on_unweighted);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_exec_invalid_node_ids);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_exec_bfs_and_dfs_empty_graph);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_exec_bfs_and_dfs_null_visitor);
    FOSSIL_TEST_ADD(c_algorithm_graph_fixture, c_test_graph_exec_bfs_and_dfs_with_visitor);

    FOSSIL_TEST_REGISTER(c_algorithm_graph_fixture);
} // end of tests
