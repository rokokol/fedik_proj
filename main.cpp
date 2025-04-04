#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "fedik_proj.h"
#include "matrix.h"
#include "matrix_lazy.h"
#include "searches.h"

#include <chrono>
#include <functional>

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
    std::vector<int> x(1'000);
    std::vector<int> y(1'000);
    for (int i = 0; i < x.size(); ++i) {
        x[i] = y[i] = i;
    }

    printf("My binary search recursion:\n");
    measure_execution_time([&x]() {
        for (int i = 0; i < 100'000'000; ++i) {
            sigma_searches::binary_search_recursion(x, 1, 0, static_cast<int>(x.size()));
        }
    });

    printf("\nMy binary search:\n");
    measure_execution_time([&x]() {
        for (int i = 0; i < 100'000'000; ++i) {
            sigma_searches::binary_search(x, std::rand());
        }
    });

    printf("\nstd binary search:\n");
    measure_execution_time([&y]() {
        for (int i = 0; i < 100'000'000; ++i) {
            std::binary_search(y.begin(), y.end(), std::rand());
        }
    });
}

void test_lazy_matrices()
{
    matrices::matrix_lazy<int> A{{{1, 2}, {3, 4}}};
    matrices::matrix_lazy<int> B{{{2, 0}, {0, 2}}};

    for (int i = 0; i < 1'000'000; i++) {
        (A + B).eval();
    }
}

void test_matrices()
{
    matrices::matrix<int> A{{{1, 2}, {3, 4}}};
    matrices::matrix<int> B{{{2, 0}, {0, 2}}};

    for (int i = 0; i < 1'000'000; i++) {
        A + B;
    }
}

int main()
{
    // test_searches();
    // measure_execution_time(test_lazy_matrices);
    // measure_execution_time(test_matrices);
    measure_execution_time(fedik_proj::test);
    return 0;
}
