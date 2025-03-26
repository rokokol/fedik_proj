#pragma once
#include "array2d_meta.h"
#include <span>
#include <vector>

namespace linal {
template<typename T>
class array2d
{
    template<typename, typename, typename>
    friend class add_operation_array_2d;
    // private:
public:
    std::vector<T> data;
    std::vector<size_t> shape; // Есть ли какая-либо константная реализация?

public:
    array2d(std::span<size_t> shape) // Нормально ли передавать спан не по ссылке?
        : data(std::reduce(shape.begin(), shape.end(), 1ull, std::multiplies<>()))
        , shape(shape.begin(), shape.end())
    {}

    array2d(std::initializer_list<int> shape) // А почему инишилайзер лист не по ссылке?
        : data(std::reduce(shape.begin(), shape.end(), 1ull, std::multiplies<>()))
        , shape(shape.begin(), shape.end())
    {}

    array2d(std::initializer_list<std::initializer_list<T>> init)
    {
        shape.reserve(init.size());
        for (auto it = init.begin(); it != init.end(); ++it) {
            data.reserve(it->size());
            shape.push_back(it->size());
            for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
                data.push_back(*it2);
            }
        }

        data.shrink_to_fit();
    }

    array2d(array2d<T> &rhs) = default;
    array2d(const array2d<T> &rhs) = default; // Почему без него у меня не работает?

    array2d(array2d &&) noexcept = default;

    array2d<T> &operator=(const array2d<T> &rhs) = default;

    T &operator()(const size_t col, const size_t row)
    { // А вот вообще норм передавать в reduce 1ull
        auto step = std::reduce(shape.begin(), shape.begin() + col);
        return data[step + row];
    }

    template<typename RHS>
    auto operator+(const RHS &rhs) const
    {
        return add_operation_array_2d<array2d<T>, RHS, T>(*this, rhs);
    }
};
} // namespace linal
