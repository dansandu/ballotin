#pragma once

namespace dansandu::ballotin::predictive_random_generator
{
class PredictiveRandomGenerator
{
public:
    using result_type = unsigned int;

    static constexpr result_type min() noexcept
    {
        return 0U;
    }

    static constexpr result_type max() noexcept
    {
        return 100U;
    }

    static PredictiveRandomGenerator& instance()
    {
        static auto generator = PredictiveRandomGenerator{};
        return generator;
    }

    constexpr result_type operator()() const noexcept
    {
        return 50U;
    }
};
}
