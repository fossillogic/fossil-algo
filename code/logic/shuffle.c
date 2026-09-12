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
#include "fossil/algorithm/shuffle.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

// ======================================================
// Internal Helpers
// ======================================================

static uint64_t fossil_algorithm_shuffle_rand_seed(uint64_t seed, const char *mode_id)
{
    if (mode_id == NULL || strcmp(mode_id, "auto") == 0)
        return (uint64_t)time(NULL) ^ (uint64_t)(uintptr_t)&seed;
    if (strcmp(mode_id, "seeded") == 0)
        return seed ? seed : (uint64_t)time(NULL);
    if (strcmp(mode_id, "secure") == 0)
    {
        // fallback to auto if no secure entropy source
        return (uint64_t)time(NULL) ^ (uint64_t)(uintptr_t)&seed;
    }
    return (uint64_t)time(NULL);
}

static inline void fossil_algorithm_shuffle_swap(void *a, void *b, size_t size)
{
    unsigned char tmp;
    unsigned char *x = (unsigned char *)a;
    unsigned char *y = (unsigned char *)b;
    while (size--)
    {
        tmp = *x;
        *x++ = *y;
        *y++ = tmp;
    }
}

static inline uint64_t fossil_algorithm_shuffle_next(uint64_t *state)
{
    uint64_t x = *state ? *state : UINT64_C(0x9e3779b97f4a7c15);
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    *state = x;
    return x * UINT64_C(0x2545f4914f6cdd1d);
}

static inline size_t fossil_algorithm_shuffle_bounded(uint64_t *state, size_t bound)
{
    return (size_t)(fossil_algorithm_shuffle_next(state) % bound);
}

// ======================================================
// Type Size Resolution
// ======================================================

size_t fossil_algorithm_shuffle_type_sizeof(const char *type_id)
{
    if (!type_id)
        return 0;

    if (strcmp(type_id, "i8") == 0 || strcmp(type_id, "u8") == 0 || strcmp(type_id, "char") == 0 || strcmp(type_id, "bool") == 0)
        return 1;
    if (strcmp(type_id, "i16") == 0 || strcmp(type_id, "u16") == 0)
        return 2;
    if (strcmp(type_id, "i32") == 0 || strcmp(type_id, "u32") == 0 || strcmp(type_id, "f32") == 0)
        return 4;
    if (strcmp(type_id, "i64") == 0 || strcmp(type_id, "u64") == 0 || strcmp(type_id, "f64") == 0 || strcmp(type_id, "size") == 0)
        return 8;
    if (strcmp(type_id, "cstr") == 0)
        return sizeof(char *);
    if (strcmp(type_id, "hex") == 0 || strcmp(type_id, "oct") == 0 || strcmp(type_id, "bin") == 0 ||
        strcmp(type_id, "datetime") == 0 || strcmp(type_id, "duration") == 0)
        return sizeof(uint64_t);
    if (strcmp(type_id, "any") == 0 || strcmp(type_id, "null") == 0)
        return sizeof(void *);
    return 0;
}

bool fossil_algorithm_shuffle_type_supported(const char *type_id)
{
    return fossil_algorithm_shuffle_type_sizeof(type_id) > 0;
}

// ======================================================
// Shuffle Algorithms
// ======================================================

static void fossil_algorithm_shuffle_fisher_yates(void *base, size_t count, size_t size, uint64_t seed)
{
    unsigned char *data = (unsigned char *)base;
    uint64_t state = seed;

    for (size_t i = count - 1; i > 0; --i)
    {
        size_t j = fossil_algorithm_shuffle_bounded(&state, i + 1);
        fossil_algorithm_shuffle_swap(data + i * size, data + j * size, size);
    }
}

static void fossil_algorithm_shuffle_inside_out(void *base, size_t count, size_t size, uint64_t seed)
{
    unsigned char *data = (unsigned char *)base;
    uint64_t state = seed;

    for (size_t i = 1; i < count; ++i)
    {
        size_t j = fossil_algorithm_shuffle_bounded(&state, i + 1);
        if (j != i)
            fossil_algorithm_shuffle_swap(data + i * size, data + j * size, size);
    }
}

/* Inside-out Fisher-Yates.  The public API supplies an existing array, so the
 * array itself is used as the destination buffer. */
static void fossil_algorithm_shuffle_knuth(void *base, size_t count, size_t size, uint64_t seed)
{
    fossil_algorithm_shuffle_fisher_yates(base, count, size, seed);
}

static void fossil_algorithm_shuffle_transposition(void *base, size_t count, size_t size, uint64_t seed)
{
    if (count < 2) return;
    unsigned char *data = (unsigned char *)base;
    uint64_t state = seed;
    for (size_t i = 0; i < count * 2; ++i)
    {
        size_t a = fossil_algorithm_shuffle_bounded(&state, count);
        size_t b = fossil_algorithm_shuffle_bounded(&state, count);
        fossil_algorithm_shuffle_swap(data + a * size, data + b * size, size);
    }
}

static void fossil_algorithm_shuffle_riffle(void *base, size_t count, size_t size, uint64_t seed)
{
    if (count < 2) return;
    unsigned char *data = (unsigned char *)base;
    unsigned char *tmp = (unsigned char *)malloc(count * size);
    if (!tmp) return;
    uint64_t state = seed;
    size_t cut = fossil_algorithm_shuffle_bounded(&state, count + 1);
    size_t left = 0, right = cut, out = 0;
    while (left < cut || right < count)
    {
        bool take_left = right == count || (left < cut &&
            fossil_algorithm_shuffle_bounded(&state, (cut - left) + (count - right)) < cut - left);
        size_t source = take_left ? left++ : right++;
        memcpy(tmp + out++ * size, data + source * size, size);
    }
    memcpy(data, tmp, count * size);
    free(tmp);
}

static void fossil_algorithm_shuffle_overhand(void *base, size_t count, size_t size, uint64_t seed)
{
    /* Randomly sized packets, placed from the front, model an overhand pass. */
    if (count < 2) return;
    unsigned char *data = (unsigned char *)base;
    uint64_t state = seed;
    size_t pos = 0;
    while (pos < count)
    {
        size_t n = 1 + fossil_algorithm_shuffle_bounded(&state, count - pos);
        for (size_t i = 0; i < n / 2; ++i)
            fossil_algorithm_shuffle_swap(data + (pos + i) * size,
                                           data + (pos + n - 1 - i) * size, size);
        pos += n;
    }
    fossil_algorithm_shuffle_riffle(base, count, size, state);
}

static void fossil_algorithm_shuffle_permutation(void *base, size_t count, size_t size, uint64_t seed)
{
    /* Applying swaps while generating the permutation is equivalent to
     * generating an explicit permutation, without a second index array. */
    fossil_algorithm_shuffle_fisher_yates(base, count, size, seed);
}

static void fossil_algorithm_shuffle_derangement(void *base, size_t count, size_t size, uint64_t seed)
{
    if (count < 2) return;
    unsigned char *data = (unsigned char *)base;
    uint64_t state = seed;
    for (size_t i = count - 1; i > 0; --i)
    {
        size_t j = fossil_algorithm_shuffle_bounded(&state, i);
        fossil_algorithm_shuffle_swap(data + i * size, data + j * size, size);
    }
}

static void fossil_algorithm_shuffle_block(void *base, size_t count, size_t size, uint64_t seed)
{
    if (count < 2) return;
    size_t block = count < 8 ? 2 : count / 8;
    unsigned char *data = (unsigned char *)base;
    uint64_t state = seed;
    for (size_t end = count; end > 0; )
    {
        size_t start = end > block ? end - block : 0;
        size_t target = fossil_algorithm_shuffle_bounded(&state, end - start + start);
        if (target != start)
            for (size_t i = 0; i < end - start && target + i < count; ++i)
                fossil_algorithm_shuffle_swap(data + (start + i) * size,
                                               data + (target + i) * size, size);
        end = start;
    }
}

static void fossil_algorithm_shuffle_sattolo(void *base, size_t count, size_t size, uint64_t seed)
{
    if (count < 2) return;

    unsigned char *data = (unsigned char *)base;
    uint64_t state = seed;

    for (size_t i = count - 1; i > 0; --i)
    {
        size_t j = fossil_algorithm_shuffle_bounded(&state, i);
        fossil_algorithm_shuffle_swap(data + i * size, data + j * size, size);
    }
}

// ======================================================
// Main Exec
// ======================================================

int fossil_algorithm_shuffle_exec(
    void *base,
    size_t count,
    const char *type_id,
    const char *algorithm_id,
    const char *mode_id,
    uint64_t seed)
{
    if (!base || count == 0 || !type_id)
        return -1;

    size_t size = fossil_algorithm_shuffle_type_sizeof(type_id);
    if (size == 0)
        return -2;

    const char *algo = algorithm_id ? algorithm_id : "auto";
    uint64_t final_seed = fossil_algorithm_shuffle_rand_seed(seed, mode_id);

    // AI-inspired auto selection
    if (strcmp(algo, "auto") == 0) {
        /* Small arrays favor the low-overhead inside-out variant. */
        if (count < 32) {
            fossil_algorithm_shuffle_inside_out(base, count, size, final_seed);
        } else if (strcmp(type_id, "u8") == 0 || strcmp(type_id, "i8") == 0) {
            /* Byte-sized values are efficiently mixed in blocks. */
            fossil_algorithm_shuffle_block(base, count, size, final_seed);
        } else if (strcmp(type_id, "u32") == 0 || strcmp(type_id, "i32") == 0) {
            /* Fixed-width integers use the Knuth/Fisher-Yates strategy. */
            fossil_algorithm_shuffle_knuth(base, count, size, final_seed);
        } else if (strcmp(type_id, "f32") == 0 || strcmp(type_id, "f64") == 0) {
            /* Floating-point data uses a generic riffle strategy. */
            fossil_algorithm_shuffle_riffle(base, count, size, final_seed);
        } else {
            /* General-purpose unbiased fallback. */
            fossil_algorithm_shuffle_fisher_yates(base, count, size, final_seed);
        }
        return 0;
    }

    // Specific algorithm selection
    if (strcmp(algo, "fisher_yates") == 0) {
        fossil_algorithm_shuffle_fisher_yates(base, count, size, final_seed);
    } else if (strcmp(algo, "inside_out") == 0) {
        fossil_algorithm_shuffle_inside_out(base, count, size, final_seed);
    } else if (strcmp(algo, "knuth") == 0) {
        fossil_algorithm_shuffle_knuth(base, count, size, final_seed);
    } else if (strcmp(algo, "transposition") == 0) {
        fossil_algorithm_shuffle_transposition(base, count, size, final_seed);
    } else if (strcmp(algo, "riffle") == 0) {
        fossil_algorithm_shuffle_riffle(base, count, size, final_seed);
    } else if (strcmp(algo, "overhand") == 0) {
        fossil_algorithm_shuffle_overhand(base, count, size, final_seed);
    } else if (strcmp(algo, "permutation") == 0) {
        fossil_algorithm_shuffle_permutation(base, count, size, final_seed);
    } else if (strcmp(algo, "derangement") == 0) {
        fossil_algorithm_shuffle_derangement(base, count, size, final_seed);
    } else if (strcmp(algo, "block") == 0) {
        fossil_algorithm_shuffle_block(base, count, size, final_seed);
    } else if (strcmp(algo, "sattolo") == 0) {
        fossil_algorithm_shuffle_sattolo(base, count, size, final_seed);
    } else {
        return -3; // unknown algorithm
    }
    return 0;
}
