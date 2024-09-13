#include "dansandu/ballotin/date_time.hpp"

#include <ctime>

namespace dansandu::ballotin::date_time
{

std::string getLocalDateTime()
{
    auto timeInput = time_t{};
    time(&timeInput);

    auto timeOutput = tm{};

#ifdef _WIN32
    localtime_s(&timeOutput, &timeInput);
#else
    localtime_s(&timeInput, &timeOutput);
#endif

    char buffer[64];
    strftime(buffer, sizeof(buffer) / sizeof(*buffer), "%Y-%m-%d %H:%M:%S%z", &timeOutput);

    return buffer;
}

}
