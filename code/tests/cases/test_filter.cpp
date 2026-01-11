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

FOSSIL_SUITE(cpp_algorithm_filter_fixture);

FOSSIL_SETUP(cpp_algorithm_filter_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(cpp_algorithm_filter_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Filter
// * * * * * * * * * * * * * * * * * * * * * * * *

// Predicate: keep even integers
static bool keep_even(const void *element, void *user) {
    (void)user;
    return (*(const int *)element) % 2 == 0;
}

// Predicate: keep double values > threshold (user points to double)
static bool keep_gt_threshold(const void *element, void *user) {
    double value = *(const double *)element;
    double threshold = *(const double *)user;
    return value > threshold;
}

// Predicate: keep non-null pointers
static bool keep_non_null(const void *element, void *user) {
    (void)user;
    return *(const void * const *)element != NULL;
}

// Predicate: keep true booleans
static bool keep_true(const void *element, void *user) {
    (void)user;
    return *(const bool *)element;
}

FOSSIL_TEST(cpp_test_filter_exec_i32_inplace_even) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    int expected[] = {2, 4, 6};
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 6, "i32", "inplace", NULL, 0, keep_even, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(out_count == 3);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(expected)) == 0);
}

FOSSIL_TEST(cpp_test_filter_exec_f64_stable_gt_threshold) {
    double arr[] = {1.5, 3.2, 2.1, 4.8, 0.9};
    double expected[] = {3.2, 2.1, 4.8};
    double threshold = 2.0;
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 5, "f64", "stable", NULL, 0, keep_gt_threshold, &threshold, &out_count
    );
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(out_count == 3);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(expected)) == 0);
}

FOSSIL_TEST(cpp_test_filter_exec_cstr_lane_compact_non_null) {
    const char *arr[] = {"apple", NULL, "banana", NULL, "pear"};
    const char *expected[] = {"apple", "banana", "pear"};
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 5, "cstr", "lane-compact", NULL, 2, keep_non_null, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(out_count == 3);
    ASSUME_ITS_TRUE(memcmp(arr, expected, sizeof(expected)) == 0);
}

FOSSIL_TEST(cpp_test_filter_exec_bool_count_only_true) {
    bool arr[] = {true, false, true, false, false, true};
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 6, "bool", "count-only", NULL, 0, keep_true, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(out_count == 3);
}

FOSSIL_TEST(cpp_test_filter_exec_i32_partition_even) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    int expected_prefix[] = {2, 4, 6};
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 6, "i32", "partition", NULL, 0, keep_even, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status == 0);
    // Only the first out_count elements are guaranteed to be kept
    ASSUME_ITS_TRUE(out_count == 3);
    ASSUME_ITS_TRUE(memcmp(arr, expected_prefix, sizeof(expected_prefix)) == 0);
}

FOSSIL_TEST(cpp_test_filter_exec_invalid_type) {
    int arr[] = {1, 2, 3};
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 3, "notatype", "inplace", NULL, 0, keep_even, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status == -2);
}

FOSSIL_TEST(cpp_test_filter_exec_invalid_algorithm) {
    int arr[] = {1, 2, 3};
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 3, "i32", "notalgo", NULL, 0, keep_even, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status == -3);
}

FOSSIL_TEST(cpp_test_filter_type_sizeof_supported) {
    ASSUME_ITS_TRUE(fossil_algorithm_filter_type_sizeof("i32") == sizeof(int32_t));
    ASSUME_ITS_TRUE(fossil_algorithm_filter_type_sizeof("f64") == sizeof(double));
    ASSUME_ITS_TRUE(fossil_algorithm_filter_type_sizeof("cstr") == sizeof(char *));
}

FOSSIL_TEST(cpp_test_filter_type_supported_true_false) {
    ASSUME_ITS_TRUE(fossil_algorithm_filter_type_supported("i16") == true);
    ASSUME_ITS_TRUE(fossil_algorithm_filter_type_supported("null") == false);
}

FOSSIL_TEST(cpp_test_filter_exec_dry_run) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    size_t out_count = 0;
    int status = fossil_algorithm_filter_exec(
        arr, 6, "i32", "auto", "dry-run", 0, keep_even, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status == 0);
    ASSUME_ITS_TRUE(out_count == 3);
}

FOSSIL_TEST(cpp_test_filter_exec_first_last) {
    int arr[] = {1, 3, 5, 8, 9};
    size_t out_count = 0;
    int status_first = fossil_algorithm_filter_exec(
        arr, 5, "i32", "first", NULL, 0, keep_even, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status_first == 0);
    ASSUME_ITS_TRUE(out_count == 1);

    int status_last = fossil_algorithm_filter_exec(
        arr, 5, "i32", "last", NULL, 0, keep_even, NULL, &out_count
    );
    ASSUME_ITS_TRUE(status_last == 0);
    ASSUME_ITS_TRUE(out_count == 1);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(cpp_filter_tests) {
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_i32_inplace_even);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_f64_stable_gt_threshold);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_cstr_lane_compact_non_null);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_bool_count_only_true);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_i32_partition_even);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_invalid_type);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_invalid_algorithm);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_type_sizeof_supported);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_type_supported_true_false);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_dry_run);
    FOSSIL_TEST_ADD(cpp_algorithm_filter_fixture, cpp_test_filter_exec_first_last);

    FOSSIL_TEST_REGISTER(cpp_algorithm_filter_fixture);
} // end of tests
