#pragma once
#include <cmath>
#include <cstddef>
#include <initializer_list>

namespace linal
{
template <typename T = double> class vector
{
  private:
    T *data;

  public:
    size_t const size;

    vector(std::initializer_list<T> init) : size(init.size())
    {
        data = new T[size];
        size_t i = 0;
        for (auto &&value : init)
        {
            data[i++] = value; // std::views::enumerate
        }
    }

    explicit vector(const size_t size) : size(size), data(new T[size])
    {
    }

    vector(const vector &other) : size(other.size), data(new T[other.size])
    {
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    vector(vector &&other) noexcept : size(other.size), data(new T[other.size])
    {
        for (size_t i = 0; i < size; i++)
        {
            data[i] = other.data[i];
        }
    }

    T &operator[](size_t i)
    {
        return this->data[i];
    }

    T &operator[](size_t i) const
    {
        return this->data[i];
    }

    vector<T> &operator*=(const vector<T> &rhs)
    {
        for (size_t i = 0; i < this->size; i++)
        {
            this->data[i] *= rhs.data[i];
        }

        return *this;
    }

    vector<T> &operator+=(const vector<T> &rhs)
    {
        for (size_t i = 0; i < this->size; i++)
        {
            this->data[i] += rhs.data[i];
        }

        return *this;
    }

    vector<T> operator+(const vector<T> &rhs)
    {
        vector<T> tmp(rhs.size);
        for (size_t i = 0; i < this->size; i++)
        {
            tmp[i] = this->data[i] + rhs.data[i];
        }

        return tmp;
    }

    vector<T> &operator-=(const vector<T> &rhs)
    {
        for (size_t i = 0; i < this->size; i++)
        {
            this->data[i] -= rhs.data[i];
        }

        return *this;
    }

    vector<T> operator-(const vector<T> &rhs)
    {
        vector<T> tmp(rhs.size);
        for (size_t i = 0; i < this->size; i++)
        {
            tmp[i] = this->data[i] - rhs.data[i];
        }

        return tmp;
    }

    vector<T> operator*(const vector<T> &rhs)
    {
        vector<T> tmp(rhs.size);
        for (size_t i = 0; i < this->size; i++)
        {
            tmp[i] = this->data[i] * rhs.data[i];
        }

        return tmp;
    }

    vector<T> &operator=(const vector<T> other)
    {
        for (size_t i = 0; i < this->size; i++)
        {
            this->data[i] = other.data[i];
        }

        return *this;
    }

    double dot(vector<T> const &rhs)
    {
        double result = 0;
        for (int i = 0; i < this->size; i++)
        {
            result += this->data[i] * rhs.data[i];
        }

        return result;
    }

    double norm()
    {
        return std::sqrt(dot(*this));
    }
};
} // namespace linal
