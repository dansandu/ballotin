#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/predictive_random_generator.hpp"

#include <random>

using Catch::Detail::Approx;
using dansandu::ballotin::predictive_random_generator::PredictiveRandomGenerator;

TEST_CASE("PredictiveRandomGenerator")
{
    auto& generator = PredictiveRandomGenerator::instance();

    SECTION("fixed value")
    {
        REQUIRE(generator() == 50U);

        REQUIRE(generator() == 50U);

        REQUIRE(generator() == 50U);
    }

    SECTION("uniform integer distribution")
    {
        auto distribution = std::uniform_int_distribution{0, 10};

        REQUIRE(distribution(generator) == 2);

        REQUIRE(distribution(generator) == 2);
    }

    SECTION("uniform real distribution")
    {
        auto distribution = std::uniform_real_distribution{0.0, 1.0};

        REQUIRE(distribution(generator) == Approx(0.5));

        REQUIRE(distribution(generator) == Approx(0.5));
    }
}
