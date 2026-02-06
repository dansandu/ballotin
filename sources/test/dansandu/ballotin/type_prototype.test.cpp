#include "dansandu/ballotin/type_prototype.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <sstream>

using dansandu::ballotin::type_prototype::TypeFeature;
using dansandu::ballotin::type_prototype::TypePrototype;

class CustomTypeTag
{
};

using CustomType = TypePrototype<CustomTypeTag, int, TypeFeature::all>;

TEST_CASE("type_prototype")
{
    const auto ia = 9;

    const auto ib = 4;

    auto a = CustomType{ia};

    auto b = CustomType{ib};

    SECTION("underlying conversion")
    {
        REQUIRE(a.getUnderlying() == ia);

        REQUIRE(b.getUnderlying() == ib);
    }

    SECTION("addition")
    {
        REQUIRE(a + b == CustomType{ia + ib});
    }

    SECTION("implace addition")
    {
        a += b;

        REQUIRE(a.getUnderlying() == ia + ib);

        REQUIRE(b.getUnderlying() == ib);
    }

    SECTION("pre-increment")
    {
        const auto preIncremented = ++a;

        REQUIRE(preIncremented.getUnderlying() == ia + 1);

        REQUIRE(a.getUnderlying() == ia + 1);
    }

    SECTION("post-increment")
    {
        const auto postIncremented = a++;

        REQUIRE(postIncremented.getUnderlying() == ia);

        REQUIRE(a.getUnderlying() == ia + 1);
    }

    SECTION("subtraction")
    {
        REQUIRE(a - b == CustomType{ia - ib});
    }

    SECTION("implace subtraction")
    {
        a -= b;

        REQUIRE(a.getUnderlying() == ia - ib);

        REQUIRE(b.getUnderlying() == ib);
    }

    SECTION("pre-decrement")
    {
        const auto preDecrement = --a;

        REQUIRE(preDecrement.getUnderlying() == ia - 1);

        REQUIRE(a.getUnderlying() == ia - 1);
    }

    SECTION("post-decrement")
    {
        const auto postDecrement = a--;

        REQUIRE(postDecrement.getUnderlying() == ia);

        REQUIRE(a.getUnderlying() == ia - 1);
    }

    SECTION("multiplication")
    {
        REQUIRE(a * b == CustomType{ia * ib});
    }

    SECTION("implace multiplication")
    {
        a *= b;

        REQUIRE(a.getUnderlying() == ia * ib);

        REQUIRE(b.getUnderlying() == ib);
    }

    SECTION("division")
    {
        REQUIRE(a / b == CustomType{ia / ib});
    }

    SECTION("implace division")
    {
        a /= b;

        REQUIRE(a.getUnderlying() == ia / ib);

        REQUIRE(b.getUnderlying() == ib);
    }

    SECTION("string conversion")
    {
        REQUIRE(a.toString() == "9");

        REQUIRE(b.toString() == "4");
    }

    SECTION("stream operations #1")
    {
        auto stream = std::stringstream{};

        stream << a;

        REQUIRE(stream.str() == "9");
    }

    SECTION("stream operations #2")
    {
        auto stream = std::stringstream{};

        stream << b;

        REQUIRE(stream.str() == "4");
    }

    SECTION("relational operations")
    {
        REQUIRE(a == a);

        REQUIRE(a <= a);

        REQUIRE(a >= a);

        REQUIRE(a != b);

        REQUIRE(b < a);

        REQUIRE(a > b);

        REQUIRE(b <= a);

        REQUIRE(a >= b);
    }
}
