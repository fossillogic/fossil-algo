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
    srand((unsigned int)(seed & 0xFFFFFFFFULL));

    for (size_t i = count - 1; i > 0; --i)
    {
        size_t j = (size_t)(rand() % (i + 1));
        fossil_algorithm_shuffle_swap(data + i * size, data + j * size, size);
    }
}

static void fossil_algorithm_shuffle_inside_out(void *base, size_t count, size_t size, uint64_t seed)
{
    unsigned char *data = (unsigned char *)base;
    srand((unsigned int)(seed & 0xFFFFFFFFULL));

    for (size_t i = 1; i < count; ++i)
    {
        size_t j = (size_t)(rand() % (i + 1));
        if (j != i)
            fossil_algorithm_shuffle_swap(data + i * size, data + j * size, size);
    }
}

static void fossil_algorithm_shuffle_block(void *base, size_t count, size_t size, uint64_t seed)
{
    if (count < 2) return;

    unsigned char *data = (unsigned char *)base;
    size_t block_size = (count < 16) ? 1 : 16; // AI-inspired heuristic
    srand((unsigned int)(seed & 0xFFFFFFFFULL));

    // shuffle blocks
    for (size_t i = count - 1; i > 0; i -= block_size)
    {
        size_t j = (size_t)(rand() % ((i / block_size) + 1)) * block_size;
        fossil_algorithm_shuffle_swap(data + i * size, data + j * size, block_size * size);
    }

    // then shuffle elements inside each block
    for (size_t b = 0; b < count; b += block_size)
    {
        size_t end = (b + block_size > count) ? count : b + block_size;
        for (size_t i = b + 1; i < end; ++i)
        {
            size_t j = b + (size_t)(rand() % (i - b + 1));
            fossil_algorithm_shuffle_swap(data + i * size, data + j * size, size);
        }
    }
}

static void fossil_algorithm_shuffle_sattolo(void *base, size_t count, size_t size, uint64_t seed)
{
    if (count < 2) return;

    unsigned char *data = (unsigned char *)base;
    srand((unsigned int)(seed & 0xFFFFFFFFULL));

    for (size_t i = count - 1; i > 0; --i)
    {
        size_t j = (size_t)(rand() % i); // note: % i, not % (i+1)
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

    // Algorithm selection
    if (strcmp(algo, "auto") == 0) {
        if (count < 32) {
            fossil_algorithm_shuffle_inside_out(base, count, size, final_seed);
        } else if (size > 16) {
            fossil_algorithm_shuffle_block(base, count, size, final_seed);
        } else if (!strcmp(type_id, "cstr") || !strcmp(type_id, "any")) {
            fossil_algorithm_shuffle_sattolo(base, count, size, final_seed);
        } else {
            fossil_algorithm_shuffle_fisher_yates(base, count, size, final_seed);
        }
        return 0;
    }

    return -3; // unknown algorithm
}
