#include "dansandu/ballotin/type_traits.hpp"
#include "catchorg/catch2/catch_test_macros.hpp"

using dansandu::ballotin::type_traits::TypeDictionary;
using dansandu::ballotin::type_traits::TypeEntry;
using dansandu::ballotin::type_traits::TypePack;

TEST_CASE("type_traits")
{
    REQUIRE(TypePack<int, float, double>::contains<int>);

    REQUIRE(!TypePack<int, float, double>::contains<char>);

    using Dictionary = TypeDictionary<TypeEntry<int, unsigned>, TypeEntry<char, unsigned char>>;

    REQUIRE(Dictionary::containsKey<int>);

    REQUIRE(!Dictionary::containsKey<double>);

    REQUIRE(std::is_same_v<Dictionary::Get<double>, void>);

    REQUIRE(std::is_same_v<Dictionary::Get<int>, unsigned>);
}
