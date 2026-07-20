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
 * Date: 04/05/2013
 *
 * Copyright (C) 2013-Current Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <fossil/maip/framework.h>

#include "fossil/algorithm/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_SUITE(c_algorithm_sort_fixture);

FOSSIL_SETUP(c_algorithm_sort_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(c_algorithm_sort_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_sort_exec_i32_merge_desc) {
    int arr[] = {1, 4, 2, 8, 6};
    int expected[] = {8, 6, 4, 2, 1};
    int status = fossil_algorithm_sort_exec(arr, 5, "i32", "merge", "desc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_f64_heap_asc) {
    double arr[] = {2.5, 1.1, 3.3, 0.9};
    double expected[] = {0.9, 1.1, 2.5, 3.3};
    int status = fossil_algorithm_sort_exec(arr, 4, "f64", "heap", "asc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_cstr_insertion_desc) {
    const char *arr[] = {"pear", "apple", "banana"};
    const char *expected[] = {"pear", "banana", "apple"};
    int status = fossil_algorithm_sort_exec(arr, 3, "cstr", "insertion", "desc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_u8_counting_asc) {
    uint8_t arr[] = {4, 2, 5, 1, 3};
    uint8_t expected[] = {1, 2, 3, 4, 5};
    int status = fossil_algorithm_sort_exec(arr, 5, "u8", "counting", "asc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_u32_radix_desc) {
    uint32_t arr[] = {10, 100, 1, 50};
    uint32_t expected[] = {100, 50, 10, 1};
    int status = fossil_algorithm_sort_exec(arr, 4, "u32", "radix", "desc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_char_shell_asc) {
    char arr[] = {'d', 'a', 'c', 'b'};
    char expected[] = {'a', 'b', 'c', 'd'};
    int status = fossil_algorithm_sort_exec(arr, 4, "char", "shell", "asc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_bool_bubble_desc) {
    bool arr[] = {true, false, true, false};
    bool expected[] = {true, true, false, false};
    int status = fossil_algorithm_sort_exec(arr, 4, "bool", "bubble", "desc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_invalid_algorithm) {
    int arr[] = {1, 2, 3};
    int status = fossil_algorithm_sort_exec(arr, 3, "i32", "notalgo", "asc");
    ASSUME_ITS_TRUE(status == -3);
}

FOSSIL_TEST(c_test_sort_type_sizeof_supported) {
    ASSUME_ITS_TRUE(fossil_algorithm_sort_type_sizeof("i32") == sizeof(int32_t));
    ASSUME_ITS_TRUE(fossil_algorithm_sort_type_sizeof("f64") == sizeof(double));
    ASSUME_ITS_TRUE(fossil_algorithm_sort_type_sizeof("cstr") == sizeof(char *));
}

FOSSIL_TEST(c_test_sort_type_supported_true_false) {
    ASSUME_ITS_TRUE(fossil_algorithm_sort_type_supported("i16") == true);
    ASSUME_ITS_TRUE(fossil_algorithm_sort_type_supported("null") == false);
}

FOSSIL_TEST(c_test_sort_exec_i64_merge_asc) {
    int64_t arr[] = {10000000000LL, 1LL, -5LL, 0LL, 999LL};
    int64_t expected[] = {-5LL, 0LL, 1LL, 999LL, 10000000000LL};
    int status = fossil_algorithm_sort_exec(arr, 5, "i64", "merge", "asc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_u16_heap_desc) {
    uint16_t arr[] = {10, 2, 30, 25, 5};
    uint16_t expected[] = {30, 25, 10, 5, 2};
    int status = fossil_algorithm_sort_exec(arr, 5, "u16", "heap", "desc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_f32_shell_asc) {
    float arr[] = {3.1f, 2.2f, 5.5f, 1.0f};
    float expected[] = {1.0f, 2.2f, 3.1f, 5.5f};
    int status = fossil_algorithm_sort_exec(arr, 4, "f32", "shell", "asc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_size_bubble_desc) {
    size_t arr[] = {100, 50, 200, 10};
    size_t expected[] = {200, 100, 50, 10};
    int status = fossil_algorithm_sort_exec(arr, 4, "size", "bubble", "desc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

FOSSIL_TEST(c_test_sort_exec_datetime_insertion_asc) {
    int64_t arr[] = {1672531200LL, 1609459200LL, 1640995200LL};
    int64_t expected[] = {1609459200LL, 1640995200LL, 1672531200LL};
    int status = fossil_algorithm_sort_exec(arr, 3, "datetime", "insertion", "asc");
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(arr)) == 0);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_sort_tests) {
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_i32_merge_desc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_f64_heap_asc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_cstr_insertion_desc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_u8_counting_asc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_u32_radix_desc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_char_shell_asc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_bool_bubble_desc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_invalid_algorithm);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_type_sizeof_supported);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_type_supported_true_false);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_i64_merge_asc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_u16_heap_desc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_f32_shell_asc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_size_bubble_desc);
    FOSSIL_ADD_TEST(c_algorithm_sort_fixture, c_test_sort_exec_datetime_insertion_asc);

    FOSSIL_ADD_SUITE(c_algorithm_sort_fixture);
} // end of tests
