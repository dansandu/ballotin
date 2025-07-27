#include "dansandu/ballotin/hashing.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <string>

using dansandu::ballotin::hashing::getHashCode;
using dansandu::ballotin::hashing::hashCombine;

enum class MyEnum
{
    a,
    b,
    c,
};

TEST_CASE("hashing")
{
    SECTION("hash integers")
    {
        const auto a = getHashCode(13);

        const auto b = getHashCode(29);

        REQUIRE(a != b);

        const auto combined = hashCombine(a, b);

        REQUIRE(a != combined);

        REQUIRE(b != combined);
    }

    SECTION("hash enums")
    {
        const auto a = getHashCode(MyEnum::a);

        const auto b = getHashCode(MyEnum::b);

        REQUIRE(a != b);

        const auto combined = hashCombine(a, b);

        REQUIRE(a != combined);

        REQUIRE(b != combined);
    }

    SECTION("hash strings")
    {
        const auto a = getHashCode(std::string("abcd"));

        const auto b = getHashCode(std::string("abce"));

        REQUIRE(a != b);

        const auto combined = hashCombine(a, b);

        REQUIRE(a != combined);

        REQUIRE(b != combined);
    }
}
