#include <cmath>
#include <cstdio>
#include <vector>

namespace fedik_proj {
double constexpr H = 0.00001;
int constexpr N = 500'000 + 1;

int constexpr S = 5;
int constexpr B[5] = {1, 0, 0, 4, 1};

std::vector<std::pair<double, double>> rungekut(
    double (*f)(double, double, double),
    double (*g)(double, double, double),
    double x_0,
    double y_0,
    double h = H,
    double n = N)
{
    std::vector<std::pair<double, double>> res{};
    res.resize(n);
    res[0] = {x_0, y_0};

    double t_n = 0;
    for (int i = 0; i < N - 1; i++) {
        auto [x_n, y_n] = res[i];

        double k_1_x = f(t_n, x_n, y_n);
        double k_2_x = f(t_n + h / 3, x_n + h * k_1_x / 3, y_n + h * k_1_x / 3);
        double k_3_x
            = f(t_n + h / 3,
                x_n + h * k_1_x / 6 + h * k_2_x / 6,
                y_n + h * k_1_x / 6 + h * k_2_x / 6);
        double k_4_x
            = f(t_n + h / 2,
                x_n + h / 8 * k_1_x + 3. / 8 * h * k_3_x,
                y_n + h / 8 * k_1_x + 3. / 8 * h * k_3_x);
        double k_5_x
            = f(t_n + h,
                x_n + h / 2 * k_1_x - 3. / 2 * h * k_3_x + 2 * h * k_4_x,
                y_n + h / 2 * k_1_x - 3. / 2 * h * k_3_x + 2 * h * k_4_x);
        double x_next = x_n + h * (k_1_x + 4 * k_4_x + k_5_x) / 6;

        double k_1_y = g(t_n, x_n, y_n);
        double k_2_y = g(t_n + h / 3, x_n + h * k_1_y / 3, y_n + h * k_1_y / 3);
        double k_3_y
            = g(t_n + h / 3,
                x_n + h * k_1_y / 6 + h * k_2_y / 6,
                y_n + h * k_1_y / 6 + h * k_2_y / 6);
        double k_4_y
            = g(t_n + h / 2,
                x_n + h / 8 * k_1_y + 3. / 8 * h * k_3_y,
                y_n + h / 8 * k_1_y + 3. / 8 * h * k_3_y);
        double k_5_y
            = g(t_n + h,
                x_n + h / 2 * k_1_y - 3. / 2 * h * k_3_y + 2 * h * k_4_y,
                y_n + h / 2 * k_1_y - 3. / 2 * h * k_3_y + 2 * h * k_4_y);
        double y_next = y_n + h * (k_1_y + 4 * k_4_y + k_5_y) / 6;

        res[i + 1] = std::make_pair(x_next, y_next);
        t_n += h;
    }

    return res;
}

auto test_x = [](double t, double x, double y) -> double {
    return -sin(t) / sqrt(1 + exp(2 * t)) + x * (x * x + y * y - 1);
};

auto test_y = [](double t, double x, double y) -> double {
    return cos(t) / sqrt(1 + exp(2 * t)) + y * (x * x + y * y - 1);
};

auto real_test_x = [](double t) -> double { return cos(t) / sqrt(1 + exp(2 * t)); };

auto real_test_y = [](double t) -> double { return sin(t) / sqrt(1 + exp(2 * t)); };

int main()
{
    auto res = rungekut(test_x, test_y, 1 / sqrt(2), 0);

    double max_autox = 0;
    double max_autoy = 0;

    double t = 0;
    printf("%-6s %-10s %-10s %-10s %-10s %-10s %-10s\n", "t", "x'", "x", "y'", "y", "ex", "ey");
    for (int i = 0; i < N; i += 100000) {
        auto rx = real_test_x(t);
        auto ry = real_test_y(t);
        auto absx = fabs(res[i].first - rx);
        auto absy = fabs(res[i].second - ry);

        if (absx > max_autox)
            max_autox = absx;
        if (absy > max_autoy)
            max_autoy = absy;

        printf(
            "%-6.3f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f\n",
            t,
            res[i].first,
            rx,
            res[i].second,
            ry,
            absx / fabs(rx),
            absy / fabs(ry));

        t += H * 100000;
    }

    printf("%-10.6f %-10.6f", max_autox / pow(H, S), max_autoy / pow(H, S));

    return 0;
}
} // namespace fedik_proj
