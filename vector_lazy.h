#pragma once
#include <initializer_list>
#include <vector>

#include "lazy_tools.h"
#include "lazy_vector_meta.h"

namespace linal {
template<typename T = double>
class vector_lazy
{
    template<typename, typename, typename>
    friend class add_operation_vector;

    template<typename, typename, typename>
    friend class dot_operation_vector;

private:
    std::vector<T> data;

public:
    size_t const size;

    vector_lazy(std::initializer_list<T> init)
        : size(init.size())
        , data(init)
    {}

    explicit vector_lazy(const size_t size)
        : size(size)
        , data(size)
    {}

    vector_lazy(const vector_lazy &other)
        : size(other.size)
    {
        data = other.data;
    }

    vector_lazy(vector_lazy &&other) noexcept
        : size(other.size)
    {
        data = std::move(other.data);
    }

    T &operator[](size_t i) { return this->data[i]; } // Норм делать вот так вот 2 штуки?
    T &operator[](size_t i) const { return this->data[i]; }

    vector_lazy &operator=(const vector_lazy &other)
    {
        data = other.data;
        return *this;
    }

    template<typename RHS>
    auto operator+(const RHS &rhs) const
    {
        return add_operation_vector<vector_lazy<T>, RHS, T>(*this, rhs);
    }

    template<typename RHS>
    auto operator*(const RHS &rhs) const
    {
        return dot_operation_vector<vector_lazy<T>, RHS, T>(*this, rhs);
    }
};

} // namespace linal
