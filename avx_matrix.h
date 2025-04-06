#pragma once
#include "avx_vector.h"
#include "matrix.h"
#include <cstdio>
#include <immintrin.h>
#include <initializer_list>

namespace avx {

class matrix // Обертка над матрицей 4x4 double которая работает с помощью интриксиков
{
private:
    std::array<__m256d, 4> rows;

public:
    matrix(const std::initializer_list<std::initializer_list<double>> init)
    {
        for (int i = 0; i < 4; i++) {
            rows[i] = _mm256_loadu_pd(init.begin()[i].begin());
        }
    }
    matrix(const double init)
    {
        for (int i = 0; i < 4; i++) {
            rows[i] = _mm256_set1_pd(init);
        }
    }
    matrix()
        : matrix(0)
    {}

    const __m256d &operator[](int i) const { return rows[i]; }
    __m256d &operator[](int i) { return rows[i]; }

    matrix operator+(const matrix &other) const
    {
        matrix res(0);
        for (int i = 0; i < 4; i++) {
            res[i] = _mm256_add_pd(this->rows[i], other[i]);
        }

        return res;
    }

    matrix &operator+=(const matrix &other)
    {
        for (int i = 0; i < 4; i++) {
            this->rows[i] = _mm256_add_pd(this->rows[i], other[i]);
        }

        return *this;
    }

    matrix operator-(const matrix &other) const
    {
        matrix res(0);
        for (int i = 0; i < 4; i++) {
            res[i] = _mm256_sub_pd(this->rows[i], other[i]);
        }

        return res;
    }

    matrix &operator-=(const matrix &other)
    {
        for (int i = 0; i < 4; i++) {
            this->rows[i] = _mm256_sub_pd(this->rows[i], other[i]);
        }

        return *this;
    }

    matrix operator*(const matrix &other) const
    {
        matrix res(0);
        for (int i = 0; i < 4; i++) {
            res[i] = _mm256_mul_pd(this->rows[i], other[i]);
        }

        return res;
    }

    matrix &operator*=(const matrix &other)
    {
        for (int i = 0; i < 4; i++) {
            this->rows[i] = _mm256_mul_pd(this->rows[i], other[i]);
        }

        return *this;
    }

    matrix operator/(const matrix &other) const
    {
        matrix res(0);
        for (int i = 0; i < 4; i++) {
            res[i] = _mm256_div_pd(this->rows[i], other[i]);
        }

        return res;
    }

    matrix &operator/=(const matrix &other)
    {
        for (int i = 0; i < 4; i++) {
            this->rows[i] = _mm256_div_pd(this->rows[i], other[i]);
        }

        return *this;
    }

    matrix T() const
    {
        matrix res(0);

        __m256d t0 = _mm256_unpacklo_pd(rows[0], rows[1]); // t0: {a0, b0, a2, b2}
        __m256d t1 = _mm256_unpackhi_pd(rows[0], rows[1]); // t1: {a1, b1, a3, b3}
        __m256d t2 = _mm256_unpacklo_pd(rows[2], rows[3]); // t2: {c0, d0, c2, d2}
        __m256d t3 = _mm256_unpackhi_pd(rows[2], rows[3]); // t3: {c1, d1, c3, d3}

        res[0] = _mm256_permute2f128_pd(t0, t2, 0x20); // {a0, b0, c0, d0}
        res[1] = _mm256_permute2f128_pd(t1, t3, 0x20); // {a1, b1, c1, d1}
        res[2] = _mm256_permute2f128_pd(t0, t2, 0x31); // {a2, b2, c2, d2}
        res[3] = _mm256_permute2f128_pd(t1, t3, 0x31); // {a3, b3, c3, d3}

        return res;
    }

    vector dot(const vector &other) const
    {
        alignas(32) double res[4];
        for (int i = 0; i < 4; i++) {
            auto temp = vector(rows[i]);
            res[i] = other.dot(temp);
        }

        return vector(_mm256_loadu_pd(res));
    }

    matrix dot(const matrix &other) const
    {
        matrix res(0);
        matrix temp = other.T();
        for (int i = 0; i < 4; i++) {
            res[i] = this->dot(vector(temp[i]));
        }

        return res.T();
    }

    operator matrices::matrix<double>() const
    {
        alignas(32) double res[16];
        for (int i = 0; i < 4; ++i) {
            _mm256_storeu_pd(res + i * 4, rows[i]);
        }

        return {res, 4, 4};
    }

    void print()
    {
        for (auto &&row : rows) {
            alignas(32) double raw[4];
            _mm256_storeu_pd(raw, row);
            printf("{ %f, %f, %f, %f }\n", raw[0], raw[1], raw[2], raw[3]);
        }
    }
};

vector vector::dot(const matrix &other) const
{
    alignas(32) double res[4];
    auto temp = other.T();
    for (int i = 0; i < 4; i++) {
        res[i] = this->dot(vector(temp[i]));
    }

    return vector(_mm256_loadu_pd(res));
}
}
