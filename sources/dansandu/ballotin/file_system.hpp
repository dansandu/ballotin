#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace dansandu::ballotin::file_system
{

PRALINE_EXPORT void writeBinaryFile(const std::string& path, const std::vector<uint8_t>& bytes);

}
