#include "dansandu/ballotin/hashing.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <string>

using dansandu::ballotin::hashing::getHashCode32;
using dansandu::ballotin::hashing::getHashCode64;
using dansandu::ballotin::hashing::hashCombine;

enum class MyEnum
{
    a,
    b,
    c,
};

TEST_CASE("hashing")
{
    SECTION("32-bit hash")
    {
        SECTION("hash integers")
        {
            const auto a = getHashCode32(13);

            const auto b = getHashCode32(29);

            REQUIRE(a != b);

            REQUIRE(a == 1008382536u);

            REQUIRE(b == 1054778200u);

            const auto combined = hashCombine(a, b);

            REQUIRE(a != combined);

            REQUIRE(b != combined);
        }

        SECTION("hash enums")
        {
            const auto a = getHashCode32(MyEnum::a);

            const auto b = getHashCode32(MyEnum::b);

            REQUIRE(a != b);

            REQUIRE(a == 1268118805u);

            REQUIRE(b == 4218009092u);

            const auto combined = hashCombine(a, b);

            REQUIRE(a != combined);

            REQUIRE(b != combined);
        }

        SECTION("hash strings")
        {
            const auto a = getHashCode32(std::string("abcd"));

            const auto b = getHashCode32(std::string("abce"));

            REQUIRE(a != b);

            REQUIRE(a == 3459545533u);

            REQUIRE(b == 3442767914u);

            const auto combined = hashCombine(a, b);

            REQUIRE(a != combined);

            REQUIRE(b != combined);
        }
    }

    SECTION("64-bit hash")
    {
        SECTION("hash integers")
        {
            const auto a = getHashCode64(13);

            const auto b = getHashCode64(29);

            REQUIRE(a != b);

            REQUIRE(a == 3248637502162946472ull);

            REQUIRE(b == 3224642333295973048ull);

            const auto combined = hashCombine(a, b);

            REQUIRE(a != combined);

            REQUIRE(b != combined);
        }

        SECTION("hash enums")
        {
            const auto a = getHashCode64(MyEnum::a);

            const auto b = getHashCode64(MyEnum::b);

            REQUIRE(a != b);

            REQUIRE(a == 5558979605539197941ull);

            REQUIRE(b == 12478008331234465636ull);

            const auto combined = hashCombine(a, b);

            REQUIRE(a != combined);

            REQUIRE(b != combined);
        }

        SECTION("hash strings")
        {
            const auto a = getHashCode64(std::string("abcd"));

            const auto b = getHashCode64(std::string("abce"));

            REQUIRE(a != b);

            REQUIRE(a == 18165163011005162717ull);

            REQUIRE(b == 18165161911493534506ull);

            const auto combined = hashCombine(a, b);

            REQUIRE(a != combined);

            REQUIRE(b != combined);
        }
    }
}
