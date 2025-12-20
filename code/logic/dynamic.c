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
#include "fossil/algorithm/dynamic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ======================================================
// Internal Structures
// ======================================================

struct fossil_dynamic {
    char algorithm[32];
    void *state;      // algorithm-specific state
};

// ======================================================
// Internal Utilities
// ======================================================

static int str_eq(const char *a, const char *b) {
    return a && b && strcmp(a, b) == 0;
}

// ======================================================
// Example Algorithm Implementations
// ======================================================

// --- dp-fib ---
static int exec_dp_fib(int n) {
    if (n < 0) return -1;
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

// --- dp-knapsack ---
static int exec_dp_knapsack(int *values, int *weights, int n, int capacity, fossil_dynamic_metric_fn metric, void *user) {
    if (!values || !weights || n <= 0) return -2;
    int *dp = (int*)calloc(capacity+1, sizeof(int));
    if (!dp) return -1;

    for (int i = 0; i < n; i++) {
        for (int w = capacity; w >= weights[i]; w--) {
            int candidate = dp[w - weights[i]] + values[i];
            if (candidate > dp[w]) dp[w] = candidate;
        }
        if (metric) metric(i, dp[capacity], user);
    }

    int result = dp[capacity];
    free(dp);
    return result;
}

// --- dp-lcs ---
static int exec_dp_lcs(const char *a, const char *b) {
    if (!a || !b) return -2;
    size_t m = strlen(a), n = strlen(b);
    int *dp = (int*)calloc((m+1)*(n+1), sizeof(int));
    if (!dp) return -1;

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (a[i-1] == b[j-1])
                dp[i*(n+1)+j] = dp[(i-1)*(n+1)+(j-1)] + 1;
            else {
                int x = dp[(i-1)*(n+1)+j];
                int y = dp[i*(n+1)+(j-1)];
                dp[i*(n+1)+j] = x > y ? x : y;
            }
        }
    }

    int result = dp[m*(n+1)+n];
    free(dp);
    return result;
}

// ======================================================
// Public Exec Interface
// ======================================================

int fossil_algorithm_dynamic_exec(
    fossil_dynamic_t *alg,
    const char *algorithm_id,
    const fossil_dynamic_param_t *param,
    size_t param_count,
    fossil_dynamic_metric_fn metric,
    void *user
) {
    if (!alg || !algorithm_id) return -2;
    if (!str_eq(alg->algorithm, algorithm_id)) return -3;

    if (str_eq(algorithm_id, "dp-fib")) {
        int n = 10; // default
        for (size_t i = 0; i < param_count; i++) {
            if (str_eq(param[i].key, "n")) n = atoi(param[i].value);
        }
        return exec_dp_fib(n);
    }

    if (str_eq(algorithm_id, "dp-knapsack")) {
        int values[100] = {0}, weights[100] = {0}, n = 0, capacity = 50;
        for (size_t i = 0; i < param_count; i++) {
            if (str_eq(param[i].key, "capacity")) capacity = atoi(param[i].value);
            // parsing values/weights could be extended
        }
        return exec_dp_knapsack(values, weights, n, capacity, metric, user);
    }

    if (str_eq(algorithm_id, "dp-lcs")) {
        const char *s1 = "abc", *s2 = "ac";
        for (size_t i = 0; i < param_count; i++) {
            if (str_eq(param[i].key, "a")) s1 = param[i].value;
            if (str_eq(param[i].key, "b")) s2 = param[i].value;
        }
        return exec_dp_lcs(s1, s2);
    }

    return -3; // unsupported algorithm
}

// ======================================================
// Lifecycle
// ======================================================

fossil_dynamic_t *fossil_algorithm_dynamic_create(const char *algorithm_id) {
    if (!algorithm_id) return NULL;
    fossil_dynamic_t *alg = (fossil_dynamic_t*)calloc(1, sizeof(*alg));
    strncpy(alg->algorithm, algorithm_id, sizeof(alg->algorithm)-1);
    alg->state = NULL;
    return alg;
}

void fossil_algorithm_dynamic_destroy(fossil_dynamic_t *alg) {
    if (!alg) return;
    free(alg->state);
    free(alg);
}

// ======================================================
// Utility API
// ======================================================

bool fossil_algorithm_dynamic_supported(const char *algorithm_id) {
    return str_eq(algorithm_id, "dp-fib") ||
           str_eq(algorithm_id, "dp-knapsack") ||
           str_eq(algorithm_id, "dp-lcs") ||
           str_eq(algorithm_id, "adaptive-search");
}
