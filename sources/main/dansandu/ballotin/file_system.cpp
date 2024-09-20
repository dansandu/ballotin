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

static auto standardOutputMutex = std::mutex{};

void writeToStandardOutput(const std::string_view string, const bool flush)
{
    const auto lock = std::lock_guard<std::mutex>{standardOutputMutex};
    std::cout << string;
    if (flush)
    {
        std::cout.flush();
    }
}

void writeToStandardOutput(const std::wstring_view string, const bool flush)
{
    const auto lock = std::lock_guard<std::mutex>{standardOutputMutex};
    std::wcout << string;
    if (flush)
    {
        std::wcout.flush();
    }
}

void writeToStandardError(const std::string_view string, const bool flush)
{
    const auto lock = std::lock_guard<std::mutex>{standardOutputMutex};
    std::cerr << string;
    if (flush)
    {
        std::cerr.flush();
    }
}

void writeToStandardError(const std::wstring_view string, const bool flush)
{
    const auto lock = std::lock_guard<std::mutex>{standardOutputMutex};
    std::wcerr << string;
    if (flush)
    {
        std::wcerr.flush();
    }
}

}
