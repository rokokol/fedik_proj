#pragma once

namespace linal {
template<typename ValueType>
concept Evaluatable = requires(ValueType value)
{
    value.eval();
};
} // namespace linal
