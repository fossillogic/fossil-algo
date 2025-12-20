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
#ifndef FOSSIL_ALGORITHM_SORT_H
#define FOSSIL_ALGORITHM_SORT_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm Sort — Exec Interface
// ======================================================

/**
 * @brief Executes a sorting operation using string-based configuration.
 *
 * This function provides a flexible runtime interface for sorting arrays of
 * various types, using the algorithm, order, and type specified by string
 * identifiers. It supports multiple algorithms (merge, heap, insertion, shell,
 * bubble, counting, radix) and both ascending and descending order.
 *
 * Internally, the function dispatches to the appropriate algorithm stub based
 * on the algorithm_id string. Type safety is managed via type_id and a
 * comparator function. For unsupported types or algorithms, error codes are
 * returned.
 *
 * Notes:
 *   - "auto" algorithm_id defaults to merge sort.
 *   - Counting sort only supports "u8" type; radix sort only supports "u32".
 *   - Returns negative error codes for invalid input, unknown type, or unknown algorithm.
 *   - Sorting is performed in-place.
 *
 * Example:
 * @code
 * float values[] = { 3.2f, 1.5f, 9.1f };
 * fossil_algorithm_sort_exec(values, 3, "f32", "merge", "asc");
 * @endcode
 *
 * @param base Pointer to the array to sort.
 * @param count Number of elements in the array.
 * @param type_id String identifier for data type (e.g., "i32", "f64", "cstr").
 * @param algorithm_id String identifier for sorting algorithm ("auto", "merge", etc.).
 * @param order_id String identifier for sort order ("asc", "desc").
 * @return int Status code:
 *   - `0` on success
 *   - `-1` for invalid input
 *   - `-2` for unknown type or comparator
 *   - `-3` for unknown algorithm
 *   - Other negative values for algorithm-specific errors
 */
int fossil_algorithm_sort_exec(
    void *base,
    size_t count,
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
size_t fossil_algorithm_sort_type_sizeof(const char *type_id);

/**
 * @brief Checks whether the given type identifier is supported.
 *
 * @param type_id Type identifier string.
 * @return bool True if supported, false otherwise.
 */
bool fossil_algorithm_sort_type_supported(const char *type_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {

    namespace algorithm {

        /**
         * @brief RAII-friendly C++ wrapper for Fossil sorting algorithms.
         *
         * Provides static methods to sort arrays using various algorithms.
         * All methods are thin wrappers around the C API, accepting raw pointers
         * and sorting options. Default options are used if none are provided.
         *
         * Usage:
         *   - Pass a pointer to the array, element count, element size, comparator, and options.
         *   - Sorting is performed in-place.
         *   - Returns 0 on success, non-zero on error.
         */
        class Sort
        {
        public:
            /**
             * @brief Sorts an array using the Fossil C API.
             *
             * @param base Pointer to the array to sort.
             * @param count Number of elements in the array.
             * @param type_id String identifier for data type (e.g., "i32", "f64", "cstr").
             * @param algorithm_id String identifier for sorting algorithm ("auto", "quick", "merge", etc.).
             * @param order_id String identifier for sort order ("asc", "desc").
             * @return int Status code (0 on success, negative on error).
             */
            static int exec(
            void *base,
            size_t count,
            const std::string &type_id,
            const std::string &algorithm_id = "auto",
            const std::string &order_id = "asc"
            )
            {
            return fossil_algorithm_sort_exec(
                base,
                count,
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
            static size_t type_sizeof(const std::string &type_id)
            {
            return fossil_algorithm_sort_type_sizeof(type_id.c_str());
            }

            /**
             * @brief Checks whether the given type identifier is supported.
             *
             * @param type_id Type identifier string.
             * @return bool True if supported, false otherwise.
             */
            static bool type_supported(const std::string &type_id)
            {
            return fossil_algorithm_sort_type_supported(type_id.c_str());
            }
        };

    } // namespace bluecrab

} // namespace fossil

#endif

#endif /* FOSSIL_CRABDB_FRAMEWORK_H */
