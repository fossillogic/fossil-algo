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
#ifndef FOSSIL_ALGORITHM_DYNAMIC_H
#define FOSSIL_ALGORITHM_DYNAMIC_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm Dynamic — Core Types
// ======================================================

/**
 * @brief Opaque dynamic algorithm handle.
 *
 * The concrete internal structure may store:
 * - Algorithm parameters
 * - Memoization tables
 * - Grok-guided heuristics
 * - Persistent state
 */
typedef struct fossil_dynamic fossil_dynamic_t;

/**
 * @brief Dynamic algorithm parameter descriptor.
 *
 * Key-value pairs provided at runtime to configure algorithm behavior.
 */
typedef struct fossil_dynamic_param {
    const char *key;
    const char *value;
} fossil_dynamic_param_t;

/**
 * @brief Metric / progress callback for dynamic algorithms.
 *
 * @param step Current step or iteration
 * @param value Current metric (e.g., cost, score)
 * @param user User context pointer
 * @return true to continue, false to abort early
 */
typedef bool (*fossil_dynamic_metric_fn)(
    size_t step,
    double value,
    void *user
);

// ======================================================
// Fossil Algorithm Dynamic — Exec Interface
// ======================================================

/**
 * @brief Executes a dynamic algorithm.
 *
 * Algorithm identifiers (string-based):
 *   - "dp-fib"          (dynamic programming Fibonacci)
 *   - "dp-knapsack"     (0/1 knapsack)
 *   - "dp-lcs"          (longest common subsequence)
 *   - "adaptive-search" (adaptive search / optimization)
 *
 * Parameters:
 *   - param: optional key-value configuration
 *   - param_count: number of key-value pairs
 *
 * Metric:
 *   - Optional callback to monitor progress
 *
 * Return values:
 *   >= 0 : algorithm-specific result
 *   -1   : execution failed
 *   -2   : invalid input
 *   -3   : unsupported algorithm
 *
 * Example:
 * @code
 * fossil_algorithm_dynamic_exec(
 *     alg,
 *     "dp-fib",
 *     params,
 *     0,
 *     metric_cb,
 *     NULL
 * );
 * @endcode
 */
int fossil_algorithm_dynamic_exec(
    fossil_dynamic_t *alg,
    const char *algorithm_id,
    const fossil_dynamic_param_t *param,
    size_t param_count,
    fossil_dynamic_metric_fn metric,
    void *user
);

// ======================================================
// Lifecycle Utilities
// ======================================================

/**
 * @brief Creates a dynamic algorithm handle.
 */
fossil_dynamic_t *fossil_algorithm_dynamic_create(
    const char *algorithm_id
);

/**
 * @brief Destroys a dynamic algorithm handle.
 */
void fossil_algorithm_dynamic_destroy(
    fossil_dynamic_t *alg
);

// ======================================================
// Utility API
// ======================================================

/**
 * @brief Checks whether a dynamic algorithm is supported.
 */
bool fossil_algorithm_dynamic_supported(const char *algorithm_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {
    namespace algorithm {
    
    /**
     * @brief RAII wrapper for Fossil dynamic algorithms.
     */
    class Dynamic
    {
    public:
        static fossil_dynamic_t *create(const std::string &algorithm_id) {
            return fossil_algorithm_dynamic_create(algorithm_id.c_str());
        }
    
        static void destroy(fossil_dynamic_t *alg) {
            fossil_algorithm_dynamic_destroy(alg);
        }
    
        static int exec(
            fossil_dynamic_t *alg,
            const std::string &algorithm_id,
            const fossil_dynamic_param_t *param = nullptr,
            size_t param_count = 0,
            fossil_dynamic_metric_fn metric = nullptr,
            void *user = nullptr
        ) {
            return fossil_algorithm_dynamic_exec(
                alg,
                algorithm_id.c_str(),
                param,
                param_count,
                metric,
                user
            );
        }
    
        static bool supported(const std::string &algorithm_id) {
            return fossil_algorithm_dynamic_supported(algorithm_id.c_str());
        }
    };
    
    } // namespace algorithm
} // namespace fossil

#endif /* __cplusplus */

#endif /* FOSSIL_ALGORITHM_DYNAMIC_H */
