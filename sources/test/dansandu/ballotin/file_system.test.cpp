#include "dansandu/ballotin/file_system.hpp"
#include "dansandu/radiance/radiance.hpp"

using dansandu::ballotin::file_system::getFileName;
using dansandu::ballotin::file_system::isSubpath;
using dansandu::ballotin::file_system::readAsciiFile;
using dansandu::ballotin::file_system::readBinaryFile;
using dansandu::ballotin::file_system::replaceBackSlashes;
using dansandu::ballotin::file_system::tryGetRelativePath;
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

    SECTION("isSubpath")
    {
        REQUIRE(isSubpath("C:/sources/main/organization/artifact/file.cpp", "C:/sources"));

        REQUIRE(isSubpath("C:/sources/test/organization/artifact/file.test.cpp", "C:/sources/"));

        REQUIRE(isSubpath("C:/sources/", "C:/sources/"));

        REQUIRE(isSubpath("C:/sources", "C:/sources/"));

        REQUIRE(isSubpath("C:/sources/", "C:/sources"));

        REQUIRE(isSubpath("C:/sources", "C:/sources"));

        REQUIRE(isSubpath("C:/sources", ""));

        REQUIRE(isSubpath("", ""));

        REQUIRE(isSubpath("/", "/"));

        REQUIRE(isSubpath("a/b/c", "a/b"));

        REQUIRE(!isSubpath("C:/sources/test", "C:/sources/t"));

        REQUIRE(!isSubpath("", "C:/sources"));

        REQUIRE(!isSubpath("/a", "/b"));

        REQUIRE(!isSubpath("a", "b"));
    }

    SECTION("tryGetRelativePath")
    {
        auto relativePath = std::string{};

        REQUIRE(tryGetRelativePath("C:/sources/main/organization/artifact/file.cpp", "C:/sources", relativePath));

        REQUIRE(relativePath == "main/organization/artifact/file.cpp");

        REQUIRE(tryGetRelativePath("C:/sources/test/organization/artifact/file.test.cpp", "C:/sources/", relativePath));

        REQUIRE(relativePath == "test/organization/artifact/file.test.cpp");

        REQUIRE(tryGetRelativePath("C:/sources/", "C:/sources/", relativePath));

        REQUIRE(relativePath == "");

        REQUIRE(tryGetRelativePath("C:/sources", "C:/sources/", relativePath));

        REQUIRE(relativePath == "");

        REQUIRE(tryGetRelativePath("C:/sources/", "C:/sources", relativePath));

        REQUIRE(relativePath == "");

        REQUIRE(tryGetRelativePath("C:/sources", "C:/sources", relativePath));

        REQUIRE(relativePath == "");

        REQUIRE(tryGetRelativePath("C:/sources", "", relativePath));

        REQUIRE(relativePath == "C:/sources");

        REQUIRE(tryGetRelativePath("", "", relativePath));

        REQUIRE(relativePath == "");

        REQUIRE(tryGetRelativePath("/", "/", relativePath));

        REQUIRE(relativePath == "");

        REQUIRE(tryGetRelativePath("a/b/c", "a/b", relativePath));

        REQUIRE(relativePath == "c");

        REQUIRE(!tryGetRelativePath("C:/sources/test", "C:/sources/t", relativePath));

        REQUIRE(!tryGetRelativePath("", "C:/sources", relativePath));

        REQUIRE(!tryGetRelativePath("/a", "/b", relativePath));

        REQUIRE(!tryGetRelativePath("a", "b", relativePath));
    }

    SECTION("file name")
    {
        REQUIRE(getFileName("") == std::string());

        REQUIRE(getFileName("a") == std::string("a"));

        REQUIRE(getFileName("abc") == std::string("abc"));

        REQUIRE(getFileName("abc.txt") == std::string("abc.txt"));

        REQUIRE(getFileName("path\\to\\file") == std::string("file"));

        REQUIRE(getFileName("path/to/file") == std::string("file"));

        REQUIRE(getFileName("\\file") == std::string("file"));

        REQUIRE(getFileName("/file") == std::string("file"));

        REQUIRE(getFileName("\\") == std::string());

        REQUIRE(getFileName("/") == std::string());
    }
}
