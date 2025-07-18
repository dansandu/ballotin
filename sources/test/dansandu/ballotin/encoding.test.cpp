#include "dansandu/ballotin/encoding.hpp"
#include "dansandu/radiance/radiance.hpp"

using dansandu::ballotin::encoding::codeToString;
using dansandu::ballotin::encoding::decode64;
using dansandu::ballotin::encoding::encode64;
using dansandu::ballotin::encoding::getBase64Characters;
using dansandu::ballotin::encoding::stringToCode;

using BytesType = std::vector<uint8_t>;

TEST_CASE("encoding")
{
    SECTION("base64")
    {
        SECTION("characters")
        {
            const auto characters = getBase64Characters();

            const auto expected = std::array<uint8_t, 64>{
                {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'}};

            REQUIRE(characters == expected);
        }

        SECTION("encoding with no padding")
        {
            const auto code = encode64(stringToCode("The message to encode..."));

            const auto expected = "VGhlIG1lc3NhZ2UgdG8gZW5jb2RlLi4u";

            REQUIRE(codeToString(code) == expected);
        }

        SECTION("encoding with one character padding")
        {
            const auto code = encode64(stringToCode("The message to encode now."));

            const auto expected = "VGhlIG1lc3NhZ2UgdG8gZW5jb2RlIG5vdy4=";

            REQUIRE(codeToString(code) == expected);
        }

        SECTION("encoding with two characters padding")
        {
            const auto code = encode64(stringToCode("The message to encode to."));

            const auto expected = "VGhlIG1lc3NhZ2UgdG8gZW5jb2RlIHRvLg==";

            REQUIRE(codeToString(code) == expected);
        }

        SECTION("decoding with no padding")
        {
            const auto code = decode64(stringToCode("VGhlIG1lc3NhZ2UgdG8gZW5jb2RlLi4u"));

            const auto expected = "The message to encode...";

            REQUIRE(codeToString(code) == expected);
        }

        SECTION("decoding with one character padding")
        {
            const auto code = decode64(stringToCode("VGhlIG1lc3NhZ2UgdG8gZW5jb2RlIG5vdy4="));

            const auto expected = "The message to encode now.";

            REQUIRE(codeToString(code) == expected);
        }

        SECTION("decoding with two characters padding")
        {
            const auto code = decode64(stringToCode("VGhlIG1lc3NhZ2UgdG8gZW5jb2RlIHRvLg=="));

            const auto expected = "The message to encode to.";

            REQUIRE(codeToString(code) == expected);
        }

        SECTION("code to string")
        {
            const auto string = codeToString({0x4D, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0xF1, 0xE0});

            const auto expected = "Message\xF1\xE0";

            REQUIRE(string == expected);
        }

        SECTION("string to code")
        {
            const auto code = stringToCode("Message\xF1\xE0");

            const BytesType expected = {0x4D, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0xF1, 0xE0};

            REQUIRE(code == expected);
        }
    }
}
