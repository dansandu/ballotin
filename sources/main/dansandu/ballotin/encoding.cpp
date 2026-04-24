#include "dansandu/ballotin/encoding.hpp"
#include "dansandu/ballotin/binary.hpp"
#include "dansandu/journey/exception.hpp"

#include <array>
#include <vector>

using dansandu::ballotin::binary::bitsPerByte;
using dansandu::ballotin::binary::getMostSignificantBits;
using dansandu::ballotin::binary::pushBitsMostSignificant;

namespace dansandu::ballotin::encoding
{

namespace
{

constexpr auto base64PaddingCharacter = '=';

constexpr auto base64BitsPerCharacter = size_t{6};

constexpr void fillCharaters(const uint8_t start, const uint8_t end, std::array<uint8_t, 64>& output, size_t& offset)
{
    const auto charactersCount = static_cast<int>(end - start + 1);
    for (auto code = 0; code < charactersCount; ++code)
    {
        output[offset++] = code + start;
    }
}

constexpr std::array<uint8_t, 64> generateBase64Characters()
{
    std::array<uint8_t, 64> characters;
    size_t offset = 0;

    fillCharaters('A', 'Z', characters, offset);
    fillCharaters('a', 'z', characters, offset);
    fillCharaters('0', '9', characters, offset);

    characters[62] = '-';
    characters[63] = '_';

    return characters;
}

}

const std::array<uint8_t, 64>& getBase64Characters()
{
    static constexpr auto base64Characters = generateBase64Characters();
    return base64Characters;
}

std::vector<uint8_t> encode64(const std::vector<uint8_t>& bytes)
{
    const auto inputBitsCount = bytes.size() * bitsPerByte;
    const auto zeroPadding =
        (base64BitsPerCharacter - inputBitsCount % base64BitsPerCharacter) % base64BitsPerCharacter;
    const auto characterPadding = zeroPadding / size_t{2};

    auto encoded = std::vector<uint8_t>{};
    auto bitOffset = size_t{0};

    while (bitOffset < inputBitsCount)
    {
        const auto bitsToRead = std::min(inputBitsCount - bitOffset, base64BitsPerCharacter);
        const auto binary = getMostSignificantBits(bytes, bitOffset, bitsToRead);
        const auto shifted = binary << (base64BitsPerCharacter - bitsToRead);
        const auto code = getBase64Characters()[shifted];
        encoded.push_back(code);
        bitOffset += bitsToRead;
    }

    for (auto index = size_t{0}; index < characterPadding; ++index)
    {
        encoded.push_back(base64PaddingCharacter);
    }

    return encoded;
}

std::vector<uint8_t> decode64(const std::vector<uint8_t>& bytes)
{
    auto decoded = std::vector<uint8_t>{};
    auto bitsCount = size_t{0};

    const auto characterPadding = std::count(bytes.cbegin(), bytes.cend(), base64PaddingCharacter);
    const auto zeroPadding = size_t{2} * characterPadding;
    const auto actualSize = bytes.size() - characterPadding;

    for (auto index = size_t{0}; index < actualSize; ++index)
    {
        const auto code = static_cast<int>(bytes[index]);

        auto offset = (('A' <= code) & (code <= 'Z')) * (code - 'A' + 1) +
                      (('a' <= code) & (code <= 'z')) * (code - 'a' + 27) +
                      (('0' <= code) & (code <= '9')) * (code - '0' + 53) + (code == '-') * 63 + (code == '_') * 64 - 1;

        if (offset < 0)
        {
            THROW(std::invalid_argument, "invalid character '", code, "' in base64 encoding");
        }

        const auto lastIteration = (index + size_t{1} == actualSize);
        const auto bitsToTake = base64BitsPerCharacter - zeroPadding * lastIteration;
        offset >>= zeroPadding * lastIteration;

        pushBitsMostSignificant(decoded, bitsCount, offset, bitsToTake);
    }

    return decoded;
}

std::string codeToString(const std::vector<uint8_t>& bytes)
{
    return std::string(bytes.cbegin(), bytes.cend());
}

std::vector<uint8_t> stringToCode(const std::string& string)
{
    return std::vector<uint8_t>(string.cbegin(), string.cend());
}

}
