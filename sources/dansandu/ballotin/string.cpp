#include "dansandu/ballotin/string.hpp"
#include "dansandu/ballotin/exception.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::string {

std::vector<std::string> split(std::string_view string, std::string_view delimiter) {
    if (delimiter.empty())
        THROW(std::invalid_argument, "delimiter cannot be empty");
    auto result = std::vector<std::string>{};
    auto subStringEnd = string.cbegin(), subStringBegin = subStringEnd;
    while ((subStringEnd = std::search(subStringBegin, string.cend(), delimiter.cbegin(), delimiter.cend())) !=
           string.cend()) {
        if (subStringBegin != subStringEnd)
            result.emplace_back(subStringBegin, subStringEnd);
        subStringBegin = subStringEnd + delimiter.size();
    }
    if (subStringBegin != subStringEnd)
        result.emplace_back(subStringBegin, subStringEnd);
    return result;
}

std::string trim(std::string string) {
    auto notWhiteSpace = [](auto c) { return !std::isspace(c); };
    string.erase(string.begin(), std::find_if(string.begin(), string.end(), notWhiteSpace));
    auto rightTrimSize = std::find_if(string.rbegin(), string.rend(), notWhiteSpace) - string.rbegin();
    string.erase(string.end() - rightTrimSize, string.end());
    return string;
}

}
