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
    const auto generateBar = 
        [](const int fill) { return std::string(fill, '=') + std::string(50 - fill, '-'); };

    SECTION("zero resolution success")
    {
        const std::string expectedLines[] = {
            "\rstage " + generateBar( 0) + "  0.00%                                         ",
            "\rstage " + generateBar(50) + "  \033[32mdone\033[0m                                          \n",
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
            "\rstage " + generateBar(0) + "  0.00%                                         ",
            "\rstage " + generateBar(0) + " \033[31mhalted\033[0m                                         \n",
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
            "\rstage " + generateBar( 0) + "  0.00%                                         ",
            "\rstage " + generateBar( 0) + "  0.00%                \033[34mshort/text\033[0m               ",
            "\rstage " + generateBar(25) + " 50.00%                \033[34mshort/text\033[0m               ",
            "\rstage " + generateBar(25) + " 50.00% \033[34m...ng/to/display/inside/the/progress/bar\033[0m",
            "\rstage " + generateBar(49) + " 99.99% \033[34m...ng/to/display/inside/the/progress/bar\033[0m",
            "\rstage " + generateBar(50) + "  \033[32mdone\033[0m                                          \n",
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
            "\rstage " + generateBar( 0) + "  0.00%                                         ",
            "\rstage " + generateBar(10) + " 20.00%                                         ",
            "\rstage " + generateBar(50) + "  \033[32mdone\033[0m                                          \n",
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
            "\rstage " + generateBar( 0) + "  0.00%                                         ",
            "\rstage " + generateBar(10) + " 20.00%                                         ",
            "\rstage " + generateBar(10) + " \033[31mhalted\033[0m                                         \n",
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
            "\rstage " + generateBar( 0) + "  0.00%                                         ",
            "\rstage " + generateBar(49) + " 99.99%                                         ",
            "\rstage " + generateBar(49) + " \033[31mhalted\033[0m                                         \n",
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
