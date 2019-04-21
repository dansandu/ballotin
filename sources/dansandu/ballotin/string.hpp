#pragma once

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::string {

template<typename I>
auto join(const I& iterable, std::string_view separator) {
    std::stringstream ss;
    for (const auto& element : iterable)
        ss << element << separator;
    auto result = ss.str();
    result.erase(result.end() - std::min(separator.size(), result.size()), result.end());
    return result;
}

inline auto formatWork(std::ostream&) {}

template<typename A, typename... AA>
auto formatWork(std::ostream& buffer, A&& argument, AA&&... arguments) {
    buffer << std::forward<A>(argument);
    formatWork(buffer, std::forward<AA>(arguments)...);
}

template<typename... AA>
auto format(AA&&... arguments) {
    std::stringstream buffer;
    formatWork(buffer, std::forward<AA>(arguments)...);
    return buffer.str();
}

std::vector<std::string> split(std::string_view string, std::string_view delimiter);

std::string trim(std::string string);

}
