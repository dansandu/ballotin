#include "dansandu/ballotin/scope.hpp"
#include "dansandu/radiance/radiance.hpp"

#include <stdexcept>

TEST_CASE("scope")
{
    SECTION("failure guard")
    {
        SECTION("with success")
        {
            auto triggered = false;

            {
                SCOPE_FAILURE([&] { triggered = true; });
            }

            REQUIRE(!triggered);
        }

        SECTION("with failure")
        {
            auto triggered = false;

            const auto failingFunction = [&]()
            {
                SCOPE_FAILURE([&] { triggered = true; });

                throw std::runtime_error("error");
            };

            REQUIRE_THROW(failingFunction(), std::runtime_error);

            REQUIRE(triggered);
        }
    }
}
