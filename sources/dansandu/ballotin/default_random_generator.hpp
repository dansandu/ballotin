#pragma once

#include <random>

namespace dansandu::ballotin::default_random_generator
{

struct DefaultRandomGenerator
{
    static std::mt19937& instance();
};

}
