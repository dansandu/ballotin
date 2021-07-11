#include "dansandu/ballotin/binary.hpp"

#include <cstdint>
#include <vector>

namespace dansandu::ballotin::binary
{

void pushBits(std::vector<uint8_t>& bytes, int& bitsCount, const unsigned bitsToAppend, const int bitsToAppendCount)
{
    constexpr auto bitsPerByte = 8;

    auto remainingBits = bitsToAppend;
    auto remainingBitsCount = bitsToAppendCount;

    while (remainingBitsCount > 0)
    {
        const auto freeBitsCount = static_cast<int>(bytes.size()) * bitsPerByte - bitsCount;

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
