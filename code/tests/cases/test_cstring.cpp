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

FOSSIL_SUITE(cpp_algorithm_cstring_fixture);

FOSSIL_SETUP(cpp_algorithm_cstring_fixture) {
    // Setup the test fixture
}

FOSSIL_TEARDOWN(cpp_algorithm_cstring_fixture) {
    // Teardown the test fixture
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Sort
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_cstring_class_exec_find_found) {
    std::string out;
    int idx = fossil::algorithm::CString::exec("hello world", "world", "find", &out);
    ASSUME_ITS_EQUAL_I32(idx, 6);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_exec_find_not_found) {
    std::string out;
    int idx = fossil::algorithm::CString::exec("hello world", "mars", "find", &out);
    ASSUME_ITS_EQUAL_I32(idx, -1);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_exec_rfind_found) {
    std::string out;
    int idx = fossil::algorithm::CString::exec("one two one", "one", "rfind", &out);
    ASSUME_ITS_EQUAL_I32(idx, 8);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_exec_count) {
    std::string out;
    int cnt = fossil::algorithm::CString::exec("ababab", "ab", "count", &out);
    ASSUME_ITS_EQUAL_I32(cnt, 3);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_exec_equals_true) {
    std::string out;
    int eq = fossil::algorithm::CString::exec("abc", "abc", "equals", &out);
    ASSUME_ITS_EQUAL_I32(eq, 1);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_exec_iequals_false) {
    std::string out;
    int eq = fossil::algorithm::CString::exec("Hello", "World", "iequals", &out);
    ASSUME_ITS_EQUAL_I32(eq, -1);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_exec_toupper) {
    std::string out;
    int rc = fossil::algorithm::CString::exec("abcDe", "", "toupper", &out);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_TRUE(out == "ABCDE");
}

FOSSIL_TEST(cpp_test_cstring_class_exec_tolower) {
    std::string out;
    int rc = fossil::algorithm::CString::exec("AbCDE", "", "tolower", &out);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_TRUE(out == "abcde");
}

FOSSIL_TEST(cpp_test_cstring_class_exec_reverse) {
    std::string out;
    int rc = fossil::algorithm::CString::exec("abcde", "", "reverse", &out);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_TRUE(out == "edcba");
}

FOSSIL_TEST(cpp_test_cstring_class_exec_invalid_algorithm) {
    std::string out;
    int rc = fossil::algorithm::CString::exec("abc", "abc", "notalgo", &out);
    ASSUME_ITS_EQUAL_I32(rc, -3);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_exec_null_output) {
    int rc = fossil::algorithm::CString::exec("abcde", "", "reverse", nullptr);
    ASSUME_ITS_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(cpp_test_cstring_class_exec_empty_input) {
    std::string out;
    int rc = fossil::algorithm::CString::exec("", "", "toupper", &out);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_TRUE(out.empty());
}

FOSSIL_TEST(cpp_test_cstring_class_supported_true) {
    ASSUME_ITS_TRUE(fossil::algorithm::CString::supported("find"));
    ASSUME_ITS_TRUE(fossil::algorithm::CString::supported("toupper"));
}

FOSSIL_TEST(cpp_test_cstring_class_supported_false) {
    ASSUME_ITS_TRUE(!fossil::algorithm::CString::supported("notalgo"));
    ASSUME_ITS_TRUE(!fossil::algorithm::CString::supported(""));
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *
FOSSIL_TEST_GROUP(cpp_cstring_tests) {
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_find_found);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_find_not_found);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_rfind_found);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_count);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_equals_true);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_iequals_false);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_toupper);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_tolower);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_reverse);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_invalid_algorithm);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_null_output);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_exec_empty_input);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_supported_true);
    FOSSIL_TEST_ADD(cpp_algorithm_cstring_fixture, cpp_test_cstring_class_supported_false);

    FOSSIL_TEST_REGISTER(cpp_algorithm_cstring_fixture);
} // end of tests

