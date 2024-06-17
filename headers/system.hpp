#pragma once

#include <ctime>

// time
inline double time() {
    #if defined(__MACOS__)
    timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec+t.tv_nsec*0.000000001;
    #else
    return 0;
    #endif
}