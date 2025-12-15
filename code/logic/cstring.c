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
#include "fossil/algorithm/cstring.h"
#include <string.h>
#include <ctype.h>

// ======================================================
// Utilities
// ======================================================

static bool
algo_eq(const char *a, const char *b)
{
    return a && b && strcmp(a, b) == 0;
}

// ======================================================
// Search Algorithms
// ======================================================

static int
cstring_find(const char *s, const char *sub)
{
    if (!s || !sub) return -2;

    const char *p = strstr(s, sub);
    return p ? (int)(p - s) : -1;
}

static int
cstring_rfind(const char *s, const char *sub)
{
    if (!s || !sub) return -2;

    int last = -1;
    size_t len = strlen(sub);

    for (size_t i = 0; s[i]; i++)
        if (strncmp(&s[i], sub, len) == 0)
            last = (int)i;

    return last;
}

static int
cstring_count(const char *s, const char *sub)
{
    if (!s || !sub || !*sub) return -2;

    int count = 0;
    size_t len = strlen(sub);

    for (size_t i = 0; s[i]; i++)
        if (strncmp(&s[i], sub, len) == 0)
            count++;

    return count;
}

// ======================================================
// Comparison Algorithms
// ======================================================

static int
cstring_equals(const char *a, const char *b)
{
    return strcmp(a, b) == 0 ? 1 : -1;
}

static int
cstring_iequals(const char *a, const char *b)
{
    while (*a && *b) {
        if (tolower(*a++) != tolower(*b++))
            return -1;
    }
    return (*a == *b) ? 1 : -1;
}

// ======================================================
// Transform Algorithms
// ======================================================

static int
cstring_toupper(const char *s, char *out, size_t n)
{
    if (!out || n == 0) return -2;

    size_t i = 0;
    for (; s[i] && i + 1 < n; i++)
        out[i] = (char)toupper(s[i]);

    out[i] = '\0';
    return 0;
}

static int
cstring_tolower(const char *s, char *out, size_t n)
{
    if (!out || n == 0) return -2;

    size_t i = 0;
    for (; s[i] && i + 1 < n; i++)
        out[i] = (char)tolower(s[i]);

    out[i] = '\0';
    return 0;
}

static int
cstring_reverse(const char *s, char *out, size_t n)
{
    size_t len = strlen(s);
    if (!out || n <= len) return -2;

    for (size_t i = 0; i < len; i++)
        out[i] = s[len - 1 - i];

    out[len] = '\0';
    return 0;
}

// ======================================================
// Exec Dispatcher
// ======================================================

int
fossil_algorithm_cstring_exec(
    const char *input,
    const char *arg,
    const char *algorithm_id,
    char *output,
    size_t output_size
) {
    if (!input || !algorithm_id)
        return -2;

    if (algo_eq(algorithm_id, "find"))
        return cstring_find(input, arg);

    if (algo_eq(algorithm_id, "rfind"))
        return cstring_rfind(input, arg);

    if (algo_eq(algorithm_id, "count"))
        return cstring_count(input, arg);

    if (algo_eq(algorithm_id, "equals"))
        return cstring_equals(input, arg);

    if (algo_eq(algorithm_id, "iequals"))
        return cstring_iequals(input, arg);

    if (algo_eq(algorithm_id, "toupper"))
        return cstring_toupper(input, output, output_size);

    if (algo_eq(algorithm_id, "tolower"))
        return cstring_tolower(input, output, output_size);

    if (algo_eq(algorithm_id, "reverse"))
        return cstring_reverse(input, output, output_size);

    return -3;
}

// ======================================================
// Utility API
// ======================================================

bool
fossil_algorithm_cstring_supported(const char *algorithm_id)
{
    return algo_eq(algorithm_id, "find") ||
           algo_eq(algorithm_id, "rfind") ||
           algo_eq(algorithm_id, "count") ||
           algo_eq(algorithm_id, "equals") ||
           algo_eq(algorithm_id, "iequals") ||
           algo_eq(algorithm_id, "toupper") ||
           algo_eq(algorithm_id, "tolower") ||
           algo_eq(algorithm_id, "reverse");
}
