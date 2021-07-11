#include "dansandu/ballotin/file_system.hpp"
#include "dansandu/ballotin/exception.hpp"

#include <fstream>
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
            THROW(std::runtime_error, "could not write bytes to file");
        }
    }
    file.close();
}

}
