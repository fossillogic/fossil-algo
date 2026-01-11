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
#include "fossil/algorithm/filter.h"

#include <string.h>
#include <stdlib.h>

/* ======================================================
 * Internal Type Table (UNCHANGED)
 * ====================================================== */

typedef struct {
    const char *id;
    size_t      size;
} fossil_filter_type_entry;

static const fossil_filter_type_entry g_filter_types[] = {
    /* Signed Int */
    { "i8",   sizeof(int8_t)  },
    { "i16",  sizeof(int16_t) },
    { "i32",  sizeof(int32_t) },
    { "i64",  sizeof(int64_t) },

    /* Unsigned Int */
    { "u8",   sizeof(uint8_t)  },
    { "u16",  sizeof(uint16_t) },
    { "u32",  sizeof(uint32_t) },
    { "u64",  sizeof(uint64_t) },

    /* Floating */
    { "f32",  sizeof(float)  },
    { "f64",  sizeof(double) },

    /* String / Text */
    { "char", sizeof(char)   },
    { "cstr", sizeof(char*) },

    /* Boolean */
    { "bool", sizeof(bool)  },

    /* Extended */
    { "hex",      sizeof(uint64_t) },
    { "oct",      sizeof(uint64_t) },
    { "bin",      sizeof(uint64_t) },
    { "size",     sizeof(size_t)   },
    { "datetime", sizeof(int64_t)  },
    { "duration", sizeof(int64_t)  },

    /* Generic */
    { "any",  1 },
    { "null", 0 }
};

static const size_t g_filter_type_count =
    sizeof(g_filter_types) / sizeof(g_filter_types[0]);

/* ======================================================
 * Utilities
 * ====================================================== */

size_t fossil_algorithm_filter_type_sizeof(const char *type_id)
{
    if (!type_id) return 0;
    for (size_t i = 0; i < g_filter_type_count; ++i) {
        if (strcmp(g_filter_types[i].id, type_id) == 0)
            return g_filter_types[i].size;
    }
    return 0;
}

bool fossil_algorithm_filter_type_supported(const char *type_id)
{
    return fossil_algorithm_filter_type_sizeof(type_id) != 0;
}

/* ======================================================
 * Core Filter Algorithms
 * ====================================================== */

static size_t
filter_inplace(
    unsigned char *base,
    size_t count,
    size_t stride,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    size_t write = 0;
    for (size_t read = 0; read < count; ++read) {
        void *elem = base + read * stride;
        if (pred(elem, user)) {
            if (write != read) {
                memmove(base + write * stride, elem, stride);
            }
            ++write;
        }
    }
    return write;
}

static size_t
filter_stable(
    unsigned char *base,
    size_t count,
    size_t stride,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    unsigned char *tmp = (unsigned char *)malloc(count * stride);
    if (!tmp) return 0;

    size_t kept = 0;
    for (size_t i = 0; i < count; ++i) {
        void *elem = base + i * stride;
        if (pred(elem, user)) {
            memcpy(tmp + kept * stride, elem, stride);
            ++kept;
        }
    }

    memcpy(base, tmp, kept * stride);
    free(tmp);
    return kept;
}

static size_t
filter_count_only(
    unsigned char *base,
    size_t count,
    size_t stride,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    (void)stride;
    size_t hits = 0;
    for (size_t i = 0; i < count; ++i) {
        if (pred(base + i * stride, user))
            ++hits;
    }
    return hits;
}

static size_t
filter_first(
    unsigned char *base,
    size_t count,
    size_t stride,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    for (size_t i = 0; i < count; ++i)
        if (pred(base + i * stride, user))
            return 1;
    return 0;
}

static size_t
filter_last(
    unsigned char *base,
    size_t count,
    size_t stride,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    for (size_t i = count; i-- > 0; )
        if (pred(base + i * stride, user))
            return 1;
    return 0;
}

static size_t
filter_partition(
    unsigned char *base,
    size_t count,
    size_t stride,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    size_t left = 0;
    size_t right = count;

    while (left < right) {
        if (pred(base + left * stride, user)) {
            ++left;
        } else {
            --right;
            memmove(
                base + left * stride,
                base + right * stride,
                stride
            );
        }
    }
    return left;
}

static size_t
filter_lane_compact(
    unsigned char *base,
    size_t count,
    size_t stride,
    size_t lanes,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    if (lanes == 0) lanes = 1;
    if (lanes > count) lanes = count;

    size_t lane_size = (count + lanes - 1) / lanes;
    size_t write = 0;

    for (size_t lane = 0; lane < lanes; ++lane) {
        size_t start = lane * lane_size;
        size_t end   = start + lane_size;
        if (start >= count) break;
        if (end > count) end = count;

        for (size_t i = start; i < end; ++i) {
            void *elem = base + i * stride;
            if (pred(elem, user)) {
                memmove(base + write * stride, elem, stride);
                ++write;
            }
        }
    }
    return write;
}

static size_t
filter_lane_stable(
    unsigned char *base,
    size_t count,
    size_t stride,
    size_t lanes,
    fossil_algorithm_filter_predicate pred,
    void *user
) {
    if (lanes == 0) lanes = 1;
    if (lanes > count) lanes = count;

    unsigned char *tmp = (unsigned char *)malloc(count * stride);
    if (!tmp) return 0;

    size_t out = 0;
    size_t lane_size = (count + lanes - 1) / lanes;

    for (size_t lane = 0; lane < lanes; ++lane) {
        size_t start = lane * lane_size;
        size_t end   = start + lane_size;
        if (start >= count) break;
        if (end > count) end = count;

        for (size_t i = start; i < end; ++i) {
            void *elem = base + i * stride;
            if (pred(elem, user)) {
                memcpy(tmp + out * stride, elem, stride);
                ++out;
            }
        }
    }

    memcpy(base, tmp, out * stride);
    free(tmp);
    return out;
}

/* ======================================================
 * Execution Entry Point
 * ====================================================== */

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
) {
    if (!base || !predicate)
        return -1;

    size_t stride = fossil_algorithm_filter_type_sizeof(type_id);
    if (stride == 0)
        return -2;

    if (!algorithm_id) algorithm_id = "auto";
    if (!mode_id)      mode_id = "auto";

    unsigned char *bytes = (unsigned char *)base;
    size_t result = 0;

    /* AUTO resolution */
    if (strcmp(algorithm_id, "auto") == 0) {
        if (strcmp(mode_id, "lane") == 0 || lanes > 1)
            algorithm_id = "lane-compact";
        else
            algorithm_id = "inplace";
    }

    /* DRY-RUN forces count-only */
    if (strcmp(mode_id, "dry-run") == 0)
        algorithm_id = "count-only";

    /* Dispatch */
    if (strcmp(algorithm_id, "inplace") == 0 ||
        strcmp(algorithm_id, "compact") == 0) {

        result = filter_inplace(bytes, count, stride, predicate, user);

    } else if (strcmp(algorithm_id, "stable") == 0) {

        result = filter_stable(bytes, count, stride, predicate, user);

    } else if (strcmp(algorithm_id, "lane-compact") == 0) {

        result = filter_lane_compact(bytes, count, stride, lanes, predicate, user);

    } else if (strcmp(algorithm_id, "lane-stable") == 0) {

        result = filter_lane_stable(bytes, count, stride, lanes, predicate, user);

    } else if (strcmp(algorithm_id, "count-only") == 0) {

        result = filter_count_only(bytes, count, stride, predicate, user);

    } else if (strcmp(algorithm_id, "first") == 0) {

        result = filter_first(bytes, count, stride, predicate, user);

    } else if (strcmp(algorithm_id, "last") == 0) {

        result = filter_last(bytes, count, stride, predicate, user);

    } else if (strcmp(algorithm_id, "partition") == 0) {

        result = filter_partition(bytes, count, stride, predicate, user);

    } else {
        return -3;
    }

    if (out_count)
        *out_count = result;

    return 0;
}
