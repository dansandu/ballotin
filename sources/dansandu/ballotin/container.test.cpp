#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/container.hpp"

#include <map>
#include <sstream>
#include <string>
#include <vector>

using dansandu::ballotin::container::setInsert;
using dansandu::ballotin::container::setUnion;
using dansandu::ballotin::container::operator<<;

TEST_CASE("Container") {
    SECTION("union") {
        REQUIRE(setUnion<int>({1, 3, 5}, {0, 1, 6}) == std::vector<int>{{0, 1, 3, 5, 6}});

        REQUIRE(setUnion<int>({}, {2, 6}) == std::vector<int>{{2, 6}});

        REQUIRE(setUnion<int>({1, 2}, {1, 2}) == std::vector<int>{{1, 2}});

        REQUIRE(setUnion<int>({}, {}) == std::vector<int>{});
    }

    SECTION("insertion into set") {
        auto container = std::vector<int>{{1, 3, 5, 7}};

        SECTION("front") {
            setInsert(container, 0);
            REQUIRE(container == std::vector<int>{{0, 1, 3, 5, 7}});
        }

        SECTION("middle") {
            setInsert(container, 4);
            REQUIRE(container == std::vector<int>{{1, 3, 4, 5, 7}});
        }

        SECTION("back") {
            setInsert(container, 8);
            REQUIRE(container == std::vector<int>{{1, 3, 5, 7, 8}});
        }

        SECTION("duplicate") {
            setInsert(container, 7);
            REQUIRE(container == std::vector<int>{{1, 3, 5, 7}});
        }
    }

    SECTION("pretty print") {
        auto stream = std::stringstream{};

        SECTION("empty vector") {
            stream << std::vector<int>{};
            REQUIRE(stream.str() == "[]");
        }

        SECTION("singleton vector") {
            stream << std::vector<int>{1};
            REQUIRE(stream.str() == "[1]");
        }

        SECTION("many elements vector") {
            stream << std::vector<int>{{1, 2, 3, 4}};
            REQUIRE(stream.str() == "[1, 2, 3, 4]");
        }

        SECTION("empty map") {
            stream << std::map<std::string, int>{};
            REQUIRE(stream.str() == "{}");
        }

        SECTION("singleton map") {
            stream << std::map<std::string, int>{{{"key", 17}}};
            REQUIRE(stream.str() == "{key: 17}");
        }

        SECTION("many elements map") {
            stream << std::map<std::string, int>{{{"key", 17}, {"other", 20}, {"another", 23}}};
            REQUIRE(stream.str() == "{another: 23, key: 17, other: 20}");
        }
    }
}
