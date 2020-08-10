#pragma once

#include <algorithm>
#include <map>
#include <optional>
#include <ostream>
#include <vector>

namespace dansandu::ballotin::container
{

template<typename T, typename E>
bool contains(std::vector<T>& container, const E& element)
{
    return std::find(container.cbegin(), container.cend(), element) != container.cend();
}

template<typename T, typename E>
void uniquePushBack(std::vector<T>& container, E&& element)
{
    if (auto position = std::find(container.cbegin(), container.cend(), element); position == container.cend())
    {
        container.push_back(std::forward<E>(element));
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& container)
{
    auto addComma = false;
    stream << "[";
    for (const auto& element : container)
    {
        if (addComma)
            stream << ", ";
        stream << element;
        addComma = true;
    }
    return stream << "]";
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& stream, const std::map<K, V>& container)
{
    auto addComma = false;
    stream << "{";
    for (const auto& pair : container)
    {
        if (addComma)
            stream << ", ";
        stream << pair.first << ": " << pair.second;
        addComma = true;
    }
    return stream << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::optional<T>& optional)
{
    stream << "Optional(";
    if (optional)
        stream << *optional;
    return stream << ")";
}

}
