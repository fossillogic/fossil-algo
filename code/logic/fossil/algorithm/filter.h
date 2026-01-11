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
#ifndef FOSSIL_ALGORITHM_FILTER_H
#define FOSSIL_ALGORITHM_FILTER_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ======================================================
 * Filter Predicate Signature
 * ====================================================== */

/**
 * @brief Predicate function used by filter algorithms.
 *
 * @param element Pointer to the element
 * @param user    Optional user context
 * @return true to keep element, false to discard
 */
typedef bool (*fossil_algorithm_filter_predicate)(
    const void *element,
    void *user
);

/* ======================================================
 * Filter Execution Interface
 * ====================================================== */

/**
 * @brief Executes a filter algorithm over an array.
 *
 * Supports algorithm selection, execution mode, and
 * thread-mimic behavior via lane partitioning.
 *
 * @param base         Pointer to input array
 * @param count        Number of elements
 * @param type_id      Type identifier ("i32", "f64", "ptr", etc.)
 * @param algorithm_id Filter algorithm ("auto", "stable", "compact", "inplace")
 * @param mode_id      Execution mode ("auto", "lane", "deterministic")
 * @param lanes        Number of virtual lanes (0 = auto)
 * @param predicate    Predicate function
 * @param user         User context passed to predicate
 * @param out_count    Receives number of kept elements
 *
 * @return int status:
 *   0  success
 *  -1  invalid input
 *  -2  unknown type
 *  -3  unknown algorithm
 */
int fossil_algorithm_filter_exec(
    void *base,
    size_t count,
    const char *type_id,
    const char *algorithm_id,
    const char *mode_id,
    size_t lanes,
    fossil_algorithm_filter_predicate predicate,
    void *user,
    size_t *out_count
);

/* ======================================================
 * Utilities
 * ====================================================== */

size_t fossil_algorithm_filter_type_sizeof(const char *type_id);
bool   fossil_algorithm_filter_type_supported(const char *type_id);

#ifdef __cplusplus
}
#endif

/* ======================================================
 * C++ Wrapper
 * ====================================================== */

#ifdef __cplusplus
#include <string>

namespace fossil {
namespace algorithm {

class Filter {
public:
    static int exec(
        void *base,
        size_t count,
        const std::string &type_id,
        fossil_algorithm_filter_predicate predicate,
        void *user = nullptr,
        size_t *out_count = nullptr,
        const std::string &algorithm_id = "auto",
        const std::string &mode_id = "auto",
        size_t lanes = 0
    ) {
        return fossil_algorithm_filter_exec(
            base,
            count,
            type_id.c_str(),
            algorithm_id.c_str(),
            mode_id.c_str(),
            lanes,
            predicate,
            user,
            out_count
        );
    }

    static size_t type_sizeof(const std::string &type_id) {
        return fossil_algorithm_filter_type_sizeof(type_id.c_str());
    }

    static bool type_supported(const std::string &type_id) {
        return fossil_algorithm_filter_type_supported(type_id.c_str());
    }
};

} // namespace algorithm
} // namespace fossil
#endif

#endif /* FOSSIL_ALGORITHM_FILTER_H */
