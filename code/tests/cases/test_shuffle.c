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
#include <fossil/maip/framework.h>

#include "fossil/algorithm/framework.h"

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilities
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_SUITE(c_algorithm_shuffle_fixture);

FOSSIL_SETUP(c_algorithm_shuffle_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(c_algorithm_shuffle_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_shuffle_exec_fisher_yates_i32) {
    int arr[] = {1, 2, 3, 4, 5};
    int orig[] = {1, 2, 3, 4, 5};
    int status = fossil_algorithm_shuffle_exec(arr, 5, "i32", "fisher-yates", "auto", 0);
    ASSUME_ITS_EQUAL_I32(status, 0);
    // At least one element should be different after shuffle
    bool changed = false;
    for (int i = 0; i < 5; ++i) {
        if (arr[i] != orig[i]) {
            changed = true;
            break;
        }
    }
    ASSUME_ITS_TRUE(changed);
}

FOSSIL_TEST(c_test_shuffle_exec_inside_out_f64) {
    double arr[] = {1.1, 2.2, 3.3, 4.4};
    double orig[] = {1.1, 2.2, 3.3, 4.4};
    int status = fossil_algorithm_shuffle_exec(arr, 4, "f64", "inside-out", "auto", 0);
    ASSUME_ITS_EQUAL_I32(status, 0);
    bool changed = false;
    for (int i = 0; i < 4; ++i) {
        if (arr[i] != orig[i]) {
            changed = true;
            break;
        }
    }
    ASSUME_ITS_TRUE(changed);
}

FOSSIL_TEST(c_test_shuffle_exec_auto_u8_seeded) {
    uint8_t arr[] = {10, 20, 30, 40};
    uint8_t orig[] = {10, 20, 30, 40};
    int status = fossil_algorithm_shuffle_exec(arr, 4, "u8", "auto", "seeded", 12345);
    ASSUME_ITS_EQUAL_I32(status, 0);
    bool changed = false;
    for (int i = 0; i < 4; ++i) {
        if (arr[i] != orig[i]) {
            changed = true;
            break;
        }
    }
    ASSUME_ITS_TRUE(changed);
}

FOSSIL_TEST(c_test_shuffle_exec_invalid_type) {
    int arr[] = {1, 2, 3};
    int status = fossil_algorithm_shuffle_exec(arr, 3, "notatype", "auto", "auto", 0);
    ASSUME_ITS_EQUAL_I32(status, -2);
}

FOSSIL_TEST(c_test_shuffle_exec_invalid_algorithm) {
    int arr[] = {1, 2, 3};
    int status = fossil_algorithm_shuffle_exec(arr, 3, "i32", "notalgo", "auto", 0);
    ASSUME_ITS_EQUAL_I32(status, -3);
}

FOSSIL_TEST(c_test_shuffle_exec_null_base) {
    int status = fossil_algorithm_shuffle_exec(NULL, 3, "i32", "auto", "auto", 0);
    ASSUME_ITS_EQUAL_I32(status, -1);
}

FOSSIL_TEST(c_test_shuffle_exec_zero_count) {
    int arr[] = {1, 2, 3};
    int status = fossil_algorithm_shuffle_exec(arr, 0, "i32", "auto", "auto", 0);
    ASSUME_ITS_EQUAL_I32(status, -1);
}

FOSSIL_TEST(c_test_shuffle_exec_null_type_id) {
    int arr[] = {1, 2, 3};
    int status = fossil_algorithm_shuffle_exec(arr, 3, NULL, "auto", "auto", 0);
    ASSUME_ITS_EQUAL_I32(status, -1);
}

FOSSIL_TEST(c_test_shuffle_type_sizeof_supported) {
    ASSUME_ITS_TRUE(fossil_algorithm_shuffle_type_sizeof("i32") == sizeof(int32_t));
    ASSUME_ITS_TRUE(fossil_algorithm_shuffle_type_sizeof("f64") == sizeof(double));
    ASSUME_ITS_TRUE(fossil_algorithm_shuffle_type_sizeof("cstr") == sizeof(char *));
}

FOSSIL_TEST(c_test_shuffle_type_supported_true_false) {
    ASSUME_ITS_TRUE(fossil_algorithm_shuffle_type_supported("u16") == true);
    ASSUME_ITS_TRUE(fossil_algorithm_shuffle_type_supported("notatype") == false);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_shuffle_tests) {
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_fisher_yates_i32);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_inside_out_f64);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_auto_u8_seeded);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_invalid_type);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_invalid_algorithm);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_null_base);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_zero_count);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_exec_null_type_id);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_type_sizeof_supported);
    FOSSIL_TEST_ADD(c_algorithm_shuffle_fixture, c_test_shuffle_type_supported_true_false);

    FOSSIL_TEST_REGISTER(c_algorithm_shuffle_fixture);
} // end of tests
