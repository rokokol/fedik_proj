#pragma once
#include "lazy_tools.h"

namespace linal {
template<typename T>
class array2d;

template<typename LHS, typename RHS, typename T>
class add_operation_array_2d
{
private:
    const LHS &lhs;
    const RHS &rhs;

public:
    add_operation_array_2d(const LHS &lhs, const RHS &rhs)
        : lhs(lhs)
        , rhs(rhs)
    {}

    template<typename _RHS>
    auto operator+(const _RHS &rhs)
    {
        return add_operation_array_2d<add_operation_array_2d<LHS, RHS, T>, _RHS, T>(*this, rhs);
    }

    array2d<T> eval() const { return try_eval<T>(lhs) + try_eval<T>(rhs); }

    operator array2d<T>() const
    {
        array2d<T> arr1 = try_eval<T>(lhs);
        array2d<T> arr2 = try_eval<T>(rhs);

        for (int i = 0; i < arr1.data.size(); i++) {
            arr1.data[i] += arr2.data[i];
        }

        return arr1;
    }
};
} // namespace linal