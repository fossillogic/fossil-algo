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

FOSSIL_SUITE(c_algorithm_search_fixture);

FOSSIL_SETUP(c_algorithm_search_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(c_algorithm_search_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_search_exec_linear_i32_found) {
    int arr[] = {5, 2, 9, 1, 7};
    int key = 9;
    int idx = fossil_algorithm_search_exec(arr, 5, &key, "i32", "linear", "asc");
    ASSUME_ITS_EQUAL_I32(idx, 2);
}

FOSSIL_TEST(c_test_search_exec_linear_i32_not_found) {
    int arr[] = {5, 2, 9, 1, 7};
    int key = 3;
    int idx = fossil_algorithm_search_exec(arr, 5, &key, "i32", "linear", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -1);
}

FOSSIL_TEST(c_test_search_exec_binary_f64_asc_found) {
    double arr[] = {1.1, 2.2, 3.3, 4.4};
    double key = 3.3;
    int idx = fossil_algorithm_search_exec(arr, 4, &key, "f64", "binary", "asc");
    ASSUME_ITS_EQUAL_I32(idx, 2);
}

FOSSIL_TEST(c_test_search_exec_binary_f64_desc_found) {
    double arr[] = {4.4, 3.3, 2.2, 1.1};
    double key = 2.2;
    int idx = fossil_algorithm_search_exec(arr, 4, &key, "f64", "binary", "desc");
    ASSUME_ITS_EQUAL_I32(idx, 2);
}

FOSSIL_TEST(c_test_search_exec_jump_u8_asc_found) {
    uint8_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    uint8_t key = 6;
    int idx = fossil_algorithm_search_exec(arr, 8, &key, "u8", "jump", "asc");
    ASSUME_ITS_EQUAL_I32(idx, 5);
}

FOSSIL_TEST(c_test_search_exec_interpolation_i32_asc_found) {
    int32_t arr[] = {10, 20, 30, 40, 50};
    int32_t key = 30;
    int idx = fossil_algorithm_search_exec(arr, 5, &key, "i32", "interpolation", "asc");
    ASSUME_ITS_EQUAL_I32(idx, 2);
}

FOSSIL_TEST(c_test_search_exec_interpolation_i32_unsupported_type) {
    float arr[] = {1.0f, 2.0f, 3.0f};
    float key = 2.0f;
    int idx = fossil_algorithm_search_exec(arr, 3, &key, "f32", "interpolation", "asc");
    ASSUME_ITS_EQUAL_I32(idx, 1);
}

FOSSIL_TEST(c_test_search_exec_exponential_i32_asc_found) {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int key = 7;
    int idx = fossil_algorithm_search_exec(arr, 10, &key, "i32", "exponential", "asc");
    ASSUME_ITS_EQUAL_I32(idx, 6);
}

FOSSIL_TEST(c_test_search_exec_exponential_i32_desc_found) {
    int arr[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int key = 4;
    int idx = fossil_algorithm_search_exec(arr, 10, &key, "i32", "exponential", "desc");
    ASSUME_ITS_EQUAL_I32(idx, 6);
}

FOSSIL_TEST(c_test_search_exec_exponential_i32_not_found) {
    int arr[] = {1, 2, 3, 4, 5};
    int key = 8;
    int idx = fossil_algorithm_search_exec(arr, 5, &key, "i32", "exponential", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -1);
}

FOSSIL_TEST(c_test_search_exec_interpolation_u16_asc_found) {
    uint16_t arr[] = {2, 4, 6, 8, 10, 12, 14, 16};
    uint16_t key = 10;
    int idx = fossil_algorithm_search_exec(arr, 8, &key, "u16", "interpolation", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -4);
}

FOSSIL_TEST(c_test_search_exec_fibonacci_i64_desc_found) {
    int64_t arr[] = {100, 90, 80, 70, 60, 50};
    int64_t key = 70;
    int idx = fossil_algorithm_search_exec(arr, 6, &key, "i64", "fibonacci", "desc");
    ASSUME_ITS_EQUAL_I32(idx, 3);
}

FOSSIL_TEST(c_test_search_exec_cstr_linear_found) {
    const char *arr[] = {"apple", "banana", "pear"};
    const char *key = "banana";
    int idx = fossil_algorithm_search_exec(arr, 3, &key, "cstr", "linear", "asc");
    ASSUME_ITS_EQUAL_I32(idx, 1);
}

FOSSIL_TEST(c_test_search_exec_invalid_type) {
    int arr[] = {1, 2, 3};
    int key = 2;
    int idx = fossil_algorithm_search_exec(arr, 3, &key, "notatype", "linear", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -3);
}

FOSSIL_TEST(c_test_search_exec_invalid_algorithm) {
    int arr[] = {1, 2, 3};
    int key = 2;
    int idx = fossil_algorithm_search_exec(arr, 3, &key, "i32", "notalgo", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -4);
}

FOSSIL_TEST(c_test_search_exec_null_base) {
    int key = 1;
    int idx = fossil_algorithm_search_exec(NULL, 3, &key, "i32", "linear", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -2);
}

FOSSIL_TEST(c_test_search_exec_null_key) {
    int arr[] = {1, 2, 3};
    int idx = fossil_algorithm_search_exec(arr, 3, NULL, "i32", "linear", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -2);
}

FOSSIL_TEST(c_test_search_exec_zero_count) {
    int arr[] = {1, 2, 3};
    int key = 2;
    int idx = fossil_algorithm_search_exec(arr, 0, &key, "i32", "linear", "asc");
    ASSUME_ITS_EQUAL_I32(idx, -2);
}

FOSSIL_TEST(c_test_search_type_sizeof_supported) {
    ASSUME_ITS_TRUE(fossil_algorithm_search_type_sizeof("i64") == sizeof(int64_t));
    ASSUME_ITS_TRUE(fossil_algorithm_search_type_sizeof("f32") == sizeof(float));
    ASSUME_ITS_TRUE(fossil_algorithm_search_type_sizeof("cstr") == sizeof(char *));
}

FOSSIL_TEST(c_test_search_type_supported_true_false) {
    ASSUME_ITS_TRUE(fossil_algorithm_search_type_supported("u32") == true);
    ASSUME_ITS_TRUE(fossil_algorithm_search_type_supported("null") == false);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_search_tests) {
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_linear_i32_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_linear_i32_not_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_binary_f64_asc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_binary_f64_desc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_jump_u8_asc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_interpolation_i32_asc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_interpolation_i32_unsupported_type);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_exponential_i32_asc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_exponential_i32_desc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_exponential_i32_not_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_interpolation_u16_asc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_fibonacci_i64_desc_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_cstr_linear_found);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_invalid_type);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_invalid_algorithm);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_null_base);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_null_key);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_exec_zero_count);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_type_sizeof_supported);
    FOSSIL_TEST_ADD(c_algorithm_search_fixture, c_test_search_type_supported_true_false);

    FOSSIL_TEST_REGISTER(c_algorithm_search_fixture);
} // end of tests
