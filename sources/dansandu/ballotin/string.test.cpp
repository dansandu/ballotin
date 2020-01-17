#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/exception.hpp"
#include "dansandu/ballotin/string.hpp"

#include <vector>

using dansandu::ballotin::string::format;
using dansandu::ballotin::string::join;
using dansandu::ballotin::string::split;
using dansandu::ballotin::string::trim;

TEST_CASE("String")
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

    SECTION("split")
    {
        REQUIRE_THROWS_AS(split("a b c", ""), std::invalid_argument);
        REQUIRE(split("  a bcd ef ", " ") == std::vector<std::string>{{{"a"}, {"bcd"}, {"ef"}}});
        REQUIRE(split("abc", " ") == std::vector<std::string>{1, "abc"});
        REQUIRE(split("", " ") == std::vector<std::string>{});
        REQUIRE(split("***abc***def******gh**", "***") == std::vector<std::string>{{{"abc"}, {"def"}, {"gh**"}}});
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
}
