#include "dansandu/ballotin/random.hpp"
#include "catchorg/catch/catch.hpp"

#include <algorithm>

using dansandu::ballotin::random::PredictableBitGenerator;

TEST_CASE("random")
{
    SECTION("PredictableBitGenerator")
    {
        SECTION("seedless")
        {
            SECTION("samples")
            {
                auto generator = PredictableBitGenerator{};

                REQUIRE(generator.min() == 0U);

                REQUIRE(generator.max() == 255U);

                const auto expected = std::vector<unsigned>{{0, 1, 2, 3, 4, 5, 6, 7}};

                auto actual = std::vector<unsigned>{};
                for (auto i = 0U; i < expected.size(); ++i)
                {
                    actual.push_back(generator());
                }

                REQUIRE(expected == actual);
            }

            SECTION("shuffle")
            {
                const auto samples = 1000;

                auto ordered = std::vector<int>{};
                for (auto index = 0; index < samples; ++index)
                {
                    ordered.push_back(index);
                }

                const auto seed = 0U;

                auto generator = PredictableBitGenerator{seed};
                auto shuffled = ordered;
                std::shuffle(shuffled.begin(), shuffled.end(), generator);

                REQUIRE(shuffled.size() == ordered.size());

                REQUIRE(std::is_permutation(shuffled.cbegin(), shuffled.cend(), ordered.cbegin(), ordered.cend()));

                const auto expected = shuffled;

                generator.seed(seed);

                shuffled = ordered;
                std::shuffle(shuffled.begin(), shuffled.end(), generator);

                REQUIRE(expected == shuffled);
            }
        }

        SECTION("with seed")
        {
            auto generator = PredictableBitGenerator{252U};

            REQUIRE(generator.min() == 0U);

            REQUIRE(generator.max() == 255U);

            const auto expected = std::vector<unsigned>{{252, 253, 254, 255, 0, 1, 2}};

            auto actual = std::vector<unsigned>{};
            for (auto i = 0U; i < expected.size(); ++i)
            {
                actual.push_back(generator());
            }

            REQUIRE(expected == actual);
        }
    }
}
