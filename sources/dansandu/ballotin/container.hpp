#pragma once

#include <algorithm>
#include <map>
#include <optional>
#include <ostream>
#include <vector>

namespace dansandu::ballotin::container {

template<typename T>
std::vector<T> setUnion(const std::vector<T>& left, const std::vector<T>& right) {
    auto result = std::vector<T>{};
    std::set_union(left.cbegin(), left.cend(), right.cbegin(), right.cend(), std::back_inserter(result));
    return result;
}

template<typename T, typename E>
void setInsert(std::vector<T>& container, E&& element) {
    auto position = std::lower_bound(container.begin(), container.end(), element);
    if (position == container.end() || element != *position)
        container.insert(position, std::forward<E>(element));
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& container) {
    auto addComma = false;
    stream << "[";
    for (const auto& element : container) {
        if (addComma)
            stream << ", ";
        stream << element;
        addComma = true;
    }
    return stream << "]";
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& stream, const std::map<K, V>& container) {
    auto addComma = false;
    stream << "{";
    for (const auto& pair : container) {
        if (addComma)
            stream << ", ";
        stream << pair.first << ": " << pair.second;
        addComma = true;
    }
    return stream << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const std::optional<T>& optional) {
    stream << "Optional(";
    if (optional)
        stream << *optional;
    return stream << ")";
}

}
