#pragma once

namespace lazy {
template<typename ValueType>
concept Evaluatable = requires(ValueType value)
{
    value.eval();
};

template<typename T>
auto try_eval(const T &value)
{
    if constexpr (Evaluatable<T>)
        return value.eval();
    else
        return value;
}
} // namespace lazy
