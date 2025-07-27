#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

namespace dansandu::ballotin::hashing
{

template<typename T>
uint64_t hash(const T& value)
{
    using ValueType = std::decay_t<T>;

    constexpr uint64_t offset = 0xCBF29CE484222325ull;
    constexpr uint64_t prime = 0x00000100000001B3ull;
    constexpr size_t bitsPerByte = 8;

    uint64_t result = offset;

    if constexpr (std::is_arithmetic_v<ValueType>)
    {
        for (size_t index = 0; index < sizeof(value); ++index)
        {
            result ^= (value >> (bitsPerByte * index)) & 0xFF;
            result *= prime;
        }
    }
    else if constexpr (std::is_enum_v<ValueType>)
    {
        using UnderlyingType = std::underlying_type_t<ValueType>;

        const auto actualValue = static_cast<UnderlyingType>(value);

        for (size_t index = 0; index < sizeof(UnderlyingType); ++index)
        {
            result ^= (actualValue >> (bitsPerByte * index)) & 0xFF;
            result *= prime;
        }
    }
    else if constexpr (std::is_same_v<ValueType, std::string>)
    {
        for (const auto character : value)
        {
            result ^= character;
            result *= prime;
        }
    }

    return result;
}

inline uint64_t hashCombine(const uint64_t seedHash, const uint64_t otherHash)
{
    return seedHash ^ (otherHash + (seedHash << 6) + (seedHash >> 2) + 0x9E3779B9);
}

}
