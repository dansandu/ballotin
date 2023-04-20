
#include "dansandu/ballotin/environment.hpp"

#include <cstdlib>
#include <memory>
#include <optional>
#include <string>

namespace dansandu::ballotin::environment
{

std::optional<std::string> getEnvironmentVariable(const std::string& variable)
{
#ifdef _WIN32
    size_t requiredSize;

    getenv_s(&requiredSize, nullptr, 0, variable.c_str());
    if (requiredSize == 0)
    {
        return {};
    }

    const auto value = std::make_unique<char[]>(requiredSize);

    getenv_s(&requiredSize, value.get(), requiredSize, variable.c_str());

    return std::optional<std::string>{std::in_place, value.get()};
#else
    const auto value = secure_getenv(variable.c_str());
    if (value != nullptr)
    {
        return std::optional<std::string>{std::in_place, value};
    }
    else
    {
        return {};
    }
#endif
}

}
