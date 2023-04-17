#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::string
{

template<typename Iterable>
auto join(const Iterable& iterable, const std::string_view separator)
{
    auto stream = std::stringstream{};
    for (const auto& element : iterable)
    {
        stream << element << separator;
    }
    auto result = stream.str();
    result.erase(result.end() - std::min(separator.size(), result.size()), result.end());
    return result;
}

template<typename... Arguments>
auto format(Arguments&&... arguments)
{
    auto stream = std::stringstream{};
    (stream << ... << arguments);
    return stream.str();
}

PRALINE_EXPORT std::vector<std::string> split(const std::string_view string, const std::string_view delimiter);

PRALINE_EXPORT std::string trim(const std::string_view string);

enum class TextHighlight
{
    None,
    Red,
    Green,
    Blue,
};

PRALINE_EXPORT std::string highlightText(const std::string& text, const TextHighlight textHighlight);

}
