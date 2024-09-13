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
static constexpr auto statusSize = 6;
static constexpr auto filledBarCharacter = '=';
static constexpr auto emptyBarCharacter = '=';

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

std::string formatStatus(const double percentage)
{
    if (percentage < 1.0)
    {
        char formattedPercentage[8];
        std::snprintf(formattedPercentage, 8, "%5.2f%%", percentage * 100.0);
        return formattedPercentage;
    }
    else
    {
        return "99.99%";
    }
}

void ProgressBar::display(bool exiting, bool success)
{
    auto header = std::string(headerSize_, ' ');
    std::copy_n(header_.cbegin(), std::min(headerSize_, static_cast<int>(header_.size())), header.begin());

    auto bar = std::string{};
    auto status = std::string{};
    auto summary = std::string{};
    auto ending = "";

    if (exiting)
    {
        if (success)
        {
            bar = highlightText(std::string(barSize, filledBarCharacter), TextHighlight::Green);
            status = std::string(statusSize, ' ');
            summary = formatSummary("", TextHighlight::None);
        }
        else
        {
            if (resolution_ > 0)
            {
                const auto percentage = static_cast<double>(progress_) / resolution_;
                const auto filledSize = std::min(static_cast<int>(percentage * barSize), barSize - 1);
                bar = highlightText(std::string(filledSize, filledBarCharacter), TextHighlight::Red) +
                      std::string(barSize - filledSize, emptyBarCharacter);
                status = formatStatus(percentage);
            }
            else
            {
                bar = highlightText(std::string(barSize, filledBarCharacter), TextHighlight::Red);
                status = std::string(statusSize, ' ');
            }
            summary = formatSummary(summary_, TextHighlight::None);
        }
        ending = "\n";
    }
    else
    {
        if (resolution_ > 0)
        {
            const auto percentage = static_cast<double>(progress_) / resolution_;
            const auto filledSize = std::min(static_cast<int>(percentage * barSize), barSize - 1);
            bar = highlightText(std::string(filledSize, filledBarCharacter), TextHighlight::Blue) +
                  std::string(barSize - filledSize, emptyBarCharacter);
            status = formatStatus(percentage);
        }
        else
        {
            bar = highlightText(std::string(barSize, emptyBarCharacter), TextHighlight::None);
            status = std::string(statusSize, ' ');
        }
        summary = formatSummary(summary_, TextHighlight::None);
    }

    const auto line = "\r" + header + " " + bar + " " + status + " " + summary + "\r" + ending;

    printer_(line);
}

}
