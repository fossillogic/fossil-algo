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
#ifndef FOSSIL_ALGORITHM_ML_H
#define FOSSIL_ALGORITHM_ML_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ======================================================
// Fossil Algorithm ML — Core Types
// ======================================================

/**
 * @brief Opaque ML model handle.
 *
 * The concrete model implementation (linear model, tree, NN, etc.)
 * is hidden from the algorithm interface.
 */
typedef struct fossil_ml_model fossil_ml_model_t;

/**
 * @brief Opaque dataset handle.
 *
 * Allows datasets to be backed by memory, storage, streams,
 * or AI core tensors.
 */
typedef struct fossil_ml_dataset fossil_ml_dataset_t;

/**
 * @brief Training phase identifier.
 */
typedef enum fossil_ml_phase {
    FOSSIL_ML_PHASE_TRAIN,
    FOSSIL_ML_PHASE_EVAL,
    FOSSIL_ML_PHASE_PREDICT
} fossil_ml_phase_t;

/**
 * @brief Metric callback for training and evaluation.
 *
 * @param value Metric value (loss, accuracy, etc.)
 * @param step Training step or epoch.
 * @param user User context pointer.
 * @return true to continue, false to stop early.
 */
typedef bool (*fossil_ml_metric_fn)(
    double value,
    size_t step,
    void *user
);

// ======================================================
// Fossil Algorithm ML — Exec Interface
// ======================================================

/**
 * @brief Executes a machine learning algorithm.
 *
 * Supported algorithm identifiers (implementation-defined):
 *   - Supervised: "linear-regression", "logistic-regression",
 *                 "svm", "knn"
 *   - Unsupervised: "kmeans", "dbscan", "pca"
 *   - Tree-based: "decision-tree", "random-forest"
 *   - Neural: "mlp", "cnn", "rnn"
 *
 * Notes:
 * - Training, evaluation, and prediction are unified.
 * - Model state persists across calls.
 * - Dataset format and feature layout are backend-defined.
 * - Some algorithms require labels (training only).
 *
 * Return values:
 *   >= 0 : success or algorithm-specific result
 *   -1   : training failed or prediction invalid
 *   -2   : invalid input or incompatible dataset
 *   -3   : unknown or unsupported algorithm
 *   -4   : unsupported phase or configuration
 *
 * Example:
 * @code
 * fossil_algorithm_ml_exec(
 *     model,
 *     dataset,
 *     "kmeans",
 *     FOSSIL_ML_PHASE_TRAIN,
 *     metric_cb,
 *     NULL
 * );
 * @endcode
 *
 * @param model ML model handle.
 * @param dataset Dataset handle.
 * @param algorithm_id Algorithm identifier string.
 * @param phase Training / evaluation / prediction phase.
 * @param metric Optional metric callback.
 * @param user User context pointer.
 * @return int Status or algorithm-specific code.
 */
int fossil_algorithm_ml_exec(
    fossil_ml_model_t *model,
    fossil_ml_dataset_t *dataset,
    const char *algorithm_id,
    fossil_ml_phase_t phase,
    fossil_ml_metric_fn metric,
    void *user
);

// ======================================================
// Model Lifecycle Utilities
// ======================================================

/**
 * @brief Creates a model instance for a given algorithm.
 *
 * @param algorithm_id Algorithm identifier string.
 * @return fossil_ml_model_t* Model handle or NULL on failure.
 */
fossil_ml_model_t *fossil_algorithm_ml_model_create(
    const char *algorithm_id
);

/**
 * @brief Destroys a model and releases resources.
 */
void fossil_algorithm_ml_model_destroy(
    fossil_ml_model_t *model
);

// ======================================================
// Extended Utility API
// ======================================================

/**
 * @brief Checks whether an ML algorithm is supported.
 *
 * @param algorithm_id Algorithm identifier.
 * @return true if supported.
 */
bool fossil_algorithm_ml_supported(const char *algorithm_id);

/**
 * @brief Checks whether an algorithm requires labels.
 *
 * @param algorithm_id Algorithm identifier.
 * @return true if labels are required.
 */
bool fossil_algorithm_ml_requires_labels(const char *algorithm_id);

#ifdef __cplusplus
}

#include <string>

namespace fossil {
    namespace algorithm {
    
    /**
     * @brief RAII-friendly C++ wrapper for Fossil ML algorithms.
     */
    class ML
    {
    public:
        /**
         * @brief Create a model for an algorithm.
         */
        static fossil_ml_model_t *create(
            const std::string &algorithm_id
        ) {
            return fossil_algorithm_ml_model_create(
                algorithm_id.c_str()
            );
        }
    
        /**
         * @brief Destroy a model.
         */
        static void destroy(
            fossil_ml_model_t *model
        ) {
            fossil_algorithm_ml_model_destroy(model);
        }
    
        /**
         * @brief Execute an ML algorithm phase.
         */
        static int exec(
            fossil_ml_model_t *model,
            fossil_ml_dataset fossil_ml_dataset_t *dataset,
            const std::string &algorithm_id,
            fossil_ml_phase_t phase,
            fossil_ml_metric_fn metric = nullptr,
            void *user = nullptr
        ) {
            return fossil_algorithm_ml_exec(
                model,
                dataset,
                algorithm_id.c_str(),
                phase,
                metric,
                user
            );
        }
    
        /**
         * @brief Checks algorithm support.
         */
        static bool supported(
            const std::string &algorithm_id
        ) {
            return fossil_algorithm_ml_supported(
                algorithm_id.c_str()
            );
        }
    
        /**
         * @brief Checks label requirement.
         */
        static bool requires_labels(
            const std::string &algorithm_id
        ) {
            return fossil_algorithm_ml_requires_labels(
                algorithm_id.c_str()
            );
        }
    };
    
    } // namespace algorithm
} // namespace fossil

#endif /* __cplusplus */

#endif /* FOSSIL_ALGORITHM_ML_H */
