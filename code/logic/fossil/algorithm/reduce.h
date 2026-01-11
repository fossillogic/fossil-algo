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
#ifndef FOSSIL_ALGORITHM_REDUCE_H
#define FOSSIL_ALGORITHM_REDUCE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm Reduce — Reducer Signature
// ======================================================

/**
 * @brief Reducer function for custom reductions.
 *
 * @param accum   Pointer to accumulator (mutable)
 * @param element Pointer to input element
 * @param user    Optional user context
 */
typedef void (*fossil_algorithm_reduce_fn)(
    void *accum,
    const void *element,
    void *user
);

// ======================================================
// Fossil Algorithm Reduce — Exec Interface
// ======================================================

/**
 * @brief Executes a reduction over an array.
 *
 * Performs aggregation such as sum, min, max, count, or
 * user-defined reductions with deterministic execution.
 *
 * Example:
 * @code
 * int32_t values[] = {1,2,3,4};
 * int32_t result;
 * fossil_algorithm_reduce_exec(
 *     values, 4,
 *     "i32",
 *     "sum",
 *     "auto",
 *     0,
 *     &result,
 *     NULL,
 *     NULL
 * );
 * @endcode
 *
 * @param base        Pointer to input array
 * @param count       Number of elements
 * @param type_id     Data type identifier ("i32", "f64", etc.)
 * @param op_id       Reduction operation ("sum", "min", "max", "count", "custom")
 * @param lanes       Number of virtual lanes (0 = auto)
 * @param out_result  Pointer to output accumulator
 * @param reduce_fn   Custom reducer (required if op_id == "custom")
 * @param user        Optional user context
 *
 * @return int status:
 *   0  success
 *  -1  invalid input
 *  -2  unknown type
 *  -3  unknown operation
 */
int fossil_algorithm_reduce_exec(
    const void *base,
    size_t count,
    const char *type_id,
    const char *op_id,
    size_t lanes,
    void *out_result,
    fossil_algorithm_reduce_fn reduce_fn,
    void *user
);

// ======================================================
// Utilities
// ======================================================

/**
 * @brief Returns the byte size of a type based on its string identifier.
 */
size_t fossil_algorithm_reduce_type_sizeof(const char *type_id);

/**
 * @brief Checks whether the given type identifier is supported.
 */
bool fossil_algorithm_reduce_type_supported(const char *type_id);

#ifdef __cplusplus
}
#endif

// ======================================================
// C++ RAII Wrapper
// ======================================================

#ifdef __cplusplus
#include <string>

namespace fossil {

namespace algorithm {

class Reduce
{
public:
    static int exec(
        const void *base,
        size_t count,
        const std::string &type_id,
        void *out_result,
        const std::string &op_id = "auto",
        size_t lanes = 0,
        fossil_algorithm_reduce_fn fn = nullptr,
        void *user = nullptr
    ) {
        return fossil_algorithm_reduce_exec(
            base,
            count,
            type_id.c_str(),
            op_id.c_str(),
            lanes,
            out_result,
            fn,
            user
        );
    }

    static size_t type_sizeof(const std::string &type_id) {
        return fossil_algorithm_reduce_type_sizeof(type_id.c_str());
    }

    static bool type_supported(const std::string &type_id) {
        return fossil_algorithm_reduce_type_supported(type_id.c_str());
    }
};

} // namespace algorithm

} // namespace fossil
#endif

#endif /* FOSSIL_ALGORITHM_REDUCE_H */
