#pragma once
#include <cstdio>
#include <immintrin.h>
#include <initializer_list>
#include <span>
#include <vector>

namespace avx {

class matrix;

class vector // Обертка над вектором из 4 double которая работает с помощью интриксиков
{
private:
    __m256d data;

public:
    vector(const std::initializer_list<double> init)
        : data(_mm256_loadu_pd(init.begin()))
    {}
    explicit vector(const __m256d init)
        : data(init)
    {}
    vector(const std::span<double> init)
        : data(_mm256_set_pd(init[3], init[2], init[1], init[0]))
    {}
    explicit vector(const double init)
        : data(_mm256_set1_pd(init))
    {}
    vector()
        : vector(0)
    {}

    vector operator+(const vector &other) const
    {
        return vector(_mm256_add_pd(this->data, other.data));
    }

    vector &operator+=(const vector &other)
    {
        this->data = _mm256_add_pd(this->data, other.data);
        return *this;
    }

    vector operator-(const vector &other) const
    {
        return vector(_mm256_sub_pd(this->data, other.data));
    }

    vector &operator-=(const vector &other)
    {
        this->data = _mm256_sub_pd(this->data, other.data);
        return *this;
    }

    vector operator*(const vector &other) const
    {
        return vector(_mm256_mul_pd(this->data, other.data));
    }

    vector &operator*=(const vector &other)
    {
        this->data = _mm256_mul_pd(this->data, other.data);
        return *this;
    }

    vector operator/(const vector &other) const
    {
        return vector(_mm256_div_pd(this->data, other.data));
    }

    vector &operator/=(const vector &other)
    {
        this->data = _mm256_div_pd(this->data, other.data);
        return *this;
    }

    double dot(const vector &other) const
    {
        auto temp = _mm256_mul_pd(this->data, other.data);
        __m128d low = _mm256_castpd256_pd128(temp);
        __m128d high = _mm256_extractf128_pd(temp, 1);
        __m128d sum = _mm_add_pd(low, high);
        __m128d res = _mm_hadd_pd(sum, sum);
        return _mm_cvtsd_f64(res);
    }

    vector dot(const matrix &other) const;

    operator std::array<double, 4>() const
    {
        std::array<double, 4> result{};
        _mm256_storeu_pd(result.data(), data);
        return result;
    }

    operator std::vector<double>() const
    {
        std::vector<double> result{};
        _mm256_storeu_pd(result.data(), data);
        return result;
    }

    operator __m256d() const
    {
        return data;
    }

    void print()
    {
        std::array<double, 4> raw = *this;
        printf("{ %f, %f, %f, %f }\n", raw[0], raw[1], raw[2], raw[3]);
    }
};
} // namespace avx
