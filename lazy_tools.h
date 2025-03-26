#pragma once

namespace linal {
template<typename ValueType>
concept Evaluatable = requires(ValueType value)
{
    value.eval();
};

template<typename T, typename P>
auto try_eval(const P &value)
{
    if constexpr (Evaluatable<T>)
        return value.eval();
    else
        return value;
}
} // namespace linal
