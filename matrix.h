#pragma once
#include "matrix_operations.h"
#include <functional>
#include <vector>

namespace matrices {
template<typename T>
class matrix
{
    template<typename U>
    friend matrix<U> operations::elementwise(
        const matrix<U> &lhs, const matrix<U> &rhs, std::function<U(U, U)> op);

private:
    std::vector<T> data;

public:
    const size_t rows{};
    const size_t cols{};

    matrix(const size_t rows, const size_t cols)
        : rows(rows)
        , cols(cols)
        , data(rows * cols)
    {}

    matrix(std::initializer_list<std::initializer_list<T>> init)
        : rows(init.size())
        , cols(init.begin()->size())
        , data(init.size() * init.begin()->size())
    {
        size_t i = 0;
        for (const auto &row : init) {
            for (const auto &elem : row) {
                data[i++] = elem;
            }
        }
    }

    matrix() = default;
    matrix(const matrix &rhs) = default;
    matrix(matrix &&rhs) = default;
    matrix &operator=(const matrix &rhs)
    {
        data = rhs.data;
        return *this;
    }

    T &operator()(int row, int col) { return this->data[row * cols + col]; }
    const T &operator()(int row, int col) const { return this->data[row * cols + col]; }

    matrix operator+(const matrix &rhs)
    {
        return operations::sum<T>()(*this, rhs);
    }

    matrix &operator+=(const matrix &rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    matrix operator-(const matrix &rhs)
    {
        return operations::difference<T>()(*this, rhs);
    }

    matrix &operator-=(const matrix &rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    matrix operator*(const matrix &rhs)
    {
        return operations::product<T>()(*this, rhs);
    }

    matrix &operator*=(const matrix &rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    matrix operator/(const matrix &rhs)
    {
        return operations::quotient<T>()(*this, rhs);
    }

    matrix &operator/=(const matrix &rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    matrix dot(const matrix &rhs)
    {
        return operations::multiply<T>()(*this, rhs);
    }
};
} // namespace matrices
