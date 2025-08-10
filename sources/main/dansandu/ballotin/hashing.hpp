#pragma once

#include "dansandu/ballotin/binary.hpp"

#include <cstdint>
#include <string>
#include <type_traits>

namespace dansandu::ballotin::hashing
{

template<typename T>
uint32_t getHashCode32(const T& value)
{
    using dansandu::ballotin::binary::bitsPerByte;
    using ValueType = std::decay_t<T>;

    constexpr uint32_t offset = 0x811C9DC5u;
    constexpr uint32_t prime = 0x01000193u;

    uint32_t hashCode = offset;

    if constexpr (std::is_arithmetic_v<ValueType>)
    {
        for (size_t index = 0; index < sizeof(value); ++index)
        {
            hashCode ^= (value >> (bitsPerByte * index)) & 0xFF;
            hashCode *= prime;
        }
    }
    else if constexpr (std::is_enum_v<ValueType>)
    {
        using UnderlyingType = std::underlying_type_t<ValueType>;

        const auto actualValue = static_cast<UnderlyingType>(value);

        for (size_t index = 0; index < sizeof(UnderlyingType); ++index)
        {
            hashCode ^= (actualValue >> (bitsPerByte * index)) & 0xFF;
            hashCode *= prime;
        }
    }
    else if constexpr (std::is_same_v<ValueType, std::string>)
    {
        for (const auto character : value)
        {
            hashCode ^= character;
            hashCode *= prime;
        }
    }
    else
    {
        static_assert("Type not supported");
    }

    return hashCode;
}

template<typename T>
uint64_t getHashCode64(const T& value)
{
    using dansandu::ballotin::binary::bitsPerByte;
    using ValueType = std::decay_t<T>;

    constexpr uint64_t offset = 0xCBF29CE484222325ull;
    constexpr uint64_t prime = 0x00000100000001B3ull;

    uint64_t hashCode = offset;

    if constexpr (std::is_arithmetic_v<ValueType>)
    {
        for (size_t index = 0; index < sizeof(value); ++index)
        {
            hashCode ^= (value >> (bitsPerByte * index)) & 0xFF;
            hashCode *= prime;
        }
    }
    else if constexpr (std::is_enum_v<ValueType>)
    {
        using UnderlyingType = std::underlying_type_t<ValueType>;

        const auto actualValue = static_cast<UnderlyingType>(value);

        for (size_t index = 0; index < sizeof(UnderlyingType); ++index)
        {
            hashCode ^= (actualValue >> (bitsPerByte * index)) & 0xFF;
            hashCode *= prime;
        }
    }
    else if constexpr (std::is_same_v<ValueType, std::string>)
    {
        for (const auto character : value)
        {
            hashCode ^= character;
            hashCode *= prime;
        }
    }
    else
    {
        static_assert("Type not supported");
    }

    return hashCode;
}

inline uint32_t hashCombine(const uint32_t seedHash, const uint32_t otherHash)
{
    return seedHash ^ (otherHash + (seedHash << 6) + (seedHash >> 2) + 0x9E3779B9);
}

inline uint64_t hashCombine(const uint64_t seedHash, const uint64_t otherHash)
{
    return seedHash ^ (otherHash + (seedHash << 6) + (seedHash >> 2) + 0x9E3779B9);
}

}
