#pragma once

#include "dansandu/ballotin/string.hpp"

#include <stacktrace>
#include <stdexcept>
#include <thread>

#define THROW(exception, ...)                                                                                          \
    throw exception{dansandu::ballotin::string::format(                                                                \
        "'", #exception, "' exception in thread '", std::this_thread::get_id(),                                        \
        "': ", dansandu::ballotin::string::format(__VA_ARGS__), "\n", std::stacktrace::current())};

#define WTHROW(exception, ...)                                                                                         \
    throw exception{dansandu::ballotin::string::wformat(                                                               \
        "'", #exception, "' exception in thread '", std::this_thread::get_id(),                                        \
        "': ", dansandu::ballotin::string::wformat(__VA_ARGS__), "\n", std::to_string(std::stacktrace::current()))};
