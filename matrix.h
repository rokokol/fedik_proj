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

    template<typename U>
    friend matrix<U> operations::elementwise_scalar(
        const matrix<U> &lhs, const U &rhs, std::function<U(U, U)> op);

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

    T at(int row, int col) { return (*this)(row, col); }

    matrix operator+(const matrix &rhs) { return operations::sum<T>()(*this, rhs); }

    matrix operator+(const T &rhs) { return operations::sum_scalar<T>()(*this, rhs); }

    template<typename RHS>
    matrix &operator+=(const RHS &rhs)
    {
        *this = *this + rhs;
        return *this;
    }

    matrix operator-(const matrix &rhs) { return operations::difference<T>()(*this, rhs); }

    matrix operator-(const T &rhs) { return operations::difference_scalar<T>()(*this, rhs); }

    template<typename RHS>
    matrix &operator-=(const RHS &rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    matrix operator*(const matrix &rhs) { return operations::product<T>()(*this, rhs); }

    matrix operator*(const T &rhs) { return operations::product_scalar<T>()(*this, rhs); }

    template<typename RHS>
    matrix &operator*=(const RHS &rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    matrix operator/(const matrix &rhs) { return operations::quotient<T>()(*this, rhs); }

    matrix operator/(const T &rhs) { return operations::quotient_scalar<T>()(*this, rhs); }

    template<typename RHS>
    matrix &operator/=(const RHS &rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    matrix dot(const matrix &rhs) const { return operations::multiply<T>()(*this, rhs); }

    T inner_sum() { return std::reduce(data.begin(), data.end(), T(0), std::plus<T>()); }

    T inner_prod() { return std::reduce(data.begin(), data.end(), T(1), std::multiplies<T>()); }

    void print()
    {
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                std::cout << (*this)(i, j) << ",\t";
            }
            std::cout <<std::endl;
        }
    }
};
} // namespace matrices
