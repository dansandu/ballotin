#include "dansandu/ballotin/type_traits.hpp"
#include "dansandu/radiance/radiance.hpp"

using dansandu::ballotin::type_traits::Invokable;
using dansandu::ballotin::type_traits::TypeDictionary;
using dansandu::ballotin::type_traits::TypeEntry;
using dansandu::ballotin::type_traits::TypeList;

TEST_CASE("type_traits")
{
    SECTION("type list")
    {
        REQUIRE(TypeList<int, float, double>::contains<int>);

        REQUIRE(!TypeList<int, float, double>::contains<char>);
    }

    SECTION("type dictionary")
    {
        using Dictionary = TypeDictionary<TypeEntry<int, unsigned>, TypeEntry<char, unsigned char>>;

        REQUIRE(Dictionary::containsKey<int>);

        REQUIRE(!Dictionary::containsKey<double>);

        REQUIRE(std::is_same_v<Dictionary::Get<double>, void>);

        REQUIRE(std::is_same_v<Dictionary::Get<int>, unsigned>);
    }

    SECTION("invokable void(int) const")
    {
        struct SomeInvokableType
        {
            void operator()(int) const
            {
            }
        };

        REQUIRE(Invokable<SomeInvokableType, void, int>);

        REQUIRE(!Invokable<SomeInvokableType, int, int>);

        REQUIRE(!Invokable<SomeInvokableType, void, int&>);

        REQUIRE(!Invokable<SomeInvokableType, void, int&&>);

        REQUIRE(!Invokable<SomeInvokableType, void>);
    }

    SECTION("invokable int()")
    {
        struct AnotherInvokableType
        {
            int operator()()
            {
                return 0;
            }
        };

        REQUIRE(Invokable<AnotherInvokableType, int>);

        REQUIRE(!Invokable<AnotherInvokableType, int, int>);

        REQUIRE(!Invokable<AnotherInvokableType, void>);
    }
}
