#pragma once
#include "matrix.h"
#include <cmath>
#include <complex>

#include <cstdio>
#include <functional>

#include <valarray>
#include <vector>

namespace fedik_proj {
using matrix = matrices::matrix<double>;

double constexpr H = 0.00001;
int constexpr N = 500'000 + 1;
int constexpr S = 5;
inline matrix b_def{{1. / 6, 0., 0., 4. / 6, 1. / 6}};
inline matrix c_def{{0., 1. / 3, 1. / 3, 1. / 2, 1.}};
inline matrix A_def{{
    {0., 0., 0., 0., 0.},
    {1. / 3, 0., 0., 0., 0.},
    {1. / 6, 1. / 6, 0., 0., 0.},
    {1. / 8, 0., 3. / 8, 0., 0.},
    {1. / 2, 0., -3. / 2, 2., 0.},
}};

inline std::vector<std::vector<double>> rungekut(
    const std::vector<std::function<double(const std::vector<double> &)>> &functions,
    const std::vector<double> &cauchy_list,

    double h = H,
    int n = N,
    int s = S,
    const matrix &A = A_def,
    const matrix &b = b_def,
    const matrix &c = c_def)
{
    std::vector<std::vector<double>> res(n, std::vector<double>(cauchy_list.size()));
    res[0] = cauchy_list;
    // TODO: реализовать через matrix K(s, cauchy_list.size());
    std::vector<matrix> K(cauchy_list.size() - 1, matrix(s, 1));

    for (int i = 0; i < n - 1; i++) {
        auto point_n = res[i];
        auto t_n = point_n[0];

        for (int j = 0; j < s; j++) {
            auto point_stage = point_n;
            point_stage[0] += c(0, j) * h;

            for (int k = 0; k < K.size(); k++) {
                point_stage[k + 1] += h * A.dot(K[k])(j, 0);
            } // Это можно сделать через векторно-матричное умножение A @ K

            for (int k = 0; k < K.size(); k++) {
                K[k](j, 0) = functions[k](point_stage);
            } // Это тоже можно сделать через векторно-матричное умножение
        }

        res[i + 1][0] = t_n + h;
        for (int k = 0; k < K.size(); k++) {
            res[i + 1][k + 1] = point_n[k + 1] + h * b.dot(K[k])(0, 0);
        } // Это тоже можно сделать через векторно-матричное умножение
    }

    return res;
}

inline auto test_x(double t, double x, double y)
{
    return -sin(t) / sqrt(1 + exp(2 * t)) + x * (x * x + y * y - 1);
}

inline auto test_y(double t, double x, double y)
{
    return cos(t) / sqrt(1 + exp(2 * t)) + y * (x * x + y * y - 1);
}

inline auto real_test_x(double t)
{
    return cos(t) / sqrt(1 + exp(2 * t));
};

inline auto real_test_y(double t)
{
    return sin(t) / sqrt(1 + exp(2 * t));
};

inline int main()
{
    auto res = rungekut(
        {[](std::vector<double> init) -> double { return test_x(init[0], init[1], init[2]); },
         [](std::vector<double> init) -> double { return test_y(init[0], init[1], init[2]); }},
        {0, 1 / sqrt(2), 0});

    double max_autox = 0;
    double max_autoy = 0;

    printf("%-6s %-10s %-10s %-10s %-10s %-10s %-10s\n", "t", "x'", "x", "y'", "y", "ex", "ey");
    for (int i = 0; i < N; i += 100000) {
        auto t = res[i][0];
        auto x = res[i][1];
        auto y = res[i][2];

        auto rx = real_test_x(t);
        auto ry = real_test_y(t);
        auto absx = fabs(x - rx);
        auto absy = fabs(y - ry);

        if (absx > max_autox)
            max_autox = absx;
        if (absy > max_autoy)
            max_autoy = absy;

        printf(
            "%-6.3f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f\n",
            t,
            x,
            rx,
            y,
            ry,
            absx / fabs(rx),
            absy / fabs(ry));
    }

    // printf("%-10.6f %-10.6f", max_autox / pow(H, S), max_autoy / pow(H, S));

    return 0;
}
} // namespace fedik_proj
