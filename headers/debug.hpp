#pragma once

#include <cassert>

#ifdef DEBUG_MODE
#define debug_assert(x) assert(x)
#else
#define debug_assert(x)
#endif