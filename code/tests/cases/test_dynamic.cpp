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

FOSSIL_SUITE(cpp_algorithm_dynamic_fixture);

FOSSIL_SETUP(cpp_algorithm_dynamic_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(cpp_algorithm_dynamic_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

using fossil::algorithm::Dynamic;

FOSSIL_TEST(cpp_test_dynamic_create_and_destroy) {
    fossil_dynamic_t *alg = Dynamic::create("dp-fib");
    ASSUME_ITS_TRUE(alg != NULL);
    Dynamic::destroy(alg);
}

FOSSIL_TEST(cpp_test_dynamic_create_null) {
    fossil_dynamic_t *alg = Dynamic::create("");
    ASSUME_ITS_TRUE(alg == NULL);
}

FOSSIL_TEST(cpp_test_dynamic_supported_true) {
    ASSUME_ITS_TRUE(Dynamic::supported("dp-fib"));
    ASSUME_ITS_TRUE(Dynamic::supported("dp-knapsack"));
    ASSUME_ITS_TRUE(Dynamic::supported("dp-lcs"));
    ASSUME_ITS_TRUE(Dynamic::supported("adaptive-search"));
}

FOSSIL_TEST(cpp_test_dynamic_supported_false) {
    ASSUME_ITS_TRUE(!Dynamic::supported("not-supported"));
    ASSUME_ITS_TRUE(!Dynamic::supported(""));
}

FOSSIL_TEST(cpp_test_dynamic_exec_dp_fib_default) {
    fossil_dynamic_t *alg = Dynamic::create("dp-fib");
    int result = Dynamic::exec(alg, "dp-fib");
    ASSUME_ITS_EQUAL_I32(result, 55); // fib(10)
    Dynamic::destroy(alg);
}

FOSSIL_TEST(cpp_test_dynamic_exec_dp_fib_param) {
    fossil_dynamic_t *alg = Dynamic::create("dp-fib");
    fossil_dynamic_param_t params[] = {{"n", "7"}};
    int result = Dynamic::exec(alg, "dp-fib", params, 1);
    ASSUME_ITS_EQUAL_I32(result, 13); // fib(7)
    Dynamic::destroy(alg);
}

FOSSIL_TEST(cpp_test_dynamic_exec_dp_fib_invalid_alg) {
    fossil_dynamic_t *alg = Dynamic::create("dp-fib");
    int result = Dynamic::exec(alg, "notalgo");
    ASSUME_ITS_EQUAL_I32(result, -3);
    Dynamic::destroy(alg);
}

FOSSIL_TEST(cpp_test_dynamic_exec_null_handle) {
    int result = Dynamic::exec(nullptr, "dp-fib");
    ASSUME_ITS_EQUAL_I32(result, -2);
}

FOSSIL_TEST(cpp_test_dynamic_exec_null_algorithm_id) {
    fossil_dynamic_t *alg = Dynamic::create("dp-fib");
    int result = Dynamic::exec(alg, "");
    ASSUME_ITS_EQUAL_I32(result, -2);
    Dynamic::destroy(alg);
}

FOSSIL_TEST(cpp_test_dynamic_exec_dp_lcs_default) {
    fossil_dynamic_t *alg = Dynamic::create("dp-lcs");
    int result = Dynamic::exec(alg, "dp-lcs");
    ASSUME_ITS_EQUAL_I32(result, 2); // lcs("abc", "ac")
    Dynamic::destroy(alg);
}

FOSSIL_TEST(cpp_test_dynamic_exec_dp_lcs_params) {
    fossil_dynamic_t *alg = Dynamic::create("dp-lcs");
    fossil_dynamic_param_t params[] = {{"a", "abcdef"}, {"b", "abdf"}};
    int result = Dynamic::exec(alg, "dp-lcs", params, 2);
    ASSUME_ITS_EQUAL_I32(result, 4); // lcs("abcdef", "abdf") == "abdf"
    Dynamic::destroy(alg);
}

FOSSIL_TEST(cpp_test_dynamic_exec_dp_knapsack_default) {
    fossil_dynamic_t *alg = Dynamic::create("dp-knapsack");
    int result = Dynamic::exec(alg, "dp-knapsack");
    ASSUME_ITS_EQUAL_I32(result, 0); // default n=0, so result is 0
    Dynamic::destroy(alg);
}


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(cpp_dynamic_tests) {
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_create_and_destroy);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_create_null);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_supported_true);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_supported_false);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_dp_fib_default);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_dp_fib_param);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_dp_fib_invalid_alg);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_null_handle);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_null_algorithm_id);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_dp_lcs_default);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_dp_lcs_params);
    FOSSIL_TEST_ADD(cpp_algorithm_dynamic_fixture, cpp_test_dynamic_exec_dp_knapsack_default);

    FOSSIL_TEST_REGISTER(cpp_algorithm_dynamic_fixture);
} // end of tests
