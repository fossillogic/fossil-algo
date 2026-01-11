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
#include "fossil/algorithm/sort.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

// ======================================================
// Supported Identifiers
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

/**
 * @brief Supported algorithm identifiers for @ref fossil_algorithm_sort_exec.
 *
 * | Algorithm | Description |
 * |------------|----------------------------|
 * | "auto"     | Automatically selects the best algorithm |
 * | "merge"    | Stable merge sort                        |
 * | "heap"     | Heap sort (memory-efficient)              |
 * | "insertion"| Simple insertion sort (small arrays)      |
 * | "shell"    | Shell sort (incremental gap sort)         |
 * | "radix"    | Radix sort (integer keys only)            |
 * | "counting" | Counting sort (integer range keys only)   |
 * | "bubble"   | Bubble sort (testing/educational only)    |
 */

/**
 * @brief Supported order identifiers for @ref fossil_algorithm_sort_exec.
 *
 * - "asc"  → ascending order (default)
 * - "desc" → descending order
 */

// ======================================================
// Local helpers
// ======================================================

typedef int (*fossil_sort_compare_fn)(const void *, const void *, bool desc);

static inline int compare_i8(const void *a, const void *b, bool desc) {
    int8_t va = *(const int8_t *)a;
    int8_t vb = *(const int8_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_i16(const void *a, const void *b, bool desc) {
    int16_t va = *(const int16_t *)a;
    int16_t vb = *(const int16_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_i32(const void *a, const void *b, bool desc) {
    int32_t va = *(const int32_t *)a;
    int32_t vb = *(const int32_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_i64(const void *a, const void *b, bool desc) {
    int64_t va = *(const int64_t *)a;
    int64_t vb = *(const int64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u8(const void *a, const void *b, bool desc) {
    uint8_t va = *(const uint8_t *)a;
    uint8_t vb = *(const uint8_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u16(const void *a, const void *b, bool desc) {
    uint16_t va = *(const uint16_t *)a;
    uint16_t vb = *(const uint16_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u32(const void *a, const void *b, bool desc) {
    uint32_t va = *(const uint32_t *)a;
    uint32_t vb = *(const uint32_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_u64(const void *a, const void *b, bool desc) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

// Hex, Oct, Bin: treat as unsigned integer for sorting
static inline int compare_hex(const void *a, const void *b, bool desc) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_oct(const void *a, const void *b, bool desc) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_bin(const void *a, const void *b, bool desc) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_f32(const void *a, const void *b, bool desc) {
    float va = *(const float *)a;
    float vb = *(const float *)b;
    if (va < vb) return desc ? 1 : -1;
    if (va > vb) return desc ? -1 : 1;
    return 0;
}

static inline int compare_f64(const void *a, const void *b, bool desc) {
    double va = *(const double *)a;
    double vb = *(const double *)b;
    if (va < vb) return desc ? 1 : -1;
    if (va > vb) return desc ? -1 : 1;
    return 0;
}

static inline int compare_cstr(const void *a, const void *b, bool desc) {
    const char *sa = *(const char * const *)a;
    const char *sb = *(const char * const *)b;
    int cmp = strcmp(sa ? sa : "", sb ? sb : "");
    return desc ? -cmp : cmp;
}

static inline int compare_char(const void *a, const void *b, bool desc) {
    char va = *(const char *)a;
    char vb = *(const char *)b;
    if (va == vb) return 0;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_bool(const void *a, const void *b, bool desc) {
    bool va = *(const bool *)a;
    bool vb = *(const bool *)b;
    if (va == vb) return 0;
    return desc ? (vb ? 1 : -1) : (va ? 1 : -1);
}

static inline int compare_size(const void *a, const void *b, bool desc) {
    size_t va = *(const size_t *)a;
    size_t vb = *(const size_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

// For datetime and duration, treat as int64_t for sorting
static inline int compare_datetime(const void *a, const void *b, bool desc) {
    int64_t va = *(const int64_t *)a;
    int64_t vb = *(const int64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}

static inline int compare_duration(const void *a, const void *b, bool desc) {
    int64_t va = *(const int64_t *)a;
    int64_t vb = *(const int64_t *)b;
    return desc ? (vb > va) - (vb < va) : (va > vb) - (va < vb);
}


// ======================================================
// Type system utilities
// ======================================================

size_t fossil_algorithm_sort_type_sizeof(const char *type_id) {
    if (!type_id) return 0;

    if (!strcmp(type_id, "i8"))  return sizeof(int8_t);
    if (!strcmp(type_id, "i16")) return sizeof(int16_t);
    if (!strcmp(type_id, "i32")) return sizeof(int32_t);
    if (!strcmp(type_id, "i64")) return sizeof(int64_t);

    if (!strcmp(type_id, "u8"))  return sizeof(uint8_t);
    if (!strcmp(type_id, "u16")) return sizeof(uint16_t);
    if (!strcmp(type_id, "u32")) return sizeof(uint32_t);
    if (!strcmp(type_id, "u64")) return sizeof(uint64_t);

    if (!strcmp(type_id, "f32")) return sizeof(float);
    if (!strcmp(type_id, "f64")) return sizeof(double);

    if (!strcmp(type_id, "bool")) return sizeof(bool);
    if (!strcmp(type_id, "char")) return sizeof(char);
    if (!strcmp(type_id, "cstr")) return sizeof(char *);

    if (!strcmp(type_id, "size")) return sizeof(size_t);

    if (!strcmp(type_id, "hex")) return sizeof(uint64_t);
    if (!strcmp(type_id, "oct")) return sizeof(uint64_t);
    if (!strcmp(type_id, "bin")) return sizeof(uint64_t);

    if (!strcmp(type_id, "datetime")) return sizeof(int64_t);
    if (!strcmp(type_id, "duration")) return sizeof(int64_t);

    // Extended or dynamic types like "any" and "null" have no fixed width
    return 0;
}

bool fossil_algorithm_sort_type_supported(const char *type_id) {
    return fossil_algorithm_sort_type_sizeof(type_id) != 0;
}

// ======================================================
// Algorithm selection
// ======================================================

static fossil_sort_compare_fn fossil_sort_select_comparator(const char *type_id) {
    if (!strcmp(type_id, "i8"))       return compare_i8;
    if (!strcmp(type_id, "i16"))      return compare_i16;
    if (!strcmp(type_id, "i32"))      return compare_i32;
    if (!strcmp(type_id, "i64"))      return compare_i64;

    if (!strcmp(type_id, "u8"))       return compare_u8;
    if (!strcmp(type_id, "u16"))      return compare_u16;
    if (!strcmp(type_id, "u32"))      return compare_u32;
    if (!strcmp(type_id, "u64"))      return compare_u64;

    if (!strcmp(type_id, "hex"))      return compare_hex;
    if (!strcmp(type_id, "oct"))      return compare_oct;
    if (!strcmp(type_id, "bin"))      return compare_bin;

    if (!strcmp(type_id, "f32"))      return compare_f32;
    if (!strcmp(type_id, "f64"))      return compare_f64;

    if (!strcmp(type_id, "bool"))     return compare_bool;
    if (!strcmp(type_id, "char"))     return compare_char;
    if (!strcmp(type_id, "cstr"))     return compare_cstr;

    if (!strcmp(type_id, "size"))     return compare_size;

    if (!strcmp(type_id, "datetime")) return compare_datetime;
    if (!strcmp(type_id, "duration")) return compare_duration;

    return NULL;
}

// ======================================================
// Algorithm stubs
// ======================================================

static void fossil_merge(
    char *base, size_t left, size_t mid, size_t right, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    size_t n1 = mid - left + 1;
    size_t n2 = right - mid;

    char *L = malloc(n1 * type_size);
    char *R = malloc(n2 * type_size);

    if (!L || !R) {
        free(L); free(R);
        return;
    }

    memcpy(L, base + left * type_size, n1 * type_size);
    memcpy(R, base + (mid + 1) * type_size, n2 * type_size);

    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (cmp(L + i * type_size, R + j * type_size, desc) <= 0) {
            memcpy(base + k * type_size, L + i * type_size, type_size);
            i++;
        } else {
            memcpy(base + k * type_size, R + j * type_size, type_size);
            j++;
        }
        k++;
    }
    while (i < n1) {
        memcpy(base + k * type_size, L + i * type_size, type_size);
        i++; k++;
    }
    while (j < n2) {
        memcpy(base + k * type_size, R + j * type_size, type_size);
        j++; k++;
    }
    free(L); free(R);
}

static void fossil_merge_sort_rec(
    char *base, size_t left, size_t right, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        fossil_merge_sort_rec(base, left, mid, type_size, cmp, desc);
        fossil_merge_sort_rec(base, mid + 1, right, type_size, cmp, desc);
        fossil_merge(base, left, mid, right, type_size, cmp, desc);
    }
}

static int fossil_sort_merge_stub(
    void *base, size_t count, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    if (!base || count < 2 || !cmp || type_size == 0)
        return -10;
    fossil_merge_sort_rec((char *)base, 0, count - 1, type_size, cmp, desc);
    return 0;
}

static void fossil_heapify(
    char *base, size_t count, size_t root, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    size_t largest = root;
    size_t left = 2 * root + 1;
    size_t right = 2 * root + 2;

    char *tmp = malloc(type_size);
    if (!tmp) return;

    if (left < count && cmp(base + left * type_size, base + largest * type_size, desc) > 0)
        largest = left;
    if (right < count && cmp(base + right * type_size, base + largest * type_size, desc) > 0)
        largest = right;

    if (largest != root) {
        memcpy(tmp, base + root * type_size, type_size);
        memcpy(base + root * type_size, base + largest * type_size, type_size);
        memcpy(base + largest * type_size, tmp, type_size);

        fossil_heapify(base, count, largest, type_size, cmp, desc);
    }
    free(tmp);
}

static int fossil_sort_heap_stub(
    void *base, size_t count, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    if (!base || count < 2 || !cmp || type_size == 0)
        return -11;

    char *arr = (char *)base;

    // Build heap
    for (long i = (long)count / 2 - 1; i >= 0; --i)
        fossil_heapify(arr, count, (size_t)i, type_size, cmp, desc);

    char *tmp = malloc(type_size);
    if (!tmp) return -11;

    // Extract elements from heap
    for (size_t i = count - 1; i > 0; --i) {
        memcpy(tmp, arr, type_size);
        memcpy(arr, arr + i * type_size, type_size);
        memcpy(arr + i * type_size, tmp, type_size);

        fossil_heapify(arr, i, 0, type_size, cmp, desc);
    }
    free(tmp);
    return 0;
}

static int fossil_sort_insertion_stub(
    void *base, size_t count, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    if (!base || count < 2 || !cmp || type_size == 0)
        return -12;

    char *arr = (char *)base;
    char *tmp = malloc(type_size);
    if (!tmp) return -12;

    for (size_t i = 1; i < count; ++i) {
        memcpy(tmp, arr + i * type_size, type_size);
        size_t j = i;
        while (j > 0 && cmp(arr + (j - 1) * type_size, tmp, desc) > 0) {
            memcpy(arr + j * type_size, arr + (j - 1) * type_size, type_size);
            --j;
        }
        memcpy(arr + j * type_size, tmp, type_size);
    }
    free(tmp);
    return 0;
}

// Shell Sort
static int fossil_sort_shell_stub(
    void *base, size_t count, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    if (!base || count < 2 || !cmp || type_size == 0)
        return -13;

    char *arr = (char *)base;
    char *tmp = malloc(type_size);
    if (!tmp) return -13;

    for (size_t gap = count / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < count; ++i) {
            memcpy(tmp, arr + i * type_size, type_size);
            size_t j = i;
            while (j >= gap && cmp(arr + (j - gap) * type_size, tmp, desc) > 0) {
                memcpy(arr + j * type_size, arr + (j - gap) * type_size, type_size);
                j -= gap;
            }
            memcpy(arr + j * type_size, tmp, type_size);
        }
    }
    free(tmp);
    return 0;
}

// Bubble Sort
static int fossil_sort_bubble_stub(
    void *base, size_t count, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    if (!base || count < 2 || !cmp || type_size == 0)
        return -14;

    char *arr = (char *)base;
    char *tmp = malloc(type_size);
    if (!tmp) return -14;

    for (size_t i = 0; i < count - 1; ++i) {
        for (size_t j = 0; j < count - i - 1; ++j) {
            if (cmp(arr + j * type_size, arr + (j + 1) * type_size, desc) > 0) {
                memcpy(tmp, arr + j * type_size, type_size);
                memcpy(arr + j * type_size, arr + (j + 1) * type_size, type_size);
                memcpy(arr + (j + 1) * type_size, tmp, type_size);
            }
        }
    }
    free(tmp);
    return 0;
}

// Counting Sort (only for integer types)
static int fossil_sort_counting_stub(
    void *base, size_t count, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    // Only supports u8 for simplicity
    if (!base || count < 2 || !cmp || type_size != sizeof(uint8_t))
        return -15;

    uint8_t *arr = (uint8_t *)base;
    uint8_t max = arr[0], min = arr[0];
    for (size_t i = 1; i < count; ++i) {
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
    }
    size_t range = max - min + 1;
    size_t *count_arr = calloc(range, sizeof(size_t));
    if (!count_arr) return -15;

    for (size_t i = 0; i < count; ++i)
        count_arr[arr[i] - min]++;

    if (!desc) {
        size_t idx = 0;
        for (size_t i = 0; i < range; ++i)
            for (size_t j = 0; j < count_arr[i]; ++j)
                arr[idx++] = (uint8_t)(i + min);
    } else {
        size_t idx = 0;
        for (size_t i = range; i-- > 0;)
            for (size_t j = 0; j < count_arr[i]; ++j)
                arr[idx++] = (uint8_t)(i + min);
    }
    free(count_arr);
    return 0;
}

// Radix Sort (only for u32)
static int fossil_sort_radix_stub(
    void *base, size_t count, size_t type_size, fossil_sort_compare_fn cmp, bool desc)
{
    if (!base || count < 2 || !cmp || type_size != sizeof(uint32_t))
        return -16;

    uint32_t *arr = (uint32_t *)base;
    uint32_t max = arr[0];
    for (size_t i = 1; i < count; ++i)
        if (arr[i] > max) max = arr[i];

    uint32_t *output = malloc(count * sizeof(uint32_t));
    if (!output) return -16;

    for (uint32_t exp = 1; max / exp > 0; exp *= 10) {
        size_t bucket[10] = {0};
        for (size_t i = 0; i < count; ++i)
            bucket[(arr[i] / exp) % 10]++;
        for (size_t i = 1; i < 10; ++i)
            bucket[i] += bucket[i - 1];
        for (size_t i = count; i-- > 0;) {
            size_t idx = (arr[i] / exp) % 10;
            output[--bucket[idx]] = arr[i];
        }
        memcpy(arr, output, count * sizeof(uint32_t));
    }
    if (desc) {
        for (size_t i = 0; i < count / 2; ++i) {
            uint32_t tmp = arr[i];
            arr[i] = arr[count - i - 1];
            arr[count - i - 1] = tmp;
        }
    }
    free(output);
    return 0;
}

// ======================================================
// Algorithm dispatch
// ======================================================

int fossil_algorithm_sort_exec(
    void *base,
    size_t count,
    const char *type_id,
    const char *algorithm_id,
    const char *order_id)
{
    if (!base || count == 0 || !type_id)
        return -1; // invalid input

    bool desc = (order_id && strcmp(order_id, "desc") == 0);
    size_t type_size = fossil_algorithm_sort_type_sizeof(type_id);
    if (type_size == 0)
        return -2; // unknown type

    fossil_sort_compare_fn cmp = fossil_sort_select_comparator(type_id);
    if (!cmp)
        return -2; // unknown comparator

    // -----------------------------
    // AI-inspired auto selection
    // -----------------------------
    if (!algorithm_id || strcmp(algorithm_id, "auto") == 0) {
        // Small arrays → insertion sort
        if (count < 32) {
            return fossil_sort_insertion_stub(base, count, type_size, cmp, desc);
        }

        // Integer types with small range → counting sort
        if (!strcmp(type_id, "u8") || !strcmp(type_id, "i8")) {
            return fossil_sort_counting_stub(base, count, type_size, cmp, desc);
        }

        // Integer 32-bit with moderate size → radix sort
        if (!strcmp(type_id, "u32") || !strcmp(type_id, "i32")) {
            return fossil_sort_radix_stub(base, count, type_size, cmp, desc);
        }

        // Floating types → merge sort (stable, generic)
        if (!strcmp(type_id, "f32") || !strcmp(type_id, "f64")) {
            return fossil_sort_merge_stub(base, count, type_size, cmp, desc);
        }

        // Default → merge sort (stable, general-purpose)
        return fossil_sort_merge_stub(base, count, type_size, cmp, desc);
    }

    // -----------------------------
    // Explicit algorithm dispatch
    // -----------------------------
    if (!strcmp(algorithm_id, "merge")) {
        return fossil_sort_merge_stub(base, count, type_size, cmp, desc);
    }
    if (!strcmp(algorithm_id, "heap")) {
        return fossil_sort_heap_stub(base, count, type_size, cmp, desc);
    }
    if (!strcmp(algorithm_id, "insertion")) {
        return fossil_sort_insertion_stub(base, count, type_size, cmp, desc);
    }
    if (!strcmp(algorithm_id, "shell")) {
        return fossil_sort_shell_stub(base, count, type_size, cmp, desc);
    }
    if (!strcmp(algorithm_id, "bubble")) {
        return fossil_sort_bubble_stub(base, count, type_size, cmp, desc);
    }
    if (!strcmp(algorithm_id, "counting")) {
        return fossil_sort_counting_stub(base, count, type_size, cmp, desc);
    }
    if (!strcmp(algorithm_id, "radix")) {
        return fossil_sort_radix_stub(base, count, type_size, cmp, desc);
    }

    return -3; // unknown algorithm
}
