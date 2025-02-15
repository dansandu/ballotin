#include "dansandu/ballotin/console.hpp"

#include <stdexcept>

namespace dansandu::ballotin::console
{

std::wstring highlightText(const std::wstring& text, const TextHighlight textHighlight)
{
    if (text.empty())
    {
        return text;
    }

    switch (textHighlight)
    {
    case TextHighlight::None:
        return text;
    case TextHighlight::Red:
        return L"\x1B[31m" + text + L"\x1B[0m";
    case TextHighlight::Green:
        return L"\x1B[32m" + text + L"\x1B[0m";
    case TextHighlight::Yellow:
        return L"\x1B[33m" + text + L"\x1B[0m";
    case TextHighlight::Blue:
        return L"\x1B[34m" + text + L"\x1B[0m";
    case TextHighlight::Magenta:
        return L"\x1B[35m" + text + L"\x1B[0m";
    default:
        throw std::logic_error{"unknown text highlight"};
    }
}

}
