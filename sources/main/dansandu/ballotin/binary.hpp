#pragma once

#include <cstdint>
#include <vector>

namespace dansandu::ballotin::binary
{

PRALINE_EXPORT void pushBits(std::vector<uint8_t>& bytes, int& bitsCount, const unsigned bitsToAppend,
                             const int bitsToAppendCount);

}
