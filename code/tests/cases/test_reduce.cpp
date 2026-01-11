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

FOSSIL_SUITE(cpp_algorithm_reduce_fixture);

FOSSIL_SETUP(cpp_algorithm_reduce_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(cpp_algorithm_reduce_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_reduce_exec_i32_sum) {
    int32_t arr[] = {1, 2, 3, 4, 5};
    int32_t result = 0;
    int status = fossil::algorithm::Reduce::exec(arr, 5, "i32", &result, "sum");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(result == 15);
}

FOSSIL_TEST(cpp_test_reduce_exec_f64_min) {
    double arr[] = {2.5, 1.1, 3.3, 0.9};
    double result = 0.0;
    int status = fossil::algorithm::Reduce::exec(arr, 4, "f64", &result, "min");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(result == 0.9);
}

FOSSIL_TEST(cpp_test_reduce_exec_u8_max) {
    uint8_t arr[] = {4, 2, 5, 1, 3};
    uint8_t result = 0;
    int status = fossil::algorithm::Reduce::exec(arr, 5, "u8", &result, "max");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(result == 5);
}

FOSSIL_TEST(cpp_test_reduce_exec_bool_any) {
    bool arr[] = {false, false, true, false};
    bool result = false;
    int status = fossil::algorithm::Reduce::exec(arr, 4, "bool", &result, "any");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(result == true);
}

FOSSIL_TEST(cpp_test_reduce_exec_bool_all) {
    bool arr[] = {true, true, true};
    bool result = false;
    int status = fossil::algorithm::Reduce::exec(arr, 3, "bool", &result, "all");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(result == true);
}

FOSSIL_TEST(cpp_test_reduce_exec_count) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    size_t result = 0;
    int status = fossil::algorithm::Reduce::exec(arr, 6, "i32", &result, "count");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(result == 6);
}

static void custom_product(void *accum, const void *elem, void *user) {
    (void)user;
    *(int32_t*)accum *= *(const int32_t*)elem;
}

FOSSIL_TEST(cpp_test_reduce_exec_custom_product) {
    int32_t arr[] = {2, 3, 4};
    int32_t result = 1;
    int status = fossil::algorithm::Reduce::exec(arr, 3, "i32", &result, "custom", 0, custom_product, nullptr);
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(result == 24);
}

FOSSIL_TEST(cpp_test_reduce_type_sizeof_supported) {
    ASSUME_ITS_TRUE(fossil::algorithm::Reduce::type_sizeof("i32") == sizeof(int32_t));
    ASSUME_ITS_TRUE(fossil::algorithm::Reduce::type_sizeof("f64") == sizeof(double));
    ASSUME_ITS_TRUE(fossil::algorithm::Reduce::type_sizeof("bool") == sizeof(bool));
}

FOSSIL_TEST(cpp_test_reduce_type_supported_true_false) {
    ASSUME_ITS_TRUE(fossil::algorithm::Reduce::type_supported("i16") == true);
    ASSUME_ITS_TRUE(fossil::algorithm::Reduce::type_supported("null") == false);
}

FOSSIL_TEST(cpp_test_reduce_exec_invalid_type) {
    int arr[] = {1, 2, 3};
    int result = 0;
    int status = fossil::algorithm::Reduce::exec(arr, 3, "notatype", &result, "sum");
    ASSUME_ITS_TRUE(status == -2);
}

FOSSIL_TEST(cpp_test_reduce_exec_invalid_op) {
    int arr[] = {1, 2, 3};
    int result = 0;
    int status = fossil::algorithm::Reduce::exec(arr, 3, "i32", &result, "notop");
    ASSUME_ITS_TRUE(status == -3);
}

FOSSIL_TEST(cpp_test_reduce_exec_null_input) {
    int result = 0;
    int status = fossil::algorithm::Reduce::exec(nullptr, 3, "i32", &result, "sum");
    ASSUME_ITS_TRUE(status == -1);
}

FOSSIL_TEST(cpp_test_reduce_exec_custom_null_fn) {
    int arr[] = {1, 2, 3};
    int result = 0;
    int status = fossil::algorithm::Reduce::exec(arr, 3, "i32", &result, "custom", 0, nullptr, nullptr);
    ASSUME_ITS_TRUE(status == -1);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(cpp_reduce_tests) {
    // Reduce tests
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_i32_sum);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_f64_min);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_u8_max);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_bool_any);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_bool_all);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_count);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_custom_product);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_type_sizeof_supported);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_type_supported_true_false);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_invalid_type);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_invalid_op);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_null_input);
    FOSSIL_TEST_ADD(cpp_algorithm_reduce_fixture, cpp_test_reduce_exec_custom_null_fn);

    FOSSIL_TEST_REGISTER(cpp_algorithm_reduce_fixture);
} // end of tests
