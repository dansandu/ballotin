#include "dansandu/ballotin/time.hpp"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

namespace dansandu::ballotin::time {

std::string getDatetimeAsString(const char* format) {
    auto time = std::time(nullptr);
    auto localtime = *std::localtime(&time);
    std::ostringstream stream;
    stream << std::put_time(&localtime, format);
    return stream.str();
}

}
