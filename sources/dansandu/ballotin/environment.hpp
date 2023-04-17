#pragma once

#include <optional>
#include <string>

namespace dansandu::ballotin::environment
{

PRALINE_EXPORT std::optional<std::string> getEnvironmentVariable(const std::string& variable);

}
