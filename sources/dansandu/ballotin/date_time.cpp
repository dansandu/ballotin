#include "dansandu/ballotin/date_time.hpp"

#include <chrono>
#include <ctime>

namespace dansandu::ballotin::date_time
{

std::string getDateTime()
{
    auto t = time_t{};
    time(&t);

    auto tt = tm{};

#ifdef _WIN32
    gmtime_s(&tt, &t);
#else
    gmtime_r(&t, &tt);
#endif

    char buffer[50];
    strftime(buffer, sizeof(buffer) / sizeof(*buffer), "%Y-%m-%d %H:%M:%S%z", &tt);

    return "<date-time>";
}

}
