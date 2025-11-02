#include "dansandu/ballotin/string.hpp"
#include "dansandu/ballotin/exception.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <vector>

using dansandu::ballotin::string::format;
using dansandu::ballotin::string::join;
using dansandu::ballotin::string::replaceBackSlashes;
using dansandu::ballotin::string::split;
using dansandu::ballotin::string::trim;
using dansandu::ballotin::string::wformat;

TEST_CASE("string")
{
    SECTION("join")
    {
        SECTION("nonempty separator and list")
        {
            std::vector<int> integers = {1, 2, 3, 4};

            REQUIRE(join(integers, ", ") == "1, 2, 3, 4");
        }

        SECTION("empty separator and nonempty list")
        {
            std::vector<int> integers = {1, 2, 3, 4};

            REQUIRE(join(integers, "") == "1234");
        }

        SECTION("nonempty separator and empty list")
        {
            std::vector<int> integers;

            REQUIRE(join(integers, "#") == "");
        }

        SECTION("empty separator and empty list")
        {
            std::vector<int> integers;

            REQUIRE(join(integers, "") == "");
        }
    }

    SECTION("format")
    {
        REQUIRE(format(2, " + ", 3, " = ", 5) == "2 + 3 = 5");

        REQUIRE(format("The ", "cat", " jumped the ", "fence", " twice") == "The cat jumped the fence twice");
    }

    SECTION("wformat")
    {
        REQUIRE(wformat(2, " + ", 3, L" = ", 5) == L"2 + 3 = 5");

        REQUIRE(wformat("The ", L"cat", " jumped the ", L"fence", " twice") == L"The cat jumped the fence twice");
    }

    SECTION("split")
    {
        SECTION("split case #1")
        {
            REQUIRE_THROW(split("a b c", ""), std::invalid_argument);
        }

        SECTION("split case #2")
        {
            const auto expected = std::vector<std::string>{{{"a"}, {"bcd"}, {"ef"}}};

            REQUIRE(split("  a bcd ef ", " ") == expected);
        }

        SECTION("split case #3")
        {
            const auto expected = std::vector<std::string>(1, "abc");

            REQUIRE(split("abc", " ") == expected);
        }

        SECTION("split case #4")
        {
            const auto expected = std::vector<std::string>{};

            REQUIRE(split("", " ") == expected);
        }

        SECTION("split case #5")
        {
            const auto expected = std::vector<std::string>{{{"abc"}, {"def"}, {"gh**"}}};

            REQUIRE(split("***abc***def******gh**", "***") == expected);
        }
    }

    SECTION("trim")
    {
        REQUIRE(trim("") == "");

        REQUIRE(trim("  \t\n") == "");

        REQUIRE(trim("   abc") == "abc");

        REQUIRE(trim("24f\t\t\t") == "24f");

        REQUIRE(trim("  @ $!\t>  ") == "@ $!\t>");

        REQUIRE(trim("gg") == "gg");
    }

    SECTION("replaceBackSlashes")
    {
        REQUIRE(replaceBackSlashes("a/b/c\\d\\\\e/f\\") == "a/b/c/d//e/f/");

        REQUIRE(replaceBackSlashes("") == "");
    }
}
