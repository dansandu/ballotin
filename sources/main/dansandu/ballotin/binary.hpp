#pragma once

#include "dansandu/ballotin/exception.hpp"

#include <span>
#include <vector>

namespace dansandu::ballotin::binary
{

constexpr auto bitsPerByte = size_t{8};

constexpr auto getMask(const size_t bitsCount)
{
    return (size_t{1} << bitsCount) - size_t{1};
}

constexpr size_t numberOfBitsToNumberOfBytes(const size_t numberOfBits)
{
    return numberOfBits / bitsPerByte + (numberOfBits % bitsPerByte > 0);
}

PRALINE_EXPORT void pushBitsLeastSignificant(std::vector<uint8_t>& bytes, size_t& bitsCount, const size_t bitsToAppend,
                                             const size_t bitsToAppendCount);

PRALINE_EXPORT void pushBitsMostSignificant(std::vector<uint8_t>& bytes, size_t& bitsCount, const size_t bitsToAppend,
                                            const size_t bitsToAppendCount);

PRALINE_EXPORT size_t getMostSignificantBits(const std::span<const uint8_t> input, const size_t inputStartBitOffset,
                                             const size_t bitsCount);

}
