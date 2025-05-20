#pragma once

#include <chrono>
#include <ctime>

// time
inline double time() {
    using namespace std::chrono;
    auto t = steady_clock::now();
    return duration_cast<microseconds>(t.time_since_epoch()).count()*1e-6;
}