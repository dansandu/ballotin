#include "dansandu/ballotin/file_system.hpp"
#include "dansandu/radiance/radiance.hpp"

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

        REQUIRE(actual == expected);
    }
}
