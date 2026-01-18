#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace dansandu::ballotin::encoding
{

PRALINE_EXPORT const std::array<uint8_t, 64>& getBase64Characters();

PRALINE_EXPORT std::vector<uint8_t> encode64(const std::vector<uint8_t>& bytes);

PRALINE_EXPORT std::vector<uint8_t> decode64(const std::vector<uint8_t>& bytes);

PRALINE_EXPORT std::string codeToString(const std::vector<uint8_t>& bytes);

PRALINE_EXPORT std::vector<uint8_t> stringToCode(const std::string& bytes);

}
