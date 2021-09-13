#include "dansandu/ballotin/hash.hpp"
#include "catchorg/catch/catch.hpp"

using dansandu::ballotin::hash::hashCombine;

TEST_CASE("Hash")
{
    SECTION("hash combine")
    {
        auto i = 13;
        auto j = 29;
        auto iHash = std::hash<int>{}(i);
        auto jHash = std::hash<int>{}(j);
        auto hash = hashCombine(iHash, j);

        REQUIRE(iHash != hash);

        REQUIRE(jHash != hash);
    }
}
