#include "dansandu/ballotin/file_system.hpp"
#include "dansandu/journey/exception.hpp"

#include <fstream>
#include <streambuf>
#include <string>
#include <vector>

namespace dansandu::ballotin::file_system
{

void writeBinaryFile(const std::string& path, const std::span<const uint8_t> bytes)
{
    auto file = std::ofstream{path, std::ios_base::binary};
    file << std::noskipws;
    file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    if (!file)
    {
        THROW(std::runtime_error, "could not write bytes to file '", path, "'");
    }
}

std::vector<uint8_t> readBinaryFile(const std::string& path)
{
    auto bytes = std::vector<uint8_t>{};

    auto file = std::ifstream{path, std::ios_base::binary};
    if (!(file >> std::noskipws))
    {
        THROW(std::runtime_error, "file '", path, "' does not exist");
    }

    auto byte = uint8_t{};
    while (file >> byte)
    {
        bytes.push_back(byte);
    }

    return bytes;
}

void writeAsciiFile(const std::string& path, const std::span<const char> bytes)
{
    auto file = std::ofstream{path};
    file.write(bytes.data(), bytes.size());
    if (!file)
    {
        THROW(std::runtime_error, "could not write data to file '", path, "'");
    }
}

std::string readAsciiFile(const std::string& path)
{
    auto file = std::ifstream{path};
    if (!file)
    {
        THROW(std::runtime_error, "file '", path, "' does not exist");
    }

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

}
