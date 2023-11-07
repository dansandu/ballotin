#include "dansandu/ballotin/progress_bar.hpp"
#include "catchorg/catch/catch.hpp"

#include <stdexcept>
#include <string>

using dansandu::ballotin::progress_bar::ProgressBar;

class InterruptedException : std::exception
{
};

// clang-format off
TEST_CASE("progress_bar")
{
    SECTION("zero resolution success")
    {
        const std::string expectedLines[] = {
            "\rstage ==================================================                                                \r",
            "\rstage \033[32m==================================================\033[0m                                                \r\n",
        };

        const auto header = "stage";
        const auto headerSize = 5;
        const auto resolution = 0;

        auto index = 0;

        {
            auto progressBar = ProgressBar(header, headerSize, resolution,
                [&index, &expectedLines](const std::string& text)
                {
                    REQUIRE(index < std::size(expectedLines));
                    REQUIRE(text == expectedLines[index]);
                    ++index;
                });

            REQUIRE_THROWS_AS(progressBar.advance(), std::invalid_argument);
        }

        REQUIRE(index == std::size(expectedLines));
    }

    SECTION("zero resolution failure")
    {
        const std::string expectedLines[] = {
            "\rstage ==================================================                                                \r",
            "\rstage \033[31m==================================================\033[0m                                                \r\n",
        };

        const auto header = "stage";
        const auto headerSize = 5;
        const auto resolution = 0;

        auto index = 0;

        try
        {
            auto progressBar = ProgressBar(header, headerSize, resolution,
                [&index, &expectedLines](const std::string& text)
                {
                    REQUIRE(index < std::size(expectedLines));
                    REQUIRE(text == expectedLines[index]);
                    ++index;
                });

            throw InterruptedException();
        }
        catch (const InterruptedException&)
        {

        }

        REQUIRE(index == std::size(expectedLines));
    }

    SECTION("nonzero resolution")
    {
        const std::string expectedLines[] = {
            "\rstage ==================================================  0.00%                                         \r",
            "\rstage ==================================================  0.00%                short/text               \r",
            "\rstage \033[34m=========================\033[0m========================= 50.00%                short/text               \r",
            "\rstage \033[34m=========================\033[0m========================= 50.00% ...ng/to/display/inside/the/progress/bar\r",
            "\rstage \033[34m=================================================\033[0m= 99.99% ...ng/to/display/inside/the/progress/bar\r",
            "\rstage \033[32m==================================================\033[0m                                                \r\n",
        };

        const auto header = "stage";
        const auto headerSize = 5;
        const auto resolution = 2;

        auto index = 0;

        {
            auto progressBar = ProgressBar(header, headerSize, resolution,
                [&index, &expectedLines](const std::string& text)
                {
                    REQUIRE(index < std::size(expectedLines));
                    REQUIRE(text == expectedLines[index]);
                    ++index;
                });

            progressBar.updateSummary("short/text");

            progressBar.advance();

            progressBar.updateSummary("long/path/to/file/too/long/to/display/inside/the/progress/bar");

            progressBar.advance();
        }

        REQUIRE(index == std::size(expectedLines));
    }

    SECTION("early exit")
    {
        const std::string expectedLines[] = {
            "\rstage ==================================================  0.00%                                         \r",
            "\rstage \033[34m==========\033[0m======================================== 20.00%                                         \r",
            "\rstage \033[32m==================================================\033[0m                                                \r\n",
        };

        const auto header = "stage";
        const auto headerSize = 5;
        const auto resolution = 5;

        auto index = 0;

        {
            auto progressBar = ProgressBar(header, headerSize, resolution,
                [&index, &expectedLines](const std::string& text)
                {
                    REQUIRE(index < std::size(expectedLines));
                    REQUIRE(text == expectedLines[index]);
                    ++index;
                });

            progressBar.advance();
        }

        REQUIRE(index == std::size(expectedLines));
    }

    SECTION("exception")
    {
        const std::string expectedLines[] = {
            "\rstage ==================================================  0.00%                                         \r",
            "\rstage \033[34m==========\033[0m======================================== 20.00%                                         \r",
            "\rstage \033[31m==========\033[0m======================================== 20.00%                                         \r\n",
        };

        const auto header = "stage";
        const auto headerSize = 5;
        const auto resolution = 5;

        auto index = 0;

        try
        {
            auto progressBar = ProgressBar(header, headerSize, resolution,
                [&index, &expectedLines](const std::string& text)
                {
                    REQUIRE(index < std::size(expectedLines));
                    REQUIRE(text == expectedLines[index]);
                    ++index;
                });

            progressBar.advance();

            throw InterruptedException();
        }
        catch (const InterruptedException&)
        {

        }

        REQUIRE(index == std::size(expectedLines));
    }

    SECTION("last inch exception")
    {
        const std::string expectedLines[] = {
            "\rstage ==================================================  0.00%                                         \r",
            "\rstage ==================================================  0.00%                short/text               \r",
            "\rstage \033[34m=================================================\033[0m= 99.99%                short/text               \r",
            "\rstage \033[31m=================================================\033[0m= 99.99%                short/text               \r\n",
        };

        const auto header = "stage";
        const auto headerSize = 5;
        const auto resolution = 1;

        auto index = 0;

        try
        {
            auto progressBar = ProgressBar(header, headerSize, resolution,
                [&index, &expectedLines](const std::string& text)
                {
                    REQUIRE(index < std::size(expectedLines));
                    REQUIRE(text == expectedLines[index]);
                    ++index;
                });

            progressBar.updateSummary("short/text");

            progressBar.advance();

            throw InterruptedException();
        }
        catch (const InterruptedException&)
        {

        }

        REQUIRE(index == std::size(expectedLines));
    }
}
// clang-format on
