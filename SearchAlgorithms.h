#ifndef SEARCHALGORITHMS_H
#define SEARCHALGORITHMS_H

#include "SearchReport.h"
#include <chrono>
#include <cstddef>
#include <string>

// Each function counts target-to-element comparisons and times only the search loop.
template <typename T>
SearchReport linearSearch(const T* values, std::size_t size, const T& target) {
    long long comparisons = 0;
    int result = -1;
    const auto start = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < size; ++i) {
        ++comparisons;
        if (values[i] == target) {
            result = static_cast<int>(i);
            break; // Sequential search naturally finds the lowest matching index.
        }
    }

    const auto stop = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    return SearchReport("Linear Search", result, comparisons, elapsed);
}

template <typename T>
SearchReport binarySearch(const T* values, std::size_t size, const T& target, bool sorted) {
    if (!sorted) return SearchReport("Binary Search", -1, 0, 0, "REJECTED: dataset is unsorted");
    if (size == 0) return SearchReport("Binary Search", -1, 0, 0, "Dataset is empty");

    long long comparisons = 0;
    int result = -1;
    std::size_t low = 0, high = size;
    const auto start = std::chrono::steady_clock::now();

    // Lower-bound-style binary search ensures the first duplicate is returned.
    while (low < high) {
        const std::size_t mid = low + (high - low) / 2;
        ++comparisons;
        if (values[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    if (low < size) {
        ++comparisons;
        if (values[low] == target) result = static_cast<int>(low);
    }

    const auto stop = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    return SearchReport("Binary Search", result, comparisons, elapsed);
}

template <typename T>
SearchReport ternarySearch(const T* values, std::size_t size, const T& target, bool sorted) {
    if (!sorted) return SearchReport("Ternary Search", -1, 0, 0, "REJECTED: dataset is unsorted");
    if (size == 0) return SearchReport("Ternary Search", -1, 0, 0, "Dataset is empty");

    long long comparisons = 0;
    int result = -1;
    std::size_t low = 0, high = size - 1;
    const auto start = std::chrono::steady_clock::now();

    while (low <= high) {
        const std::size_t third = (high - low) / 3;
        const std::size_t mid1 = low + third;
        const std::size_t mid2 = high - third;

        ++comparisons;
        if (values[mid1] == target) {
            result = static_cast<int>(mid1);
            high = mid1 == 0 ? 0 : mid1 - 1;
            if (mid1 == 0) break;
            continue;
        }

        ++comparisons;
        if (values[mid2] == target) {
            result = static_cast<int>(mid2);
            high = mid2 == 0 ? 0 : mid2 - 1;
            if (mid2 == 0) break;
            continue;
        }

        ++comparisons;
        if (target < values[mid1]) {
            if (mid1 == 0) break;
            high = mid1 - 1;
        } else {
            ++comparisons;
            if (target > values[mid2]) {
                low = mid2 + 1;
            } else {
                low = mid1 + 1;
                if (mid2 == 0) break;
                high = mid2 - 1;
            }
        }
    }

    // The search above may encounter a duplicate after the first one; normalize
    // to the first matching index while counting those equality checks.
    if (result >= 0) {
        while (result > 0) {
            ++comparisons;
            if (!(values[result - 1] == target)) break;
            --result;
        }
    }

    const auto stop = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    return SearchReport("Ternary Search", result, comparisons, elapsed);
}

template <typename T>
SearchReport interpolationSearch(const T* values, std::size_t size, const T& target, bool sorted) {
    if (!sorted) return SearchReport("Interpolation Search", -1, 0, 0, "REJECTED: dataset is unsorted");
    if (size == 0) return SearchReport("Interpolation Search", -1, 0, 0, "Dataset is empty");

    long long comparisons = 0;
    int result = -1;
    std::size_t low = 0, high = size - 1;
    const auto start = std::chrono::steady_clock::now();

    while (low <= high) {
        ++comparisons;
        if (target < values[low] || target > values[high]) break;

        if (values[low] == values[high]) {
            ++comparisons;
            if (values[low] == target) result = static_cast<int>(low);
            break; // Avoid division by zero when boundary values are equal.
        }

        // Convert to long double for a stable estimate with both int and double.
        const long double numerator =
            static_cast<long double>(target) - static_cast<long double>(values[low]);
        const long double denominator =
            static_cast<long double>(values[high]) - static_cast<long double>(values[low]);
        long double fraction = numerator / denominator;
        if (fraction < 0.0L) fraction = 0.0L;
        if (fraction > 1.0L) fraction = 1.0L;

        std::size_t pos = low + static_cast<std::size_t>(
            fraction * static_cast<long double>(high - low));
        if (pos < low) pos = low;
        if (pos > high) pos = high;

        ++comparisons;
        if (values[pos] == target) {
            result = static_cast<int>(pos);
            break;
        }

        ++comparisons;
        if (values[pos] < target) {
            if (pos == high) break;
            low = pos + 1;
        } else {
            if (pos == 0) break;
            high = pos - 1;
        }
    }

    // Interpolation can land anywhere within a duplicate run; find its first item.
    if (result >= 0) {
        while (result > 0) {
            ++comparisons;
            if (!(values[result - 1] == target)) break;
            --result;
        }
    }

    const auto stop = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    return SearchReport("Interpolation Search", result, comparisons, elapsed);
}

#endif
