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
#ifndef FOSSIL_ALGORITHM_SHUFFLE_H
#define FOSSIL_ALGORITHM_SHUFFLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm Shuffle — Exec Interface
// ======================================================

/**
 * @brief Executes an array shuffle using string-based configuration.
 *
 * Provides a flexible runtime interface to randomize arrays of various types
 * using algorithm, type, and mode specified by string identifiers.
 *
 * Example:
 * @code
 * int32_t values[] = {1, 2, 3, 4, 5};
 * fossil_algorithm_shuffle_exec(values, 5, "i32", "fisher-yates", "auto", 0);
 * @endcode
 *
 * @param base Pointer to the array to shuffle.
 * @param count Number of elements in the array.
 * @param type_id String identifier for data type (e.g., "i32", "f64", "cstr").
 * @param algorithm_id String identifier for shuffle algorithm ("auto", "fisher-yates", "inside-out", etc.).
 * @param mode_id String identifier for shuffle mode ("auto", "seeded", "secure").
 * @param seed Optional seed value (ignored if mode is "auto" or "secure").
 * @return int Status code:
 *   - `0` on success
 *   - `-1` invalid input
 *   - `-2` unknown type
 *   - `-3` unknown algorithm
 */
int fossil_algorithm_shuffle_exec(
    void *base,
    size_t count,
    const char *type_id,
    const char *algorithm_id,
    const char *mode_id,
    uint64_t seed
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
size_t fossil_algorithm_shuffle_type_sizeof(const char *type_id);

/**
 * @brief Checks whether the given type identifier is supported.
 *
 * @param type_id Type identifier string.
 * @return bool True if supported, false otherwise.
 */
bool fossil_algorithm_shuffle_type_supported(const char *type_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {

    namespace algorithm {

        /**
         * @brief RAII-friendly C++ wrapper for Fossil shuffle algorithms.
         *
         * Provides static methods to shuffle arrays using various algorithms.
         * All methods are thin wrappers around the C API, accepting raw pointers
         * and shuffle options. Default options are used if none are provided.
         *
         * Usage:
         *   - Pass a pointer to the array, element count, type, algorithm, mode, and seed.
         *   - Returns status code (0 on success, negative on error).
         */
        class Shuffle
        {
        public:
            /**
             * @brief Shuffles an array using the Fossil C API.
             *
             * @param base Pointer to the array to shuffle.
             * @param count Number of elements in the array.
             * @param type_id String identifier for data type (e.g., "i32", "f64", "cstr").
             * @param algorithm_id String identifier for shuffle algorithm ("auto", "fisher-yates", "inside-out", etc.).
             * @param mode_id String identifier for shuffle mode ("auto", "seeded", "secure").
             * @param seed Optional seed value (ignored if mode is "auto" or "secure").
             * @return int Status code (0 on success, negative on error).
             */
            static int exec(
            void *base,
            size_t count,
            const std::string &type_id,
            const std::string &algorithm_id = "auto",
            const std::string &mode_id = "auto",
            uint64_t seed = 0
            ) {
            return fossil_algorithm_shuffle_exec(
                base,
                count,
                type_id.c_str(),
                algorithm_id.c_str(),
                mode_id.c_str(),
                seed
            );
            }

            /**
             * @brief Returns the byte size of a type based on its string identifier.
             *
             * @param type_id Type identifier (e.g., "i32", "f64", "cstr").
             * @return size_t Size of the type in bytes, or 0 if unknown.
             */
            static size_t type_sizeof(const std::string &type_id) {
            return fossil_algorithm_shuffle_type_sizeof(type_id.c_str());
            }

            /**
             * @brief Checks whether the given type identifier is supported.
             *
             * @param type_id Type identifier string.
             * @return bool True if supported, false otherwise.
             */
            static bool type_supported(const std::string &type_id) {
            return fossil_algorithm_shuffle_type_supported(type_id.c_str());
            }
        };

    } // namespace bluecrab

} // namespace fossil

#endif

#endif /* FOSSIL_CRABDB_FRAMEWORK_H */
