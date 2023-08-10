#include <gaffer.hh>
#include <util/time.hh>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>
#include <string>

namespace gaffer::util
{
    namespace c = std::chrono;

    std::string Time::local(std::string const& fmt) {
        return Time::time_point_to_str(Time::clock::now(), fmt);
    }

    template<typename Clock>
    std::string Time::now(const std::string& fmt) {
        return Time::time_point_to_str<Clock>(Clock::now(), fmt);
    }

    template<typename Clock>
    std::string Time::time_point_to_str(c::time_point<Clock> point,
        std::string const& fmt)
    {
        auto in_time_t = Clock::to_time_t(point);
        std::ostringstream ts;
        ts << std::put_time(std::localtime(&in_time_t), fmt.data());

        return ts.str();
    }
}