#include "dansandu/ballotin/binary.hpp"
#include "dansandu/ballotin/exception.hpp"

#include <cstdint>
#include <vector>

namespace dansandu::ballotin::binary
{

void pushBits(std::vector<uint8_t>& bytes, int& bitsCount, const unsigned bitsToAppend, const int bitsToAppendCount)
{
    constexpr auto bitsPerByte = 8;

    const auto bytesCount = bitsCount / bitsPerByte + (0 < bitsCount % bitsPerByte);

    if (static_cast<int>(bytes.size()) < bytesCount)
    {
        THROW(std::invalid_argument, "invalid bit count ", bitsCount,
              " -- bit count cannot be larger than current bytes size", bytes.size());
    }

    while (static_cast<int>(bytes.size()) > bytesCount)
    {
        bytes.pop_back();
    }

    const auto bitsCapacity = bytesCount * bitsPerByte;

    auto remainingBits = bitsToAppend;
    auto remainingBitsCount = bitsToAppendCount;

    while (remainingBitsCount > 0)
    {
        const auto freeBitsCount = bitsCapacity - bitsCount;

        if (const auto implaceBitsCount = std::min(freeBitsCount, remainingBitsCount); implaceBitsCount > 0)
        {
            const auto implaceMask = (1U << implaceBitsCount) - 1U;
            bytes.back() |= (remainingBits & implaceMask) << (bitsPerByte - freeBitsCount);

            remainingBits >>= implaceBitsCount;
            remainingBitsCount -= implaceBitsCount;
            bitsCount += implaceBitsCount;
        }

        if (const auto extendedBitsCount = std::min(bitsPerByte, remainingBitsCount); extendedBitsCount > 0)
        {
            const auto extendedMask = (1U << extendedBitsCount) - 1U;
            bytes.push_back(remainingBits & extendedMask);

            remainingBits >>= extendedBitsCount;
            remainingBitsCount -= extendedBitsCount;
            bitsCount += extendedBitsCount;
        }
    }
}

}
