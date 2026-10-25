#include "dansandu/ballotin/binary.hpp"
#include "dansandu/journey/exception.hpp"

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace dansandu::ballotin::binary
{

constexpr auto bitsPerSize = sizeof(size_t) * bitsPerByte;

static void reserveBits(std::vector<uint8_t>& bytes, const size_t bitsCount, const size_t bitsToAppendCount)
{
    if (numberOfBitsToNumberOfBytes(bitsCount) != bytes.size())
    {
        THROW(std::invalid_argument, "bits count ", bitsCount, " does not match number of bytes ", bytes.size());
    }

    if (bitsToAppendCount > bitsPerSize)
    {
        THROW(std::invalid_argument, "bits to append count ", bitsToAppendCount, " cannot exceed ", bitsPerSize);
    }

    bytes.resize(numberOfBitsToNumberOfBytes(bitsCount + bitsToAppendCount));
}

void pushBitsLeastSignificant(std::vector<uint8_t>& bytes, size_t& bitsCount, const size_t bitsToAppend,
                              const size_t bitsToAppendCount)
{
    reserveBits(bytes, bitsCount, bitsToAppendCount);

    auto remainingBits = bitsToAppend & getMask(bitsToAppendCount);
    auto usedBitsCount = bitsCount % bitsPerByte;

    for (auto byteIndex = bitsCount / bitsPerByte; byteIndex < bytes.size(); ++byteIndex)
    {
        bytes[byteIndex] |= static_cast<uint8_t>(remainingBits << usedBitsCount);
        remainingBits >>= bitsPerByte - usedBitsCount;
        usedBitsCount = 0;
    }

    bitsCount += bitsToAppendCount;
}

void pushBitsMostSignificant(std::vector<uint8_t>& bytes, size_t& bitsCount, const size_t bitsToAppend,
                             const size_t bitsToAppendCount)
{
    reserveBits(bytes, bitsCount, bitsToAppendCount);

    if (bitsToAppendCount == 0)
    {
        return;
    }

    const auto bits = bitsToAppend & getMask(bitsToAppendCount);
    auto byteIndex = bitsCount / bitsPerByte;
    auto freeBitsCount = bitsPerByte - bitsCount % bitsPerByte;
    auto remainingBitsCount = bitsToAppendCount;

    while (remainingBitsCount > freeBitsCount)
    {
        remainingBitsCount -= freeBitsCount;
        bytes[byteIndex++] |= static_cast<uint8_t>(bits >> remainingBitsCount);
        freeBitsCount = bitsPerByte;
    }

    bytes[byteIndex] |= static_cast<uint8_t>(bits << (freeBitsCount - remainingBitsCount));

    bitsCount += bitsToAppendCount;
}

size_t getMostSignificantBits(const std::span<const uint8_t> input, const size_t inputStartBitOffset,
                              const size_t bitsCount)
{
    const auto inputBitsCount = input.size() * bitsPerByte;

    if (inputBitsCount < inputStartBitOffset + bitsCount)
    {
        THROW(std::invalid_argument, "the number of bits in the input ", inputBitsCount,
              " is smaller than the requested bits with offset ", inputStartBitOffset, " and bits count ", bitsCount);
    }

    if (bitsCount > bitsPerSize)
    {
        THROW(std::invalid_argument, "bits count exceeds the output bit count ", bitsCount);
    }

    const auto inputEndBitOffset = inputStartBitOffset + bitsCount;
    const auto endByteIndex = inputEndBitOffset / bitsPerByte;
    const auto trailingBitsCount = inputEndBitOffset % bitsPerByte;

    // Bits preceding the start offset are either shifted out or masked away at the end.
    auto output = 0uz;

    for (auto byteIndex = inputStartBitOffset / bitsPerByte; byteIndex < endByteIndex; ++byteIndex)
    {
        output = (output << bitsPerByte) | input[byteIndex];
    }

    if (trailingBitsCount > 0)
    {
        output = (output << trailingBitsCount) | (input[endByteIndex] >> (bitsPerByte - trailingBitsCount));
    }

    return output & getMask(bitsCount);
}

}
