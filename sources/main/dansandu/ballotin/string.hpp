#pragma once

#include "dansandu/journey/utility.hpp"

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

PRALINE_EXPORT std::vector<std::string> split(const std::string_view string, const std::string_view delimiter);

PRALINE_EXPORT std::string trim(const std::string_view string);

using dansandu::journey::utility::format;

using dansandu::journey::utility::wformat;

using dansandu::journey::utility::toWideString;

using dansandu::journey::utility::replaceBackSlashes;

}
