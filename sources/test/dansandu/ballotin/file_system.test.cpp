#include "dansandu/ballotin/file_system.hpp"
#include "dansandu/radiance/radiance.hpp"

using dansandu::ballotin::file_system::readAsciiFile;
using dansandu::ballotin::file_system::readBinaryFile;
using dansandu::ballotin::file_system::writeAsciiFile;
using dansandu::ballotin::file_system::writeBinaryFile;

TEST_CASE("file_system")
{
    SECTION("write and read binary")
    {
        const auto expected = std::vector<uint8_t>({1, 2, 3, 4, 5, 6});

        const auto fileName = "target/temporary/test_binary.bin";

        writeBinaryFile(fileName, expected);

        const auto actual = readBinaryFile(fileName);

        REQUIRE(actual == expected);
    }

    SECTION("write and read ASCII")
    {
        const auto expected = std::string{"\tsome text to write\n"};

        const auto fileName = "target/temporary/test_ascii.txt";

        writeAsciiFile(fileName, expected);

        const auto actual = readAsciiFile(fileName);

        REQUIRE(actual == expected);
    }
}
