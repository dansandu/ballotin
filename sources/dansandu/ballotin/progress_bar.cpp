#include "dansandu/ballotin/progress_bar.hpp"
#include "dansandu/ballotin/exception.hpp"
#include "dansandu/ballotin/string.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

using dansandu::ballotin::string::highlightText;
using dansandu::ballotin::string::TextHighlight;

namespace dansandu::ballotin::progress_bar
{

static constexpr auto maxSummarySize = 40;
static constexpr auto barSize = 50;
static constexpr auto completedSummary = "done";
static constexpr auto cancelledSummary = "halted";

static std::string formatSummary(const std::string& summary, const TextHighlight textHighlight = TextHighlight::None)
{
    const auto summarySize = static_cast<int>(summary.size());
    if (summarySize > maxSummarySize)
    {
        return highlightText("..." + summary.substr(summarySize - maxSummarySize + 3), textHighlight);
    }
    else
    {
        const auto paddingCharacter = ' ';
        const auto padding = maxSummarySize - summarySize;
        const auto leftPadding = padding / 2;
        const auto rightPadding = padding / 2 + padding % 2;
        return std::string(leftPadding, paddingCharacter) + highlightText(summary, textHighlight) +
               std::string(rightPadding, paddingCharacter);
    }
}

ProgressBar::ProgressBar(const std::string& header, const int headerSize, const int resolution,
                         std::function<void(const std::string&)> printer)
    : header_{header},
      headerSize_{headerSize},
      progress_{0},
      resolution_{resolution},
      printer_{std::move(printer)},
      uncaughtExceptions_{std::uncaught_exceptions()}
{
    if (headerSize <= 0)
    {
        THROW(std::invalid_argument, "progress bar header size must be greater than 0");
    }

    if (resolution < 0)
    {
        THROW(std::invalid_argument, "progress bar resolution must be greater or equal to 0");
    }

    display();
}

ProgressBar::~ProgressBar()
{
    const auto exiting = true;
    const auto success = uncaughtExceptions_ >= std::uncaught_exceptions();
    display(exiting, success);
}

void ProgressBar::updateSummary(const std::string& summary)
{
    summary_ = summary;
    display();
}

void ProgressBar::advance(const int amount)
{
    if (resolution_ == 0)
    {
        THROW(std::invalid_argument,
              "cannot advance if progress bar resolution is 0 -- the bar will be filled on successful deconstruction");
    }

    if (amount <= 0)
    {
        THROW(std::invalid_argument, "progress bar advance amount must be greater than 0");
    }

    progress_ = std::min(progress_ + amount, resolution_);
    display();
}

void ProgressBar::display(bool exiting, bool success)
{
    auto formattedSummary = std::string{};
    if (exiting)
    {
        if (success && progress_ == resolution_)
        {
            progress_ = resolution_ = 100;
            formattedSummary = formatSummary(completedSummary, TextHighlight::Green);
        }
        else
        {
            formattedSummary = formatSummary(cancelledSummary, TextHighlight::Red);
        }
    }
    else
    {
        formattedSummary = formatSummary(summary_, TextHighlight::Blue);
    }

    auto formattedHeader = std::string(headerSize_, ' ');
    std::copy_n(header_.cbegin(), std::min(headerSize_, static_cast<int>(header_.size())), formattedHeader.begin());

    const auto percentage = resolution_ != 0 ? static_cast<double>(progress_) / resolution_ : 0.0;
    const auto filledSize = static_cast<int>(std::round(percentage * barSize));
    const auto emptySize = barSize - filledSize;

    char formattedPercentage[8];
    std::snprintf(formattedPercentage, 8, "%6.2f%%", percentage * 100.0);

    const auto ending = exiting ? "\n" : "";

#ifdef _WIN32
    const auto filledBarCharacter = '\xDB';
    const auto emptyBarCharacter = '\xB1';
    auto bar = std::string(filledSize, filledBarCharacter) + std::string(emptySize, emptyBarCharacter);
#else
    const auto filledBar = "\u2588";
    const auto emptyBar = "\u2592";
    auto bar = std::string{};
    for (auto index = 0; index < filledSize; ++index)
    {
        bar += filledBar;
    }
    for (auto index = 0; index < emptySize; ++index)
    {
        bar += emptyBar;
    }
#endif

    const auto line = "\r" + formattedHeader + " " + bar + " " + formattedPercentage + " " + formattedSummary + ending;

    printer_(line);
}

}
