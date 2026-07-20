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
#include "fossil/algorithm/search.h"
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// ======================================================
// Search API Supported Identifiers
// ======================================================

/**
 * @brief Supported type identifiers for @ref fossil_algorithm_sort_exec.
 *
 * | Category   | Types |
 * |-------------|-------------------------------|
 * | Signed Int  | "i8", "i16", "i32", "i64"     |
 * | Unsigned Int| "u8", "u16", "u32", "u64"     |
 * | Floating    | "f32", "f64"                  |
 * | String/Text | "cstr", "char"                |
 * | Boolean     | "bool"                        |
 * | Extended    | "hex", "oct", "bin", "size", "datetime", "duration" |
 * | Generic     | "any", "null"                 |
 */
#define FOSSIL_SEARCH_SUPPORTED_TYPE_IDS \
    "i8, i16, i32, i64, u8, u16, u32, u64, " \
    "f32, f64, cstr, char, bool, " \
    "hex, oct, bin, size, datetime, duration, any, null"

/**
 * @brief Supported algorithm identifiers for @ref fossil_algorithm_search_exec.
 *
 * | Algorithm   | Description                              |
 * |-------------|------------------------------------------|
 * | "auto"      | Automatically selects the best algorithm |
 * | "linear"    | Linear search (default, general purpose) |
 * | "binary"    | Binary search (sorted arrays only)       |
 * | "jump"      | Jump search (sorted arrays only)         |
 * | "interpolation" | Interpolation search (sorted, uniform)|
 * | "exponential"   | Exponential search (sorted arrays)   |
 * | "fibonacci"     | Fibonacci search (sorted arrays)     |
 */
#define FOSSIL_SEARCH_SUPPORTED_ALGO_IDS \
    "auto, linear, binary, jump, interpolation, exponential, fibonacci"

/**
 * @brief Supported order identifiers for @ref fossil_algorithm_sort_exec.
 *
 * - "asc"  → ascending order (default)
 * - "desc" → descending order
 */
#define FOSSIL_SEARCH_SUPPORTED_ORDER_IDS "asc, desc"

// ======================================================
// Local Comparison Helpers
// ======================================================

typedef int (*fossil_search_compare_fn)(const void *, const void *, bool desc);

static inline int compare_i8(const void *a, const void *b, bool desc)
{
    int8_t va = *(const int8_t *)a;
    int8_t vb = *(const int8_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_i16(const void *a, const void *b, bool desc)
{
    int16_t va = *(const int16_t *)a;
    int16_t vb = *(const int16_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_i32(const void *a, const void *b, bool desc)
{
    int32_t va = *(const int32_t *)a;
    int32_t vb = *(const int32_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_i64(const void *a, const void *b, bool desc)
{
    int64_t va = *(const int64_t *)a;
    int64_t vb = *(const int64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u8(const void *a, const void *b, bool desc)
{
    uint8_t va = *(const uint8_t *)a;
    uint8_t vb = *(const uint8_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u16(const void *a, const void *b, bool desc)
{
    uint16_t va = *(const uint16_t *)a;
    uint16_t vb = *(const uint16_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u32(const void *a, const void *b, bool desc)
{
    uint32_t va = *(const uint32_t *)a;
    uint32_t vb = *(const uint32_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u64(const void *a, const void *b, bool desc)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_f32(const void *a, const void *b, bool desc)
{
    float va = *(const float *)a;
    float vb = *(const float *)b;
    if (va < vb) return desc ? 1 : -1;
    if (va > vb) return desc ? -1 : 1;
    return 0;
}

static inline int compare_f64(const void *a, const void *b, bool desc)
{
    double va = *(const double *)a;
    double vb = *(const double *)b;
    if (va < vb) return desc ? 1 : -1;
    if (va > vb) return desc ? -1 : 1;
    return 0;
}

static inline int compare_char(const void *a, const void *b, bool desc)
{
    char va = *(const char *)a;
    char vb = *(const char *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_cstr(const void *a, const void *b, bool desc)
{
    const char *sa = *(const char * const *)a;
    const char *sb = *(const char * const *)b;
    int cmp = strcmp(sa ? sa : "", sb ? sb : "");
    return desc ? -cmp : cmp;
}

static inline int compare_bool(const void *a, const void *b, bool desc)
{
    bool va = *(const bool *)a;
    bool vb = *(const bool *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_size(const void *a, const void *b, bool desc)
{
    size_t va = *(const size_t *)a;
    size_t vb = *(const size_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

// ======================================================
// Type System Utilities
// ======================================================

size_t fossil_algorithm_search_type_sizeof(const char *type_id)
{
    if (!type_id) return 0;

    if (!strcmp(type_id, "i8"))      return sizeof(int8_t);
    if (!strcmp(type_id, "i16"))     return sizeof(int16_t);
    if (!strcmp(type_id, "i32"))     return sizeof(int32_t);
    if (!strcmp(type_id, "i64"))     return sizeof(int64_t);

    if (!strcmp(type_id, "u8"))      return sizeof(uint8_t);
    if (!strcmp(type_id, "u16"))     return sizeof(uint16_t);
    if (!strcmp(type_id, "u32"))     return sizeof(uint32_t);
    if (!strcmp(type_id, "u64"))     return sizeof(uint64_t);

    if (!strcmp(type_id, "f32"))     return sizeof(float);
    if (!strcmp(type_id, "f64"))     return sizeof(double);

    if (!strcmp(type_id, "bool"))    return sizeof(bool);
    if (!strcmp(type_id, "char"))    return sizeof(char);
    if (!strcmp(type_id, "cstr"))    return sizeof(char *);

    if (!strcmp(type_id, "hex"))     return sizeof(uint64_t);
    if (!strcmp(type_id, "oct"))     return sizeof(uint64_t);
    if (!strcmp(type_id, "bin"))     return sizeof(uint64_t);

    if (!strcmp(type_id, "size"))    return sizeof(size_t);

    // For "datetime", "duration", "any", "null" types, return 0 (unsupported)
    if (!strcmp(type_id, "datetime")) return 0;
    if (!strcmp(type_id, "duration")) return 0;
    if (!strcmp(type_id, "any"))      return 0;
    if (!strcmp(type_id, "null"))     return 0;

    return 0;
}

bool fossil_algorithm_search_type_supported(const char *type_id)
{
    return fossil_algorithm_search_type_sizeof(type_id) != 0;
}

// ======================================================
// Comparator Selector
// ======================================================

static fossil_search_compare_fn fossil_search_select_comparator(const char *type_id)
{
    if (!strcmp(type_id, "i8"))   return compare_i8;
    if (!strcmp(type_id, "i16"))  return compare_i16;
    if (!strcmp(type_id, "i32"))  return compare_i32;
    if (!strcmp(type_id, "i64"))  return compare_i64;

    if (!strcmp(type_id, "u8"))   return compare_u8;
    if (!strcmp(type_id, "u16"))  return compare_u16;
    if (!strcmp(type_id, "u32"))  return compare_u32;
    if (!strcmp(type_id, "u64"))  return compare_u64;

    if (!strcmp(type_id, "f32"))  return compare_f32;
    if (!strcmp(type_id, "f64"))  return compare_f64;

    if (!strcmp(type_id, "char")) return compare_char;
    if (!strcmp(type_id, "cstr")) return compare_cstr;

    if (!strcmp(type_id, "bool")) return compare_bool;
    if (!strcmp(type_id, "size")) return compare_size;

    return NULL;
}

// ======================================================
// Search Implementations
// ======================================================

static int search_linear(
    const void *base, size_t count, const void *key,
    size_t size, fossil_search_compare_fn cmp, bool desc)
{
    const unsigned char *ptr = (const unsigned char *)base;
    for (size_t i = 0; i < count; ++i) {
        const void *elem = ptr + (i * size);
        if (cmp(elem, key, desc) == 0)
            return (int)i;
    }
    return -1;
}

static int search_binary(
    const void *base, size_t count, const void *key,
    size_t size, fossil_search_compare_fn cmp, bool desc)
{
    size_t low = 0, high = count;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        const void *mid_elem = (const unsigned char *)base + mid * size;
        int c = cmp(mid_elem, key, desc);
        if (c == 0)
            return (int)mid;
        if (c < 0)
            low = mid + 1;
        else
            high = mid;
    }
    return -1;
}

static int search_jump(
    const void *base, size_t count, const void *key,
    size_t size, fossil_search_compare_fn cmp, bool desc)
{
    size_t step = (size_t)sqrt((double)count);
    size_t prev = 0;
    const unsigned char *ptr = (const unsigned char *)base;

    while (prev < count && cmp(ptr + (step < count ? step - 1 : count - 1) * size, key, desc) < 0) {
        prev = step;
        step += (size_t)sqrt((double)count);
        if (prev >= count) return -1;
    }

    for (size_t i = prev; i < count && i < step; ++i)
        if (cmp(ptr + i * size, key, desc) == 0)
            return (int)i;

    return -1;
}

static int search_interpolation(
    const void *base, size_t count, const void *key,
    size_t size, fossil_search_compare_fn cmp, bool desc)
{
    // Only works for uniformly distributed sorted numeric arrays
    if (count == 0 || !base || !key) return -2;

    size_t low = 0, high = count - 1;

    if (size == sizeof(int32_t)) {
        const int32_t *arr = (const int32_t *)base;
        int32_t k = *(const int32_t *)key;
        while (low <= high &&
               ((desc && k <= arr[low] && k >= arr[high]) ||
                (!desc && k >= arr[low] && k <= arr[high]))) {
            if (arr[high] == arr[low]) {
                if (cmp(&arr[low], &k, desc) == 0) return (int)low;
                break;
            }
            size_t pos = low + (size_t)(((double)(high - low) * (k - arr[low])) / (arr[high] - arr[low]));
            if (pos < low || pos > high) break;
            if (cmp(&arr[pos], &k, desc) == 0)
                return (int)pos;
            if ((desc && arr[pos] < k) || (!desc && arr[pos] > k))
                high = pos - 1;
            else
                low = pos + 1;
        }
        return -1;
    } else if (size == sizeof(int64_t)) {
        const int64_t *arr = (const int64_t *)base;
        int64_t k = *(const int64_t *)key;
        while (low <= high &&
               ((desc && k <= arr[low] && k >= arr[high]) ||
                (!desc && k >= arr[low] && k <= arr[high]))) {
            if (arr[high] == arr[low]) {
                if (cmp(&arr[low], &k, desc) == 0) return (int)low;
                break;
            }
            size_t pos = low + (size_t)(((double)(high - low) * (k - arr[low])) / (arr[high] - arr[low]));
            if (pos < low || pos > high) break;
            if (cmp(&arr[pos], &k, desc) == 0)
                return (int)pos;
            if ((desc && arr[pos] < k) || (!desc && arr[pos] > k))
                high = pos - 1;
            else
                low = pos + 1;
        }
        return -1;
    } else if (size == sizeof(uint32_t)) {
        const uint32_t *arr = (const uint32_t *)base;
        uint32_t k = *(const uint32_t *)key;
        while (low <= high &&
               ((desc && k <= arr[low] && k >= arr[high]) ||
                (!desc && k >= arr[low] && k <= arr[high]))) {
            if (arr[high] == arr[low]) {
                if (cmp(&arr[low], &k, desc) == 0) return (int)low;
                break;
            }
            size_t pos = low + (size_t)(((double)(high - low) * (k - arr[low])) / (arr[high] - arr[low]));
            if (pos < low || pos > high) break;
            if (cmp(&arr[pos], &k, desc) == 0)
                return (int)pos;
            if ((desc && arr[pos] < k) || (!desc && arr[pos] > k))
                high = pos - 1;
            else
                low = pos + 1;
        }
        return -1;
    } else if (size == sizeof(uint64_t)) {
        const uint64_t *arr = (const uint64_t *)base;
        uint64_t k = *(const uint64_t *)key;
        while (low <= high &&
               ((desc && k <= arr[low] && k >= arr[high]) ||
                (!desc && k >= arr[low] && k <= arr[high]))) {
            if (arr[high] == arr[low]) {
                if (cmp(&arr[low], &k, desc) == 0) return (int)low;
                break;
            }
            size_t pos = low + (size_t)(((double)(high - low) * (k - arr[low])) / (arr[high] - arr[low]));
            if (pos < low || pos > high) break;
            if (cmp(&arr[pos], &k, desc) == 0)
                return (int)pos;
            if ((desc && arr[pos] < k) || (!desc && arr[pos] > k))
                high = pos - 1;
            else
                low = pos + 1;
        }
        return -1;
    } else if (size == sizeof(float)) {
        const float *arr = (const float *)base;
        float k = *(const float *)key;
        while (low <= high &&
               ((desc && k <= arr[low] && k >= arr[high]) ||
                (!desc && k >= arr[low] && k <= arr[high]))) {
            if (arr[high] == arr[low]) {
                if (cmp(&arr[low], &k, desc) == 0) return (int)low;
                break;
            }
            size_t pos = low + (size_t)(((double)(high - low) * (k - arr[low])) / (arr[high] - arr[low]));
            if (pos < low || pos > high) break;
            if (cmp(&arr[pos], &k, desc) == 0)
                return (int)pos;
            if ((desc && arr[pos] < k) || (!desc && arr[pos] > k))
                high = pos - 1;
            else
                low = pos + 1;
        }
        return -1;
    } else if (size == sizeof(double)) {
        const double *arr = (const double *)base;
        double k = *(const double *)key;
        while (low <= high &&
               ((desc && k <= arr[low] && k >= arr[high]) ||
                (!desc && k >= arr[low] && k <= arr[high]))) {
            if (arr[high] == arr[low]) {
                if (cmp(&arr[low], &k, desc) == 0) return (int)low;
                break;
            }
            size_t pos = low + (size_t)(((double)(high - low) * (k - arr[low])) / (arr[high] - arr[low]));
            if (pos < low || pos > high) break;
            if (cmp(&arr[pos], &k, desc) == 0)
                return (int)pos;
            if ((desc && arr[pos] < k) || (!desc && arr[pos] > k))
                high = pos - 1;
            else
                low = pos + 1;
        }
        return -1;
    }
    // Not supported for other types
    return -4;
}

static int search_exponential(
    const void *base, size_t count, const void *key,
    size_t size, fossil_search_compare_fn cmp, bool desc)
{
    if (count == 0 || !base || !key || !cmp) return -1;
    size_t bound = 1;
    const unsigned char *ptr = (const unsigned char *)base;

    // Find range where key may be present
    while (bound < count && cmp(ptr + bound * size, key, desc) < 0)
        bound *= 2;

    size_t low = bound / 2;
    size_t high = bound < count ? bound : count;

    // Binary search in found range
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        const void *mid_elem = ptr + mid * size;
        int c = cmp(mid_elem, key, desc);
        if (c == 0)
            return (int)mid;
        if (c < 0)
            low = mid + 1;
        else
            high = mid;
    }
    return -1;
}

static int search_fibonacci(
    const void *base, size_t count, const void *key,
    size_t size, fossil_search_compare_fn cmp, bool desc)
{
    size_t fibMMm2 = 0; // (m-2)'th Fibonacci No.
    size_t fibMMm1 = 1; // (m-1)'th Fibonacci No.
    size_t fibM = fibMMm2 + fibMMm1; // m'th Fibonacci

    while (fibM < count) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    size_t offset = -1;
    const unsigned char *ptr = (const unsigned char *)base;

    while (fibM > 1) {
        size_t i = (offset + fibMMm2 < count - 1) ? offset + fibMMm2 : count - 1;
        int c = cmp(ptr + i * size, key, desc);
        if (c < 0) {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        } else if (c > 0) {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else {
            return (int)i;
        }
    }

    if (fibMMm1 && offset + 1 < count &&
        cmp(ptr + (offset + 1) * size, key, desc) == 0)
        return (int)(offset + 1);

    return -1;
}

// ======================================================
// Dispatcher
// ======================================================

int fossil_algorithm_search_exec(
    const void *base,
    size_t count,
    const void *key,
    const char *type_id,
    const char *algorithm_id,
    const char *order_id)
{
    if (!base || !key || count == 0 || !type_id)
        return -2; // invalid input

    size_t type_size = fossil_algorithm_search_type_sizeof(type_id);
    if (type_size == 0)
        return -3; // unknown type

    bool desc = (order_id && strcmp(order_id, "desc") == 0);
    fossil_search_compare_fn cmp = fossil_search_select_comparator(type_id);
    if (!cmp)
        return -3; // unknown type

    if (!algorithm_id || !strcmp(algorithm_id, "auto") || !strcmp(algorithm_id, "linear"))
        return search_linear(base, count, key, type_size, cmp, desc);

    if (!strcmp(algorithm_id, "binary"))
        return search_binary(base, count, key, type_size, cmp, desc);

    if (!strcmp(algorithm_id, "jump"))
        return search_jump(base, count, key, type_size, cmp, desc);

    if (!strcmp(algorithm_id, "interpolation")) {
        // Only support int32_t and int64_t for interpolation
        if (!(type_size == sizeof(int32_t) || type_size == sizeof(int64_t)))
            return -4;
        return search_interpolation(base, count, key, type_size, cmp, desc);
    }

    if (!strcmp(algorithm_id, "exponential"))
        return search_exponential(base, count, key, type_size, cmp, desc);

    if (!strcmp(algorithm_id, "fibonacci"))
        return search_fibonacci(base, count, key, type_size, cmp, desc);

    return -4; // unknown algorithm
}
