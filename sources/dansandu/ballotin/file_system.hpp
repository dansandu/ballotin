#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace dansandu::ballotin::file_system
{

PRALINE_EXPORT void writeBinaryFile(const std::string& path, const std::vector<uint8_t>& bytes);

PRALINE_EXPORT std::vector<uint8_t> readBinaryFile(const std::string& path);

PRALINE_EXPORT void writeToStandardOutput(const std::string_view string);

PRALINE_EXPORT void writeToStandardError(const std::string_view string);

}
