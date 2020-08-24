#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/container.hpp"

#include <map>
#include <sstream>
#include <string>
#include <vector>

using dansandu::ballotin::container::contains;
using dansandu::ballotin::container::pop;
using dansandu::ballotin::container::uniquePushBack;
using dansandu::ballotin::container::operator<<;

TEST_CASE("Container")
{
    SECTION("pop")
    {
        auto stack = std::vector<int>{{7, 11, 13}};

        REQUIRE(pop(stack) == 13);

        REQUIRE(pop(stack) == 11);

        REQUIRE(pop(stack) == 7);
    }

    SECTION("contains")
    {
        auto container = std::vector<int>{{3, 5, 7, 10}};

        REQUIRE(contains(container, 5));

        REQUIRE(!contains(container, 8));
    }

    SECTION("unique push back")
    {
        auto container = std::vector<int>{{1, 3, 5, 7}};

        SECTION("unique")
        {
            uniquePushBack(container, 0);

            REQUIRE(container == std::vector<int>{{1, 3, 5, 7, 0}});
        }

        SECTION("duplicate")
        {
            uniquePushBack(container, 3);

            REQUIRE(container == std::vector<int>{{1, 3, 5, 7}});
        }
    }

    SECTION("pretty print")
    {
        auto stream = std::stringstream{};

        SECTION("empty vector")
        {
            stream << std::vector<int>{};
            REQUIRE(stream.str() == "[]");
        }

        SECTION("singleton vector")
        {
            stream << std::vector<int>{1};
            REQUIRE(stream.str() == "[1]");
        }

        SECTION("many elements vector")
        {
            stream << std::vector<int>{{1, 2, 3, 4}};
            REQUIRE(stream.str() == "[1, 2, 3, 4]");
        }

        SECTION("empty map")
        {
            stream << std::map<std::string, int>{};
            REQUIRE(stream.str() == "{}");
        }

        SECTION("singleton map")
        {
            stream << std::map<std::string, int>{{{"key", 17}}};
            REQUIRE(stream.str() == "{key: 17}");
        }

        SECTION("many elements map")
        {
            stream << std::map<std::string, int>{{{"key", 17}, {"other", 20}, {"another", 23}}};
            REQUIRE(stream.str() == "{another: 23, key: 17, other: 20}");
        }
    }
}
