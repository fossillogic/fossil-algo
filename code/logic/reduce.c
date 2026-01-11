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
#include "fossil/algorithm/reduce.h"
#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>

/* ======================================================
 * Internal Type Table
 * ====================================================== */

typedef struct {
    const char *id;
    size_t      size;
} fossil_reduce_type_entry;

static const fossil_reduce_type_entry g_reduce_types[] = {
    { "i8",   sizeof(int8_t) },
    { "i16",  sizeof(int16_t) },
    { "i32",  sizeof(int32_t) },
    { "i64",  sizeof(int64_t) },

    { "u8",   sizeof(uint8_t) },
    { "u16",  sizeof(uint16_t) },
    { "u32",  sizeof(uint32_t) },
    { "u64",  sizeof(uint64_t) },

    { "f32",  sizeof(float) },
    { "f64",  sizeof(double) },

    { "bool", sizeof(bool) },

    { "size", sizeof(size_t) },
    { "datetime", sizeof(int64_t) },
    { "duration", sizeof(int64_t) },

    { "any", 1 },
    { "null", 0 }
};

static const size_t g_reduce_type_count =
    sizeof(g_reduce_types) / sizeof(g_reduce_types[0]);

size_t fossil_algorithm_reduce_type_sizeof(const char *type_id)
{
    if (!type_id) return 0;
    for (size_t i = 0; i < g_reduce_type_count; ++i)
        if (strcmp(type_id, g_reduce_types[i].id) == 0)
            return g_reduce_types[i].size;
    return 0;
}

bool fossil_algorithm_reduce_type_supported(const char *type_id)
{
    return fossil_algorithm_reduce_type_sizeof(type_id) != 0;
}

/* ======================================================
 * Internal Reducers
 * ====================================================== */

/* Sum numeric */
static void reduce_sum(void *accum, const void *elem, const char *type_id)
{
    if (strcmp(type_id, "i8") == 0)
        *(int8_t*)accum += *(const int8_t*)elem;
    else if (strcmp(type_id, "i16") == 0)
        *(int16_t*)accum += *(const int16_t*)elem;
    else if (strcmp(type_id, "i32") == 0)
        *(int32_t*)accum += *(const int32_t*)elem;
    else if (strcmp(type_id, "i64") == 0)
        *(int64_t*)accum += *(const int64_t*)elem;
    else if (strcmp(type_id, "u8") == 0)
        *(uint8_t*)accum += *(const uint8_t*)elem;
    else if (strcmp(type_id, "u16") == 0)
        *(uint16_t*)accum += *(const uint16_t*)elem;
    else if (strcmp(type_id, "u32") == 0)
        *(uint32_t*)accum += *(const uint32_t*)elem;
    else if (strcmp(type_id, "u64") == 0)
        *(uint64_t*)accum += *(const uint64_t*)elem;
    else if (strcmp(type_id, "f32") == 0)
        *(float*)accum += *(const float*)elem;
    else if (strcmp(type_id, "f64") == 0)
        *(double*)accum += *(const double*)elem;
}

/* Min numeric */
static void reduce_min(void *accum, const void *elem, const char *type_id)
{
    if (strcmp(type_id, "i8") == 0) {
        int8_t v = *(const int8_t*)elem;
        if (v < *(int8_t*)accum) *(int8_t*)accum = v;
    } else if (strcmp(type_id, "i16") == 0) {
        int16_t v = *(const int16_t*)elem;
        if (v < *(int16_t*)accum) *(int16_t*)accum = v;
    } else if (strcmp(type_id, "i32") == 0) {
        int32_t v = *(const int32_t*)elem;
        if (v < *(int32_t*)accum) *(int32_t*)accum = v;
    } else if (strcmp(type_id, "i64") == 0) {
        int64_t v = *(const int64_t*)elem;
        if (v < *(int64_t*)accum) *(int64_t*)accum = v;
    } else if (strcmp(type_id, "f32") == 0) {
        float v = *(const float*)elem;
        if (v < *(float*)accum) *(float*)accum = v;
    } else if (strcmp(type_id, "f64") == 0) {
        double v = *(const double*)elem;
        if (v < *(double*)accum) *(double*)accum = v;
    }
}

/* Max numeric */
static void reduce_max(void *accum, const void *elem, const char *type_id)
{
    if (strcmp(type_id, "i8") == 0) {
        int8_t v = *(const int8_t*)elem;
        if (v > *(int8_t*)accum) *(int8_t*)accum = v;
    } else if (strcmp(type_id, "i16") == 0) {
        int16_t v = *(const int16_t*)elem;
        if (v > *(int16_t*)accum) *(int16_t*)accum = v;
    } else if (strcmp(type_id, "i32") == 0) {
        int32_t v = *(const int32_t*)elem;
        if (v > *(int32_t*)accum) *(int32_t*)accum = v;
    } else if (strcmp(type_id, "i64") == 0) {
        int64_t v = *(const int64_t*)elem;
        if (v > *(int64_t*)accum) *(int64_t*)accum = v;
    } else if (strcmp(type_id, "f32") == 0) {
        float v = *(const float*)elem;
        if (v > *(float*)accum) *(float*)accum = v;
    } else if (strcmp(type_id, "f64") == 0) {
        double v = *(const double*)elem;
        if (v > *(double*)accum) *(double*)accum = v;
    }
}

/* Boolean any/all */
static void reduce_any(void *accum, const void *elem)
{
    *(bool*)accum = *(bool*)accum || *(const bool*)elem;
}

static void reduce_all(void *accum, const void *elem)
{
    *(bool*)accum = *(bool*)accum && *(const bool*)elem;
}

/* ======================================================
 * Public Execution Entry
 * ====================================================== */

int fossil_algorithm_reduce_exec(
    const void *base,
    size_t count,
    const char *type_id,
    const char *op_id,
    size_t lanes,
    void *out_result,
    fossil_algorithm_reduce_fn reduce_fn,
    void *user
) {
    if (!base || !out_result || !type_id || !op_id)
        return -1;

    size_t stride = fossil_algorithm_reduce_type_sizeof(type_id);
    if (stride == 0) return -2;

    if (strcmp(op_id, "custom") == 0 && !reduce_fn)
        return -1;

    const unsigned char *bytes = (const unsigned char*)base;

    /* Initialize accumulator */
    if (strcmp(op_id, "sum") == 0 || strcmp(op_id, "count") == 0) {
        memset(out_result, 0, stride);
    } else if (strcmp(op_id, "min") == 0) {
        if (strcmp(type_id, "i8") == 0) *(int8_t*)out_result = INT8_MAX;
        else if (strcmp(type_id, "i16") == 0) *(int16_t*)out_result = INT16_MAX;
        else if (strcmp(type_id, "i32") == 0) *(int32_t*)out_result = INT32_MAX;
        else if (strcmp(type_id, "i64") == 0) *(int64_t*)out_result = INT64_MAX;
        else if (strcmp(type_id, "f32") == 0) *(float*)out_result = FLT_MAX;
        else if (strcmp(type_id, "f64") == 0) *(double*)out_result = DBL_MAX;
    } else if (strcmp(op_id, "max") == 0) {
        if (strcmp(type_id, "i8") == 0) *(int8_t*)out_result = INT8_MIN;
        else if (strcmp(type_id, "i16") == 0) *(int16_t*)out_result = INT16_MIN;
        else if (strcmp(type_id, "i32") == 0) *(int32_t*)out_result = INT32_MIN;
        else if (strcmp(type_id, "i64") == 0) *(int64_t*)out_result = INT64_MIN;
        else if (strcmp(type_id, "f32") == 0) *(float*)out_result = -FLT_MAX;
        else if (strcmp(type_id, "f64") == 0) *(double*)out_result = -DBL_MAX;
    } else if (strcmp(op_id, "any") == 0) {
        *(bool*)out_result = false;
    } else if (strcmp(op_id, "all") == 0) {
        *(bool*)out_result = true;
    }

    /* Lane partitioning (thread-mimic) */
    if (lanes == 0) lanes = 1;
    size_t lane_size = (count + lanes - 1) / lanes;

    for (size_t lane = 0; lane < lanes; ++lane) {
        size_t start = lane * lane_size;
        size_t end = start + lane_size;
        if (start >= count) break;
        if (end > count) end = count;

        for (size_t i = start; i < end; ++i) {
            const void *elem = bytes + i * stride;
            if (strcmp(op_id, "sum") == 0) reduce_sum(out_result, elem, type_id);
            else if (strcmp(op_id, "min") == 0) reduce_min(out_result, elem, type_id);
            else if (strcmp(op_id, "max") == 0) reduce_max(out_result, elem, type_id);
            else if (strcmp(op_id, "count") == 0) *(size_t*)out_result += 1;
            else if (strcmp(op_id, "any") == 0) reduce_any(out_result, elem);
            else if (strcmp(op_id, "all") == 0) reduce_all(out_result, elem);
            else if (strcmp(op_id, "custom") == 0) reduce_fn(out_result, elem, user);
            else return -3;
        }
    }

    return 0;
}
