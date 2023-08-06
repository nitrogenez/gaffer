#pragma once

#include <gaffer.hh>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace gaffer::util
{
    namespace chr = std::chrono;

    class Time {
    public:
        using clock = chr::system_clock;
        using precise_clock = chr::high_resolution_clock;
        using steady_clock = chr::steady_clock;

        using nanos = chr::nanoseconds;
        using micros = chr::microseconds;
        using millis = chr::milliseconds;
        using secs = chr::seconds;
        using mins = chr::minutes;
        using hours = chr::hours;
        using days = chr::days;
        using weeks = chr::weeks;
        using months = chr::months;
        using years = chr::years;

        static std::string local(const std::string& fmt = "%Y-%m-%dT%H:%M:%S") {
            return Time::time_point_to_str(Time::clock::now(), fmt);    
        }

        template<typename Clock = Time::precise_clock>
        static std::string now(const std::string& fmt = "%Y-%m-%dT%H:%M:%S") {
            return Time::time_point_to_str<Clock>(Clock::now(), fmt);
        }

        template<typename Clock = Time::clock>
        static std::string time_point_to_str(
            std::chrono::time_point<Clock> point,
            const std::string& fmt)
        {
            auto in_time_t = Clock::to_time_t(point);
            std::ostringstream ts;
            ts << std::put_time(std::localtime(&in_time_t), fmt.data());

            return ts.str();
        }
    };
}