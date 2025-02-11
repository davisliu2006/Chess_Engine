#pragma once

#include <bits/stdc++.h>
#include "system.hpp"

// random number generator
inline std::mt19937 gen32;
inline std::mt19937_64 gen64;
inline int32_t randint(int32_t lo, int32_t hi) {
    std::uniform_int_distribution<int32_t> distr(lo, hi);
    return distr(gen32);
}
inline int64_t randlong(int64_t lo, int64_t hi) {
    std::uniform_int_distribution<int64_t> distr(lo, hi);
    return distr(gen64);
}
inline double randdbl(double lo, double hi) {
    std::uniform_real_distribution<double> distr(lo, hi);
    return distr(gen64);
}

// initialize
inline void rng_init(int seed) {
    gen32.seed(seed);
    gen64.seed(seed);
}