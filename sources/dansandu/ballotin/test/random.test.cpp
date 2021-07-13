#include "catchorg/catch/catch.hpp"
#include "dansandu/ballotin/test/random.hpp"

#include <algorithm>

using dansandu::ballotin::test::random::PredictableBitGenerator;

TEST_CASE("random")
{
    SECTION("PredictableBitGenerator")
    {
        auto generator = PredictableBitGenerator{};

        SECTION("samples")
        {
            auto expected = std::vector<unsigned>{{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1}};
            auto actual = std::vector<unsigned>{};

            for (auto i = 0U; i < expected.size(); ++i)
            {
                actual.push_back(generator());
            }

            REQUIRE(expected == actual);
        }

        SECTION("predictable shuffle")
        {
            auto ordered = std::vector<int>{{1, 2, 3, 4, 5, 6}};
            auto shuffled = ordered;
            std::shuffle(shuffled.begin(), shuffled.end(), generator);
            auto expected = shuffled;

            shuffled = ordered;
            std::shuffle(shuffled.begin(), shuffled.end(), generator);

            REQUIRE(expected == shuffled);

            shuffled = ordered;
            std::shuffle(shuffled.begin(), shuffled.end(), generator);

            REQUIRE(expected == shuffled);

            shuffled = ordered;
            std::shuffle(shuffled.begin(), shuffled.end(), generator);

            REQUIRE(expected == shuffled);
        }
    }
}
