#pragma once
#include "matrix.h"
#include <cmath>
#include <complex>

#include <cstdio>
#include <fstream>
#include <functional>
#include <iomanip>

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

double constexpr m = 0.012277471;
double constexpr M = 1 - m;
double constexpr T = 11.124340337;

inline std::vector<std::vector<double>> rungekut(
    const std::vector<std::function<double(const std::vector<double> &)>> &functions,
    const std::vector<double> &cauchy_list,

    double h = H,
    int T = 5,
    int s = S,
    const matrix &A = A_def,
    const matrix &b = b_def,
    const matrix &c = c_def)
{
    std::vector<std::vector<double>> res(1);
    res[0] = cauchy_list;
    // TODO: реализовать через matrix K(s, cauchy_list.size());
    std::vector<matrix> K(cauchy_list.size() - 1, matrix(s, 1));

    double t = cauchy_list[0];
    while (t < T) {
        auto point_n = res[res.size() - 1];
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

        std::vector<double> next(cauchy_list.size());
        t = t_n + h;
        next[0] = t;
        for (int k = 0; k < K.size(); k++) {
            next[k + 1] = point_n[k + 1] + h * b.dot(K[k])(0, 0);
        } // Это тоже можно сделать через векторно-матричное умножение

        res.push_back(next);
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

inline int test()
{
    // std::ofstream file("../test_scalar.csv");

    if (file.is_open()) {
        // file << "n;errx;erry\n";
        // file << std::fixed << std::setprecision(6);
        // for (int n = 10'000; n < N*1000; n += 10'000) {
            // double h = 5. / n;
            auto res = rungekut(
                {[](std::vector<double> init) -> double {
                     return test_x(init[0], init[1], init[2]);
                 },
                 [](std::vector<double> init) -> double {
                     return test_y(init[0], init[1], init[2]);
                 }},
                {0, 1 / sqrt(2), 0},
                H);

            double max_autox = 0;
            double max_autoy = 0;

            printf(
                "%-6s %-10s %-10s %-10s %-10s %-10s %-10s\n", "t", "x'", "x", "y'", "y", "ex", "ey");
            for (int i = 0; i < res.size(); i += 1) {
                auto t = res[i][0];
                auto x = res[i][1];
                auto y = res[i][2];

                auto rx = real_test_x(t);
                auto ry = real_test_y(t);

                // file << t << ';' << x << ';' << y << ';' << rx << ';' << ry << std::endl;
                auto absx = fabs(x - rx);
                auto absy = fabs(y - ry);

                if (absx > max_autox)
                    max_autox = absx;
                if (absy > max_autoy)
                    max_autoy = absy;

                if (i % (N / 5) == 0)
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

            // file << n << ';' << max_autox / (h * h * h * h * h) << ';' << max_autox / (h * h * h * h * h) << std::endl;
        }
        file.close();
    // } else {
    //     std::cerr << "Не удалось открыть файл!\n";
    // }

    return 0;
}

inline double v1(std::vector<double> vals)
{
    const double x = vals[2];
    const double y = vals[3];
    const double v2 = vals[1];

    const double R1 = pow((x + m) * (x + m) + y * y, 1.5);
    const double R2 = pow((x - M) * (x - M) + y * y, 1.5);

    return x + 2 * v2 - M * (x + m) / R1 - m * (x - M) / R2;
}

inline double v2(std::vector<double> vals)
{
    const double x = vals[2];
    const double y = vals[3];
    const double v1 = vals[0];

    const double R1 = pow((x + m) * (x + m) + y * y, 1.5);
    const double R2 = pow((x - M) * (x - M) + y * y, 1.5);

    return y - 2 * v1 - M * y / R1 - m * y / R2;
}

inline double x(std::vector<double> vals)
{
    return vals[0];
}

inline double y(std::vector<double> vals)
{
    return vals[1];
}

inline int solve()
{
    double n = 11'124'340;
    std::ofstream file("../main.csv");

    if (file.is_open()) {
        auto res = rungekut({v1, v2, x, y}, {0, 0, -2.031732629557337, 0.994, 0}, 0.000001, n);
        // printf("%-6s %-10s %-10s %-10s %-10s\n", "t", "x'", "y'", "x", "y");
        file << "t;x';y';x;y\n";
        file << std::fixed << std::setprecision(6);

        for (int i = 0; i < n; i += 1'000) {
            auto t = res[i][0];
            auto x_diff = res[i][1];
            auto y_diff = res[i][2];
            auto x = res[i][3];
            auto y = res[i][4];

            file << t << ';' << x_diff << ';' << y_diff << ';' << x << ';' << y << std::endl;

            // printf("%-6.3f %-10.6f %-10.6f %-10.6f %-10.6f\n",
            //     t,
            //     x_diff,
            //     y_diff,
            //     x,
            //     y);
        }
        file.close();
    } else {
        std::cerr << "Не удалось открыть файл!\n";
    }

    return 0;
}

} // namespace fedik_proj
