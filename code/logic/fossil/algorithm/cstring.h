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
#ifndef FOSSIL_ALGORITHM_CSTRING_H
#define FOSSIL_ALGORITHM_CSTRING_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm CString — Exec Interface
// ======================================================

/**
 * @brief Executes a string algorithm on C strings.
 *
 * Supported algorithm identifiers:
 *   - Search:
 *       "find"        (substring search)
 *       "rfind"       (reverse search)
 *       "count"       (count substring occurrences)
 *
 *   - Comparison:
 *       "equals"
 *       "iequals"
 *       "compare"
 *
 *   - Transformation:
 *       "tolower"
 *       "toupper"
 *       "reverse"
 *
 * Notes:
 * - Input strings must be null-terminated.
 * - Output buffer is optional and algorithm-dependent.
 * - No allocation is performed internally.
 *
 * Return values:
 *   >= 0 : algorithm-specific result
 *   -1   : not found / false
 *   -2   : invalid input
 *   -3   : unknown or unsupported algorithm
 *
 * Example:
 * @code
 * int idx = fossil_algorithm_cstring_exec(
 *     "hello world",
 *     "world",
 *     "find",
 *     out,
 *     sizeof(out)
 * );
 * @endcode
 *
 * @param input Input string.
 * @param arg Optional argument string (substring, compare target, etc.)
 * @param algorithm_id Algorithm identifier string.
 * @param output Optional output buffer.
 * @param output_size Size of output buffer.
 * @return int Result or error code.
 */
int fossil_algorithm_cstring_exec(
    const char *input,
    const char *arg,
    const char *algorithm_id,
    char *output,
    size_t output_size
);

// ======================================================
// Utility API
// ======================================================

/**
 * @brief Checks whether a string algorithm is supported.
 */
bool fossil_algorithm_cstring_supported(const char *algorithm_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {
    namespace algorithm {
    
    /**
     * @brief RAII-friendly C++ wrapper for CString algorithms.
     */
    class CString
    {
    public:
        static int exec(
            const std::string &input,
            const std::string &arg,
            const std::string &algorithm_id,
            std::string *output = nullptr
        ) {
            char buffer[1024] = {0};
    
            int rc = fossil_algorithm_cstring_exec(
                input.c_str(),
                arg.empty() ? nullptr : arg.c_str(),
                algorithm_id.c_str(),
                output ? buffer : nullptr,
                sizeof(buffer)
            );
    
            if (output && rc >= 0)
                *output = buffer;
    
            return rc;
        }
    
        static bool supported(const std::string &algorithm_id) {
            return fossil_algorithm_cstring_supported(algorithm_id.c_str());
        }
    };
    
    } // namespace algorithm
} // namespace fossil

#endif /* __cplusplus */

#endif /* FOSSIL_ALGORITHM_CSTRING_H */
