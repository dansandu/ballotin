#include "catchorg/catch2/catch_session.hpp"
#include "catchorg/catch2/catch_test_case_info.hpp"
#include "catchorg/catch2/reporters/catch_reporter_event_listener.hpp"
#include "catchorg/catch2/reporters/catch_reporter_registrars.hpp"
#include "dansandu/ballotin/environment.hpp"
#include "dansandu/ballotin/logging.hpp"
#include "dansandu/ballotin/progress_bar.hpp"

#include <iostream>
#include <set>

using dansandu::ballotin::environment::getEnvironmentVariable;
using dansandu::ballotin::logging::Level;
using dansandu::ballotin::logging::Logger;
using dansandu::ballotin::logging::UnitTestsHandler;
using dansandu::ballotin::progress_bar::ProgressBar;

class ProgressBarEventListener : public Catch::EventListenerBase
{
public:
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override
    {
        auto tests = std::set<Catch::TestCaseHandle const*>{};
        auto const& allTestCases = getAllTestCasesSorted(*m_config);
        auto const& testSpec = m_config->testSpec();
        if (!testSpec.hasFilters())
        {
            for (auto const& test : allTestCases)
            {
                if (!test.getTestCaseInfo().isHidden())
                {
                    tests.emplace(&test);
                }
            }
        }
        else
        {
            auto matches = testSpec.matchesByFilter(allTestCases, *m_config);
            for (auto const& match : matches)
            {
                tests.insert(match.tests.begin(), match.tests.end());
            }
        }
        testCount_ = tests.size();
    }

    void testCaseStarting(Catch::TestCaseInfo const& testInfo) override
    {
        if (progressBar_ == nullptr)
        {
            const auto header = std::string{"test"};
            const auto headerSizeStr = getEnvironmentVariable("PRALINE_PROGRESS_BAR_HEADER_LENGTH");
            const auto headerSize = headerSizeStr.has_value() ? std::stoi(headerSizeStr.value()) : header.size();

            progressBar_ = std::make_unique<ProgressBar>(header, headerSize, testCount_,
                                                         [](const auto& text)
                                                         {
                                                             std::cout << text;
                                                             std::cout.flush();
                                                         });
        }
        progressBar_->updateSummary(testInfo.name);
    }

    void testCaseEnded(Catch::TestCaseStats const&) override
    {
        progressBar_->advance();
    }

    void testRunEnded(Catch::TestRunStats const&) override
    {
        progressBar_.reset();
    }

private:
    int testCount_;
    std::unique_ptr<ProgressBar> progressBar_;
};

CATCH_REGISTER_LISTENER(ProgressBarEventListener);

int main(const int argumentsCount, const char* const* const arguments)
{
    auto& logger = Logger::globalInstance();
    logger.setLevel(Level::debug);
    logger.addHandler("UnitTests", Level::debug, UnitTestsHandler{"unit_tests.log"});

    const auto catchResult = Catch::Session().run(argumentsCount, arguments);

    return catchResult;
}
