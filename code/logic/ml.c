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
#include "fossil/algorithm/lm.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ======================================================
// Internal Structures
// ======================================================

struct fossil_ml_dataset {
    size_t samples;
    size_t features;
    double *x;      /* samples × features */
    double *y;      /* optional labels */
};

struct fossil_ml_model {
    char algorithm[32];
    size_t features;

    /* Linear model */
    double *weights;
    double bias;

    /* KMeans */
    size_t k;
    double *centroids;
};

// ======================================================
// Utilities
// ======================================================

static bool
algo_eq(const char *a, const char *b)
{
    return a && b && strcmp(a, b) == 0;
}

// ======================================================
// Linear Regression
// ======================================================

static int
linear_train(
    fossil_ml_model_t *m,
    fossil_ml_dataset_t *d,
    fossil_ml_metric_fn metric,
    void *user
) {
    if (!d->y) return -2;

    size_t n = d->samples;
    size_t f = d->features;
    double lr = 0.01;

    if (!m->weights) {
        m->features = f;
        m->weights = calloc(f, sizeof(double));
        m->bias = 0.0;
    }

    for (size_t epoch = 0; epoch < 100; epoch++) {
        double loss = 0.0;

        for (size_t i = 0; i < n; i++) {
            double pred = m->bias;
            for (size_t j = 0; j < f; j++)
                pred += m->weights[j] * d->x[i*f + j];

            double err = pred - d->y[i];
            loss += err * err;

            for (size_t j = 0; j < f; j++)
                m->weights[j] -= lr * err * d->x[i*f + j];

            m->bias -= lr * err;
        }

        loss /= n;
        if (metric && !metric(loss, epoch, user))
            break;
    }

    return 0;
}

static int
linear_predict(
    fossil_ml_model_t *m,
    fossil_ml_dataset_t *d
) {
    if (!m->weights) return -1;

    for (size_t i = 0; i < d->samples; i++) {
        double pred = m->bias;
        for (size_t j = 0; j < m->features; j++)
            pred += m->weights[j] * d->x[i*m->features + j];

        d->y[i] = pred;
    }
    return 0;
}

// ======================================================
// K-Means
// ======================================================

static double
dist2(const double *a, const double *b, size_t n)
{
    double s = 0.0;
    for (size_t i = 0; i < n; i++) {
        double d = a[i] - b[i];
        s += d * d;
    }
    return s;
}

static int
kmeans_train(
    fossil_ml_model_t *m,
    fossil_ml_dataset_t *d,
    fossil_ml_metric_fn metric,
    void *user
) {
    if (!m->centroids) {
        m->k = 2;
        m->features = d->features;
        m->centroids = malloc(m->k * m->features * sizeof(double));
        memcpy(m->centroids, d->x, m->k * m->features * sizeof(double));
    }

    size_t *assign = malloc(d->samples * sizeof(size_t));

    for (size_t iter = 0; iter < 50; iter++) {
        double loss = 0.0;

        for (size_t i = 0; i < d->samples; i++) {
            size_t best = 0;
            double best_d = dist2(
                &d->x[i*d->features],
                &m->centroids[0],
                d->features
            );

            for (size_t c = 1; c < m->k; c++) {
                double d2 = dist2(
                    &d->x[i*d->features],
                    &m->centroids[c*m->features],
                    d->features
                );
                if (d2 < best_d) {
                    best = c;
                    best_d = d2;
                }
            }
            assign[i] = best;
            loss += best_d;
        }

        memset(m->centroids, 0, m->k * m->features * sizeof(double));
        size_t *count = calloc(m->k, sizeof(size_t));

        for (size_t i = 0; i < d->samples; i++) {
            size_t c = assign[i];
            count[c]++;
            for (size_t j = 0; j < d->features; j++)
                m->centroids[c*m->features + j] += d->x[i*d->features + j];
        }

        for (size_t c = 0; c < m->k; c++)
            for (size_t j = 0; j < d->features; j++)
                m->centroids[c*m->features + j] /= (count[c] ? count[c] : 1);

        free(count);

        if (metric && !metric(loss / d->samples, iter, user))
            break;
    }

    free(assign);
    return 0;
}

// ======================================================
// Public Exec Interface
// ======================================================

int
fossil_algorithm_ml_exec(
    fossil_ml_model_t *model,
    fossil_ml_dataset_t *dataset,
    const char *algorithm_id,
    fossil_ml_phase_t phase,
    fossil_ml_metric_fn metric,
    void *user
) {
    if (!model || !dataset || !algorithm_id)
        return -2;

    if (algo_eq(algorithm_id, "linear-regression")) {
        if (phase == FOSSIL_ML_PHASE_TRAIN)
            return linear_train(model, dataset, metric, user);
        if (phase == FOSSIL_ML_PHASE_PREDICT)
            return linear_predict(model, dataset);
        return 0;
    }

    if (algo_eq(algorithm_id, "kmeans")) {
        if (phase != FOSSIL_ML_PHASE_TRAIN)
            return -4;
        return kmeans_train(model, dataset, metric, user);
    }

    return -3;
}

// ======================================================
// Model Lifecycle
// ======================================================

fossil_ml_model_t *
fossil_algorithm_ml_model_create(const char *algorithm_id)
{
    if (!algorithm_id) return NULL;

    fossil_ml_model_t *m = calloc(1, sizeof(*m));
    strncpy(m->algorithm, algorithm_id, sizeof(m->algorithm) - 1);
    return m;
}

void
fossil_algorithm_ml_model_destroy(fossil_ml_model_t *model)
{
    if (!model) return;
    free(model->weights);
    free(model->centroids);
    free(model);
}

// ======================================================
// Utility API
// ======================================================

bool
fossil_algorithm_ml_supported(const char *algorithm_id)
{
    return algo_eq(algorithm_id, "linear-regression") ||
           algo_eq(algorithm_id, "kmeans");
}

bool
fossil_algorithm_ml_requires_labels(const char *algorithm_id)
{
    return algo_eq(algorithm_id, "linear-regression") ||
           algo_eq(algorithm_id, "logistic-regression") ||
           algo_eq(algorithm_id, "svm");
}
