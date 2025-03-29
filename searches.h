#pragma once
#include <vector>

namespace sigma_searches {
template<typename T>
int linear_search(const std::vector<T> &arr, T key)
{
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == key) {
            return i;
        }
    }

    return -1;
}

template<typename T>
int binary_search(const std::vector<T> &arr, T key)
{
    int low = 0;
    int high = arr.size();
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] < key) {
            low = mid + 1;
        } else if (arr[mid] > key) {
            high = mid - 1;
        }
        if (arr[mid] == key) {
            return mid;
        }
    }

    return -1;
}

template<typename T>
int binary_search_recursion(const std::vector<T> &arr, T key, T low, T high)
{
    if (low == high && arr[low] != key || low > high) {
        return -1;

    } else {
        int mid = (low + high) / 2;
        if (arr[mid] == key) {
            return mid;
        } else if (arr[mid] < key) {
            return binary_search_recursion(arr, key, mid + 1, high);
        } else {
            return binary_search_recursion(arr, key, low, mid - 1);
        }
    }
}

} // namespace sigma_searches
