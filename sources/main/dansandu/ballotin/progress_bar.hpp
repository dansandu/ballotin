#pragma once

#include <functional>
#include <string>

namespace dansandu::ballotin::progress_bar
{

class PRALINE_EXPORT ProgressBar
{
public:
    ProgressBar(const std::string& header, const int headerSize, const int resolution,
                std::function<void(const std::string&)> printer);

    ~ProgressBar();

    void updateSummary(const std::string& summary);

    void advance(const int amount = 1);

private:
    void display(bool exiting = false, bool success = false);

    std::string header_;
    int headerSize_;
    std::string summary_;
    int progress_;
    int resolution_;
    std::function<void(const std::string&)> printer_;
    int uncaughtExceptions_;
};

}
