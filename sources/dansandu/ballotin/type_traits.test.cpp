#include "dansandu/ballotin/type_traits.hpp"
#include "catchorg/catch/catch.hpp"

using dansandu::ballotin::type_traits::TypePack;

TEST_CASE("TypeTraits")
{
    REQUIRE(TypePack<int, float, double>::contains<int>);

    REQUIRE(!TypePack<int, float, double>::contains<char>);
}
