#pragma once

namespace linal {
template<typename T>
class vector_lazy;

template<typename LHS, typename RHS, typename T>
class add_operation_vector
{
private:
    const LHS &lhs;
    const RHS &rhs;

public:
    add_operation_vector(const LHS &lhs, const RHS &rhs)
        : lhs(lhs)
        , rhs(rhs)
    {}

    template<typename _RHS>
    auto operator+(const _RHS &rhs) const
    {
        return add_operation_vector<add_operation_vector<LHS, RHS, T>, _RHS, T>(*this, rhs);
    }

    vector_lazy<T> eval() const
    {
        return vector_lazy<T>::template try_eval(lhs) + vector_lazy<T>::template try_eval(rhs);
    }

    operator vector_lazy<T>() const
    {
        auto v1 = vector_lazy<T>::template try_eval(lhs);
        auto v2 = vector_lazy<T>::template try_eval(rhs);

        for (int i = 0; i < v1.size; i++) {
            v2.data[i] += v1.data[i];
        }

        return v2;
    }
};
} // namespace linal