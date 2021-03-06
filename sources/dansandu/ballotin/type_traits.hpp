#pragma once

#include <type_traits>
#include <variant>

namespace dansandu::ballotin::type_traits
{

struct general_case
{
};

struct best_case : general_case
{
};

struct Uncopyable
{
    Uncopyable() = default;

    Uncopyable(Uncopyable&&) = default;

    Uncopyable(const Uncopyable&) = delete;

    Uncopyable& operator=(Uncopyable&&) = default;

    Uncopyable& operator=(const Uncopyable&) = delete;
};

struct Immovable
{
    Immovable() = default;

    Immovable(Immovable&&) = delete;

    Immovable& operator=(Immovable&&) = delete;
};

template<typename... Types>
struct TypePack
{
    using VariantType = std::variant<Types...>;

    template<typename T>
    static constexpr bool contains = (... || std::is_same_v<T, Types>);
};

}
