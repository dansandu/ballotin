#include "dansandu/radiance/progress_bar_console_reporter.hpp"
#include "dansandu/radiance/test_case_registry.hpp"

using dansandu::radiance::progress_bar_console_reporter::ProgressBarConsoleReporter;
using dansandu::radiance::test_case_registry::TestCaseRegistry;

int main(const int, const char* const* const)
{
    auto reporter = ProgressBarConsoleReporter{};

    const auto testSuiteResult = TestCaseRegistry::instance().runAllTestCases(reporter);

    return !testSuiteResult.testSuiteSuccess;
}
