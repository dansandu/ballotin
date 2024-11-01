#pragma once

#include <type_traits>

namespace dansandu::ballotin::type_traits
{

template<typename... Types>
struct TypePack
{
    template<template<typename...> typename T>
    using ExpandedInto = T<Types...>;

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
