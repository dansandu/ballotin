#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/type_traits.hpp"

using dansandu::ballotin::type_traits::type_pack;

TEST_CASE("TypeTraits")
{
    REQUIRE(type_pack<int, float, double>::contains<int>);

    REQUIRE(!type_pack<int, float, double>::contains<char>);
}
