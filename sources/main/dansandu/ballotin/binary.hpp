#pragma once

#include "dansandu/journey/exception.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace dansandu::ballotin::binary
{

constexpr auto bitsPerByte = 8uz;

constexpr auto getMask(const size_t bitsCount)
{
    return bitsCount < sizeof(size_t) * bitsPerByte ? (1uz << bitsCount) - 1uz : ~0uz;
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
