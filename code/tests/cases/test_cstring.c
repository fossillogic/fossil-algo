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

FOSSIL_SUITE(c_algorithm_cstring_fixture);

FOSSIL_SETUP(c_algorithm_cstring_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(c_algorithm_cstring_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_cstring_exec_find_found) {
    char out[32];
    int idx = fossil_algorithm_cstring_exec("hello world", "world", "find", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(idx, 6);
}

FOSSIL_TEST(c_test_cstring_exec_find_not_found) {
    char out[32];
    int idx = fossil_algorithm_cstring_exec("hello world", "mars", "find", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(idx, -1);
}

FOSSIL_TEST(c_test_cstring_exec_rfind_found) {
    char out[32];
    int idx = fossil_algorithm_cstring_exec("one two one", "one", "rfind", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(idx, 8);
}

FOSSIL_TEST(c_test_cstring_exec_rfind_not_found) {
    char out[32];
    int idx = fossil_algorithm_cstring_exec("abc def", "xyz", "rfind", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(idx, -1);
}

FOSSIL_TEST(c_test_cstring_exec_count) {
    char out[32];
    int cnt = fossil_algorithm_cstring_exec("ababab", "ab", "count", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(cnt, 3);
}

FOSSIL_TEST(c_test_cstring_exec_equals_true) {
    char out[32];
    int eq = fossil_algorithm_cstring_exec("abc", "abc", "equals", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(eq, 1);
}

FOSSIL_TEST(c_test_cstring_exec_equals_false) {
    char out[32];
    int eq = fossil_algorithm_cstring_exec("abc", "def", "equals", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(eq, -1);
}

FOSSIL_TEST(c_test_cstring_exec_iequals_true) {
    char out[32];
    int eq = fossil_algorithm_cstring_exec("Hello", "hELLo", "iequals", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(eq, 1);
}

FOSSIL_TEST(c_test_cstring_exec_iequals_false) {
    char out[32];
    int eq = fossil_algorithm_cstring_exec("Hello", "World", "iequals", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(eq, -1);
}

FOSSIL_TEST(c_test_cstring_exec_toupper) {
    char out[32];
    int rc = fossil_algorithm_cstring_exec("abcDe", NULL, "toupper", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_TRUE(strcmp(out, "ABCDE") == 0);
}

FOSSIL_TEST(c_test_cstring_exec_tolower) {
    char out[32];
    int rc = fossil_algorithm_cstring_exec("AbCDE", NULL, "tolower", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_TRUE(strcmp(out, "abcde") == 0);
}

FOSSIL_TEST(c_test_cstring_exec_reverse) {
    char out[32];
    int rc = fossil_algorithm_cstring_exec("abcde", NULL, "reverse", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_TRUE(strcmp(out, "edcba") == 0);
}

FOSSIL_TEST(c_test_cstring_exec_invalid_algorithm) {
    char out[32];
    int rc = fossil_algorithm_cstring_exec("abc", "abc", "notalgo", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(rc, -3);
}

FOSSIL_TEST(c_test_cstring_exec_null_input) {
    char out[32];
    int rc = fossil_algorithm_cstring_exec(NULL, "abc", "find", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(rc, -2);
}

FOSSIL_TEST(c_test_cstring_exec_null_algorithm_id) {
    char out[32];
    int rc = fossil_algorithm_cstring_exec("abc", "abc", NULL, out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(rc, -2);
}

FOSSIL_TEST(c_test_cstring_exec_output_too_small) {
    char out[2];
    int rc = fossil_algorithm_cstring_exec("abc", NULL, "reverse", out, sizeof(out));
    ASSUME_ITS_EQUAL_I32(rc, -2);
}

FOSSIL_TEST(c_test_cstring_supported_true) {
    ASSUME_ITS_TRUE(fossil_algorithm_cstring_supported("find"));
    ASSUME_ITS_TRUE(fossil_algorithm_cstring_supported("toupper"));
    ASSUME_ITS_TRUE(fossil_algorithm_cstring_supported("reverse"));
}

FOSSIL_TEST(c_test_cstring_supported_false) {
    ASSUME_ITS_TRUE(!fossil_algorithm_cstring_supported("notalgo"));
    ASSUME_ITS_TRUE(!fossil_algorithm_cstring_supported(""));
    ASSUME_ITS_TRUE(!fossil_algorithm_cstring_supported(NULL));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(c_cstring_tests) {
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_find_found);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_find_not_found);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_rfind_found);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_rfind_not_found);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_count);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_equals_true);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_equals_false);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_iequals_true);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_iequals_false);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_toupper);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_tolower);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_reverse);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_invalid_algorithm);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_null_input);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_null_algorithm_id);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_exec_output_too_small);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_supported_true);
    FOSSIL_TEST_ADD(c_algorithm_cstring_fixture, c_test_cstring_supported_false);

    FOSSIL_TEST_REGISTER(c_algorithm_cstring_fixture);
} // end of tests
