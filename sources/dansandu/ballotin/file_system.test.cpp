#include "dansandu/ballotin/file_system.hpp"
#include "catchorg/catch/catch.hpp"

using Catch::Matches;
using dansandu::ballotin::file_system::readBinaryFile;
using dansandu::ballotin::file_system::writeBinaryFile;

TEST_CASE("file_system")
{
    SECTION("write and read binary")
    {
        const auto expected = std::vector<uint8_t>({1, 2, 3, 4, 5, 6});

        const auto binaryFileName = "target/temporary/test_binary.bin";

        writeBinaryFile(binaryFileName, expected);

        const auto actual = readBinaryFile(binaryFileName);

        REQUIRE(std::vector<int>(expected.begin(), expected.end()) == std::vector<int>(actual.begin(), actual.end()));
    }
}
