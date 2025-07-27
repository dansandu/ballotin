#include "dansandu/ballotin/hashing.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <string>

using dansandu::ballotin::hashing::hash;
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
        const auto a = hash(13);

        const auto b = hash(29);

        REQUIRE(a != b);

        const auto combined = hashCombine(a, b);

        REQUIRE(a != combined);

        REQUIRE(b != combined);
    }

    SECTION("hash enums")
    {
        const auto a = hash(MyEnum::a);

        const auto b = hash(MyEnum::b);

        REQUIRE(a != b);

        const auto combined = hashCombine(a, b);

        REQUIRE(a != combined);

        REQUIRE(b != combined);
    }

    SECTION("hash strings")
    {
        const auto a = hash(std::string("abcd"));

        const auto b = hash(std::string("abce"));

        REQUIRE(a != b);

        const auto combined = hashCombine(a, b);

        REQUIRE(a != combined);

        REQUIRE(b != combined);
    }
}
