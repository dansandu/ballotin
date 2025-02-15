#include "dansandu/ballotin/console.hpp"
#include "dansandu/radiance/radiance.hpp"

using dansandu::ballotin::console::highlightText;
using dansandu::ballotin::console::TextHighlight;

TEST_CASE("console")
{
    SECTION("text highlight")
    {
        SECTION("none")
        {
            const auto actual = highlightText(L"some text", TextHighlight::None);

            REQUIRE(actual == L"some text");
        }

        SECTION("red")
        {
            const auto actual = highlightText(L"some red text", TextHighlight::Red);

            REQUIRE(actual == L"\x1B[31msome red text\x1B[0m");
        }

        SECTION("green")
        {
            const auto actual = highlightText(L"some green text", TextHighlight::Green);

            REQUIRE(actual == L"\x1B[32msome green text\x1B[0m");
        }

        SECTION("yellow")
        {
            const auto actual = highlightText(L"some yellow text", TextHighlight::Yellow);

            REQUIRE(actual == L"\x1B[33msome yellow text\x1B[0m");
        }

        SECTION("blue")
        {
            const auto actual = highlightText(L"some blue text", TextHighlight::Blue);

            REQUIRE(actual == L"\x1B[34msome blue text\x1B[0m");
        }

        SECTION("magenta")
        {
            const auto actual = highlightText(L"some magenta text", TextHighlight::Magenta);

            REQUIRE(actual == L"\x1B[35msome magenta text\x1B[0m");
        }
    }
}
