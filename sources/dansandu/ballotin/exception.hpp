#pragma once

#include "dansandu/ballotin/string.hpp"

#include <stdexcept>
#include <thread>

#define THROW(exception, ...)                                                                                          \
    throw exception{dansandu::ballotin::string::format("'", #exception, "' exception in thread '",                     \
                                                       std::this_thread::get_id(),                                     \
                                                       "': ", dansandu::ballotin::string::format(__VA_ARGS__),         \
                                                       "\n    at ", __func__, "(", __FILE__, ":", __LINE__, ")")};
