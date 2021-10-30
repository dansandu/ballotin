#include "dansandu/ballotin/file_system.hpp"
#include "dansandu/ballotin/exception.hpp"

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

namespace dansandu::ballotin::file_system
{

void writeBinaryFile(const std::string& path, const std::vector<uint8_t>& bytes)
{
    auto file = std::ofstream{path, std::ios_base::binary};
    file << std::noskipws;
    for (auto byte : bytes)
    {
        if (!(file << byte))
        {
            THROW(std::runtime_error, "could not write bytes to file '", path, "'");
        }
    }
    file.close();
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

void writeToStandardOutput(const std::string_view string)
{
    static auto mutex = std::mutex{};
    auto lock = std::lock_guard<std::mutex>{mutex};
    std::cout << string;
}

void writeToStandardError(const std::string_view string)
{
    static auto mutex = std::mutex{};
    auto lock = std::lock_guard<std::mutex>{mutex};
    std::cerr << string;
}

}
