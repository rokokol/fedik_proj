#pragma once
#include <span>
#include <vector>

namespace arrays {
template<typename T>
class array2d
{
private:
    std::vector<T> data;
    std::vector<size_t> shape;

public:
    array2d(std::span<size_t> shape)
        : data(std::reduce(shape.begin(), shape.end()))
        , shape(shape.begin(), shape.end())
    {}

    array2d(const std::initializer_list<int> &shape)
        : data(std::reduce(shape.begin(), shape.end()))
        , shape(shape.begin(), shape.end())
    {}

    array2d(const std::initializer_list<std::initializer_list<T>> &init)
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

    array2d(const array2d<T> &rhs) = default;
    array2d(array2d &&) noexcept = default;
    array2d<T> &operator=(const array2d<T> &rhs) = default;

    T &operator()(const size_t col, const size_t row)
    {
        auto step = std::reduce(shape.begin(), shape.begin() + col);
        return data[step + row];
    }
};
} // namespace arrays
