#pragma once
#include "matrix.h"
#include "matrix_operation.h"
#include "matrix_operations.h"

namespace matrices {
template<typename T>
class matrix_lazy : public matrix<T>
{
public:
    matrix_lazy(const matrix<T> &m)
        : matrix<T>(m)
    {}

    template<typename RHS>
    auto operator+(const RHS &rhs)
    {
        return operations::matrix_operation<matrix_lazy<T>, RHS, T, operations::sum<T>>(*this, rhs);
    }

    template<typename RHS>
    auto operator-(const RHS &rhs)
    {
        return operations::matrix_operation<matrix_lazy<T>, RHS, T, operations::difference<T>>(
            *this, rhs);
    }

    template<typename RHS>
    auto operator*(const RHS &rhs)
    {
        return operations::matrix_operation<matrix_lazy<T>, RHS, T, operations::product<T>>(
            *this, rhs);
    }

    template<typename RHS>
    auto operator/(const RHS &rhs)
    {
        return operations::matrix_operation<matrix_lazy<T>, RHS, T, operations::quotient<T>>(
            *this, rhs);
    }

    template<typename RHS>
    auto dot(const RHS &rhs)
    {
        return operations::matrix_operation<matrix_lazy<T>, RHS, T, operations::multiply<T>>(
            *this, rhs);
    }

    matrix_lazy<T> &operator+=(const matrix_lazy<T> &rhs) = delete;
    matrix_lazy<T> &operator-=(const matrix_lazy<T> &rhs) = delete;
    matrix_lazy<T> &operator*=(const matrix_lazy<T> &rhs) = delete;
    matrix_lazy<T> &operator/=(const matrix_lazy<T> &rhs) = delete;
};
} // namespace matrices
