#include "dansandu/ballotin/string.hpp"
#include "dansandu/ballotin/exception.hpp"

#include <algorithm>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::string
{

std::vector<std::string> split(const std::string_view string, const std::string_view delimiter)
{
    if (delimiter.empty())
    {
        THROW(std::invalid_argument, "delimiter cannot be empty");
    }
    auto tokens = std::vector<std::string>{};
    auto tokenBegin = string.cbegin();
    auto tokenEnd = tokenBegin;
    while ((tokenEnd = std::search(tokenBegin, string.cend(), delimiter.cbegin(), delimiter.cend())) != string.cend())
    {
        if (tokenBegin != tokenEnd)
        {
            tokens.emplace_back(tokenBegin, tokenEnd);
        }
        tokenBegin = tokenEnd + delimiter.size();
    }
    if (tokenBegin != tokenEnd)
    {
        tokens.emplace_back(tokenBegin, tokenEnd);
    }
    return tokens;
}

std::string trim(const std::string_view string)
{
    auto begin = string.begin();
    while (begin < string.end() && std::isspace(*begin))
        ++begin;

    auto end = string.end() - 1;
    while (end > begin && std::isspace(*end))
        --end;

    return {begin, end + 1};
}

}
