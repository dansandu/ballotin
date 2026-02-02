#include "dansandu/ballotin/identifier_prototype.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <sstream>

using dansandu::ballotin::identifier_prototype::IdentifierPrototype;

class IntegerIdentifierTag
{
};

using IntegerIdentifier = IdentifierPrototype<IntegerIdentifierTag, int>;

TEST_CASE("identifier_prototype")
{
    const auto integer = 59;

    const auto biggerInteger = 101;

    const auto identifier = IntegerIdentifier{integer};

    const auto biggerIdentifier = IntegerIdentifier{biggerInteger};

    SECTION("integer conversion")
    {
        REQUIRE(identifier.getInteger() == integer);

        REQUIRE(biggerIdentifier.getInteger() == biggerInteger);
    }

    SECTION("string conversion")
    {
        REQUIRE(identifier.toString() == "59");

        REQUIRE(biggerIdentifier.toString() == "101");
    }

    SECTION("stream operations #1")
    {
        auto stream = std::stringstream{};

        stream << identifier;

        REQUIRE(stream.str() == "59");
    }

    SECTION("stream operations #2")
    {
        auto stream = std::stringstream{};

        stream << biggerIdentifier;

        REQUIRE(stream.str() == "101");
    }

    SECTION("relational operations")
    {
        REQUIRE(identifier == identifier);

        REQUIRE(identifier <= identifier);

        REQUIRE(identifier >= identifier);

        REQUIRE(identifier != biggerIdentifier);

        REQUIRE(identifier < biggerIdentifier);

        REQUIRE(biggerIdentifier > identifier);

        REQUIRE(identifier <= biggerIdentifier);

        REQUIRE(biggerIdentifier >= identifier);
    }
}
