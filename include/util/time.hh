#pragma once

#include <gaffer.hh>
#include <chrono>

namespace gaffer::util
{
    namespace c = std::chrono;

    class Time {
    public:
        using clock = c::system_clock;
        using precise_clock = c::high_resolution_clock;
        using steady_clock = c::steady_clock;

        using nanos = c::nanoseconds;
        using micros = c::microseconds;
        using millis = c::milliseconds;
        using secs = c::seconds;
        using mins = c::minutes;
        using hours = c::hours;
        using days = c::days;
        using weeks = c::weeks;
        using months = c::months;
        using years = c::years;

        static std::string local(const std::string& fmt = "%Y-%m-%dT%H:%M:%S");

        template<typename Clock = Time::precise_clock>
        static std::string now(const std::string& fmt = "%Y-%m-%dT%H:%M:%S");

        template<typename Clock = Time::clock>
        static std::string time_point_to_str(
            std::chrono::time_point<Clock> point, const std::string& fmt);
    };
}