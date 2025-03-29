#pragma once
#include <functional>
#include <stdexcept>

namespace matrices {
template<typename T>
class matrix;

template<typename T>
class matrix_lazy;

namespace operations {
template<typename T>
matrix<T> elementwise(const matrix<T> &lhs, const matrix<T> &rhs, std::function<T(T, T)> op)
{
    if (lhs.cols != rhs.cols || lhs.rows != rhs.rows) {
        throw std::invalid_argument("lhs and rhs must be the same size");
    }

    matrix<T> result(lhs.rows, lhs.cols);
    for (size_t i = 0; i < lhs.data.size(); i++) {
        result.data[i] = op(lhs.data[i], rhs.data[i]);
    }

    return result;
}

template<typename T>
struct action
{
    virtual ~action() = default;
    virtual matrix<T> operator()(const matrix<T> &lhs, const matrix<T> &rhs) const = 0;
};

template<typename T>
struct multiply final : action<T>
{
    matrix<T> operator()(const matrix<T> &lhs, const matrix<T> &rhs) const override
    {
        if (lhs.cols != rhs.rows) {
            throw std::invalid_argument("lhs and rhs must be the same size");
        }

        matrix<T> result(lhs.rows, rhs.cols);

        for (size_t i = 0; i < lhs.rows; i++) {
            for (size_t j = 0; j < lhs.cols; j++) {
                for (size_t k = 0; k < rhs.cols; k++) {
                    result(i, k) += lhs(i, k) * rhs(k, j);
                }
            }
        }

        return result;
    }
};

template<typename T>
struct sum final : action<T>
{
    matrix<T> operator()(const matrix<T> &lhs, const matrix<T> &rhs) const override
    {
        return elementwise(lhs, rhs, std::function<T(T, T)>(std::plus<T>()));
    }
};

template<typename T>
struct difference final : action<T>
{
    matrix<T> operator()(const matrix<T> &lhs, const matrix<T> &rhs) const override
    {
        return elementwise(lhs, rhs, std::function<T(T, T)>(std::minus<T>()));
    }
};

template<typename T>
struct product final : action<T>
{
    matrix<T> operator()(const matrix<T> &lhs, const matrix<T> &rhs) const override
    {
        return elementwise(lhs, rhs, std::function<T(T, T)>(std::multiplies<T>()));
    }
};

template<typename T>
struct quotient final : action<T>
{
    matrix<T> operator()(const matrix<T> &lhs, const matrix<T> &rhs) const override
    {
        return elementwise(lhs, rhs, std::function<T(T, T)>(std::divides<T>()));
    }
};
} // namespace operations
} // namespace matrices