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
#ifndef FOSSIL_ALGORITHM_LM_H
#define FOSSIL_ALGORITHM_LM_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm LM — Core Types
// ======================================================

/**
 * @brief Opaque Jellyfish Grok-based LM handle.
 *
 * Internals may include tensors, memory graphs, embeddings,
 * and persistent state, but are hidden from the API.
 */
typedef struct fossil_lm_model fossil_lm_model_t;

/**
 * @brief Opaque LM input/output buffer.
 *
 * May represent text, tokens, embeddings, or structured data.
 */
typedef struct fossil_lm_buffer fossil_lm_buffer_t;

/**
 * @brief LM execution role identifier.
 *
 * Identified by string IDs at runtime.
 *
 * Typical role IDs:
 *   - "ingest"     (feed new data)
 *   - "learn"      (update internal model)
 *   - "infer"      (generate output)
 *   - "reflect"    (self-analysis / grok refinement)
 *   - "summarize"
 */
typedef const char *fossil_lm_role_id;

/**
 * @brief LM metric callback (loss, confidence, entropy, etc.).
 *
 * Used for audit, learning introspection, and convergence control.
 */
typedef bool (*fossil_lm_metric_fn)(
    const char *metric_id,
    double value,
    size_t step,
    void *user
);

// ======================================================
// Fossil Algorithm LM — Exec Interface
// ======================================================

/**
 * @brief Executes a Grok-based LM operation.
 *
 * Algorithm identifiers (string-based):
 *   - "grok-lm"          (general language model)
 *   - "grok-reason"     (reasoning pass)
 *   - "grok-reflect"    (self-refinement)
 *   - "grok-memory"     (memory consolidation)
 *
 * Role identifiers (string-based):
 *   - "ingest"
 *   - "learn"
 *   - "infer"
 *   - "reflect"
 *   - "audit"
 *
 * Notes:
 * - Model state persists across calls.
 * - Buffers may be text, tokens, or embeddings.
 * - No assumption is made about tokenization strategy.
 * - Grok core determines internal learning behavior.
 *
 * Return values:
 *   >= 0 : success or tokens produced
 *   -1   : execution failed
 *   -2   : invalid input
 *   -3   : unsupported algorithm
 *   -4   : unsupported role
 *
 * Example:
 * @code
 * fossil_algorithm_lm_exec(
 *     model,
 *     "grok-lm",
 *     "infer",
 *     input,
 *     output,
 *     metric_cb,
 *     NULL
 * );
 * @endcode
 */
int fossil_algorithm_lm_exec(
    fossil_lm_model_t *model,
    const char *algorithm_id,
    fossil_lm_role_id role_id,
    fossil_lm_buffer_t *input,
    fossil_lm_buffer_t *output,
    fossil_lm_metric_fn metric,
    void *user
);

// ======================================================
// Model Lifecycle
// ======================================================

/**
 * @brief Creates a Grok-based LM model.
 *
 * @param algorithm_id Algorithm identifier ("grok-lm", etc.)
 * @return Model handle or NULL on failure.
 */
fossil_lm_model_t *fossil_algorithm_lm_model_create(
    const char *algorithm_id
);

/**
 * @brief Destroys an LM model and releases all resources.
 */
void fossil_algorithm_lm_model_destroy(
    fossil_lm_model_t *model
);

// ======================================================
// Buffer Utilities
// ======================================================

/**
 * @brief Creates an LM buffer from raw memory.
 */
fossil_lm_buffer_t *fossil_algorithm_lm_buffer_create(
    void *data,
    size_t size
);

/**
 * @brief Destroys an LM buffer.
 */
void fossil_algorithm_lm_buffer_destroy(
    fossil_lm_buffer_t *buffer
);

// ======================================================
// Utility API
// ======================================================

/**
 * @brief Checks whether an LM algorithm is supported.
 */
bool fossil_algorithm_lm_supported(const char *algorithm_id);

/**
 * @brief Checks whether a role is supported by an algorithm.
 */
bool fossil_algorithm_lm_role_supported(
    const char *algorithm_id,
    fossil_lm_role_id role_id
);

#ifdef __cplusplus
}

#include <string>

namespace fossil {
    namespace algorithm {
    
    /**
     * @brief RAII-friendly C++ wrapper for Jellyfish Grok LM.
     */
    class LM
    {
    public:
        static fossil_lm_model_t *create(
            const std::string &algorithm_id
        ) {
            return fossil_algorithm_lm_model_create(
                algorithm_id.c_str()
            );
        }
    
        static void destroy(
            fossil_lm_model_t *model
        ) {
            fossil_algorithm_lm_model_destroy(model);
        }
    
        static int exec(
            fossil_lm_model_t *model,
            const std::string &algorithm_id,
            const std::string &role_id,
            fossil_lm_buffer_t *input,
            fossil_lm_buffer_t *output,
            fossil_lm_metric_fn metric = nullptr,
            void *user = nullptr
        ) {
            return fossil_algorithm_lm_exec(
                model,
                algorithm_id.c_str(),
                role_id.c_str(),
                input,
                output,
                metric,
                user
            );
        }
    
        static bool supported(
            const std::string &algorithm_id
        ) {
            return fossil_algorithm_lm_supported(
                algorithm_id.c_str()
            );
        }
    };
    
    } // namespace algorithm
} // namespace fossil

#endif /* __cplusplus */

#endif /* FOSSIL_ALGORITHM_LM_H */
