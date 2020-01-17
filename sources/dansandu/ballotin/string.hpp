#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::string
{

template<typename I>
auto join(const I& iterable, std::string_view separator)
{
    std::stringstream ss;
    for (const auto& element : iterable)
        ss << element << separator;
    auto result = ss.str();
    result.erase(result.end() - std::min(separator.size(), result.size()), result.end());
    return result;
}

template<typename... Arguments>
auto format(Arguments&&... arguments)
{
    std::stringstream buffer;
    (buffer << ... << arguments);
    return buffer.str();
}

std::vector<std::string> split(std::string_view string, std::string_view delimiter);

std::string trim(std::string string);

}
