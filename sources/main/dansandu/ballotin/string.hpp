#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
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
auto format(const Arguments&... arguments)
{
    auto stream = std::stringstream{};
    (stream << ... << arguments);
    return stream.str();
}

template<typename... Arguments>
auto wformat(const Arguments&... arguments)
{
    auto stream = std::wstringstream{};
    auto streamWriter = [&stream]<typename T>(const T& argument)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
        {
            stream << argument.c_str();
        }
        else if constexpr (std::is_same_v<std::decay_t<T>, std::string_view>)
        {
            stream.write(argument.begin(), argument.end() - argument.begin());
        }
        else
        {
            stream << argument;
        }
    };
    (streamWriter(arguments), ...);
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

PRALINE_EXPORT const char* getFileName(const char* filePath);

}
