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
#ifndef FOSSIL_ALGORITHM_SEARCH_H
#define FOSSIL_ALGORITHM_SEARCH_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm Search — Exec Interface
// ======================================================

/**
 * @brief Executes a search operation on an array using configurable algorithms.
 *
 * This function provides a flexible runtime interface for searching arrays of
 * arbitrary types, using a variety of search algorithms and sort orders, all
 * specified by string identifiers. It supports linear, binary, jump, interpolation,
 * exponential, and Fibonacci search methods, and can handle both ascending and
 * descending order. The type of array elements is specified by a string identifier,
 * and the appropriate comparator is selected automatically.
 *
 * Limitations and notes for options:
 * - "binary", "jump", "exponential", "fibonacci": Require the array to be sorted
 *   according to the specified order ("asc" or "desc"). No runtime check is performed.
 * - "interpolation": Only supports sorted arrays of uniformly distributed integers
 *   (int32_t or int64_t). Returns -4 for unsupported types.
 * - "linear": Works for any type and order, no sorting required.
 * - If the type or algorithm is unknown or unsupported, returns -3 or -4.
 * - No runtime validation of array sorting or distribution is performed.
 *
 * The function returns the index of the found element, or a negative error code:
 *   - `-1` for not found
 *   - `-2` for invalid input (null pointers, zero count, missing type)
 *   - `-3` for unknown type or comparator
 *   - `-4` for unknown or unsupported algorithm
 *
 * Example usage:
 * @code
 * float values[] = { 3.2f, 1.5f, 9.1f };
 * float key = 1.5f;
 * int idx = fossil_algorithm_search_exec(values, 3, &key, "f32", "binary", "asc");
 * @endcode
 *
 * @param base Pointer to the array to search.
 * @param count Number of elements in the array.
 * @param key Pointer to the key to search for.
 * @param type_id String identifier for data type (e.g., "i32", "f64", "cstr").
 * @param algorithm_id String identifier for search algorithm ("auto", "binary", "linear", etc.).
 * @param order_id String identifier for sort order ("asc", "desc").
 * @return int Index of found element, or negative error code.
 */
int fossil_algorithm_search_exec(
    const void *base,
    size_t count,
    const void *key,
    const char *type_id,
    const char *algorithm_id,
    const char *order_id
);

// ======================================================
// Extended Utility API
// ======================================================

/**
 * @brief Returns the byte size of a type based on its string identifier.
 *
 * @param type_id Type identifier (e.g., "i32", "f64", "cstr").
 * @return size_t Size of the type in bytes, or 0 if unknown.
 */
size_t fossil_algorithm_search_type_sizeof(const char *type_id);

/**
 * @brief Checks whether the given type identifier is supported.
 *
 * @param type_id Type identifier string.
 * @return bool True if supported, false otherwise.
 */
bool fossil_algorithm_search_type_supported(const char *type_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {

    namespace algorithm {

        /**
         * @brief RAII-friendly C++ wrapper for Fossil searching algorithms.
         *
         * Provides static methods to search arrays using various algorithms.
         * All methods are thin wrappers around the C API, accepting raw pointers
         * and search options. Default options are used if none are provided.
         *
         * Usage:
         *   - Pass a pointer to the array, element count, pointer to key, and options.
         *   - Returns index of found element, or negative error code.
         */
        class Search
        {
        public:
            /**
             * @brief Searches an array using the Fossil C API.
             *
             * @param base Pointer to the array to search.
             * @param count Number of elements in the array.
             * @param key Pointer to the key to search for.
             * @param type_id String identifier for data type (e.g., "i32", "f64", "cstr").
             * @param algorithm_id String identifier for search algorithm ("auto", "binary", "linear", etc.).
             * @param order_id String identifier for sort order ("asc", "desc").
             * @return int Index of found element, or negative error code.
             */
            static int exec(
            const void *base,
            size_t count,
            const void *key,
            const std::string &type_id,
            const std::string &algorithm_id = "auto",
            const std::string &order_id = "asc"
            ) {
                return fossil_algorithm_search_exec(
                    base,
                    count,
                    key,
                    type_id.c_str(),
                    algorithm_id.c_str(),
                    order_id.c_str()
                );
            }

            /**
             * @brief Returns the byte size of a type based on its string identifier.
             *
             * @param type_id Type identifier (e.g., "i32", "f64", "cstr").
             * @return size_t Size of the type in bytes, or 0 if unknown.
             */
            static size_t type_sizeof(const std::string &type_id) {
                return fossil_algorithm_search_type_sizeof(type_id.c_str());
            }

            /**
             * @brief Checks whether the given type identifier is supported.
             *
             * @param type_id Type identifier string.
             * @return bool True if supported, false otherwise.
             */
            static bool type_supported(const std::string &type_id) {
                return fossil_algorithm_search_type_supported(type_id.c_str());
            }

        };

    } // namespace bluecrab

} // namespace fossil

#endif

#endif /* FOSSIL_CRABDB_FRAMEWORK_H */
