#include "dansandu/ballotin/binary.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <cstdint>
#include <vector>

using dansandu::ballotin::binary::bitsPerByte;
using dansandu::ballotin::binary::getMostSignificantBits;
using dansandu::ballotin::binary::pushBitsLeastSignificant;
using dansandu::ballotin::binary::pushBitsMostSignificant;

using BytesType = std::vector<uint8_t>;

TEST_CASE("binary")
{
    SECTION("push bits least significant")
    {
        SECTION("new byte partial write")
        {
            BytesType output = {};

            size_t bitsCount = 0;

            pushBitsLeastSignificant(output, bitsCount, 0b101U, 3);

            const BytesType expectedOutput = {0b00000101U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 1;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 3;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("previous byte parital write")
        {
            BytesType output = {0b00000101U};

            size_t bitsCount = 3;

            pushBitsLeastSignificant(output, bitsCount, 0b1100U, 4);

            const BytesType expectedOutput = {0b01100101U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 1;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 7;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("previous byte write extending to new byte")
        {
            BytesType output = {0b01100101U};

            size_t bitsCount = 7;

            pushBitsLeastSignificant(output, bitsCount, 0b10011U, 5);

            const BytesType expectedOutput = {0b11100101U, 0b00001001U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 2;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 12;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("previous byte fill")
        {
            BytesType output = {0b11100101U, 0b00001111U};

            size_t bitsCount = 12;

            pushBitsLeastSignificant(output, bitsCount, 0b0110, 4);

            const BytesType expectedOutput = {0b11100101U, 0b01101111U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 2;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 16;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("new byte fill")
        {
            BytesType output = {0b11100101U, 0b01111111U, 0b00101011U};

            size_t bitsCount = 24;

            pushBitsLeastSignificant(output, bitsCount, 0b00001100U, 8);

            const BytesType expectedOutput = {0b11100101U, 0b01111111U, 0b00101011U, 0b00001100U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 4;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 32;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("sequence")
        {
            const auto sequence = {32, 0, 34, 1, 4, 5, 6, 35, 33};

            const auto codeSize = 6;

            auto output = BytesType{};

            size_t bitsCount = 0;

            for (const auto code : sequence)
            {
                pushBitsLeastSignificant(output, bitsCount, code, codeSize);
            }

            const BytesType expectedOutput = {0b00100000U, 0b00100000U, 0b00000110U, 0b01000100U,
                                              0b01100001U, 0b10001100U, 0b00100001U};

            REQUIRE(output == expectedOutput);

            const auto expectedBitsCount = 54;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("large append")
        {
            BytesType output = {0b00000111};

            size_t bitsCount = 3;

            pushBitsLeastSignificant(output, bitsCount, 0b11111011110111011010, 20);

            const BytesType expectedOutput = {0b11010111, 0b11101110, 0b01111101};

            REQUIRE(output == expectedOutput);
        }

        SECTION("byte overflow")
        {
            BytesType output = {0b00110100};

            size_t bitsCount = 6;

            pushBitsLeastSignificant(output, bitsCount, 0b00000011, 2);

            const BytesType expectedOutput = {0b11110100};

            const auto expectedBitsCount = 8;

            REQUIRE(output == expectedOutput);

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("byte underflow")
        {
            BytesType output = {0b10101010};

            size_t bitsCount = 9;

            REQUIRE_THROW(std::invalid_argument, pushBitsLeastSignificant(output, bitsCount, 0b00110011, 6));
        }
    }

    SECTION("push bits most significat")
    {
        SECTION("large append")
        {
            BytesType output = {0b11100000};

            size_t bitsCount = 3;

            pushBitsMostSignificant(output, bitsCount, 0b01011011101111011111, 20);

            const BytesType expectedOutput = {0b11101011, 0b01110111, 0b10111110};

            REQUIRE(output == expectedOutput);
        }

        SECTION("multiple appends")
        {
            BytesType output = {};

            size_t bitsCount = 0;

            pushBitsMostSignificant(output, bitsCount, 0b010110, 6);

            pushBitsMostSignificant(output, bitsCount, 0b111011, 6);

            const BytesType expectedOutput = {0b01011011, 0b10110000};

            REQUIRE(bitsCount == 12);

            REQUIRE(output == expectedOutput);
        }
    }

    SECTION("get most significant bits")
    {
        const BytesType input = {0b10110111, 0b01111011, 0b11101111, 0b11010100};

        SECTION("no bits")
        {
            const auto offset = 0;

            const auto count = 0;

            const auto output = getMostSignificantBits(input, offset, count);

            REQUIRE(output == 0);
        }

        SECTION("bits [0, 5)")
        {
            const auto offset = 0;

            const auto count = 5;

            const auto output = getMostSignificantBits(input, offset, count);

            REQUIRE(output == 0b10110);
        }

        SECTION("bits [7, 20)")
        {
            const auto offset = 7;

            const auto count = 13;

            const auto output = getMostSignificantBits(input, offset, count);

            REQUIRE(output == 0b1011110111110);
        }

        SECTION("bits [13, 27)")
        {
            const auto offset = 13;

            const auto count = 14;

            const auto output = getMostSignificantBits(input, offset, count);

            REQUIRE(output == 0b01111101111110);
        }

        SECTION("all bits")
        {
            const auto offset = 0;

            const auto count = 32;

            const auto output = getMostSignificantBits(input, offset, count);

            REQUIRE(output == 0b10110111011110111110111111010100);
        }

        SECTION("throws if bit count overflows input")
        {
            const auto offset = 2;

            const auto count = 31;

            REQUIRE_THROW(std::invalid_argument, getMostSignificantBits(input, offset, count));
        }

        SECTION("throws if bit count overflows output")
        {
            constexpr auto size = sizeof(getMostSignificantBits(input, 0, 0));

            const auto largeInput = BytesType(size + 1);

            const auto offset = 0;

            const auto count = largeInput.size() * bitsPerByte;

            REQUIRE_THROW(std::invalid_argument, getMostSignificantBits(largeInput, offset, count));
        }
    }
}
