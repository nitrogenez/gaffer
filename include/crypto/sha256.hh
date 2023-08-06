#pragma once

#include <sstream>
#ifdef GAFFER_CRYPTO
// TODO: Implement the SHA256 sum generation using OpenSSL or custom stuff

// Now, at August 5th, 21:52, I realise, how long
// I wasn't playing around with OpenSSL. Now many things
// I used to know are deprecated, and now I remember
// how much pain in the ass that OpenSSL C-API is.

// UPD: August 6th, 12:36. I already hate using shady C-APIs.
// I use C++ to not use C. God damnit;

// UPD: fuck it. I'm implementing it myself.

// Wish me goddamn luck.

#include <gaffer.hh>
#include <core/logger.hh>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <ios>
#include <openssl/evp.h>

#define GAFFER_CRYPTO_MAX_BUFFER_SIZE 1024 * 8

namespace gaffer::crypto
{
    namespace fs = std::filesystem;

    std::string file_sum(const fs::path& file_path) {

    }
}
#endif