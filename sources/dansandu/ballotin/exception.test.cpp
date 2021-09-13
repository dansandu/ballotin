#include "dansandu/ballotin/exception.hpp"
#include "catchorg/catch/catch.hpp"

#include <stdexcept>

using Catch::Matches;

TEST_CASE("Exception")
{
    SECTION("pretty throw")
    {
        std::string message;
        try
        {
            THROW(std::invalid_argument, "ouch -- ", 1, " exception has been thrown");
        }
        catch (const std::exception& ex)
        {
            message = ex.what();
        }

        REQUIRE(!message.empty());
    }
}
