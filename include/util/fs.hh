#pragma once

#include <gaffer.hh>
#include <util/time.hh>
#include <string>
#include <filesystem>
#include <chrono>

namespace gaffer::util::fs
{
    namespace stdfs = std::filesystem;

    std::chrono::file_clock::time_point last_modified(const stdfs::path& file_path) {
        return stdfs::last_write_time(file_path);
    }
}