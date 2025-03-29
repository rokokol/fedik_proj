#pragma once
#include "lazy_tools.h"
#include "matrix_operations.h"

namespace matrices::operations {
template<typename LHS, typename RHS, typename T, typename Op>
class matrix_operation
{
private:
    const LHS &lhs;
    const RHS &rhs;
    const Op op{};

public:
    matrix_operation(const LHS &lhs, const RHS &rhs)
        : lhs(lhs)
        , rhs(rhs)
    {}

    template<typename _RHS>
    auto operator+(const _RHS &rhs)
    {
        return matrix_operation<matrix_operation<LHS, RHS, T, Op>, _RHS, T, sum<T>>(*this, rhs);
    }

    template<typename _RHS>
    auto operator-(const _RHS &rhs)
    {
        return matrix_operation<matrix_operation<LHS, RHS, T, Op>, _RHS, T, difference<T>>(*this, rhs);
    }

    template<typename _RHS>
    auto operator*(const _RHS &rhs)
    {
        return matrix_operation<matrix_operation<LHS, RHS, T, Op>, _RHS, T, product<T>>(*this, rhs);
    }

    template<typename _RHS>
    auto operator/(const _RHS &rhs)
    {
        return matrix_operation<matrix_operation<LHS, RHS, T, Op>, _RHS, T, quotient<T>>(*this, rhs);
    }

    template<typename _RHS>
    auto dot(const _RHS &rhs)
    {
        return matrix_operation<matrix_operation<LHS, RHS, T, Op>, _RHS, T, multiply<T>>(*this, rhs);
    }

    matrix_lazy<T> eval() const { return op(lazy::try_eval(lhs), lazy::try_eval(rhs)); }

    operator matrix_lazy<T>() const
    {
        auto m1 = lazy::try_eval(lhs);
        auto m2 = lazy::try_eval(rhs);

        return op(m1, m2);
    }
};
} // namespace matrices::operations
