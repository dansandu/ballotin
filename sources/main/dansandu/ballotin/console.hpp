#pragma once

#include <string>

namespace dansandu::ballotin::console
{

enum class TextHighlight
{
    None,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
};

PRALINE_EXPORT std::wstring highlightText(const std::wstring& text, const TextHighlight textHighlight);

}
