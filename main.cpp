#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "array2d.h"
#include "array2d_meta.h"
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
        for (int i = 0; i < 100'000'000; ++i) {
            sigma_searches::binary_search_recursion(x, std::rand(), 0, static_cast<int>(x.size));
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

void test_vector_lazy()
{
    linal::vector_lazy<double> v1{1, 2, 3};
    linal::vector_lazy<double> v2{3, 2, 1};
    linal::vector_lazy<double> v3{2, 1, 2};

    // На Release работает только такой код, иначе сигдев
    // auto temp = v2 + v3;
    // auto res = v1 * temp;
    auto res = v1 * (v2 + v3);
    auto eval = res.eval();
    printf("%f\n", eval);
}

int main()
{
    // TODO: template<class T> vs template<typename T>
    // Как красиво использовать cpp+h файлы вместо только лишь одних .h?

    // Почему в Release все работает логично, но с -O0 оптимизацией std поиск практически в раза 3 медленнее
    // На более мощном ноуте в релизе зачастую рекурсия работает быстрее даже
    // test_searches();

    // test_vector_lazy(); // когда я писал try_eval<LHS, T>(lhs) то программа ломалась

    // linal::array2d<int> A{
    //     {
    //         {1, 2},
    //         {3, 4, 5},
    //         {6, 7, 8, 9},
    //         {10}
    //     }};

    // std::vector<size_t> temp{1, 2, 3};
    // linal::array2d<int> B(std::span<size_t>{temp});
    // linal::array2d<int> B{1, 2, 3};
    // for (int i = 0; i < A.shape.size(); ++i) {
    //     for (int j = 0; j < A.shape[i]; ++j) {
    //         printf("%d ", A(i, j));
    //     }
    //     printf("\n");
    // }

    linal::array2d<int> A{
        {
            {1, 2},
            {3, 4, 5},
        }};

    linal::array2d<int> B{
            {
                {5, 4},
                {3, 2, 1},
            }};

    auto C = (A + B).eval();

    return 0;
}
