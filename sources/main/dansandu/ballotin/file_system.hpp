#pragma once

#include "dansandu/journey/utility.hpp"

#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::file_system
{

PRALINE_EXPORT void writeBinaryFile(const std::string& path, const std::span<const uint8_t> bytes);

PRALINE_EXPORT std::vector<uint8_t> readBinaryFile(const std::string& path);

PRALINE_EXPORT void writeAsciiFile(const std::string& path, const std::span<const char> bytes);

PRALINE_EXPORT std::string readAsciiFile(const std::string& path);

using dansandu::journey::utility::writeToStandardOutput;

using dansandu::journey::utility::writeToStandardError;

using dansandu::journey::utility::isSubpath;

using dansandu::journey::utility::tryGetRelativePath;

using dansandu::journey::utility::getFileName;

using dansandu::journey::utility::replaceBackSlashes;

}
