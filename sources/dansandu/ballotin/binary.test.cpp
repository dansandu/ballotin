#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/binary.hpp"

#include <cstdint>
#include <vector>

using dansandu::ballotin::binary::pushBits;

using bytes_type = std::vector<uint8_t>;

TEST_CASE("binary")
{
    SECTION("push bits")
    {
        SECTION("new byte partial write")
        {
            bytes_type output = {};

            auto bitsCount = 0;

            pushBits(output, bitsCount, 0b101U, 3);

            const bytes_type expectedOutput = {0b00000101U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 1;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 3;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("previous byte parital write")
        {
            bytes_type output = {0b00000101U};

            auto bitsCount = 3;

            pushBits(output, bitsCount, 0b1100U, 4);

            const bytes_type expectedOutput = {0b01100101U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 1;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 7;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("previous byte write extending to new byte")
        {
            bytes_type output = {0b01100101U};

            auto bitsCount = 7;

            pushBits(output, bitsCount, 0b10011U, 5);

            const bytes_type expectedOutput = {0b11100101U, 0b00001001U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 2;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 12;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("previous byte fill")
        {
            bytes_type output = {0b11100101U, 0b00001111U};

            auto bitsCount = 12;

            pushBits(output, bitsCount, 0b0110, 4);

            const bytes_type expectedOutput = {0b11100101U, 0b01101111U};

            REQUIRE(output == expectedOutput);

            const auto expectedByteCount = 2;

            REQUIRE(output.size() == expectedByteCount);

            const auto expectedBitsCount = 16;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("new byte fill")
        {
            bytes_type output = {0b11100101U, 0b01111111U, 0b00101011U};

            auto bitsCount = 24;

            pushBits(output, bitsCount, 0b00001100U, 8);

            const bytes_type expectedOutput = {0b11100101U, 0b01111111U, 0b00101011U, 0b00001100U};

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
            auto output = bytes_type{};
            auto bitsCount = 0;

            for (const auto code : sequence)
            {
                pushBits(output, bitsCount, code, codeSize);
            }

            const bytes_type expectedOutput = {0b00100000U, 0b00100000U, 0b00000110U, 0b01000100U,
                                               0b01100001U, 0b10001100U, 0b00100001U};

            REQUIRE(output == expectedOutput);

            const auto expectedBitsCount = 54;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("byte overflow")
        {
            bytes_type output = {0b00110100, 0b00000000, 0b00000000};
            auto bitsCount = 6;

            pushBits(output, bitsCount, 0b00000011, 2);

            const bytes_type expectedOutput = {0b11110100};

            REQUIRE(output == expectedOutput);

            const auto expectedBitsCount = 8;

            REQUIRE(bitsCount == expectedBitsCount);
        }

        SECTION("byte underflow")
        {
            bytes_type output = {0b10101010};
            auto bitsCount = 9;

            REQUIRE_THROWS_AS(pushBits(output, bitsCount, 0b00110011, 6), std::invalid_argument);
        }
    }
}
