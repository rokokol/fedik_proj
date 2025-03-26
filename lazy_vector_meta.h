#pragma once

namespace linal {
template<typename T>
class vector_lazy;
template<typename LHS, typename RHS, typename T>
class dot_operation_vector;

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

    template<typename _RHS>
    auto operator*(const _RHS &rhs) const
    {
        return dot_operation_vector<add_operation_vector<LHS, RHS, T>, _RHS, T>(*this, rhs);
    }

    vector_lazy<T> eval() const
    {
        return try_eval<T>(lhs) + try_eval<T>(rhs);
    }

    operator vector_lazy<T>() const
    {
        vector_lazy<T> v1 = try_eval<T>(lhs);
        vector_lazy<T> v2 = try_eval<T>(rhs);

        auto size = std::max(v1.size, v2.size);
        for (int i = 0; i < size; i++) {
            v2.data[i % v2.size] += v1.data[i % v1.size];
        }

        return v2;
    }
}; // add

template<typename LHS, typename RHS, typename T>
class dot_operation_vector
{
private:
    const LHS &lhs;
    const RHS &rhs;

public:
    dot_operation_vector(const LHS &lhs, const RHS &rhs)
        : lhs(lhs)
        , rhs(rhs)
    {}

    template<typename _RHS>
    auto operator+(const _RHS &rhs) const
    {
        return add_operation_vector<dot_operation_vector<LHS, RHS, T>, _RHS, T>(*this, rhs);
    }

    template<typename _RHS>
    auto operator*(const _RHS &rhs) const
    {
        return dot_operation_vector<dot_operation_vector<LHS, RHS, T>, _RHS, T>(*this, rhs);
    }

    T eval() const
    {
        vector_lazy<T> v1 = try_eval<T>(lhs);
        vector_lazy<T> v2 = try_eval<T>(rhs);

        T result = 0;
        for (int i = 0; i < v1.size; i++) {
            result += v1.data[i] * v2.data[i];
        }

        return result;
    }

    operator vector_lazy<T>() const
    {
        T result = eval();
        return vector_lazy<T>{result};
    }
}; // add

} // namespace linal