#pragma once

#include <functional>

namespace dansandu::ballotin::hash
{

template<typename ValueType>
auto hashCombine(std::size_t seed, const ValueType& value)
{
    using std::hash;
    return seed ^ (hash<ValueType>{}(value) + (seed << 6) + (seed >> 2) + 0x9e3779b9);
}

}
