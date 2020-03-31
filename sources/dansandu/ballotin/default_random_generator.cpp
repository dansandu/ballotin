#include "dansandu/ballotin/default_random_generator.hpp"

#include <random>

namespace dansandu::ballotin::default_random_generator
{

std::mt19937& DefaultRandomGenerator::instance()
{
    static auto generator = std::mt19937{std::random_device{}()};
    return generator;
}

}
