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

FOSSIL_SUITE(c_algorithm_dynamic_fixture);

FOSSIL_SETUP(c_algorithm_dynamic_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(c_algorithm_dynamic_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_dynamic_create_and_destroy) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-fib");
    ASSUME_ITS_TRUE(alg != NULL);
    fossil_algorithm_dynamic_destroy(alg);
}

FOSSIL_TEST(c_test_dynamic_create_null) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create(NULL);
    ASSUME_ITS_TRUE(alg == NULL);
}

FOSSIL_TEST(c_test_dynamic_supported_true) {
    ASSUME_ITS_TRUE(fossil_algorithm_dynamic_supported("dp-fib"));
    ASSUME_ITS_TRUE(fossil_algorithm_dynamic_supported("dp-knapsack"));
    ASSUME_ITS_TRUE(fossil_algorithm_dynamic_supported("dp-lcs"));
    ASSUME_ITS_TRUE(fossil_algorithm_dynamic_supported("adaptive-search"));
}

FOSSIL_TEST(c_test_dynamic_supported_false) {
    ASSUME_ITS_TRUE(!fossil_algorithm_dynamic_supported("not-supported"));
    ASSUME_ITS_TRUE(!fossil_algorithm_dynamic_supported(NULL));
}

FOSSIL_TEST(c_test_dynamic_exec_dp_fib_default) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-fib");
    int result = fossil_algorithm_dynamic_exec(alg, "dp-fib", NULL, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, 55); // fib(10)
    fossil_algorithm_dynamic_destroy(alg);
}

FOSSIL_TEST(c_test_dynamic_exec_dp_fib_param) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-fib");
    fossil_dynamic_param_t params[] = {{"n", "7"}};
    int result = fossil_algorithm_dynamic_exec(alg, "dp-fib", params, 1, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, 13); // fib(7)
    fossil_algorithm_dynamic_destroy(alg);
}

FOSSIL_TEST(c_test_dynamic_exec_dp_fib_invalid_alg) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-fib");
    int result = fossil_algorithm_dynamic_exec(alg, "notalgo", NULL, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, -3);
    fossil_algorithm_dynamic_destroy(alg);
}

FOSSIL_TEST(c_test_dynamic_exec_null_handle) {
    int result = fossil_algorithm_dynamic_exec(NULL, "dp-fib", NULL, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, -2);
}

FOSSIL_TEST(c_test_dynamic_exec_null_algorithm_id) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-fib");
    int result = fossil_algorithm_dynamic_exec(alg, NULL, NULL, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, -2);
    fossil_algorithm_dynamic_destroy(alg);
}

FOSSIL_TEST(c_test_dynamic_exec_dp_lcs_default) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-lcs");
    int result = fossil_algorithm_dynamic_exec(alg, "dp-lcs", NULL, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, 2); // lcs("abc", "ac")
    fossil_algorithm_dynamic_destroy(alg);
}

FOSSIL_TEST(c_test_dynamic_exec_dp_lcs_params) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-lcs");
    fossil_dynamic_param_t params[] = {{"a", "abcdef"}, {"b", "abdf"}};
    int result = fossil_algorithm_dynamic_exec(alg, "dp-lcs", params, 2, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, 4); // lcs("abcdef", "abdf") == "abdf"
    fossil_algorithm_dynamic_destroy(alg);
}

FOSSIL_TEST(c_test_dynamic_exec_dp_knapsack_default) {
    fossil_dynamic_t *alg = fossil_algorithm_dynamic_create("dp-knapsack");
    int result = fossil_algorithm_dynamic_exec(alg, "dp-knapsack", NULL, 0, NULL, NULL);
    ASSUME_ITS_EQUAL_I32(result, -2); // default params missing, expect error code -2
    fossil_algorithm_dynamic_destroy(alg);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_dynamic_tests) {
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_create_and_destroy);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_create_null);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_supported_true);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_supported_false);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_dp_fib_default);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_dp_fib_param);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_dp_fib_invalid_alg);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_null_handle);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_null_algorithm_id);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_dp_lcs_default);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_dp_lcs_params);
    FOSSIL_TEST_ADD(c_algorithm_dynamic_fixture, c_test_dynamic_exec_dp_knapsack_default);

    FOSSIL_TEST_REGISTER(c_algorithm_dynamic_fixture);
} // end of tests
