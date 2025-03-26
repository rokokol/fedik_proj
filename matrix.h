#pragma once
#include <cstddef>
#include <initializer_list>

#include "vector.h"

namespace linal
{
template <typename T> class matrix
{
  private:
    T *data;

  public:
    size_t const rows, cols;

    matrix(size_t rows, size_t cols) : rows(rows), cols(cols), data(new T[rows * cols])
    {
    }

    matrix(std::initializer_list<std::initializer_list<T>> init) : rows(init.size()), cols(init.begin()->size())
    {
        this->data = new T[rows * cols];
        size_t i = 0, j = 0;
        for (auto row = init.begin(); row != init.end(); ++row)
        {
            for (auto col = row->begin(); col != row->end(); ++col)
            {
                this->data[i * cols + j++] = *col; // std::distance
            }
            ++i;
            j = 0;
        }
    }

    matrix(matrix<T> &rhs) : rows(rhs.rows), cols(rhs.cols)
    {
        this->data = new T[rows * cols];
        std::copy(rhs.data, rhs.data + rows * cols, this->data);
    }

    matrix(matrix<T> &&rhs) noexcept : rows(rhs.rows), cols(rhs.cols)
    {
        this->data = new T[rows * cols];
        std::copy(rhs.data, rhs.data + rows * cols, this->data);
    }

    matrix<T> &operator=(const matrix<T> &rhs)
    {
        std::copy(rhs.data, rhs.data + rows * cols, this->data);
    }

    T &operator()(int row, int col)
    {
        return this->data[row * cols + col];
    }

    T &operator()(int row, int col) const
    {
        return this->data[row * cols + col];
    }

    matrix<T> &operator+=(const matrix<T> &rhs)
    {
        for (size_t i = 0; i < rows * cols; ++i)
        {
            this->data[i] += rhs.data[i];
        }

        return *this;
    }

    matrix<T> operator+(const matrix<T> &rhs)
    {
        matrix<T> result(rows, rhs.cols);

        for (size_t i = 0; i < rows * cols; ++i)
        {
            result.data[i] = this->data[i] + rhs.data[i];
        }

        return result;
    }

    matrix<T> &operator-=(const matrix<T> &rhs)
    {
        for (size_t i = 0; i < rows * cols; ++i)
        {
            this->data[i] -= rhs.data[i];
        }

        return *this;
    }

    matrix<T> operator-(const matrix<T> &rhs)
    {
        matrix<T> result(rows, rhs.cols);

        for (size_t i = 0; i < rows * cols; ++i)
        {
            result.data[i] = this->data[i] - rhs.data[i];
        }

        return result;
    }

    vector<T> operator*(const vector<T> &rhs)
    {
        vector<T> result(cols);
        for (size_t i = 0; i < rows * cols; ++i)
        {                                                      // std::span std::mdspan
            result[i % cols] += this->data[i] * rhs[i % rows]; // std::reduce std::accumulate
        }

        return result;
    }

    matrix<T> operator*(const matrix<T> &rhs)
    {
        matrix<T> result(rows, rhs.cols);
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                double temp = (*this)(i, j);
                for (size_t k = 0; k < rhs.cols; ++k)
                {
                    result(i, k) += temp * rhs(j, k);
                }
            }
        }

        return result;
    }
};
} // namespace linal
