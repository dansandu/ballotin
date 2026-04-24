#include "dansandu/ballotin/binary.hpp"
#include "dansandu/journey/exception.hpp"

#include <cstdint>
#include <vector>

namespace dansandu::ballotin::binary
{

void pushBitsLeastSignificant(std::vector<uint8_t>& bytes, size_t& bitsCount, const size_t bitsToAppend,
                              const size_t bitsToAppendCount)
{
    const auto targetBitsCount = bitsCount + bitsToAppendCount;

    const auto targetBytesCount = targetBitsCount / bitsPerByte + (0 < targetBitsCount % bitsPerByte);

    const auto currentBytesCount = bitsCount / bitsPerByte + (0 < bitsCount % bitsPerByte);

    if (currentBytesCount != bytes.size())
    {
        THROW(std::invalid_argument, "bits count ", bitsCount, " does not match number of bytes ", bytes.size());
    }

    constexpr auto maximumBitsToAppendCount = sizeof(bitsToAppend) * bitsPerByte;

    if (bitsToAppendCount > maximumBitsToAppendCount)
    {
        THROW(std::invalid_argument, "bits to append count ", bitsToAppendCount, " cannot exceed ",
              maximumBitsToAppendCount);
    }

    while (bytes.size() < targetBytesCount)
    {
        bytes.push_back(0);
    }

    auto remainingBits = bitsToAppend;
    auto remainingBitsCount = bitsToAppendCount;

    while (remainingBitsCount > 0)
    {
        const auto byteIndex = bitsCount / bitsPerByte;
        const auto freeBitsCount = bitsPerByte - bitsCount % bitsPerByte;
        const auto bitsToPushCount = std::min(freeBitsCount, remainingBitsCount);
        const auto mask = getMask(bitsToPushCount);

        bytes[byteIndex] |= (remainingBits & mask) << (bitsCount % bitsPerByte);
        remainingBits >>= bitsToPushCount;
        remainingBitsCount -= bitsToPushCount;
        bitsCount += bitsToPushCount;
    }
}

void pushBitsMostSignificant(std::vector<uint8_t>& bytes, size_t& bitsCount, const size_t bitsToAppend,
                             const size_t bitsToAppendCount)
{
    const auto targetBitsCount = bitsCount + bitsToAppendCount;

    const auto targetBytesCount = targetBitsCount / bitsPerByte + (0 < targetBitsCount % bitsPerByte);

    const auto currentBytesCount = bitsCount / bitsPerByte + (0 < bitsCount % bitsPerByte);

    if (currentBytesCount != bytes.size())
    {
        THROW(std::invalid_argument, "bits count ", bitsCount, " does not match number of bytes ", bytes.size());
    }

    constexpr auto maximumBitsToAppendCount = sizeof(bitsToAppend) * bitsPerByte;

    if (bitsToAppendCount > maximumBitsToAppendCount)
    {
        THROW(std::invalid_argument, "bits to append count ", bitsToAppendCount, " cannot exceed ",
              maximumBitsToAppendCount);
    }

    while (bytes.size() < targetBytesCount)
    {
        bytes.push_back(0);
    }

    auto appendOffset = size_t{0};

    while (appendOffset < bitsToAppendCount)
    {
        const auto byteIndex = bitsCount / bitsPerByte;
        const auto remainingBitsCount = bitsToAppendCount - appendOffset;
        const auto freeBitsCount = bitsPerByte - bitsCount % bitsPerByte;
        const auto currentBitsToAppendCount = std::min(freeBitsCount, remainingBitsCount);
        const auto mask = getMask(currentBitsToAppendCount);
        const auto appendShift = bitsToAppendCount - appendOffset - currentBitsToAppendCount;
        const auto byteShift = freeBitsCount - currentBitsToAppendCount;
        const auto append = (bitsToAppend >> appendShift) & mask;

        bytes[byteIndex] |= (append << byteShift);
        appendOffset += currentBitsToAppendCount;
        bitsCount += currentBitsToAppendCount;
    }
}

size_t getMostSignificantBits(const std::span<const uint8_t> input, const size_t inputStartBitOffset,
                              const size_t bitsCount)
{
    constexpr auto bitsPerInputElement = sizeof(*input.cbegin()) * bitsPerByte;
    constexpr auto bitsPerOutput = sizeof(size_t) * bitsPerByte;

    const auto inputBitsCount = input.size() * bitsPerInputElement;

    if (inputBitsCount < inputStartBitOffset + bitsCount)
    {
        THROW(std::invalid_argument, "the number of bits in the input ", inputBitsCount,
              " is smaller than the requested bits with offset ", inputStartBitOffset, " and bits count ", bitsCount);
    }

    if (bitsCount > bitsPerOutput)
    {
        THROW(std::invalid_argument, "bits count exceeds the output bit count ", bitsCount);
    }

    auto output = size_t{0};
    auto outputBitOffset = size_t{0};

    while (outputBitOffset < bitsCount)
    {
        const auto inputBitOffset = inputStartBitOffset + outputBitOffset;
        const auto inputIndex = inputBitOffset / bitsPerInputElement;
        const auto remainingBitsCount = bitsCount - outputBitOffset;
        const auto availableBitsCount = bitsPerInputElement - inputBitOffset % bitsPerInputElement;
        const auto bitsToConsumeCount = std::min(availableBitsCount, remainingBitsCount);
        const auto inputMask = getMask(bitsToConsumeCount);
        const auto inputShift = availableBitsCount - bitsToConsumeCount;
        const auto outputShift = bitsCount - outputBitOffset - bitsToConsumeCount;
        const auto consumedBits = (input[inputIndex] >> inputShift) & inputMask;
        output |= (consumedBits << outputShift);
        outputBitOffset += bitsToConsumeCount;
    }

    return output;
}

}
