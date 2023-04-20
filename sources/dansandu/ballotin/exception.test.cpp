#include "dansandu/ballotin/exception.hpp"
#include "catchorg/catch2/catch_test_macros.hpp"

#include <stdexcept>

TEST_CASE("exception")
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
