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

template<typename K, typename V>
struct TypeEntry
{
    using Key = K;
    using Value = V;
};

template<typename Key, typename... Entries>
struct Getter;

template<typename Key, typename CandidateKey, typename Value, typename... Entries>
struct Getter<Key, TypeEntry<CandidateKey, Value>, Entries...>
{
    using Result =
        std::conditional_t<std::is_same_v<Key, CandidateKey>, Value, typename Getter<Key, Entries...>::Result>;
};

template<typename Key>
struct Getter<Key>
{
    using Result = void;
};

template<typename... Entries>
class TypeDictionary
{
public:
    template<typename Key>
    static constexpr bool containsKey = (... || std::is_same_v<Key, typename Entries::Key>);

    template<typename Key>
    using Get = typename Getter<Key, Entries...>::Result;
};

}
