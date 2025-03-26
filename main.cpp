#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "matrix.h"
#include "searches.h"
#include "vector.h"
#include "vector_lazy.h"

#include <chrono>
#include <functional>
#include <iostream>

template<typename F, typename... Args>
void measure_execution_time(F &&func, Args &&...args)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
}

void test_searches()
{
    linal::vector<int> x(1'000);
    std::vector<int> y(1'000);
    for (int i = 0; i < x.size; ++i) {
        x[i] = y[i] = i;
    }

    printf("My binary search recursion:\n");
    measure_execution_time([&x]() {
        for (int i = 0; i < 10'000'000; ++i) {
            sigma_searches::binary_search_recursion(x, std::rand(), 0, static_cast<int>(x.size));
        }
    });

    printf("\nMy binary search:\n");
    measure_execution_time([&x]() {
        for (int i = 0; i < 10'000'000; ++i) {
            sigma_searches::binary_search(x, std::rand());
        }
    });

    printf("\nstd binary search:\n");
    measure_execution_time([&y]() {
        for (int i = 0; i < 10'000'000; ++i) {
            std::binary_search(y.begin(), y.end(), std::rand());
        }
    });
}

int main()
{
    // Почему в Release все работает логично, но с -O0 оптимизацией std поиск практически в раза 3 медленнее
    test_searches();
    // linal::vector_lazy<double> v1(10'000'000);
    // linal::vector_lazy<double> v2(10'000'000);
    //
    // std::vector<double> v3(10'000'000);
    // std::vector<double> v4(10'000'000);
    //
    // for (int i = 0; i < v1.size; i++) {
    //     v1[i] = std::rand();
    //     v2[i] = std::rand();
    //     v3[i] = std::rand();
    //     v4[i] = std::rand();
    // }
    //
    // measure_execution_time([v1, v2]() { (v1 + v2).eval(); });
    // measure_execution_time([v3, v4]() {
    //     for (size_t i = 0; i < v3.size(); ++i) {
    //         volatile auto a = v3[i] + v4[i];
    //     }
    // });

    return 0;
}
